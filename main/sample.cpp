//newFeature
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

void file_writing();
void file_reading();
void Delete();
void Update();
void Count();

void secondOption();
int ctm_id;
string ctm_name, ctm_city, ctm_state, 
       ctm_country, ctm_phone, ctm_email;

int main(){

int mode;

while (true){
    cout << "1. Write to the file" << endl;
    cout << "2. Read from a file" << endl;
    cout << "3. Exit" << endl;
    cin >> mode;

    switch(mode){
        case 1:
            file_writing();
            secondOption();
            break;

        case 2:
            file_reading();
            secondOption();
            break;
        
        case 3:
            cout << "Exiting......" << endl;
            return true;

        default:
            cout << "Error, please input a valid number." << endl;
            break;
    }

}
    
return 0;
}

void file_writing(){
    string fileName; 
    ofstream inputfile; 
 
    cout << "Please enter the file name: " << endl; 
    cin >> fileName; 
 
    inputfile.open(fileName + ".mdb"); 
    
    if (!inputfile) 
    { 
        cout << "Error: Could not create file!" << endl; 
        return; 
    } 
    else 
    { 
        cout << "File created successfully!" << endl; 
    } 

    int numCustomer; 
    cout << "Please enter the number of customers: " << endl; 
    cin >> numCustomer; 

    // Write the headers with width settings
    inputfile << left << setw(5) << "ID" << setw(20) << "Name" << setw(20) << "City" 
               << setw(20) << "State" << setw(20) << "Country" << setw(20) << "Phone" 
               << setw(30) << "Email" << endl;

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

        inputfile << left << setw(5) << id << setw(20) << ctm_name << setw(20) << ctm_city 
                   << setw(20) << ctm_state << setw(20) << ctm_country << setw(20) << ctm_phone 
                   << setw(30) << ctm_email << endl; 
    } 

    inputfile.close(); 
    cout << "Data written successfully!" << endl;

    // Wait for user to press any key 
    cout << "Press any key to continue..." << endl; 
    cin.ignore(); // Ignore the newline character left in the input buffer 
    cin.get(); // Wait for user to press any key
}

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
    // Wait for user to press any key 
    cout << "Press any key to continue..." << endl; 
    cin.ignore(); // Ignore the newline character left in the input buffer 
    cin.get(); // Wait for user to press any key

};

void secondOption() 
{ 
    while(true){
    cout 
        << "1. Delete" << endl 
        << "2. Update" << endl 
        << "3. Count" << endl 
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
            cout << "Exiting......" << endl;
            return ;
        default:
            cout << "Invalid option, please try again!" << endl;

        }
    }
}

void Update(){
    cout << "Updated!" << endl;
    
    // Wait for user to press any key 
    cout << "Press any key to continue..." << endl; 
    cin.ignore(); // Ignore the newline character left in the input buffer 
    cin.get(); // Wait for user to press any key
};

void Delete(){
    string fileName, tempFile = "temp.mdb";
    ifstream infile;
    ofstream outfile;

    cout << "Please enter the file name: ";
    cin >> fileName;

    // Display the content of the file
    infile.open(fileName + ".mdb");
    if (!infile) {
        cout << "Error: File not found or could not be opened!" << endl;
        return;
    }

    cout << "Contents of the file:" << endl;
    string line;
    while (getline(infile, line)) {
        cout << line << endl;
    }
    infile.close();

    int choice;
    cout << "\nChoose an option:" << endl;
    cout << "1. Delete the entire file" << endl;
    cout << "2. Delete a specific record by ID" << endl;
    cin >> choice;

    if (choice == 1) {     //delete entire file
        if (remove((fileName + ".mdb").c_str()) == 0) {
            cout << "File " << fileName << ".mdb deleted successfully!" << endl;
        } 
        else {
            cout << "Error: File not found or could not be deleted!" << endl;
        }
    } 
    else if (choice == 2){     //delete specific id
        infile.open(fileName + ".mdb");
        outfile.open(tempFile);

        if (!infile || !outfile){
            cout << "Error opening file!" << endl;
            return;
        }

        int deleteID;
        cout << "Enter the ID of the record to delete: ";
        cin >> deleteID;

        bool found = false;
        int currentID = 1;
        bool headerWritten = false;
        
        while (getline(infile, line)) {
            if (!headerWritten){
                outfile << line << endl;
                headerWritten = true;
                continue;
            }

            int recordID = stoi(line.substr(0, line.find(' ')));

            if (recordID == deleteID){
                found = true;
                continue;
            }

            line.replace(0, line.find(' '), to_string(currentID++));
            outfile << line << endl;
        }

        infile.close();
        outfile.close();

        if (found){
            remove((fileName + ".mdb").c_str());
            rename(tempFile.c_str(), (fileName + ".mdb").c_str());

            if (currentID == 1){
                remove((fileName + ".mdb").c_str());
                cout << "Record deleted, and file is now empty. File removed." << endl;
            }
            else {
                cout << "Record with ID " << deleteID << " deleted successfully!" << endl;
            }
        }
        else {
            remove(tempFile.c_str());
            cout << "Record not found" << endl;
        }
    }
    else {
        cout << "Invalid choice! Please select 1 or 2." << endl;
    }
    
    // Wait for user to press any key 
    cout << "Press any key to continue..." << endl; 
    cin.ignore(); // Ignore the newline character left in the input buffer 
    cin.get(); // Wait for user to press any key
};

void Count(){
    cout << "counted!" << endl;
    
    // Wait for user to press any key 
    cout << "Press any key to continue..." << endl; 
    cin.ignore(); // Ignore the newline character left in the input buffer 
    cin.get(); // Wait for user to press any key
};
