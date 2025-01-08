#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

void file_writing();
void file_reading();
void Option();
void Update();
void Delete();
void Count();
void tableCreate();

int id;
string name, city, state, country, phone, email;

int main(){

string mode;

cout << "Writing / Reading ?" << endl;
cin >> mode;
    
    if (mode == "Writing")
    {
        //writing mode
        file_writing();
        file_reading();
    } 
    else if ( mode == "Reading")
    {
        //reading mode
        file_reading();
    } 
    else 
    {
        cout << "error!";
        return 0;
    }

   

while(true)
{       cout << endl
        << "1. Delete" << endl
        << "2. Update" << endl
        << "3. Count"  << endl 
        << "4. Exit" << endl;
        int option;
        cin >> option;

        switch(option) 
     {
        case 1:
            Delete();
            break;

        case 2:
            Update();
            break;

        case 3:
            Count();
            break;
        
        case 4:
            cout << "Exiting..." << endl;
            return 0;
        
        default:
            cout << "Invalid option. Try again!" << endl;
            break;
     }
}


return 0;
}

void Update(){
    cout << "Updated!" << endl;
};//
void Delete(){
    cout << "Deleted!" << endl;
};
void Count(){
    cout << "counted!" << endl;
};
void file_writing(){
    
    string fileName;
    ofstream inputfile;

    cout << "please enter the file name : " << endl;
    cin >> fileName;
    inputfile.open(fileName +"mdb");

    int numCustomer;
    cout << "please enter the number of customer";
    cin >> numCustomer;

        for(int id = 1; id <= numCustomer; id++){
            

            cout << "please enter name" << id << endl;
            cin >> name;
            

            inputfile << id << ", " << name << endl;
        };

    inputfile.close();

};
void file_reading(){
    
    string fileName;
    ifstream infile;

    cout << "please enter the file name : " << endl;
    cin >> fileName;

    infile.open(fileName + ".mdb");
    string line;

    if (infile.is_open()) {
        // Read each line from the file and store it in the
        // 'line' variable.
        while (getline(infile, line)) {
            cout << line << endl;
        };
        }else {
            cout << "Error" << endl;
        }
        
    infile.close();

};
