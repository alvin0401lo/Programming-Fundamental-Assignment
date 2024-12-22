#include <iostream>
#include <fstream>
#include <string>
using namespace std;

void Update();
void Delete();
void Count();
void file_writing();
void file_reading();

int id;
string name, city, state, couwntry, phone, email;

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
    }

cout << endl
     << "1. Delete" << endl
     << "2. Update" << endl
     << "3. Count"  << endl; 

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
        
        default:
            cout << "error";

     }


return 0;
}

void Update(){
    cout << "Update";
};//
void Delete(){
    cout << "Delete";
};
void Count(){
    cout << "count";
};
void file_writing(){
    ofstream inputfile;
    inputfile.open("outputfile1.txt");

    int maxcustomer;
    cout << "please enter maxcustomer";
    cin >> maxcustomer;

        for(int id = 1; id <= maxcustomer; id++){
            

            cout << "please enter name" << id << endl;
            cin >> name;
            

            inputfile << id << ", " << name << endl;
        };

    inputfile.close();

};
void file_reading(){

    ifstream infile;
    infile.open("outputfile1.txt");
    string line;

    if (infile.is_open()) {
        // Read each line from the file and store it in the
        // 'line' variable.
        while (getline(infile, line)) {
            cout << line << endl;
        };
        }
        
    infile.close();

};

