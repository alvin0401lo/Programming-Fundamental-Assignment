// *********************************************************
// Program: YOUR_FILENAME.cpp
// Course: CCP6114 Programming Fundamentals
// Lecture Class: TC1L
// Tutorial Class: TT1L
// Trimester: 2430
// Member_1: 242UC241BQ | ALVIN LO JIAN WEN | ALVIN.LO.JIAN@student.mmu.edu.my | 011-2741 6523
// Member_2: 242UC244M6 | YAP HUI CHI | YAP.HUI.CHI@student.mmu.edu.my | 016-3225726
// Member_3: 242UC2426R | BIANCA LAU YING XUAN | BIANCA.LAU.YING@student.mmu.edu.my | 010-2752246
// Member_4: 242UC244KQ | NG BAI KEONG | NG.BAI.KEONG@student.mmu.edu.my | 018-6605595
// *********************************************************
// Task Distribution
// Member_1: Created updateFile, displayContent, countRow function. Made the user documentation for this programme.
// Member_2: Flowchart, debug
// Member_3: insertNewRow, check the user documentation
// Member_4: Created Delete function, check the structure diagram
// *********************************************************

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
using namespace std;

// Global variables to store the table's column headers and row data
vector<string> columnHeaders;
vector<vector<string>> tableData;

// Function declarations
void fileCreate(const string &);
void tableCreate(const string &);
void fileOpen(const string &);
void secondOption(const string &);
void displayContent(const string &);

void fileDelete(const string &);
void fileUpdate(const string &);
void fileCount();
void insertNewRow(const string &);

// Helper function: Write current data (columnHeaders, tableData) to fileName.txt in CSV format
void writeCsvToTxt(const string &);

int main()
{
    string fileName;
    while (true)
    {
        cout << "\n1. Create a file (.txt)" << endl;
        cout << "2. Open existing file (.mdb)" << endl;
        cout << "3. Exit" << endl;

        cout << "Enter your choice: ";
        int mode;
        cin >> mode;

        switch (mode)
        {
        case 1:
            // Create a new file
            cout << "Please enter the file name (no extension needed like .txt): ";
            cin >> fileName;
            fileCreate(fileName);
            tableCreate(fileName);
            secondOption(fileName);
            break;

        case 2:
            // Open an existing file
            cout << "Please enter the file name (no extension needed like .mdb): ";
            cin >> fileName;
            fileOpen(fileName);
            secondOption(fileName);
            break;

        case 3:
            // Exit the program
            cout << "Exiting program..." << endl;
            return 0;

        default:
            cout << "Invalid choice, please try again!" << endl;
        }
    }
    return 0;
}

// Creates an empty .txt file if it does not exist
void fileCreate(const string &fileName)
{
    ifstream existingFile(fileName + ".txt");
    if (existingFile.is_open())
    {
        cout << "Error: File \"" << fileName << ".txt\" already exists!" << endl;
        existingFile.close();
        return;
    }
    existingFile.close();

    ofstream file(fileName + ".txt");
    if (!file.is_open())
    {
        cout << "Error opening file for writing!" << endl;
        return;
    }
    cout << "File \"" << fileName << ".txt\" has been successfully created!" << endl;
    file.close();
}

// Creates the table structure in memory and writes CSV to fileName.txt
void tableCreate(const string &fileName)
{
    columnHeaders.clear();
    tableData.clear();

    cout << "Please enter the table name (e.g. customer): ";
    string tableName;
    cin >> tableName;

    cout << "How many columns do you want in your table? ";
    int numColumns;
    cin >> numColumns;

    cout << "Please enter the short column names (e.g., id, name, city):" << endl;
    for (int i = 0; i < numColumns; i++)
    {
        cout << "Column " << (i + 1) << ": ";
        string shortColName;
        cin >> shortColName;

        // Automatically add prefix: tableName_shortColName
        string fullColName = tableName + "_" + shortColName;
        columnHeaders.push_back(fullColName);
    }

    cout << "How many rows of data do you want to add initially? ";
    int numRows;
    cin >> numRows;

    for (int i = 0; i < numRows; i++)
    {
        cout << "Entering data for row " << (i + 1) << ":" << endl;
        vector<string> rowData;
        for (int j = 0; j < numColumns; j++)
        {
            cout << columnHeaders[j] << ": ";
            string value;
            cin >> value;
            rowData.push_back(value);
        }
        tableData.push_back(rowData);
    }

    writeCsvToTxt(fileName);

    cout << "Table created and saved in \"" << fileName << ".txt\"!" << endl;
    displayContent(fileName);
}

// Opens a .mdb file (CSV format), reads it into memory, then writes to fileName.txt
void fileOpen(const string &fileName)
{
    ifstream infile(fileName + ".mdb");
    if (!infile.is_open())
    {
        cout << "Error: Cannot open \"" << fileName << ".mdb\" for reading!" << endl;
        return;
    }

    columnHeaders.clear();
    tableData.clear();

    string line;
    bool isFirstLine = true;

    while (getline(infile, line))
    {
        if (line.empty())
            continue;

        vector<string> tokens;
        {
            stringstream ss(line);
            string token;
            while (getline(ss, token, ','))
            {
                tokens.push_back(token);
            }
        }

        if (isFirstLine)
        {
            // The first line is column headers
            columnHeaders = tokens;
            isFirstLine = false;
        }
        else
        {
            // The rest are data rows
            tableData.push_back(tokens);
        }
    }
    infile.close();

    writeCsvToTxt(fileName);

    cout << "Successfully loaded data from \"" << fileName
         << ".mdb\" and wrote to \"" << fileName << ".txt\"." << endl;

    displayContent(fileName);
}

// Displays the current table content (from memory) in CSV style
void displayContent(const string &fileName)
{
    cout << "\nTable Content for \"" << fileName << ".txt\":" << endl;

    // Print column headers
    for (int i = 0; i < columnHeaders.size(); i++)
    {
        cout << columnHeaders[i];
        if (i < columnHeaders.size() - 1) cout << ",";
    }
    cout << endl;

    // Print row data
    for (auto & row : tableData)
    {
        for (int j = 0; j < row.size(); j++)
        {
            cout << row[j];
            if (j < row.size() - 1) cout << ",";
        }
        cout << endl;
    }
}

// Secondary menu to handle delete, update, count, insert, etc.
void secondOption(const string &fileName)
{
    while (true)
    {
        cout << "Choose an option:" << endl;
        cout << "1. Delete a specific row" << endl;
        cout << "2. Update a specific field" << endl;
        cout << "3. Count the number of rows" << endl;
        cout << "4. Insert a new row" << endl;
        cout << "5. Exit program" << endl;
        cout << "6. Back to main menu" << endl;
        cout << "Enter your choice: ";
        int option;
        cin >> option;

        switch (option)
        {
        case 1:
            fileDelete(fileName);
            break;
        case 2:
            fileUpdate(fileName);
            break;
        case 3:
            fileCount();
            break;
        case 4:
            insertNewRow(fileName);
            break;
        case 5:
            cout << "Exiting program..." << endl;
            exit(0);
            break;
        case 6:
            return;
        default:
            cout << "Invalid choice!" << endl;
        }
    }
}

// Writes the current table (columnHeaders, tableData) into fileName.txt in CSV format
void writeCsvToTxt(const string &fileName)
{
    ofstream outfile(fileName + ".txt");
    if (!outfile.is_open())
    {
        cout << "Error writing to \"" << fileName << ".txt\"!" << endl;
        return;
    }

    // Column headers
    for (int i = 0; i < columnHeaders.size(); i++)
    {
        outfile << columnHeaders[i];
        if (i < columnHeaders.size() - 1)
            outfile << ",";
    }
    outfile << "\n";

    // Rows
    for (auto & row : tableData)
    {
        for (int j = 0; j < row.size(); j++)
        {
            outfile << row[j];
            if (j < row.size() - 1)
                outfile << ",";
        }
        outfile << "\n";
    }
    outfile.close();
}

// Allows deleting a specific row
void fileDelete(const string &fileName)
{
    // Display the current table content
    displayContent(fileName);

    cout << "\nDelete a specific row:" << endl;
    cout << "Enter the row number to delete: ";
    int deleteRow;
    cin >> deleteRow;

    // Check whether the row number is valid
    if (deleteRow <= 0 || deleteRow > (int)tableData.size())
    {
        cout << "Invalid row number!" << endl;
        return;
    }

    // Remove that row from the in-memory data
    tableData.erase(tableData.begin() + deleteRow - 1);

    // Overwrite the .txt file with the updated table
    writeCsvToTxt(fileName);

    cout << "Row " << deleteRow << " deleted successfully!" << endl;
}


// Modified update function: Locate record by id, then update a specified field.
void fileUpdate(const string &fileName)
{
    // Display the current table content
    displayContent(fileName);

    // Ask the user to enter the id of the record to update.
    cout << "Please enter the id of the record to update: ";
    string idValue;
    cin >> idValue;

    // Assume that the id is stored in the first column (index 0)
    int rowIndex = -1;
    for (int i = 0; i < tableData.size(); i++) {
        if (tableData[i][0] == idValue) {
            rowIndex = i;
            break;
        }
    }

    if (rowIndex == -1) {
        cout << "No record found with id \"" << idValue << "\"!" << endl;
        return;
    }

    // Display the record found for reference.
    cout << "Record found:" << endl;
    for (int j = 0; j < columnHeaders.size(); j++) {
        cout << j + 1 << ". " << columnHeaders[j] << " : " << tableData[rowIndex][j] << endl;
    }

    // Ask the user which column they want to update (by column number).
    cout << "Please enter the column number to update: ";
    int colNumber;
    cin >> colNumber;
    if (colNumber <= 0 || colNumber > (int)columnHeaders.size()) {
        cout << "Invalid column number!" << endl;
        return;
    }
    int colIndex = colNumber - 1;

    // Get the new value from the user.
    cout << "Enter the new value for " << columnHeaders[colIndex] << ": ";
    string newValue;
    cin >> newValue;

    // Update the specified field in the located record.
    tableData[rowIndex][colIndex] = newValue;

    // Write the updated data back to the file.
    writeCsvToTxt(fileName);
    cout << "Record updated successfully!" << endl;
}


// Prints the current number of rows in the table
void fileCount()
{
    cout << "The number of rows in the table: " << tableData.size() << endl;
}

// Allows inserting a new row
void insertNewRow(const string &fileName)
{
    displayContent(fileName);

    if (columnHeaders.empty())
    {
        cout << "No table structure found! Create a table first." << endl;
        return;
    }

    vector<string> newRow;
    cout << "Entering data for the new row:" << endl;
    for (int i = 0; i < columnHeaders.size(); i++)
    {
        cout << columnHeaders[i] << ": ";
        string value;
        cin >> value;
        newRow.push_back(value);
    }

    tableData.push_back(newRow);
    writeCsvToTxt(fileName);
    cout << "New row added successfully!" << endl;
}
