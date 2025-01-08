#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <string>
using namespace std;

void createDatabase(string db);

int main()
{

    return 0;
}

void createDatabase(string db)
{
    cout << "Do you want to create new database to store data? (y/n)" << endl;
    char choice;
    cin >> choice;

    if (choice == 'y')
    {
        cout << "Enter database name: ";
        cin >> db;
        cout << "Database created successfully." << endl;
        cout << "Database name: " << db << endl;
    }

    else if (choice == 'n')
    {
        cout << "Database not created. Do you want to open existing database? (y/n)" << endl;
        char choice2;
        cin >> choice2;

        if (choice2 == 'y')
        {
            cout << "Enter database name: ";
            cin >> db;
            cout << "Database opened successfully." << endl;
            cout << "Database name: " << db << endl;
        }

        else if (choice2 == 'n')
        {
            cout << "Database not opened." << endl;
        }

        else if (choice2 != 'y' || 'n')
        {
            cout << "Invalid choice. Please enter y or n." << endl;
        }

        else
        {
            cout << "Database not opened. Try check the database name." << endl;
        }
    }

    else if (choice != 'y' || 'n')
    {
        cout << "Invalid choice. Please enter y or n." << endl;
    }
}