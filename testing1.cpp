#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>
#include <sstream>

using namespace std;

// Global variables to store table data
vector<vector<string>> tableData; // Table data (each row is a vector<string>)
vector<string> columnHeaders;     // Table column headers

// Function declarations
void fileCreate(const string &);
void tableCreate(const string &);
void displayContent(const string &);
void fileOpen(const string &);
void secondOption(const string &);
void fileUpdate(const string &);
void fileDelete(const string &);
void fileCount(const string &);
void insertNewRow(const string &);

int main()
{
    int mode;
    string fileName;

    // Main menu loop
    while (true)
    {
        cout << "\n1. Create a file" << endl;
        cout << "2. Open existing file" << endl;
        cout << "3. Exit" << endl;

        cout << "Enter your choice: ";
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

// Check if a file exists and create a new file
void fileCreate(const string &fileName)
{
    ifstream existingFile(fileName + ".txt");
    if (existingFile.is_open())
    {
        cout << "Error: File \"" << fileName << ".txt\" already exists!" << endl;
        existingFile.close();
        return;
    }

    ofstream file(fileName + ".txt");
    if (!file.is_open())
    {
        cout << "Error opening file for writing!" << endl;
        return;
    }

    cout << "File \"" << fileName << ".txt\" has been successfully created!" << endl;
    file.close();

    cout << "Press any key to continue..." << endl;
    cin.ignore();
    cin.get();
}

// Create a table and save data to the file
void tableCreate(const string &fileName)
{
    ofstream inputfile(fileName + ".txt");
    if (!inputfile.is_open())
    {
        cout << "Error! Cannot open file for writing!" << endl;
        return;
    }

    string tableName;
    cout << "Please enter the table name: ";
    cin >> tableName;

    int numColumns;
    cout << "How many columns do you want in your table (excluding ID)? ";
    cin >> numColumns;

    // Initialize column headers and add the default "ID" column
    columnHeaders.clear();
    columnHeaders.push_back("ID");
    cout << "Please enter the column names (e.g., Name, City, Email):" << endl;
    for (int i = 0; i < numColumns; ++i)
    {
        string columnName;
        cout << "Column " << i + 1 << ": ";
        cin >> columnName;
        columnHeaders.push_back(columnName);
    }

    // Write table header to the file
    inputfile << "Table: " << tableName << endl;
    inputfile << string(100, '-') << endl;
    for (const auto &col : columnHeaders)
    {
        inputfile << left << setw(20) << col;
    }
    inputfile << endl;

    int numRows;
    cout << "How many rows of data do you want to add? ";
    cin >> numRows;

    tableData.clear();
    for (int i = 0; i < numRows; ++i)
    {
        vector<string> rowData;
        rowData.push_back(to_string(i + 1)); // Automatically generate ID column
        cout << "Entering data for row " << i + 1 << ":" << endl;

        // Input user-defined column data
        for (size_t j = 1; j < columnHeaders.size(); ++j)
        {
            string value;
            cout << columnHeaders[j] << ": ";
            cin >> value;
            rowData.push_back(value);
        }

        tableData.push_back(rowData);

        // Write each row to the file
        for (const auto &value : rowData)
        {
            inputfile << left << setw(20) << value;
        }
        inputfile << endl;
    }

    inputfile.close();
    cout << "Table \"" << tableName << "\" created and saved in \"" << fileName << ".txt\"!" << endl;

    displayContent(fileName);

    cout << "Press any key to continue..." << endl;
    cin.ignore();
    cin.get();
}

// Open an existing file and load its content
void fileOpen(const string &fileName)
{
    ifstream infile(fileName + ".mdb");
    if (!infile.is_open())
    {
        cout << "Error opening file for reading!" << endl;
        return;
    }

    tableData.clear();
    columnHeaders.clear();

    string line;
    int lineCount = 0;

    cout << "Debug: File Content:" << endl;

    while (getline(infile, line))
    {
        // Debug: Print each line
        cout << "Debug: Line read: \"" << line << "\"" << endl;

        lineCount++;

        // Skip empty or whitespace-only lines
        if (line.empty() || line.find_first_not_of(" \t") == string::npos)
        {
            continue;
        }

        // Skip the first two lines (table name and separator)
        if (lineCount == 1 || lineCount == 2)
        {
            continue;
        }

        // Parse column headers on the third line
        if (lineCount == 3)
        {
            stringstream ss(line);
            string col;
            while (ss >> col)
            {
                columnHeaders.push_back(col);
            }
            continue;
        }

        // Parse table rows
        stringstream ss(line);
        vector<string> rowData;
        string value;

        while (ss >> value)
        {
            rowData.push_back(value);
        }

        // Debug: Print the parsed row
        cout << "Debug: Parsed row - ";
        for (const auto &val : rowData)
        {
            cout << val << " ";
        }
        cout << endl;

        // Add the row to tableData if it's not empty
        if (!rowData.empty())
        {
            tableData.push_back(rowData);
        }
    }

    infile.close();

    // Display the loaded content
    displayContent(fileName);
}

// Display table content
void displayContent(const string &fileName)
{
    cout << "\nTable Content from \"" << fileName << ".mdb\":\n";
    for (const auto &col : columnHeaders)
    {
        cout << left << setw(20) << col;
    }
    cout << endl;
    cout << string(100, '-') << endl;

    for (const auto &row : tableData)
    {
        for (const auto &value : row)
        {
            cout << left << setw(20) << value;
        }
        cout << endl;
    }
}

// Delete a specific row or the entire file
void fileDelete(const string &fileName)
{
    displayContent(fileName);

    cout << "\nChoose an option:" << endl;
    cout << "1. Delete a specific row" << endl;
    cout << "2. Delete the entire file" << endl;
    cout << "Enter your choice: ";
    int choice;
    cin >> choice;

    if (choice == 1)
    {
        // Delete a specific row
        int deleteRow;
        cout << "Enter the row number to delete: ";
        cin >> deleteRow;

        if (deleteRow <= 0 || deleteRow > tableData.size())
        {
            cout << "Invalid row number!" << endl;
            return;
        }

        tableData.erase(tableData.begin() + deleteRow - 1);

        // Update ID column after deletion
        for (size_t i = 0; i < tableData.size(); ++i)
        {
            tableData[i][0] = to_string(i + 1);
        }

        // Write updated table data to the file
        ofstream outfile(fileName + ".txt");
        if (!outfile.is_open())
        {
            cout << "Error writing to file!" << endl;
            return;
        }

        for (const auto &col : columnHeaders)
        {
            outfile << left << setw(20) << col;
        }
        outfile << endl;

        for (const auto &row : tableData)
        {
            for (const auto &value : row)
            {
                outfile << left << setw(20) << value;
            }
            outfile << endl;
        }

        outfile.close();
        cout << "Row " << deleteRow << " deleted successfully!" << endl;
    }
    else if (choice == 2)
    {
        // Delete the entire file
        if (remove((fileName + ".txt").c_str()) == 0)
        {
            cout << "File \"" << fileName << ".txt\" has been deleted!" << endl;
            tableData.clear();
            columnHeaders.clear();
        }
        else
        {
            cout << "Error deleting file!" << endl;
        }
    }
    else
    {
        cout << "Invalid choice!" << endl;
    }

    cout << "Press any key to continue..." << endl;
    cin.ignore();
    cin.get();
}

// Count the number of rows in the table
void fileCount(const string &)
{
    cout << "The number of rows in the table: " << tableData.size() << endl;

    cout << "Press any key to continue..." << endl;
    cin.ignore();
    cin.get();
}

// Update specific fields in the table
void fileUpdate(const string &fileName)
{
    displayContent(fileName);

    string columnName, oldValue, newValue;
    cout << "Enter the column name to update: ";
    cin >> columnName;

    cout << "Enter the value to find: ";
    cin >> oldValue;

    cout << "Enter the new value: ";
    cin >> newValue;

    int columnIndex = -1;
    for (size_t i = 0; i < columnHeaders.size(); ++i)
    {
        if (columnHeaders[i] == columnName)
        {
            columnIndex = i;
            break;
        }
    }

    if (columnIndex == -1)
    {
        cout << "Column not found!" << endl;
        return;
    }

    for (auto &row : tableData)
    {
        if (row[columnIndex] == oldValue)
        {
            row[columnIndex] = newValue;
        }
    }

    ofstream outfile(fileName + ".txt");
    if (!outfile.is_open())
    {
        cout << "Error writing to file!" << endl;
        return;
    }

    for (const auto &col : columnHeaders)
    {
        outfile << left << setw(20) << col;
    }
    outfile << endl;

    for (const auto &row : tableData)
    {
        for (const auto &value : row)
        {
            outfile << left << setw(20) << value;
        }
        outfile << endl;
    }

    outfile.close();
    cout << "Update successful!" << endl;

    cout << "Press any key to continue..." << endl;
    cin.ignore();
    cin.get();
}

// Insert a new row into the table
void insertNewRow(const string &fileName)
{
    vector<string> newRow;
    if (columnHeaders.empty())
    {
        cout << "No table structure found! Create a table first." << endl;
        return;
    }

    newRow.push_back(to_string(tableData.size() + 1));
    cout << "Entering data for the new row:" << endl;

    for (size_t i = 1; i < columnHeaders.size(); ++i)
    {
        string value;
        cout << columnHeaders[i] << ": ";
        cin >> value;
        newRow.push_back(value);
    }

    tableData.push_back(newRow);

    ofstream outfile(fileName + ".txt", ios::app);
    if (!outfile.is_open())
    {
        cout << "Error writing to file!" << endl;
        return;
    }

    for (const auto &value : newRow)
    {
        outfile << left << setw(20) << value;
    }
    outfile << endl;

    outfile.close();
    cout << "New row added successfully!" << endl;

    cout << "Press any key to continue..." << endl;
    cin.ignore();
    cin.get();
}

// Menu for table operations
void secondOption(const string &fileName)
{
    while (true)
    {
        int option;
        cout << "\nChoose an option:" << endl;
        cout << "1. Delete a specific row" << endl;
        cout << "2. Update a specific field" << endl;
        cout << "3. Count the number of rows" << endl;
        cout << "4. Insert a new row" << endl;
        cout << "5. Exit" << endl;
        cout << "6. Back to main menu" << endl;

        cout << "Enter your choice: ";
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
            fileCount(fileName);
            break;
        case 4:
            insertNewRow(fileName);
            break;
        case 5:
            cout << "Exiting program..." << endl;
            exit(0);
        case 6:
            return;
        default:
            cout << "Invalid choice!" << endl;
        }
    }
}
