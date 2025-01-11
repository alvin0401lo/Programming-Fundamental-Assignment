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

void Update() {

    string fileName;
    cout << "Please enter the file name: " << endl;
    cin >> fileName;

    ifstream infile(fileName + ".mdb");
    ofstream tempfile("temp.mdb");

    string line;
    int id;
    string field, newValue;

    if (!infile.is_open()) 
    {
        cout << "Error opening file!" << endl;
        return;
    }

    displayFileContent(fileName + ".mdb");

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

        if (currentID == id) {

            ss >> ctm_name >> ctm_city >> ctm_state >> ctm_country >> ctm_phone >> ctm_email;

            if (field == "Name") ctm_city = newValue;
            else if (field == "name") ctm_name = newValue;

            else if (field == "City") ctm_city = newValue;
            else if (field == "city") ctm_city = newValue;

            else if (field == "State") ctm_state = newValue;
            else if (field == "state") ctm_state = newValue;

            else if (field == "Country") ctm_country = newValue;
            else if (field == "country") ctm_country = newValue;

            else if (field == "Phone") ctm_phone = newValue;
            else if (field == "phone") ctm_phone = newValue;

            else if (field == "Email") ctm_email = newValue;
            else if (field == "email") ctm_email = newValue;

            else 
                cout << "Error, please try again!" << endl;

            tempfile << left << setw(5) << currentID << setw(20) << ctm_name << setw(20) << ctm_city
                     << setw(20) << ctm_state << setw(20) << ctm_country << setw(20) << ctm_phone
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
    displayFileContent(fileName + ".mdb");

    cout << "Press any key to continue..." << endl;
    cin.ignore();
    cin.get();
}

void Delete(){
    cout << "Deleted!" << endl;
    
    // Wait for user to press any key 
    cout << "Press any key to continue..." << endl; 
    cin.ignore(); // Ignore the newline character left in the input buffer 
    cin.get(); // Wait for user to press any key
};

void Count(){
    
    string fileName;
    cout << "Please enter the file name: " << endl;
    cin >> fileName;

    ifstream infile(fileName + ".mdb");
    string line;

    int count = 0;

    if (!infile.is_open()) 
    {
        cout << "Error opening file!" << endl;
        return;
    }

    getline(infile, line); // Skip header line

    while (getline(infile, line)) 
    {
        if (!line.empty()) 
        { 
            count++;
        }
    }

    infile.close();

    cout << "The number of customer is: " << count << endl;

    cout << "Press any key to continue..." << endl;
    cin.ignore();
    cin.get();
};
