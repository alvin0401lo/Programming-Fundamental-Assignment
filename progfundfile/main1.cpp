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
void fileCount(const string &);
void insertNewRow(const string &);

// Helper function: Write current data (columnHeaders, tableData) to fileName.txt in CSV format
void writeCsvToTxt(const string &);

int main()
{
    while (true)
    {
        cout << "\n1. Create a file (.txt)" << endl;
        cout << "2. Open existing file (.mdb)" << endl;
        cout << "3. Exit" << endl;

        cout << "Enter your choice: ";
        int mode;
        cin >> mode;

        if (mode == 1)
        {
            // Create a new .txt file
            cout << "Please enter the file name (no extension needed): ";
            string fileName;
            cin >> fileName;

            fileCreate(fileName);
            tableCreate(fileName);
            secondOption(fileName);
        }
        else if (mode == 2)
        {
            // Open an existing .mdb file
            cout << "Please enter the file name (no extension needed): ";
            string fileName;
            cin >> fileName;

            fileOpen(fileName);
            secondOption(fileName);
        }
        else if (mode == 3)
        {
            cout << "Exiting program..." << endl;
            return 0;
        }
        else
        {
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
    cout << "\nTable Content (from memory) for \"" << fileName << ".txt\":\n";

    // Print column headers
    for (size_t i = 0; i < columnHeaders.size(); i++)
    {
        cout << columnHeaders[i];
        if (i < columnHeaders.size() - 1) cout << ",";
    }
    cout << endl;

    // Print row data
    for (auto & row : tableData)
    {
        for (size_t j = 0; j < row.size(); j++)
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
        cout << "\nChoose an option:" << endl;
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
            fileCount(fileName);
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
    for (size_t i = 0; i < columnHeaders.size(); i++)
    {
        outfile << columnHeaders[i];
        if (i < columnHeaders.size() - 1)
            outfile << ",";
    }
    outfile << "\n";

    // Rows
    for (auto & row : tableData)
    {
        for (size_t j = 0; j < row.size(); j++)
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


// Allows updating all cells in a certain column that match oldValue
void fileUpdate(const string &fileName)
{
    displayContent(fileName);

    cout << "Enter the column name to update: ";
    string columnName;
    cin >> columnName;

    cout << "Enter the value to find: ";
    string oldValue;
    cin >> oldValue;

    cout << "Enter the new value: ";
    string newValue;
    cin >> newValue;

    // Find the column index
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

    // Update rows
    for (auto & row : tableData)
    {
        if (row[columnIndex] == oldValue)
        {
            row[columnIndex] = newValue;
        }
    }

    // Write updated data back to file
    writeCsvToTxt(fileName);
    cout << "Update successful!" << endl;
}

// Prints the current number of rows in the table
void fileCount(const string &/*fileName*/)
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
    for (size_t i = 0; i < columnHeaders.size(); i++)
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
