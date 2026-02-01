#include <iostream>
#include<fstream>
#include<sstream>
#include<string>


#include "LinkedList.hpp" // 引入你的头文件

using namespace std;

void loadDataFromCSV(string filename,FlightLinkedList&list){
    ifstream file(filename);

    if(!file.is_open()){
        cout<<"Error: Cannot open file"<<filename<<"'"<<endl;
        cout<<"Please check if the file is in the same folder as your.ext!!!"<<endl;
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
        string segment;
        Passenger p;

        getline(ss,p.passengerID,',');
        getline(ss,p.name,',');
        getline(ss,p.seatRow,',');
        getline(ss,p.seatColumn,',');
        getline(ss,p.pClass);


        list.addPassenger(p);
        count++;
    }
    file.close();
    cout<<"-->Successfully loaded "<<count<<" Passenger from file."<<endl;
}

int main() {
    FlightLinkedList myFlight;

    cout<<"1.Loading data from CSV"<<endl;
    loadDataFromCSV("flight_passenger_data.csv.csv",myFlight);

    // cout<<"\n2. Displaying Manifest (Check if data is there)"<<endl;
    // myFlight.displayManifest();

    // 3. TEST: INPUT  data manually
    cout<<"\n2. Adding a new Passenger (Manual Input)"<<endl;

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

    cout<<"\n Attempting to add passenger"<<endl;
    bool result=myFlight.addPassenger(p);
    if(result){
        cout<<"Passenger: "<<p.name<<" added successfully."<<endl;
    }else{
        cout<<"Failed to add passenger: "<<p.name<<"Seat Taken or ID exists"<<endl;
    }

    return 0;

}