#include <iostream>
#include<fstream>
#include<sstream>
#include<string>

#include "LinkedList.hpp" // 引入你的头文件

using namespace std;

string cleanID(string input){
    string output="";
    for(char c:input){
        if(isalnum(c)){
            output+=c;
        }
    }
    return output;
}

void loadDataFromCSV(string filename,FlightLinkedList&list){
    ifstream file(filename);

    if(!file.is_open()){
        cout<<"Error: Cannot open file "<<filename<<"'"<<endl;
        cout<<"Please check if the file is in the same folder as your.exe!!!"<<endl;
        return;
    }

    string line;
    if(file.good()){
        getline(file, line);
    }

    int count=0;

    while(getline(file,line)){
        if (line.empty()) continue;

        stringstream ss(line);
        Passenger p;

        string tempID;
        getline(ss,tempID,',');        
        p.passengerID=cleanID(tempID);

        getline(ss,p.name,',');
        getline(ss,p.seatRow,',');
        getline(ss,p.seatColumn,',');
        getline(ss,p.pClass);


        list.addPassenger(p);
        count++;
    }
    file.close();
    cout<<">Successfully loaded "<<count<<" Passenger from file."<<endl;
}

int main() {
    FlightLinkedList myFlight;

    cout<<"1.Loading data from CSV"<<endl;
    loadDataFromCSV("flight_passenger_data.csv",myFlight);

    // cout << "\n=== DEBUG: Check String Lengths ===" << endl;
    myFlight.displayManifest();


    // 2. INPUT  data manually
    /*cout<<"\n2. Adding a new Passenger (Manual Input)"<<endl;

    Passenger p;

    cout<<"Enter Passenger ID:";
    cin>>p.passengerID;

    cout<<"Enter Name (No Spaces):";
    cin>>p.name;

    cout<<"Enter Seat Row (e.g,10):";
    cin>>p.seatRow;

    cout<<"Enter Seat Column (e.g.,A):";
    cin>>p.seatColumn;
    
    cout<<"Enter Class (e.g, First, Business, Economy): )";
    cin>>p.pClass;

    cout<<"\nAttempting to add passenger"<<endl;
    bool result=myFlight.addPassenger(p);
    if(result){
        cout<<"Passenger: "<<p.name<<" added successfully.\n"<<endl;
    }else{
        cout<<"Failed to add passenger: "<<p.name<<" Seat Taken or ID exists.\n"<<endl;
    }*/

    cout<<"\n3. Remove a Passenger"<<endl;
    string removeID;
    cout<<"Enter the Passenger ID to remove (e.g.,100000): ";
    cin>>removeID;

    cout<<"Processing removal of Passenger ID: "<<removeID<<endl;
    bool isRemoved=myFlight.removePassenger(removeID);



    return 0;

}