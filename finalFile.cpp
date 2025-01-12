#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <sstream>
using namespace std;

void fileCreate(const string &);
void tableCreate(const string &);
void displayContent(const string &);
void fileOpen(const string &);
void secondOption(const string &);
void fileUpdate(const string &);
void fileDelete(const string &);
void fileCount(const string &);
void insertNewRow(const string &);

int ctm_id;
string ctm_name, ctm_city, ctm_state,
    ctm_country, ctm_phone, ctm_email;

int main()
{
    int mode;
    string fileName;

    while (true)
    {
        cout << "1. Create a file" << endl;
        cout << "2. Open existing file" << endl;
        cout << "3. Exit" << endl;

        cin >> mode;

        switch (mode)
        {
        case 1:
            cout << "Please enter the file name: ";
            cin >> fileName;

            fileCreate(fileName);
            tableCreate(fileName);
            secondOption(fileName);
            break;

        case 2:
            cout << "Please enter the file name: ";
            cin >> fileName;
            fileOpen(fileName);
            secondOption(fileName);
            break;

        case 3: // Exit from this program
            cout << "Exiting......" << endl;
            return 0;

        default:
            cout << "Error, please input a valid number." << endl;
            break;
        }
    }

    return 0;
}

void fileCreate(const string &fileName)
{
    ofstream file(fileName + ".txt");

    if (file)
    {
        cout << "File created successfully." << endl;
        cout << "File name: " << fileName << ".txt" << endl;
        return;
    }

    else
    {
        cout << "Error creating file." << endl;
    }

    file.close();
}

void tableCreate(const string &fileName)
{

    ofstream inputfile;
    inputfile.open(fileName + ".mdb", ios::app);
    if (!inputfile.is_open())
    {
        cout << "Error!" << endl;
        return;
    }

    string tableName;
    cout << "Please enter the table name: ";
    cin >> tableName;

    inputfile << "Table: " << tableName << endl;
    inputfile << "----------------------------------------------------------------------------------------------------------------------------" << endl;
    inputfile << left << setw(5) << "ID"
              << setw(20) << "Name"
              << setw(20) << "City"
              << setw(20) << "State"
              << setw(20) << "Country"
              << setw(20) << "Phone"
              << setw(20) << "Email" << endl;

    int numCustomer;
    cout << "Please enter the number of customers: " << endl;
    cin >> numCustomer;

    for (int id = 1; id <= numCustomer; id++)
    {
        cout << "Please enter name " << id << ": " << endl;
        cin >> ctm_name;

        cout << "Please enter city " << id << ": " << endl;
        cin >> ctm_city;

        cout << "Please enter state " << id << ": " << endl;
        cin >> ctm_state;

        cout << "Please enter country " << id << ": " << endl;
        cin >> ctm_country;

        cout << "Please enter phone " << id << ": " << endl;
        cin >> ctm_phone;

        cout << "Please enter email " << id << ": " << endl;
        cin >> ctm_email;

        inputfile << left << setw(5) << id
                  << setw(20) << ctm_name
                  << setw(20) << ctm_city
                  << setw(20) << ctm_state
                  << setw(20) << ctm_country
                  << setw(20) << ctm_phone
                  << setw(20) << ctm_email << endl;
    }
    inputfile.close();
    cout << "Table " << tableName << " has been successfully created and data has been entered in " << fileName << ".mdb!" << endl;

    displayContent(fileName);

    cout << "Press any key to continue..." << endl;
    cin.ignore();
    cin.get();
}

void fileOpen(const string &fileName)
{

    ifstream infile;
    infile.open(fileName + ".mdb");
    if (!infile.is_open())
    {
        cout << "Error opening file for reading!" << endl;
        return;
    }

    string line;
    while (getline(infile, line))
    {
        cout << line << endl;
    }

    infile.close();

    cout << "Press any key to continue..." << endl;
    cin.ignore();
    cin.get();
}

void displayContent(const string &fileName)
{
    ifstream inputFile;
    inputFile.open(fileName + ".mdb");
    if (!inputFile.is_open())
    {
        cout << "Error opening file for reading!" << endl;
        return;
    }

    string line;
    while (getline(inputFile, line))
    {
        cout << line << endl;
    }

    inputFile.close();
}

void secondOption(const string &fileName)
{

    while (true)
    {

        int option;
        cout << "Choose an option:" << endl;
        cout << "1. Delete" << endl;
        cout << "2. Update" << endl;
        cout << "3. Count" << endl;
        cout << "4. Insert a new row" << endl;
        cout << "5. Exit" << endl;
        cout << "6. Back to previous page" << endl;
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
            cout << "Exiting..." << endl;
            exit(0);

        case 6:
            return;

        default:
            cout << "Invalid option, please try again!" << endl;
        }
    }
}

void fileUpdate(const string &fileName)
{

    ifstream infile;
    infile.open(fileName + ".mdb");
    ofstream tempfile;
    tempfile.open("temp.mdb");

    string line;
    int id;
    string field, newValue;

    if (!infile.is_open())
    {
        cout << "Error opening file!" << endl;
        return;
    }

    displayContent(fileName);

    cout << "Enter the ID you want to update: ";
    cin >> id;

    cout << "Enter the field you want to update (Name, City, State, Country, Phone, Email): ";
    cin >> field;

    cout << "Enter the new value: ";
    cin >> newValue;

    getline(infile, line);
    tempfile << line << endl;

    while (getline(infile, line))
    {
        int currentID;
        stringstream ss(line);
        ss >> currentID;

        if (currentID == id)
        {
            ss >> ctm_name >> ctm_city >> ctm_state >> ctm_country >> ctm_phone >> ctm_email;

            if (field == "Name" || field == "name")
                ctm_name = newValue;
            else if (field == "City" || field == "city")
                ctm_city = newValue;
            else if (field == "State" || field == "state")
                ctm_state = newValue;
            else if (field == "Country" || field == "country")
                ctm_country = newValue;
            else if (field == "Phone" || field == "phone")
                ctm_phone = newValue;
            else if (field == "Email" || field == "email")
                ctm_email = newValue;
            else
                cout << "Error, please try again!" << endl;

            tempfile << left << setw(5) << currentID
                     << setw(20) << ctm_name
                     << setw(20) << ctm_city
                     << setw(20) << ctm_state
                     << setw(20) << ctm_country
                     << setw(20) << ctm_phone
                     << setw(30) << ctm_email << endl;
        }
        else
        {
            tempfile << line << endl;
        }
    }

    infile.close();
    tempfile.close();

    remove((fileName + ".mdb").c_str());
    rename("temp.mdb", (fileName + ".mdb").c_str());

    cout << "Update successfully!" << endl;
    displayContent(fileName);

    cout << "Press any key to continue..." << endl;
    cin.ignore();
    cin.get();
}

void fileDelete(const string &fileName)
{
    string tempFile = "temp.mdb";
    ifstream infile(fileName + ".mdb");
    ofstream outfile(tempFile);

    if (!infile.is_open())
    {
        cout << "File does not exist or cannot be opened!" << endl;
        cout << "Press any key to continue..." << endl;
        cin.get();
        return;
    }

    // Display current file contents first
    displayContent(fileName);

    cout << "\n===== Delete Options =====" << endl;
    cout << "1. Delete the entire file" << endl;
    cout << "2. Delete a specific record by ID (then re-number)" << endl;
    cout << "3. Back to previous menu" << endl;
    cout << "Please choose an option (1-3): ";

    int choice;
    cin >> choice;

    if (choice == 1)
    {
        // 1. Delete the entire file
        infile.close();
        if (remove((fileName + ".mdb").c_str()) == 0)
        {
            cout << "File \"" << fileName << ".mdb\" deleted successfully!" << endl;
        }
        else
        {
            cout << "Error: file not found or could not be deleted!" << endl;
        }
    }
    else if (choice == 2)
    {
        // 2. Delete a specific record by ID and RE-NUMBER
        if (!outfile.is_open())
        {
            cout << "Error creating temporary file!" << endl;
            infile.close();
            return;
        }

        // Copy the first three lines (table name, separator, headers) directly
        string line;
        for (int i = 0; i < 3; ++i)
        {
            if (getline(infile, line))
            {
                outfile << line << endl;
            }
        }

        int deleteID;
        cout << "Enter the ID to delete: ";
        cin >> deleteID;

        bool found = false;
        int currentID = 1; // We'll re-number from 1 onwards

        while (getline(infile, line))
        {
            if (line.empty())
                continue;

            // Parse the old ID from the start of the line
            stringstream ss(line);
            int oldID;
            ss >> oldID;

            // If this record is the one to delete, skip it
            if (oldID == deleteID)
            {
                found = true;
                continue;
            }

            // Otherwise re-number the ID to 'currentID'
            // and preserve the other fields
            string restOfLine;
            // 'restOfLine' will contain everything after the old ID
            getline(ss, restOfLine);

            // Now reconstruct the line:
            // new ID + the rest of the fields
            ostringstream newLine;
            newLine << left << setw(5) << currentID << restOfLine;

            // Write to temp file, increment 'currentID'
            outfile << newLine.str() << endl;
            currentID++;
        }

        infile.close();
        outfile.close();

        if (found)
        {
            // Replace the original file with the temp file
            if (remove((fileName + ".mdb").c_str()) != 0)
            {
                cout << "Unable to delete the original file!" << endl;
                return;
            }
            if (rename(tempFile.c_str(), (fileName + ".mdb").c_str()) != 0)
            {
                cout << "Unable to rename the temporary file!" << endl;
                return;
            }

            // Now check if file has any data (besides the 3 header lines)
            ifstream checkFile(fileName + ".mdb");
            int lineCount = 0;
            while (getline(checkFile, line))
            {
                if (!line.empty())
                    lineCount++;
            }
            checkFile.close();

            // If only the 3 header lines remain, remove the file entirely
            if (lineCount <= 3)
            {
                remove((fileName + ".mdb").c_str());
                cout << "Record deleted. The file is now empty and has been removed." << endl;
            }
            else
            {
                cout << "Record with ID " << deleteID << " deleted and IDs re-numbered!" << endl;
                // Show updated file
                displayContent(fileName);
            }
        }
        else
        {
            // The requested ID wasn't found
            remove(tempFile.c_str());
            cout << "Record not found." << endl;
        }
    }
    else if (choice == 3)
    {
        // 3. Back to previous menu
        infile.close();
        return;
    }
    else
    {
        cout << "Invalid choice! Please select 1, 2, or 3." << endl;
    }

    cout << "Press any key to continue..." << endl;
    cin.get();
}

void fileCount(const string &fileName)
{
    ifstream infile;
    infile.open(fileName + ".mdb");
    string line;

    int count = 0;

    if (!infile.is_open())
    {
        cout << "Error opening file!" << endl;
        return;
    }

    for (int i = 0; i < 3; ++i)
    {
        if (!getline(infile, line))
        {

            break;
        }
    }
    while (getline(infile, line))
    {
        if (!line.empty())
        {
            count++;
        }
    }

    infile.close();

    cout << "The number of customers is: " << count << endl;

    cout << "Press any key to continue..." << endl;
    cin.ignore();
    cin.get();
}

void insertNewRow(const string &fileName)
{

    ifstream file;
    file.open(fileName + ".mdb", ios::in); // Open file in read mode
    if (!file.is_open())
    {
        cout << "Error opening file!" << endl;
        return;
    }

    // Find the next ID
    int id = 1;
    string line, lastLine;
    while (getline(file, line))
    {
        if (!line.empty())
        {
            lastLine = line;
            stringstream ss(lastLine);
            ss >> id;
        }
    }
    id++; // Increment ID for new row

    file.close(); // Close file after reading

    // Re-open file in append mode to add new row
    ofstream outFile(fileName + ".mdb", ios::out | ios::app);
    if (!outFile.is_open())
    {
        cout << "Error opening file!" << endl;
        return;
    }

    // Input new row data
    cout << "Please enter name for ID " << id << ": ";
    cin >> ctm_name;
    cout << "Please enter city for ID " << id << ": ";
    cin >> ctm_city;
    cout << "Please enter state for ID " << id << ": ";
    cin >> ctm_state;
    cout << "Please enter country for ID " << id << ": ";
    cin >> ctm_country;
    cout << "Please enter phone for ID " << id << ": ";
    cin >> ctm_phone;
    cout << "Please enter email for ID " << id << ": ";
    cin >> ctm_email;

    // Append new row to file
    outFile << left << setw(5) << id
            << setw(20) << ctm_name
            << setw(20) << ctm_city
            << setw(20) << ctm_state
            << setw(20) << ctm_country
            << setw(20) << ctm_phone
            << setw(30) << ctm_email << endl;

    outFile.close();
    cout << "New row added successfully!" << endl;

    displayContent(fileName);

    cout << "Press any key to continue..." << endl;
    cin.ignore();
    cin.get();
}
