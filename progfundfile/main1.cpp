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
// Member_1: Created fileOpen, updateFile, displayContent, countRow function.Created and wrote detailed explanation for the user documentation.
// Member_2: Created fileCreate, flowchart. Created and designed structured diagram and flowchart. Debug the programme
// Member_3: Create insertNewRow function. Created and designed the user documentation
// Member_4: Created Delete function. Checked and designed the structure diagram and flowchart
// *********************************************************

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
using namespace std;

// Global variables to store the table's column headers and row data.
vector<string> columnHeaders;
vector<vector<string>> tableData;

//Creates an empty .txt file if it does not already exist.
void fileCreate(const string &);

// Creates the table and writes the resulting CSV data to a .txt file.
void tableCreate(const string &);

// Opens an existing .mdb file (assumed CSV formatted), reads it and writes it to a .txt file.
void fileOpen(const string &);

// Displays a secondary menu for performing further operations
void secondOption(const string &);

// Displays the current table content (headers and rows) from memory in CSV format.
void displayContent(const string &);

// Writes the current table (headers and data) to a .txt file in CSV format.
void writeCsvToTxt(const string &);

// Deletes a specific row from the table as specified by the user.
void fileDelete(const string &);

// Updates a specific field in a record located by its id (assumed to be in the first column).
void fileUpdate(const string &);

//Displays the number of rows in the table.
void fileCount();

// Inserts a new row into the table and updates the file.
void insertNewRow(const string &);

int main()
{
    string fileName;
    while (true)
    {
        // Display the main menu options
        cout << "\n1. Create a file (.txt)" << endl;
        cout << "2. Open existing file (.mdb)" << endl;
        cout << "3. Exit" << endl;

        cout << "Enter your choice: ";
        int mode;
        cin >> mode;

        // Process user input using a switch-case.
        switch (mode)
        {
        case 1:
            // Option 1: Create a new file.
            cout << "Please enter the file name (no extension needed like .txt): ";
            cin >> fileName;
            fileCreate(fileName);    // Create a new file.
            tableCreate(fileName);   // Create the table structure and input data.
            secondOption(fileName);  // Display secondary menu for further operations.
            break;

        case 2:
            // Option 2: Open an existing file.
            cout << "Please enter the file name (no extension needed like .mdb): ";
            cin >> fileName;
            fileOpen(fileName);      // Open and read an existing file.
            secondOption(fileName);  // Display secondary menu for further operations.
            break;

        case 3:
            // Option 3: Exit the program.
            cout << "Exiting program..." << endl;
            return 0;

        default:
            cout << "Invalid choice, please try again!" << endl;
        }
    }
    return 0;
}

// Creates an empty .txt file if it does not exist.
void fileCreate(const string &fileName)
{
    ifstream existingFile;
    existingFile.open(fileName + ".txt");
    if (existingFile.is_open())
    {
        // If the file exists, output an error message and exit the function.
        cout << "Error: File \"" << fileName << ".txt\" already exists!" << endl;
        existingFile.close();
        return;
    }
    existingFile.close();

    // Create a new file with .txt extension.
    ofstream file;
    file.open(fileName + ".txt");
    if (!file.is_open())
    {
        // If opening the file for writing fails, output an error.
        cout << "Error opening file for writing!" << endl;
        return;
    }
    cout << "File \"" << fileName << ".txt\" has been successfully created!" << endl;
    file.close();
}

// Creates the table and writes CSV to fileName.txt.
void tableCreate(const string &fileName)
{
    columnHeaders.clear();
    tableData.clear();

    // Prompt the user to enter a table name (e.g., 'customer').
    cout << "Please enter the table name (e.g. customer): ";
    string tableName;
    cin >> tableName;

    // Prompt for the number of columns.
    cout << "How many columns do you want in your table? ";
    int numColumns;
    cin >> numColumns;

    // Prompt for each column name.
    cout << "Please enter the column names (e.g., id, name, city):" << endl;
    for (int i = 0; i < numColumns; i++)
    {
        cout << "Column " << (i + 1) << ": ";
        string shortColName;
        cin >> shortColName;

        // Concatenate table name with the short column name to form the full column name.
        string fullColName = tableName + "_" + shortColName;
        columnHeaders.push_back(fullColName);
    }

    // Prompt for the number of rows to add initially.
    cout << "How many rows of data do you want to add initially? ";
    int numRows;
    cin >> numRows;

    // Loop to enter data for each row.
    for (int i = 0; i < numRows; i++)
    {
        cout << "Entering data for row " << (i + 1) << ":" << endl;
        vector<string> rowData; // Temporary vector to hold data for the current row
        for (int j = 0; j < numColumns; j++)
        {
            // Prompt for each column value
            cout << columnHeaders[j] << ": ";
            string value;
            cin >> value;
            rowData.push_back(value);
        }
        tableData.push_back(rowData); // Add the completed row to the table data.
    }

    writeCsvToTxt(fileName); // Write the table data to the .txt file in CSV format.

    cout << "Table created and saved in \"" << fileName << ".txt\"!" << endl;
    displayContent(fileName);
}

// Opens a .mdb file (CSV format), reads its contents, and writes them to fileName.txt.
void fileOpen(const string &fileName)
{
    ifstream infile;
    infile.open(fileName + ".mdb");
    if (!infile.is_open())
    {
        // If the file cannot be opened, output an error message.
        cout << "Error: Cannot open \"" << fileName << ".mdb\" for reading!" << endl;
        return;
    }

    columnHeaders.clear();
    tableData.clear();

    string line;
    bool isFirstLine = true; // Flag to indicate the first line (headers).

    // Read the file line by line.
    while (getline(infile, line))
    {
        if (line.empty())
            continue; // Skip empty lines.

        vector<string> tokens; // Vector to hold tokens for the current line.
        {
            stringstream ss(line); // Use a stringstream to split the line.
            string token;
            // Split the line on commas.
            while (getline(ss, token, ','))
            {
                tokens.push_back(token);
            }
        }

        if (isFirstLine)
        {
            // The first line contains the column headers.
            columnHeaders = tokens;
            isFirstLine = false;
        }
        else
        {
            // Subsequent lines are data rows.
            tableData.push_back(tokens);
        }
    }
    infile.close(); // Close the input file stream.

    writeCsvToTxt(fileName); // Write the loaded data to a .txt file.

    cout << "Successfully loaded data from \"" << fileName
         << ".mdb\" and wrote to \"" << fileName << ".txt\"." << endl;

    displayContent(fileName); // Display the table content.
}

// Displays the current table content in CSV style.
void displayContent(const string &fileName)
{
    // Print the file name and indicate that this is the table content.
    cout << endl << "Table Content for \"" << fileName << ".txt\":" << endl;

    // Print column headers separated by commas.
    for (int i = 0; i < columnHeaders.size(); i++)
    {
        cout << columnHeaders[i];
        if (i < columnHeaders.size() - 1)
            cout << ",";
    }
    cout << endl;

    // Loop through each row and print its data.
    for (auto & row : tableData)
    {
        for (int j = 0; j < row.size(); j++)
        {
            cout << row[j];
            if (j < row.size() - 1)
                cout << ",";
        }
        cout << endl;
    }
}

// Displays the secondary menu and handles further file operations (delete, update, count, insert).
void secondOption(const string &fileName)
{
    while (true)
    {
        // Display secondary menu options.
        cout << "Choose an option:" << endl;
        cout << "1. Delete a specific row" << endl;
        cout << "2. Update a specific field" << endl;
        cout << "3. Count the number of rows" << endl;
        cout << "4. Insert a new row" << endl;
        cout << "5. Exit program" << endl;
        cout << "6. Back to main menu" << endl;
        cout << "Enter your choice: " << endl;

        int option;
        cin >> option;

        switch (option)
        {
        case 1:
            fileDelete(fileName); // Delete a specific row.
            break;
        case 2:
            fileUpdate(fileName); // Update a specific field in a record.
            break;
        case 3:
            fileCount(); // Display the number of rows.
            break;
        case 4:
            insertNewRow(fileName); // Insert a new row.
            break;
        case 5:
            // Exit the program.
            cout << "Exiting program..." << endl;
            exit(0);
            break;
        case 6:
            // Return to the main menu.
            return;
        default:
            // Handle invalid selections.
            cout << "Invalid choice!" << endl;
        }
    }
}

// Writes the current table into fileName.txt in CSV format.
void writeCsvToTxt(const string &fileName)
{
    ofstream outfile;
    outfile.open(fileName + ".txt");
    if (!outfile.is_open())
    {
        cout << "Error writing to \"" << fileName << ".txt\"!" << endl;
        return;
    }

    // Write the column headers to the file, separated by commas.
    for (int i = 0; i < columnHeaders.size(); i++)
    {
        outfile << columnHeaders[i];
        if (i < columnHeaders.size() - 1)
            outfile << ",";
    }
    outfile << "\n";

    // Write each row of data to the file.
    for (auto & row : tableData)
    {
        for (int j = 0; j < row.size(); j++)
        {
            outfile << row[j];
            if (j < row.size() - 1)
                outfile << ",";
        }
        outfile << "\n"; // New line after each row.
    }
    outfile.close(); // Close the file stream.
}

// Allows deleting a specific row from the table.
void fileDelete(const string &fileName)
{
    // Display the current table content so the user can choose which row to delete.
    displayContent(fileName);

    cout << endl << "Delete a specific row:" << endl;
    cout << "Enter the row number to delete: ";
    int deleteRow; // Variable to store the row number to be deleted.
    cin >> deleteRow;

    // Validate the row number provided by the user.
    if (deleteRow <= 0 || deleteRow > (int)tableData.size())
    {
        cout << "Invalid row number!" << endl;
        return;
    }

    // Remove the specified row
    tableData.erase(tableData.begin() + deleteRow - 1);

    // Update the .txt file with the modified table data
    writeCsvToTxt(fileName);

    cout << "Row " << deleteRow << " deleted successfully!" << endl << endl;
}

// Modified update function: Locate record by id, then update a specified field.
void fileUpdate(const string &fileName)
{
    // Display the current table content.
    displayContent(fileName);

    // Prompt the user to enter the id of the record to update.
    cout << "Please enter the id of the record to update: " << endl;
    string idValue; // Variable to store the id input.
    cin >> idValue;

    // Assume the id is stored in the first column (index 0).
    int rowIndex = -1; // Variable to hold the index of the found record.
    for (int i = 0; i < tableData.size(); i++) {
        if (tableData[i][0] == idValue) {
            rowIndex = i;
            break;
        }
    }

    if (rowIndex == -1) {
        // If no record is found with the given id, notify the user.
        cout << "No record found with id \"" << idValue << "\"!" << endl;
        return;
    }

    // Display the found record with numbered columns.
    cout << "Record found:" << endl;
    for (int j = 0; j < columnHeaders.size(); j++) {
        cout << j + 1 << ". " << columnHeaders[j] << " : " << tableData[rowIndex][j] << endl;
    }

    // Prompt the user to select the column number to update.
    cout << "Please enter the column number to update: ";
    int colNumber; // Variable to store the chosen column number.
    cin >> colNumber;
    if (colNumber <= 0 || colNumber > (int)columnHeaders.size()) {
        cout << "Invalid column number!" << endl;
        return;
    }
    int colIndex = colNumber - 1;

    // Prompt the user for the new value for the specified column.
    cout << "Enter the new value for " << columnHeaders[colIndex] << ": " << endl;
    string newValue;
    cin >> newValue;

    // Update the specified field in the located record.
    tableData[rowIndex][colIndex] = newValue;

    // Write the updated table data back to the file.
    writeCsvToTxt(fileName);
    cout << "Record updated successfully!" << endl << endl;
}

// Prints the current number of rows in the table.
void fileCount()
{
    // Output the number of data rows in the table.
    cout << "The number of rows in the table: " << tableData.size() << endl << endl;
}

// Allows inserting a new row into the table.
void insertNewRow(const string &fileName)
{
    // Display the current table content.
    displayContent(fileName);

    if (columnHeaders.empty())
    {
        cout << "No table structure found! Create a table first." << endl;
        return;
    }

    vector<string> newRow; // Temporary vector to hold the new row's data.
    cout << "Entering data for the new row:" << endl;
    // Prompt for each column's data.
    for (int i = 0; i < columnHeaders.size(); i++)
    {
        cout << columnHeaders[i] << ": ";
        string value; // Variable to store the input value.
        cin >> value;
        newRow.push_back(value); // Add the value to the new row.
    }

    // Append the new row to the table data.
    tableData.push_back(newRow);
    // Update the file with the new table data.
    writeCsvToTxt(fileName);
    cout << "New row added successfully!" << endl;
}
