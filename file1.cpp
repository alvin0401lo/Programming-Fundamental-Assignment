#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <cctype>

using namespace std;

// Structure to represent a Table
struct Table {
    vector<string> columnDefinitions;
    vector<vector<string>> data;
};

// Global mappings / variables
map<string, Table> tables;
string databasePath;
string g_outputFile = "defaultOutput.txt";

// Function declarations
vector<string> readMdbFile(const string &filename);
string trim(const string &str);
void appendToFile(const string &filename, const string &content);

void handleCreateFile(const string &command);
void handleDatabasesCmd(string &cmd);
void createTable(const string &command);
void insertIntoTable(const string &command);
void updateTable(const string &command);
void deleteFromTable(const string &command);
void selectAll(const string &command);
void countRows(const string &command);

// Main function
int main() {
    // Prompt user to input the .mdb file path
    string inputFile;
    cout << "Enter the .mdb file path: ";
    getline(cin, inputFile);

    databasePath = inputFile;

    // Attempt to read commands from the .mdb file
    vector<string> commands = readMdbFile(inputFile);

    // Parse and execute each command
    for (auto &cmd : commands) {
        // Convert to uppercase for matching
        string upper(cmd.size(), '\0');
        for (size_t i = 0; i < cmd.size(); i++) {
            upper[i] = toupper(cmd[i]);
        }

        // 1) CREATE fileOutput.txt; (excluding CREATE TABLE)
        if (upper.rfind("CREATE ", 0) == 0 && upper.find("CREATE TABLE") == string::npos) {
            handleCreateFile(cmd);
        }
        // 2) DATABASES;
        else if (upper.find("DATABASES") != string::npos) {
            handleDatabasesCmd(cmd);
        }
        // 3) CREATE TABLE
        else if (upper.find("CREATE TABLE") != string::npos) {
            createTable(cmd);
        }
        // 4) INSERT
        else if (upper.find("INSERT INTO") != string::npos) {
            insertIntoTable(cmd);
        }
        // 5) UPDATE
        else if (upper.find("UPDATE") != string::npos) {
            updateTable(cmd);
        }
        // 6) DELETE
        else if (upper.find("DELETE FROM") != string::npos) {
            deleteFromTable(cmd);
        }
        // 7) SELECT * FROM
        else if (upper.find("SELECT * FROM") != string::npos) {
            selectAll(cmd);
        }
        // 8) COUNT(*)
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

// Reads the .mdb file and splits commands by trailing semicolons.
vector<string> readMdbFile(const string &filename) {
    vector<string> commands;
    ifstream fin(filename);
    if (!fin.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        exit(1);
    }

    string line, current;
    while (true) {
        if (!getline(fin, line)) {
            if (!current.empty()) {
                commands.push_back(trim(current));
            }
            break;
        }
        line = trim(line);
        if (line.empty()) continue;

        if (!current.empty()) current += " ";
        current += line;

        // If the line ends with ';', treat it as the end of a command
        if (!line.empty() && line.back() == ';') {
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

// Trims whitespace from both ends of a string
string trim(const string &str) {
    if (str.empty()) return str;
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == string::npos) return "";
    size_t last  = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

// Appends content to the specified output file
void appendToFile(const string &filename, const string &content) {
    ofstream ofs(filename, ios::app);
    if (!ofs.is_open()) {
        cerr << "Error: Unable to write to file " << filename << endl;
        exit(1);
    }
    ofs << content;
    ofs.close();
}

// Handles "CREATE fileOutput.txt;" command (but not "CREATE TABLE")
void handleCreateFile(const string &command) {
    string cmd = trim(command);
    if (!cmd.empty() && cmd[0] == '>') {
        cmd.erase(cmd.begin());
        cmd = trim(cmd);
    }

    size_t pos = cmd.find("CREATE ");
    if (pos != string::npos) {
        cmd = trim(cmd.substr(pos + 7));
    }

    if (!cmd.empty() && cmd.back() == ';') {
        cmd.pop_back();
    }
    cmd = trim(cmd);

    // This is the output file name
    g_outputFile = cmd;

    // Create / clear
    ofstream ofs(g_outputFile);
    if (!ofs.is_open()) {
        cerr << "Error creating file: " << g_outputFile << endl;
        return;
    }
    ofs.close();

    // Write a short message
    ostringstream oss;
    oss << "> " << command << ";\n";
    appendToFile(g_outputFile, oss.str());
}

// Handles "DATABASES;" command (and possibly any path after it)
void handleDatabasesCmd(string &cmd) {
    // Write the command to the output file
    {
        ostringstream oss;
        oss << "> " << cmd << ";\n";
        appendToFile(g_outputFile, oss.str());
    }

    // For example, if the command is "DATABASES; C:\somePath.mdb"
    size_t pos = cmd.find("DATABASES;");
    if (pos != string::npos) {
        string after = trim(cmd.substr(pos + 10));
        if (!after.empty()) {
            databasePath = after;
        } else {
            databasePath = "(no path provided)";
        }
    }

    // Output the final database path
    ostringstream oss;
    oss << databasePath << "\n\n";
    appendToFile(g_outputFile, oss.str());
}

// CREATE TABLE customer(...)
void createTable(const string &command) {
    ostringstream oss;
    oss << "> " << command << ";\n";
    appendToFile(g_outputFile, oss.str());

    size_t start = command.find("(");
    size_t end   = command.rfind(")");
    if (start == string::npos || end == string::npos || end < start) {
        cerr << "Error: CREATE TABLE format.\n";
        return;
    }

    // "CREATE TABLE " is 12 characters long
    string tableName = trim(command.substr(12, start - 12));
    string cols = command.substr(start + 1, end - (start + 1));
    vector<string> columnDefs;
    {
        stringstream ss(cols);
        string col;
        while (getline(ss, col, ',')) {
            columnDefs.push_back(trim(col));
        }
    }
    tables[tableName] = {columnDefs, {}};

    oss.str("");
    oss.clear();
    oss << tableName << " created with columns:\n";
    for (auto &c : columnDefs) {
        oss << "  " << c << "\n";
    }
    oss << "\n";
    appendToFile(g_outputFile, oss.str());
}

// INSERT INTO ...
void insertIntoTable(const string &command) {
    {
        ostringstream oss;
        oss << "> " << command << ";\n";
        appendToFile(g_outputFile, oss.str());
    }

    size_t intoPos = command.find("INTO");
    size_t valPos  = command.find("VALUES");
    if (intoPos == string::npos || valPos == string::npos) {
        cerr << "Error: INSERT format.\n";
        return;
    }
    string sub = trim(command.substr(intoPos + 4, valPos - (intoPos + 4)));
    size_t p = sub.find("(");
    string tableName = (p == string::npos) ? sub : trim(sub.substr(0, p));

    if (tables.find(tableName) == tables.end()) {
        cerr << "Error: Table not found -> " << tableName << "\n";
        return;
    }

    size_t lp = command.find("(", valPos);
    size_t rp = command.find(")", lp);
    if (lp == string::npos || rp == string::npos) {
        cerr << "Error: INSERT missing parentheses.\n";
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
    tables[tableName].data.push_back(row);
}

// UPDATE ...
void updateTable(const string &command) {
    {
        ostringstream oss;
        oss << "> " << command << ";\n";
        appendToFile(g_outputFile, oss.str());
    }

    size_t updatePos = command.find("UPDATE");
    size_t setPos    = command.find("SET");
    size_t wherePos  = command.find("WHERE");
    if (updatePos == string::npos || setPos == string::npos || wherePos == string::npos) {
        cerr << "Error: UPDATE format.\n";
        return;
    }

    string tableName = trim(command.substr(updatePos + 6, setPos - (updatePos + 6)));
    if (tables.find(tableName) == tables.end()) {
        cerr << "Error: Table not found -> " << tableName << "\n";
        return;
    }

    string setClause   = trim(command.substr(setPos + 3, wherePos - (setPos + 3)));
    string whereClause = trim(command.substr(wherePos + 5));

    // setClause => col = val
    size_t eq = setClause.find("=");
    if (eq == string::npos) {
        cerr << "Error: invalid SET.\n";
        return;
    }
    string setCol = trim(setClause.substr(0, eq));
    string setVal = trim(setClause.substr(eq + 1));

    // where => wCol = wVal
    eq = whereClause.find("=");
    if (eq == string::npos) {
        cerr << "Error: invalid WHERE.\n";
        return;
    }
    string wCol = trim(whereClause.substr(0, eq));
    string wVal = trim(whereClause.substr(eq + 1));

    auto &colDefs = tables[tableName].columnDefinitions;
    int setIdx = -1, wIdx = -1;
    for (int i = 0; i < (int)colDefs.size(); i++) {
        size_t sp = colDefs[i].find(' ');
        string cName = (sp == string::npos) ? colDefs[i] : colDefs[i].substr(0, sp);
        if (cName == setCol) setIdx = i;
        if (cName == wCol)   wIdx = i;
    }
    if (setIdx < 0 || wIdx < 0) {
        cerr << "Error: column not found.\n";
        return;
    }

    // Perform update
    for (auto &row : tables[tableName].data) {
        if (row[wIdx] == wVal) {
            row[setIdx] = setVal;
        }
    }

    // Output the updated table
    ostringstream oss;
    oss << "> SELECT * FROM " << tableName << ";\n";
    for (int i = 0; i < (int)colDefs.size(); i++) {
        size_t sp = colDefs[i].find(' ');
        string cName = (sp == string::npos) ? colDefs[i] : colDefs[i].substr(0, sp);
        oss << cName;
        if (i < (int)colDefs.size() - 1) oss << ",";
    }
    oss << "\n";
    for (auto &r : tables[tableName].data) {
        for (int i = 0; i < (int)r.size(); i++) {
            oss << r[i];
            if (i < (int)r.size() - 1) oss << ",";
        }
        oss << "\n";
    }
    oss << "\n";
    appendToFile(g_outputFile, oss.str());
}

// DELETE FROM ...
void deleteFromTable(const string &command) {
    {
        ostringstream oss;
        oss << "> " << command << ";\n";
        appendToFile(g_outputFile, oss.str());
    }

    size_t fromPos  = command.find("FROM");
    size_t wherePos = command.find("WHERE");
    if (fromPos == string::npos || wherePos == string::npos) {
        cerr << "Error: DELETE format.\n";
        return;
    }
    string tableName = trim(command.substr(fromPos + 4, wherePos - (fromPos + 4)));
    if (tables.find(tableName) == tables.end()) {
        cerr << "Error: Table not found->" << tableName << "\n";
        return;
    }
    string wClause = trim(command.substr(wherePos + 5));
    size_t eq = wClause.find("=");
    if (eq == string::npos) {
        cerr << "Error: invalid WHERE.\n";
        return;
    }
    string wCol = trim(wClause.substr(0, eq));
    string wVal = trim(wClause.substr(eq + 1));

    auto &colDefs = tables[tableName].columnDefinitions;
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
        cerr << "Error: column not found.\n";
        return;
    }

    // Perform deletion
    vector<vector<string>> newData;
    for (auto &row : tables[tableName].data) {
        if (row[wIndex] != wVal) {
            newData.push_back(row);
        }
    }
    tables[tableName].data = newData;

    // Output the current state of the table
    ostringstream oss;
    oss << "> SELECT * FROM " << tableName << ";\n";
    for (int i = 0; i < (int)colDefs.size(); i++) {
        size_t sp = colDefs[i].find(' ');
        string cName = (sp == string::npos) ? colDefs[i] : colDefs[i].substr(0, sp);
        oss << cName;
        if (i < (int)colDefs.size() - 1) oss << ",";
    }
    oss << "\n";
    for (auto &r : tables[tableName].data) {
        for (int i = 0; i < (int)r.size(); i++) {
            oss << r[i];
            if (i < (int)r.size() - 1) oss << ",";
        }
        oss << "\n";
    }
    oss << "\n";
    appendToFile(g_outputFile, oss.str());
}

// SELECT * FROM ...
void selectAll(const string &command) {
    {
        ostringstream oss;
        oss << "> " << command << ";\n";
        appendToFile(g_outputFile, oss.str());
    }

    size_t fromPos = command.find("FROM");
    if (fromPos == string::npos) {
        cerr << "Error: SELECT format.\n";
        return;
    }
    string tableName = trim(command.substr(fromPos + 4));
    if (tables.find(tableName) == tables.end()) {
        cerr << "Error: Table not found.\n";
        return;
    }
    auto &colDefs = tables[tableName].columnDefinitions;
    auto &rows    = tables[tableName].data;

    ostringstream oss;
    // Output column names
    for (int i = 0; i < (int)colDefs.size(); i++) {
        size_t sp = colDefs[i].find(' ');
        string cName = (sp == string::npos) ? colDefs[i] : colDefs[i].substr(0, sp);
        oss << cName;
        if (i < (int)colDefs.size() - 1) oss << ",";
    }
    oss << "\n";
    // Output data rows
    for (auto &r : rows) {
        for (int i = 0; i < (int)r.size(); i++) {
            oss << r[i];
            if (i < (int)r.size() - 1) oss << ",";
        }
        oss << "\n";
    }
    oss << "\n";
    appendToFile(g_outputFile, oss.str());
}

// SELECT COUNT(*) FROM ...
void countRows(const string &command) {
    {
        ostringstream oss;
        oss << "> " << command << "\n";
        appendToFile(g_outputFile, oss.str());
    }

    size_t fromPos = command.find("FROM");
    if (fromPos == string::npos) {
        cerr << "Error: COUNT format.\n";
        return;
    }
    string tableName = trim(command.substr(fromPos + 4));
    if (tables.find(tableName) == tables.end()) {
        cerr << "Error: Table not found.\n";
        return;
    }

    ostringstream oss;
    oss << tables[tableName].data.size() << "\n\n";
    appendToFile(g_outputFile, oss.str());
}
