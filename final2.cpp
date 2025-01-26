#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cctype>

using namespace std;

//Represents a table structure, containing column definitions and row data.
struct Table {
    vector<string> columnDefinitions;
    vector<vector<string>> data;      // Each element is a row (vector<string>)
};

//A record that binds a table name with its underlying Table structure.
struct TableRecord {
    string name;
    Table table;
};

// Global variables
vector<TableRecord> g_tables;    // Holds all tables that are currently defined
string g_databasePath;           // Stores the database path from user input
string g_outputFile = "defaultOutput.txt"; // The output file name (default)

//Reads the entire .mdb file
vector<string> readMdbFile(const string &);

//Trims leading and trailing whitespace from a string.
string trim(const string &);

//Appends (writes at the end) the specified content to a file.
void appendToFile(const string &, const string &);

//Handles commands that look like "CREATE outputFileName;"
//which sets the global output file.
void handleCreateFile(const string &);

//Handles commands that have the keyword "DATABASES;".
//It also updates g_databasePath.
void handleDatabasesCmd(string &);

//Creates a table when receiving commands like:
//CREATE TABLE tableName ( colDef1, colDef2, ... );
void createTable(const string &);

//Inserts a row into a specified table,
//INSERT INTO tableName (...) VALUES (...);
void insertIntoTable(const string &);

//Updates rows in a specified table, e.g.:
//UPDATE tableName SET col = val WHERE col = val;
void updateTable(const string &);

//Deletes rows from a specified table,
//DELETE FROM tableName WHERE col = val;
void deleteFromTable(const string &);

//Selects all rows from a table:
//SELECT * FROM tableName;
void selectAll(const string &);

//Counts the number of rows in a table:
//SELECT COUNT(*) FROM tableName;
void countRows(const string &);

//Finds the index of a table in g_tables by name,
//or returns -1 if not found.
int findTableIndex(const string &);

int main() {
    cout << "Enter the .mdb file path: ";
    string inputFile;
    getline(cin, inputFile);

    g_databasePath = inputFile;

    // Read commands from .mdb file
    vector<string> commands = readMdbFile(inputFile);

    // Parse and execute each command
    for (auto &cmd : commands) {
        // Convert to uppercase for easier matching
        string upper(cmd.size(), '\0');
        for (size_t i = 0; i < cmd.size(); i++) {
            upper[i] = toupper(cmd[i]);
        }

        if (upper.rfind("CREATE ", 0) == 0 && upper.find("CREATE TABLE") == string::npos) {
            // "CREATE fileName..." but not "CREATE TABLE..."
            handleCreateFile(cmd);
        }
        else if (upper.find("DATABASES") != string::npos) {
            handleDatabasesCmd(cmd);
        }
        else if (upper.find("CREATE TABLE") != string::npos) {
            createTable(cmd);
        }
        else if (upper.find("INSERT INTO") != string::npos) {
            insertIntoTable(cmd);
        }
        else if (upper.find("UPDATE") != string::npos) {
            updateTable(cmd);
        }
        else if (upper.find("DELETE FROM") != string::npos) {
            deleteFromTable(cmd);
        }
        else if (upper.find("SELECT * FROM") != string::npos) {
            selectAll(cmd);
        }
        else if (upper.find("COUNT(*)") != string::npos) {
            countRows(cmd);
        }
        else {
            cerr << "Warning: Unrecognized command: " << cmd << endl;
        }
    }

    cout << "All done. Output is in `" << g_outputFile << "`." << endl;
    return 0;
}

vector<string> readMdbFile(const string &filename) {
/*
Reads the file line by line, concatenates them into `current`,
and splits on ';'. Returns a vector of trimmed commands (strings).
*/
    vector<string> commands;
    ifstream fin;
    fin.open(filename);
    if (!fin.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        exit(1);
    }

    string line, current;
    while (true) {
        if (!getline(fin, line)) {
            // End of file: if anything remains in `current`, push it as a command.
            if (!current.empty()) {
                commands.push_back(trim(current));
            }
            break;
        }
        line = trim(line);
        if (line.empty()) continue;

        if (!current.empty()) current += " ";
        current += line;

        // If the line ends with ';' we consider it a complete command
        if (!line.empty() && line.back() == ';') {
            // Remove the trailing semicolon
            if (!current.empty() && current.back() == ';') {
                current.pop_back();
            }
            commands.push_back(trim(current));
            current.clear();
        }
    }
    fin.close();
    return commands;
}

string trim(const string &str) {

    //Removes leading and trailing whitespace characters (" \t\r\n").
    if (str.empty())
        return str;
        size_t first = str.find_first_not_of(" \t\r\n");

    if (first == string::npos)
        return "";
        size_t last  = str.find_last_not_of(" \t\r\n");
        return str.substr(first, (last - first + 1));
}

void appendToFile(const string &filename, const string &content) {

    //Opens file in append mode and writes `content` to it.
    cout << content;
    ofstream ofs;
    ofs.open(filename, ios::app);
    if (!ofs.is_open()) {
        cout << "Error: Unable to write to file " << filename << endl;
        exit(1);
    }
    ofs << content;
    ofs.close();
}

int findTableIndex(const string &tableName) {

    /*
     Looks through g_tables to find the index of a TableRecord
     whose name matches `tableName`.
     Returns the index if found, otherwise returns -1.
    */
    for (int i = 0; i < (int)g_tables.size(); i++) {
        if (g_tables[i].name == tableName) {
            return i;
        }
    }
    return -1;
}

void handleCreateFile(const string &command) {
    /*
     Extracts the file name after "CREATE " and before the semicolon,
     sets it as the global output file, and creates (or overwrites) that file.
     Also appends the command line to the output file.
     */
    string cmd = trim(command);
    if (!cmd.empty() && cmd[0] == '>') {
        cmd.erase(cmd.begin());
        cmd = trim(cmd);
    }

    size_t pos = cmd.find("CREATE ");
    if (pos != string::npos)
        {
        cmd = trim(cmd.substr(pos + 7));
        }

    // Remove trailing semicolon if present
    if (!cmd.empty() && cmd.back() == ';')
        {
        cmd.pop_back();
        }
    cmd = trim(cmd);

    g_outputFile = cmd;
    ofstream ofs;
    ofs.open(g_outputFile);
    if (!ofs.is_open()) {
        cerr << "Error creating file: " << g_outputFile << endl;
        return;
    }
    ofs.close();

    ostringstream oss;
    oss << "> " << command << ";" << endl;
    appendToFile(g_outputFile, oss.str());
}

void handleDatabasesCmd(string &cmd) {
    /*
     When encountering a "DATABASES;" command, we append the command line to the output,
     and set g_databasePath if there's text after "DATABASES;".
    */
    {
        ostringstream oss;
        oss << "> " << cmd << ";" << endl;
        appendToFile(g_outputFile, oss.str());
    }
    size_t pos = cmd.find("DATABASES;");
    if (pos != string::npos)
        {
        string after = trim(cmd.substr(pos + 10));
        if (!after.empty()) {
            g_databasePath = after;
        }
    else
        {
            g_databasePath = "Error! no path provided";
        }
    }

    ostringstream oss;
    oss << g_databasePath << endl;
    appendToFile(g_outputFile, oss.str());
}

void createTable(const string &command) {
    /*
     1. First we format and print the CREATE TABLE command with indentation,
        then remove trailing semicolon if present.
     2. We parse the table name and column definitions,
        and create (or overwrite) the table in g_tables.
    */
    ostringstream formattedCmd;
    string temp = command;
    bool hasSemicolon = false;

    // Check if the command ends with ';'
    if (!temp.empty() && temp.back() == ';')
        {
        temp.pop_back();
        hasSemicolon = true;
        }

    // Insert newlines and indentation around parentheses and commas
    for (size_t i = 0; i < temp.size(); i++)
        {
        char c = temp[i];
        if (c == '(') {
            formattedCmd << "(\n    ";
        } else if (c == ',') {
            formattedCmd << ",\n    ";
        } else if (c == ')') {
            formattedCmd << "\n)";
        } else {
            formattedCmd << c;
        }
    }
    if (hasSemicolon)
    {
        formattedCmd << ";";
    }

    {
        // Append the formatted CREATE TABLE command to the output file
        ostringstream oss;
        oss << "> " << formattedCmd.str() << endl;
        appendToFile(g_outputFile, oss.str());
    }

    // Parse table name and column definitions
    size_t start = command.find("(");
    size_t end   = command.rfind(")");
    if (start == string::npos || end == string::npos || end < start)
        {
        cerr << "Error: invalid CREATE TABLE format." << endl;
        return;
        }
    // "CREATE TABLE " length is 10
    string tableName = trim(command.substr(12, start - 12));
    // Extract columns between parentheses
    string cols = command.substr(start + 1, end - (start + 1));

    vector<string> columnDefs;
    {
        stringstream ss(cols);
        string col;
        while (getline(ss, col, ',')) {
            columnDefs.push_back(trim(col));
        }
    }

    // Check if table already exists
    int idx = findTableIndex(tableName);
    if (idx < 0) {
        // If not found, create new
        TableRecord newRec;
        newRec.name = tableName;
        newRec.table.columnDefinitions = columnDefs;
        g_tables.push_back(newRec);
    } else {
        // If found, overwrite column definitions and clear existing data
        g_tables[idx].table.columnDefinitions = columnDefs;
        g_tables[idx].table.data.clear();
    }
}

void insertIntoTable(const string &command) {

    // Static variables for counting
    static bool initialized = false;         // True if we've already counted total inserts
    static int  totalInserts = 0;       // How many "INSERT" commands are in the .mdb file
    static int  currentInserts = 0;     // How many inserts we've actually processed

    // On the first call, read the entire .mdb file again
    if (!initialized) {
        // Use the same path the user entered at runtime, stored in g_databasePath.
        vector<string> allCommands = readMdbFile(g_databasePath);

        // Count how many lines have "INSERT INTO"
        for (auto &cmdLine : allCommands) {
            string upper = cmdLine;
            for (auto &c : upper) {
                c = toupper(c);
            }
            if (upper.find("INSERT INTO") != string::npos) {
                totalInserts++;
            }
        }

        initialized = true; // We won't do this check again
    }

    //Regular insert logic
    {
        ostringstream oss;
        oss << "> " << command << ";" << endl;
        appendToFile(g_outputFile, oss.str());
    }

    size_t intoPos = command.find("INTO");
    size_t valPos  = command.find("VALUES");
    if (intoPos == string::npos || valPos == string::npos) {
        cout << "Error: invalid INSERT syntax." << endl;
        return;
    }

    // Extract table name
    string sub = trim(command.substr(intoPos + 4, valPos - (intoPos + 4)));
    size_t p = sub.find("(");
    string tableName = (p == string::npos) ? sub : trim(sub.substr(0, p));

    int idx = findTableIndex(tableName);
    if (idx < 0) {
        cout << "Error: Table not found -> " << tableName << endl;
        return;
    }

    // Parse VALUES(...) content
    size_t lp = command.find("(", valPos);
    size_t rp = command.find(")", lp);
    if (lp == string::npos || rp == string::npos) {
        cout << "Error: INSERT missing parentheses." << endl;
        return;
    }

    string valPart = command.substr(lp + 1, rp - (lp + 1));
    vector<string> row;
    {
        stringstream ss(valPart);
        string v;
        while (getline(ss, v, ',')) {
            row.push_back(trim(v));
        }
    }

    // Insert the new row into the table
    g_tables[idx].table.data.push_back(row);

    //Increment our current insert count
    currentInserts++;

    // If this is the final insert (comparing to total), print the table
    if (currentInserts == totalInserts && totalInserts > 0) {
        // Create a pseudo "SELECT * FROM tableName;" for clarity
        ostringstream oss;
        oss << "> SELECT * FROM " << tableName << ";" << endl;

        auto &colDefs = g_tables[idx].table.columnDefinitions;
        auto &rows    = g_tables[idx].table.data;

        // Print column names
        for (int i = 0; i < (int)colDefs.size(); i++) {
            size_t sp = colDefs[i].find(' ');
            string cName = (sp == string::npos)
                         ? colDefs[i]
                         : colDefs[i].substr(0, sp);
            oss << cName;
            if (i < (int)colDefs.size() - 1) oss << ",";
        }
        oss << endl;

        // Print all rows
        for (auto &r : rows) {
            for (int i = 0; i < (int)r.size(); i++) {
                oss << r[i];
                if (i < (int)r.size() - 1) oss << ",";
            }
            oss << endl;
        }
        oss << endl;

        // Write it to the file & console
        appendToFile(g_outputFile, oss.str());
    }
}

void updateTable(const string &command) {
    /*
     Extracts the table name from "UPDATE tableName SET ... WHERE ...",
     parses the SET clause, finds which column to update,
     parses the WHERE clause, finds which rows to update.
     Then appends the updated table's rows to the output.
     */
    {
        ostringstream oss;
        oss << "> " << command << ";" << endl;
        appendToFile(g_outputFile, oss.str());
    }

    size_t updatePos = command.find("UPDATE");
    size_t setPos    = command.find("SET");
    size_t wherePos  = command.find("WHERE");
    if (updatePos == string::npos || setPos == string::npos || wherePos == string::npos)
        {
        cout << "Error: invalid UPDATE syntax." << endl;
        return;
        }

    // Extract table name
    string tableName = trim(command.substr(updatePos + 6, setPos - (updatePos + 6)));
    int idx = findTableIndex(tableName);
    if (idx < 0)
        {
        cout << "Error: Table not found > " << tableName << endl;
        return;
        }

    // Extract "col = val" from "SET"
    string setClause   = trim(command.substr(setPos + 3, wherePos - (setPos + 3)));
    // Extract "col = val" from "WHERE"
    string whereClause = trim(command.substr(wherePos + 5));

    size_t eq = setClause.find("=");
    if (eq == string::npos)
        {
        cout << "Error: invalid SET clause." << endl;
        return;
        }

    string setCol = trim(setClause.substr(0, eq));
    string setVal = trim(setClause.substr(eq + 1));

    eq = whereClause.find("=");
    if (eq == string::npos) {
        cout << "Error: invalid WHERE clause." << endl;
        return;
    }

    string wCol = trim(whereClause.substr(0, eq));
    string wVal = trim(whereClause.substr(eq + 1));

    // Find column indexes
    auto &colDefs = g_tables[idx].table.columnDefinitions;
    int setIdx = -1, wIdx = -1;
    for (int i = 0; i < (int)colDefs.size(); i++) {
        // Column definition might be "colName type", so parse out the actual colName
        size_t sp = colDefs[i].find(' ');
        string cName = (sp == string::npos) ? colDefs[i] : colDefs[i].substr(0, sp);
        if (cName == setCol) setIdx = i;
        if (cName == wCol)   wIdx = i;
    }
    if (setIdx < 0 || wIdx < 0) {
        cout << "Error: column not found in table." << endl;
        return;
    }

    // Perform the update
    for (auto &row : g_tables[idx].table.data) {
        if (row[wIdx] == wVal) {
            row[setIdx] = setVal;
        }
    }

    // Print updated table to output
    ostringstream oss;
    oss << "> SELECT * FROM " << tableName << ";" << endl;
    for (int i = 0; i < (int)colDefs.size(); i++) {
        size_t sp = colDefs[i].find(' ');
        string cName = (sp == string::npos) ? colDefs[i] : colDefs[i].substr(0, sp);
        oss << cName;
        if (i < (int)colDefs.size() - 1) oss << ",";
    }
    oss << endl;
    for (auto &r : g_tables[idx].table.data) {
        for (int i = 0; i < (int)r.size(); i++) {
            oss << r[i];
            if (i < (int)r.size() - 1) oss << ",";
        }
        oss << endl;
    }
    oss << endl;
    appendToFile(g_outputFile, oss.str());
}

void deleteFromTable(const string &command) {
    /*
     Extracts table name from "DELETE FROM tableName WHERE ...",
     locates the column in the WHERE clause, and removes matching rows.
     Finally, prints the table after deletion.
     */
    {
        ostringstream oss;
        oss << "> " << command << ";" << endl;
        appendToFile(g_outputFile, oss.str());
    }

    size_t fromPos  = command.find("FROM");
    size_t wherePos = command.find("WHERE");
    if (fromPos == string::npos || wherePos == string::npos) {
        cout << "Error: invalid DELETE syntax." << endl;
        return;
    }
    // Extract table name
    string tableName = trim(command.substr(fromPos + 4, wherePos - (fromPos + 4)));
    int idx = findTableIndex(tableName);
    if (idx < 0) {
        cout << "Error: Table not found->" << tableName << endl;
        return;
    }

    // Extract WHERE clause
    string wClause = trim(command.substr(wherePos + 5));
    size_t eq = wClause.find("=");
    if (eq == string::npos) {
        cout << "Error: invalid WHERE clause." << endl;
        return;
    }
    string wCol = trim(wClause.substr(0, eq));
    string wVal = trim(wClause.substr(eq + 1));

    // Find the column index
    auto &colDefs = g_tables[idx].table.columnDefinitions;
    int wIndex = -1;
    for (int i = 0; i < (int)colDefs.size(); i++) {
        size_t sp = colDefs[i].find(' ');
        string cName = (sp == string::npos) ? colDefs[i] : colDefs[i].substr(0, sp);
        if (cName == wCol) {
            wIndex = i;
            break;
        }
    }
    if (wIndex < 0) {
        cout << "Error: column not found." << endl;
        return;
    }

    // Remove rows that match WHERE condition
    vector<vector<string>> newData;
    for (auto &row : g_tables[idx].table.data) {
        if (row[wIndex] != wVal) {
            newData.push_back(row);
        }
    }
    g_tables[idx].table.data = newData;

    // Print updated table
    ostringstream oss;
    oss << "> SELECT * FROM " << tableName << ";" << endl;
    for (int i = 0; i < (int)colDefs.size(); i++) {
        size_t sp = colDefs[i].find(' ');
        string cName = (sp == string::npos) ? colDefs[i] : colDefs[i].substr(0, sp);
        oss << cName;
        if (i < (int)colDefs.size() - 1) oss << ",";
    }
    oss << endl;
    for (auto &r : g_tables[idx].table.data) {
        for (int i = 0; i < (int)r.size(); i++) {
            oss << r[i];
            if (i < (int)r.size() - 1) oss << ",";
        }
        oss << endl;
    }
    oss << endl;
    appendToFile(g_outputFile, oss.str());
}

void selectAll(const string &command) {
    /*
     SELECT * FROM tableName;
     Prints column headers and each row to the output file.
     */
    {
        ostringstream oss;
        oss << "> " << command << ";" << endl;
        appendToFile(g_outputFile, oss.str());
    }

    size_t fromPos = command.find("FROM");
    if (fromPos == string::npos) {
        cout << "Error: invalid SELECT syntax." << endl;
        return;
    }
    string tableName = trim(command.substr(fromPos + 4));
    int idx = findTableIndex(tableName);
    if (idx < 0) {
        cout << "Error: Table not found." << endl;
        return;
    }
    auto &colDefs = g_tables[idx].table.columnDefinitions;
    auto &rows    = g_tables[idx].table.data;

    ostringstream oss;
    // Print column names
    for (int i = 0; i < (int)colDefs.size(); i++) {
        size_t sp = colDefs[i].find(' ');
        string cName = (sp == string::npos) ? colDefs[i] : colDefs[i].substr(0, sp);
        oss << cName;
        if (i < (int)colDefs.size() - 1) oss << ",";
    }
    oss << endl;
    // Print rows
    for (auto &r : rows) {
        for (int i = 0; i < (int)r.size(); i++) {
            oss << r[i];
            if (i < (int)r.size() - 1) oss << ",";
        }
        oss << endl;
    }
    oss << endl;
    appendToFile(g_outputFile, oss.str());
}

void countRows(const string &command) {
    /*
     Simply prints the total number of rows in the table.
     */
    {
        ostringstream oss;
        oss << "> " << command << endl;
        appendToFile(g_outputFile, oss.str());
    }

    size_t fromPos = command.find("FROM");
    if (fromPos == string::npos) {
        cout << "Error: invalid COUNT syntax." << endl;
        return;
    }
    string tableName = trim(command.substr(fromPos + 4));
    int idx = findTableIndex(tableName);
    if (idx < 0) {
        cout << "Error: Table not found." << endl;
        return;
    }

    ostringstream oss;
    oss << g_tables[idx].table.data.size() << endl << endl;
    appendToFile(g_outputFile, oss.str());
}
