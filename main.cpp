#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <string>
using namespace std;

void createDatabase(string &db);

int main()
{
    string db;
    createDatabase(db);

    return 0;
}

void createDatabase(string &db)
{
    cout << "Do you want to create new database to store data? (y/n)" << endl;
    char choice;
    cin >> choice;

    if (choice == 'y')
    {
        cout << "Enter database name (exp: name.mdb): ";
        cin >> db;

        ofstream file(db);

        if (file)
        {
            cout << "Database created successfully." << endl;
            cout << "Database name: " << db << endl;
            return;
        }

        else
        {
            cout << "Error creating database." << endl;
        }

        file.close();
    }

    else if (choice == 'n')
    {
        cout << "Database not created. Program ended." << endl;
        exit(0);
    }

    else
    {
        cout << "Invalid choice. Please enter y or n." << endl;
    }
}