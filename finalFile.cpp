#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>

using namespace std;

// Global variables to store table data
vector<vector<string>> tableData; // Stores each row of the table as a vector of strings
vector<string> columnHeaders;     // Stores the column headers of the table

// Function declarations
void createFile(const string &);
void createTable(const string &);
void displayContent(const string &);
void openFile(const string &);
void secondOption(const string &);
void fileUpdate(const string &);
void fileDelete(const string &);
void fileCount();
void insertNewRow(const string &);

int main()
{
    int mode;
    string fileName;

    // Main menu loop
    while (true)
    {
        // Display main menu options
        cout << "1. Create a file"      << endl;
        cout << "2. Open existing file" << endl;
        cout << "3. Exit"               << endl;
        cout << "Enter your choice: "   << endl;
        cin >> mode;

        switch (mode)
        {
        case 1:
            // Option 1: Create a new file
            cout << "Please enter the file name (without extension .txt): ";
            cin >> fileName;
            createFile(fileName);    // Create the file
            createTable(fileName);   // Create the table within the file
            secondOption(fileName);  // Navigate to the second menu for further operations
            break;

        case 2:
            // Option 2: Open an existing file
            cout << "Please enter the file name (without extension .txt): " << endl;
            cin >> fileName;
            openFile(fileName);      // Open and load the existing file
            secondOption(fileName);  // Navigate to the secondary menu for further operations
            break;

        case 3:
            // Option 3: Exit the program
            cout << "Exiting..." << endl;
            return 0;

        default:
            // Handle invalid menu choices
            cout << "Invalid choice, please try again!" << endl;
        }
    }

    return 0;
}

// Check if a file exists and create a new file
void createFile(const string &fileName)
{
    // Check if the file already exists by attempting to open it
    ifstream existingFile;
    existingFile.open(fileName + ".txt");
    if (existingFile.is_open())
    {
        cout << "Error: File \"" << fileName << ".txt\" already exists" << endl;
        existingFile.close();
        return;
    }

    // Create and open a new file for writing
    ofstream outputFile;
    outputFile.open(fileName + ".txt");
    if (!outputFile.is_open())
    {
        cout << "Error opening file for writing!" << endl;
        return;
    }

    // Notify the user that the file has been created successfully
    cout << "File \"" << fileName << ".txt\" has been successfully created!" << endl;
    outputFile.close();

    // Pause execution until the user presses a key
    cout << "Press any key to continue..." << endl;
    cin.ignore(); 
    cin.get();  
}

// Create a table and save data to the file
void createTable(const string &fileName)
{
    // Open the file for writing
    ofstream inputfile;
    inputfile.open(fileName + ".txt");

    //Check if file cannot open
    if (!inputfile.is_open())
    {
        cout << "Error! Cannot open file. " << endl;
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
    cout << "Please enter the column names (Example: Name, City, Email):" << endl;
    for (int i = 0; i < numColumns; ++i)
    {
        string columnName;
        cout << "Column " << i + 1 << ": ";
        cin >> columnName;
        columnHeaders.push_back(columnName);
    }

    // Write table name to the file
    inputfile << "Table: " << tableName << endl;
    for (const auto &col : columnHeaders)
    {
        inputfile << left << setw(20) << col; // Align columns with fixed width
    }
    inputfile << endl;

    int numRows;
    cout << "How many rows of data do you want to add? ";
    cin >> numRows;

    tableData.clear(); // Clear any existing data
    for (int i = 0; i < numRows; ++i)
    {
        vector<string> rowData;
        rowData.push_back(to_string(i + 1)); // Automatically generate ID column
        cout << "Entering data for row " << i + 1 << ":" << endl;

        // Input data for each user-defined column
        for (size_t j = 1; j < columnHeaders.size(); ++j)
        {
            string value;
            cout << columnHeaders[j] << ": ";
            cin >> value;
            rowData.push_back(value);
        }

        tableData.push_back(rowData); // Add the row to the table data

        // Write each row to the file
        for (const auto &value : rowData)
        {
            inputfile << left << setw(20) << value;
        }
        inputfile << endl;
    }

    inputfile.close(); // Close the file after writing
    cout << "Table \"" << tableName << "\" created and saved in \"" << fileName << ".txt\"!" << endl;

    displayContent(fileName); // Display the newly created table content

    // Pause execution until the user presses a key
    cout << "Press any key to continue..." << endl;
    cin.ignore(); 
    cin.get();  
}

// Open an existing file and load its content
void openFile(const string &fileName)
{
    string mdbFileName;

    //Check if the .txt file already exists to prevent overwriting
    ifstream checkFile;
    checkFile.open(fileName + ".txt");
    if (checkFile.is_open())
    {
        cout << "Error: \"" << fileName << ".txt\" already exists! " << endl;
        checkFile.close();
        return;
    }
    checkFile.close();

    // Prompt the user to enter the .mdb file name (without extension)
    cout << "Please enter the mdb file name (without extension .mdb): ";
    cin >> mdbFileName;

    // Attempt to open the .mdb file for reading
    ifstream mdbFile;
    mdbFile.open(mdbFileName + ".mdb");
    if (!mdbFile.is_open())
    {
        cout << "Error: Unable to open \"" << mdbFileName << ".mdb\" for reading!" << endl;
        return;
    }

    // Create and open the new .txt file for writing
    ofstream txtFile;
    txtFile.open(fileName + ".txt");
    if (!txtFile.is_open())
    {
        cout << "Error: Unable to create \"" << fileName << ".txt\" for writing!" << endl;
        mdbFile.close();
        return;
    }

    //Ask for user to enter table name
    string tableName;
    cout << "Please enter the table name" << endl;
    cin >> tableName;
    txtFile << "Table: " << tableName << endl;

    // Clear existing data in global vectors
    tableData.clear();
    columnHeaders.clear();

    string line;
    int lineCount = 0;

    // Read the .mdb file line by line
    while (getline(mdbFile, line))
    {
        lineCount++;

        if (lineCount == 1)
        {
            stringstream ss(line);
            string col;
            while (ss >> col)
            {
                columnHeaders.push_back(col);
                txtFile << left << setw(20) << col;
            }
            txtFile << endl;
            continue;
        }

        stringstream ss(line);
        vector<string> rowData;
        string value;

        // Read each value in the row
        while (ss >> value)
        {
            rowData.push_back(value);
            txtFile << left << setw(20) << value;
        }
        txtFile << endl;

        // Add the row to the global tableData vector
        tableData.push_back(rowData);
    }

    // Close both files
    mdbFile.close();
    txtFile.close();

    // Display the content of the newly created .txt file
    displayContent(fileName);
}
void secondOption(const string &fileName)
{
    int option;
    while (true)
    {        
        cout << "Choose an option:"           << endl;
        cout << "1. Delete a specific row"    << endl;
        cout << "2. Update a specific field"  << endl;
        cout << "3. Count the number of rows" << endl;
        cout << "4. Insert a new row"         << endl;
        cout << "5. Back to main menu"        << endl;
        cout << "6. Exit"                     << endl;

        cout << "Enter your choice: " << endl;
        cin >> option;

        switch (option)
        {
        case 1:
            // Option 1: Delete a specific row
            fileDelete(fileName);
            break;
        case 2:
            // Option 2: Update a specific field in the table
            fileUpdate(fileName);
            break;
        case 3:
            // Option 3: Count the number of rows in the table
            fileCount();
            break;
        case 4:
            // Option 4: Insert a new row into the table
            insertNewRow(fileName);
            break;
        case 5:
            // Option 6: Return to the main menu
            return;
        case 6:
            // Option 5: Exit the entire program
            cout << "Exiting program..." << endl;
            exit(0);            

        default:
            // Handle invalid menu choices
            cout << "Invalid choice!" << endl;
        }
    }
}

// Display table content 
void displayContent(const string &fileName)
{
    // Display column headers with fixed width
    for (const auto &col : columnHeaders)
    {
        cout << left << setw(20) << col;
    }
    cout << endl;
    
    // Display a separator line
    cout << string(100, '-') << endl;

    // Display each row of the table
    for (const auto &row : tableData)
    {
        for (const auto &value : row)
        {
            cout << left << setw(20) << value;
        }
        cout << endl;
    }
}

// Delete a specific row
void deleteRow(const string &fileName)
{
    // Display the current content of the table
    displayContent(fileName);

    // Prompt the user to enter the row number they wish to delete
    int deleteRow;
    cout << "\nEnter the row number to delete: ";
    cin >> deleteRow;

    // Validate the entered row number
    if (deleteRow <= 0 || deleteRow > static_cast<int>(tableData.size()))
    {
        cout << "Invalid row number!" << endl;
        return;
    }

    // Remove the specified row from the table data
    tableData.erase(tableData.begin() + deleteRow - 1);

    // Update the ID column to maintain consistency after deletion
    for (size_t i = 0; i < tableData.size(); ++i)
    {
        tableData[i][0] = to_string(i + 1);
    }

    // Open the file for writing to update it with the new table data
    ofstream outfile(fileName + ".txt");
    if (!outfile.is_open())
    {
        cout << "Error writing to file!" << endl;
        return;
    }

    // Write the column headers to the file
    for (const auto &col : columnHeaders)
    {
        outfile << left << setw(20) << col;
    }
    outfile << endl;

    // Write each row of the updated table data to the file
    for (const auto &row : tableData)
    {
        for (const auto &value : row)
        {
            outfile << left << setw(20) << value;
        }
        outfile << endl;
    }

    // Close the file after writing
    outfile.close();
    cout << "Row " << deleteRow << " deleted successfully!" << endl;

    // Pause the program until the user presses a key to continue
    cout << "Press any key to continue..." << endl;
    cin.ignore(); // Clear the input buffer
    cin.get();    // Wait for user input
}

// Count the number of rows in the table
void countRow()
{
    cout << "The number of rows in the table: " << tableData.size() << endl;

    // Pause execution until the user presses a key
    cout << "Press any key to continue..." << endl;
    cin.ignore();
    cin.get();  
}

// Update specific data in the table
void fileUpdate(const string &fileName)
{
    displayContent(fileName); // Show current table content

    string columnName, oldValue, newValue;
    cout << "Enter the column name to update: ";
    cin >> columnName;

    cout << "Enter the value to find: ";
    cin >> oldValue;

    cout << "Enter the new value: ";
    cin >> newValue;

    int columnIndex = -1;
    // Find the index of the specified column
    for (size_t i = 0; i < columnHeaders.size(); ++i)
    {
        if (columnHeaders[i] == columnName)
        {
            columnIndex = static_cast<int>(i);
            break;
        }
    }

    if (columnIndex == -1)
    {
        // Handle case where the column is not found
        cout << "Column not found!" << endl;
        return;
    }

    // Iterate through each row and update the specified field if it matches the old value
    for (auto &row : tableData)
    {
        if (row[columnIndex] == oldValue)
        {
            row[columnIndex] = newValue;
        }
    }

    // Write the updated table data back to the file
    ofstream outfile(fileName + ".txt");
    if (!outfile.is_open())
    {
        cout << "Error writing to file!" << endl;
        return;
    }

    // Write column headers
    for (const auto &col : columnHeaders)
    {
        outfile << left << setw(20) << col;
    }
    outfile << endl;

    // Write each row
    for (const auto &row : tableData)
    {
        for (const auto &value : row)
        {
            outfile << left << setw(20) << value;
        }
        outfile << endl;
    }

    outfile.close(); // Close the file after writing
    cout << "Update successful!" << endl;

    // Pause execution until the user presses a key
    cout << "Press any key to continue..." << endl;
    cin.ignore(); // Clear the input buffer
    cin.get();    // Wait for user input
}

// Insert a new row into the table
void insertNewRow(const string &fileName)
{
    vector<string> newRow;

    // Ensure that the table structure exists
    if (columnHeaders.empty())
    {
        cout << "No table structure found! Create a table first." << endl;
        return;
    }

    // Automatically generate the ID for the new row
    newRow.push_back(to_string(tableData.size() + 1));
    cout << "Entering data for the new row:" << endl;

    // Prompt the user to enter data for each column (excluding ID)
    for (size_t i = 1; i < columnHeaders.size(); ++i)
    {
        string value;
        cout << columnHeaders[i] << ": ";
        cin >> value;
        newRow.push_back(value);
    }

    tableData.push_back(newRow); // Add the new row to the table data

    // Open the file in append mode to add the new row
    ofstream outfile(fileName + ".txt", ios::app);
    if (!outfile.is_open())
    {
        cout << "Error writing to file!" << endl;
        return;
    }

    // Write the new row to the file with fixed width formatting
    for (const auto &value : newRow)
    {
        outfile << left << setw(20) << value;
    }
    outfile << endl;

    outfile.close(); // Close the file after writing
    cout << "New row added successfully!" << endl;

    // Pause execution until the user presses a key
    cout << "Press any key to continue..." << endl;
    cin.ignore(); // Clear the input buffer
    cin.get();    // Wait for user input
}

// Menu for table operations
