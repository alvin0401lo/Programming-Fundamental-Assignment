#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <cctype> // For character conversion

using namespace std;

// *Struct to represent a table*
struct Table {
    vector<string> columnDefinitions;      // Column definitions (e.g., "ID INT")
    vector<vector<string>> data;           // Data rows
};

// *Global table mapping*
map<string, Table> tables;

// *Function declarations*
string trim(const string& str);
vector<string> readMdbFile(const string& filename);
void appendToFile(const string& filename, const string& content);
void createTable(const string& command, const string& outputFile);
void insertIntoTable(const string& command, const string& outputFile);
void updateTable(const string& command, const string& outputFile);
void deleteFromTable(const string& command, const string& outputFile);
void selectAll(const string& command, const string& outputFile);
void countRows(const string& command, const string& outputFile);

// *Main Function*
int main() {
    string outputFile, inputFile;

    // Prompt user for output file name
    cout << "📂 Enter output .txt file name: ";
    getline(cin, outputFile);

    // Clear the output file
    ofstream clearFileStream(outputFile, ios::trunc);
    if (!clearFileStream) {
        cerr << "❌ Error: Unable to create or clear file " << outputFile << endl;
        return 1;
    }
    clearFileStream.close();

    // Prompt user for input .mdb file path
    cout << "📂 Enter .mdb file path: ";
    getline(cin, inputFile);

    // Read SQL commands from the .mdb file
    vector<string> commands = readMdbFile(inputFile);

    // Process each SQL command
    for (const string &command : commands) {
        // Convert command to uppercase for matching (without altering the original command's case)
        string upperCommand = command;
        for (size_t i = 0; i < upperCommand.size(); ++i) {
            upperCommand[i] = toupper(upperCommand[i]);
        }

        // Determine the type of SQL command and execute the corresponding function
        if (upperCommand.find("CREATE TABLE") != string::npos) {
            createTable(command, outputFile);
        }
        else if (upperCommand.find("INSERT INTO") != string::npos) {
            insertIntoTable(command, outputFile);
        }
        else if (upperCommand.find("UPDATE") != string::npos) {
            updateTable(command, outputFile);
        }
        else if (upperCommand.find("DELETE FROM") != string::npos) {
            deleteFromTable(command, outputFile);
        }
        else if (upperCommand.find("SELECT * FROM") != string::npos) {
            selectAll(command, outputFile);
        }
        else if (upperCommand.find("COUNT(*)") != string::npos) {
            countRows(command, outputFile);
        }
        else {
            cerr << "⚠ Warning: Unrecognized command \"" << command << "\"\n";
        }
    }

    // Notify the user that data has been saved
    cout << "✅ Data has been saved to " << outputFile << "\n";
    return 0;
}

// *Function Definitions Below*

/**
 * @brief Trims whitespace from both ends of a string.
 *
 * @param str The input string to trim.
 * @return A new string with leading and trailing whitespace removed.
 */
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == string::npos)
        return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

/**
 * @brief Reads an .mdb file and extracts SQL commands separated by semicolons.
 *
 * @param filename The path to the .mdb file.
 * @return A vector of SQL command strings.
 */
vector<string> readMdbFile(const string& filename) {
    vector<string> commands;
    ifstream file(filename);
    if (!file) {
        cerr << "❌ Error: Unable to open file " << filename << endl;
        exit(1);
    }

    string line;
    string command = "";
    while (getline(file, line)) {
        line = trim(line);
        if (line.empty()) continue;

        // Concatenate lines to form a complete SQL command
        command += " " + line;

        // Check if the line ends with a semicolon, indicating the end of a command
        if (line.back() == ';') {
            // Remove the semicolon and trim excess whitespace
            command = trim(command.substr(0, command.size() - 1));
            commands.push_back(command);
            command = "";
        }
    }
    file.close();
    return commands;
}

/**
 * @brief Appends content to a specified output file.
 *
 * @param filename The output file name.
 * @param content The content to append.
 */
void appendToFile(const string& filename, const string& content) {
    ofstream file(filename, ios::app);
    if (!file) {
        cerr << "❌ Error: Unable to write to file " << filename << endl;
        exit(1);
    }
    file << content;
    file.close();
}

/**
 * @brief Parses and executes a CREATE TABLE command.
 *
 * @param command The SQL command string.
 * @param outputFile The output file to write results.
 */
void createTable(const string& command, const string& outputFile) {
    size_t start = command.find("(");
    size_t end = command.find(")");
    if (start == string::npos || end == string::npos) {
        cerr << "❌ Error: Incorrect format for CREATE TABLE command.\n";
        return;
    }

    // Extract table name
    string tableName = trim(command.substr(12, start - 12)); // "CREATE TABLE " length is 12

    // Extract column definitions
    string columnDefs = command.substr(start + 1, end - start - 1);
    vector<string> columnDefinitions;
    stringstream ss(columnDefs);
    string col;
    while (getline(ss, col, ',')) {
        columnDefinitions.push_back(trim(col));
    }

    // Initialize the table
    tables[tableName] = {columnDefinitions, {}};

    // Format the CREATE TABLE command for output
    string formattedTable = "> CREATE TABLE " + tableName + " (\n";
    for (size_t i = 0; i < columnDefinitions.size(); ++i) {
        formattedTable += "    " + columnDefinitions[i];
        if (i != columnDefinitions.size() - 1)
            formattedTable += ",";
        formattedTable += "\n";
    }
    formattedTable += ");\n\n";

    appendToFile(outputFile, formattedTable);
}

/**
 * @brief Parses and executes an INSERT INTO command.
 *
 * @param command The SQL command string.
 * @param outputFile The output file to write results.
 */
void insertIntoTable(const string& command, const string& outputFile) {
    size_t valuesPos = command.find("VALUES");
    if (valuesPos == string::npos) return;

    size_t start = command.find("(", 11); // "INSERT INTO " length is 11
    size_t end = command.find(")", valuesPos);
    if (start == string::npos || end == string::npos) {
        cerr << "❌ Error: Incorrect format for INSERT INTO command.\n";
        return;
    }

    // Extract table name
    string tableName = trim(command.substr(11, command.find("(", 11) - 11)); // From "INSERT INTO " onwards

    // Extract columns part
    size_t firstParen = command.find("(", 11);
    size_t commaBeforeValues = command.find(")", firstParen);
    string columnsPart = command.substr(firstParen + 1, commaBeforeValues - firstParen - 1);

    // Extract values part
    string valuesPart = command.substr(command.find("(", valuesPos) + 1, command.find(")", valuesPos) - command.find("(", valuesPos) - 1);

    // Check if the table exists
    if (tables.find(tableName) == tables.end()) {
        cerr << "❌ Error: Table " << tableName << " does not exist.\n";
        return;
    }

    // Split values
    vector<string> values;
    stringstream ss(valuesPart);
    string value;
    while (getline(ss, value, ',')) {
        values.push_back(trim(value));
    }

    // Add the data row
    tables[tableName].data.push_back(values);

    // Format the INSERT INTO command for output
    string formattedInsert = "> INSERT INTO " + tableName + " (" + columnsPart + ") VALUES (";
    for (size_t i = 0; i < values.size(); ++i) {
        formattedInsert += values[i];
        if (i < values.size() - 1) formattedInsert += ", ";
    }
    formattedInsert += ");\n";

    appendToFile(outputFile, formattedInsert);
}

/**
 * @brief Parses and executes an UPDATE command.
 *
 * @param command The SQL command string.
 * @param outputFile The output file to write results.
 */
void updateTable(const string& command, const string& outputFile) {
    size_t setPos = command.find("SET");
    size_t wherePos = command.find("WHERE");
    if (setPos == string::npos || wherePos == string::npos) {
        cerr << "❌ Error: Incorrect format for UPDATE command.\n";
        return;
    }

    // Extract table name
    string tableName = trim(command.substr(6, setPos - 6)); // "UPDATE " length is 6

    // Extract SET and WHERE clauses
    string setClause = trim(command.substr(setPos + 3, wherePos - setPos - 3));
    string whereClause = trim(command.substr(wherePos + 5));

    // Check if the table exists
    if (tables.find(tableName) == tables.end()) {
        cerr << "❌ Error: Table " << tableName << " does not exist.\n";
        return;
    }

    // Parse WHERE clause
    size_t equalPos = whereClause.find("=");
    if (equalPos == string::npos) {
        cerr << "❌ Error: Incorrect format for WHERE clause.\n";
        return;
    }
    string whereColumn = trim(whereClause.substr(0, equalPos));
    string whereValue = trim(whereClause.substr(equalPos + 1));

    // Parse SET clause
    size_t setEqualPos = setClause.find("=");
    if (setEqualPos == string::npos) {
        cerr << "❌ Error: Incorrect format for SET clause.\n";
        return;
    }
    string setColumn = trim(setClause.substr(0, setEqualPos));
    string setValue = trim(setClause.substr(setEqualPos + 1));

    // Find indices of SET and WHERE columns
    int setIndex = -1;
    int whereIndex = -1;
    for (size_t i = 0; i < tables[tableName].columnDefinitions.size(); ++i) {
        // Assume column definition is "ColumnName Type"
        size_t spacePos = tables[tableName].columnDefinitions[i].find(' ');
        string colName = (spacePos != string::npos) ? tables[tableName].columnDefinitions[i].substr(0, spacePos) : tables[tableName].columnDefinitions[i];
        if (colName == setColumn) setIndex = i;
        if (colName == whereColumn) whereIndex = i;
    }

    // Check if columns exist
    if (setIndex == -1) {
        cerr << "❌ Error: Column " << setColumn << " does not exist in table " << tableName << ".\n";
        return;
    }
    if (whereIndex == -1) {
        cerr << "❌ Error: Column " << whereColumn << " does not exist in table " << tableName << ".\n";
        return;
    }

    // Execute the update
    for (auto &row : tables[tableName].data) {
        if (row[whereIndex] == whereValue) {
            row[setIndex] = setValue;
        }
    }

    // Format the UPDATE command for output
    string formattedUpdate = "> " + command + ";\n";
    appendToFile(outputFile, formattedUpdate);

    // Output the current state of the table
    string selectCommand = "SELECT * FROM " + tableName + ";";

    string output = "> " + selectCommand + "\n";
    // Output column names
    for (size_t i = 0; i < tables[tableName].columnDefinitions.size(); ++i) {
        // Assume column definition is "ColumnName Type"
        size_t spacePos = tables[tableName].columnDefinitions[i].find(' ');
        string colName = (spacePos != string::npos) ? tables[tableName].columnDefinitions[i].substr(0, spacePos) : tables[tableName].columnDefinitions[i];
        output += colName;
        if (i != tables[tableName].columnDefinitions.size() - 1)
            output += ", ";
    }
    output += "\n";

    // Output data rows
    for (const auto &row : tables[tableName].data) {
        for (size_t i = 0; i < row.size(); ++i) {
            output += row[i];
            if (i != row.size() - 1)
                output += ", ";
        }
        output += "\n";
    }
    output += "\n";

    appendToFile(outputFile, output);
}

/**
 * @brief Parses and executes a DELETE FROM command.
 *
 * @param command The SQL command string.
 * @param outputFile The output file to write results.
 */
void deleteFromTable(const string& command, const string& outputFile) {
    size_t wherePos = command.find("WHERE");
    if (wherePos == string::npos) {
        cerr << "❌ Error: Incorrect format for DELETE command.\n";
        return;
    }

    // Extract table name
    string tableName = trim(command.substr(12, wherePos - 12)); // "DELETE FROM " length is 12

    // Extract WHERE condition
    string condition = trim(command.substr(wherePos + 5));

    // Check if the table exists
    if (tables.find(tableName) == tables.end()) {
        cerr << "❌ Error: Table " << tableName << " does not exist.\n";
        return;
    }

    // Parse WHERE condition
    size_t equalPos = condition.find("=");
    if (equalPos == string::npos) {
        cerr << "❌ Error: Incorrect format for WHERE condition.\n";
        return;
    }
    string whereColumn = trim(condition.substr(0, equalPos));
    string whereValue = trim(condition.substr(equalPos + 1));

    // Find the index of the WHERE column
    int whereIndex = -1;
    for (size_t i = 0; i < tables[tableName].columnDefinitions.size(); ++i) {
        // Assume column definition is "ColumnName Type"
        size_t spacePos = tables[tableName].columnDefinitions[i].find(' ');
        string colName = (spacePos != string::npos) ? tables[tableName].columnDefinitions[i].substr(0, spacePos) : tables[tableName].columnDefinitions[i];
        if (colName == whereColumn) {
            whereIndex = i;
            break;
        }
    }

    // Check if the WHERE column exists
    if (whereIndex == -1) {
        cerr << "❌ Error: Column " << whereColumn << " does not exist in table " << tableName << ".\n";
        return;
    }

    // Execute the deletion by creating a new data vector excluding the rows to delete
    vector<vector<string>> newData;
    for (const auto &row : tables[tableName].data) {
        if (row[whereIndex] != whereValue) {
            newData.push_back(row);
        }
    }
    tables[tableName].data = newData;

    // Format the DELETE command for output
    string formattedDelete = "> " + command + ";\n";
    appendToFile(outputFile, formattedDelete);

    // Output the current state of the table
    string selectCommand = "SELECT * FROM " + tableName + ";";

    string output = "> " + selectCommand + "\n";
    // Output column names
    for (size_t i = 0; i < tables[tableName].columnDefinitions.size(); ++i) {
        // Assume column definition is "ColumnName Type"
        size_t spacePos = tables[tableName].columnDefinitions[i].find(' ');
        string colName = (spacePos != string::npos) ? tables[tableName].columnDefinitions[i].substr(0, spacePos) : tables[tableName].columnDefinitions[i];
        output += colName;
        if (i != tables[tableName].columnDefinitions.size() - 1)
            output += ", ";
    }
    output += "\n";

    // Output data rows
    for (const auto &row : tables[tableName].data) {
        for (size_t i = 0; i < row.size(); ++i) {
            output += row[i];
            if (i != row.size() - 1)
                output += ", ";
        }
        output += "\n";
    }
    output += "\n";

    appendToFile(outputFile, output);
}

/**
 * @brief Parses and executes a SELECT * FROM command.
 *
 * @param command The SQL command string.
 * @param outputFile The output file to write results.
 */
void selectAll(const string& command, const string& outputFile) {
    size_t fromPos = command.find("FROM");
    if (fromPos == string::npos) {
        cerr << "❌ Error: Incorrect format for SELECT command.\n";
        return;
    }

    // Extract table name
    string tableName = trim(command.substr(fromPos + 4));

    // Check if the table exists
    if (tables.find(tableName) == tables.end()) {
        cerr << "❌ Error: Table " << tableName << " does not exist.\n";
        return;
    }

    // Format the SELECT command for output
    string output = "> " + command + ";\n";
    // Output column names
    for (size_t i = 0; i < tables[tableName].columnDefinitions.size(); ++i) {
        // Assume column definition is "ColumnName Type"
        size_t spacePos = tables[tableName].columnDefinitions[i].find(' ');
        string colName = (spacePos != string::npos) ? tables[tableName].columnDefinitions[i].substr(0, spacePos) : tables[tableName].columnDefinitions[i];
        output += colName;
        if (i != tables[tableName].columnDefinitions.size() - 1)
            output += ", ";
    }
    output += "\n";

    // Output data rows
    for (const auto &row : tables[tableName].data) {
        for (size_t i = 0; i < row.size(); ++i) {
            output += row[i];
            if (i != row.size() - 1)
                output += ", ";
        }
        output += "\n";
    }
    output += "\n";

    appendToFile(outputFile, output);
}

/**
 * @brief Parses and executes a COUNT(*) command.
 *
 * @param command The SQL command string.
 * @param outputFile The output file to write results.
 */
void countRows(const string& command, const string& outputFile) {
    size_t fromPos = command.find("FROM");
    if (fromPos == string::npos) {
        cerr << "❌ Error: Incorrect format for COUNT command.\n";
        return;
    }

    // Extract table name
    string tableName = trim(command.substr(fromPos + 4));

    // Check if the table exists
    if (tables.find(tableName) == tables.end()) {
        cerr << "❌ Error: Table " << tableName << " does not exist.\n";
        return;
    }

    // Format the COUNT command for output
    string output = "> " + command + "\n" + to_string(tables[tableName].data.size()) + "\n\n";
    appendToFile(outputFile, output);
}
