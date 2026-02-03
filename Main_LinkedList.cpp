#include <iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<limits>

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

/*int main() {
    FlightLinkedList myFlight;

    cout<<"1.Loading data from CSV"<<endl;
    loadDataFromCSV("flight_passenger_data.csv",myFlight);

    // cout << "\n=== DEBUG: Check String Lengths ===" << endl;
    myFlight.displayManifest();


    // 2. INPUT  data manually
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

    cout<<"\nAttempting to add passenger"<<endl;
    bool result=myFlight.addPassenger(p);
    if(result){
        cout<<"Passenger: "<<p.name<<" added successfully.\n"<<endl;
    }else{
        cout<<"Failed to add passenger: "<<p.name<<" Seat Taken or ID exists.\n"<<endl;
    }

    cout<<"\n3. Remove a Passenger"<<endl;
    string removeID;
    cout<<"Enter the Passenger ID to remove (e.g.,100000): ";
    cin>>removeID;

    cout<<"Processing removal of Passenger ID: "<<removeID<<endl;
    bool isRemoved=myFlight.removePassenger(removeID);


    cout<<"\n4. Display Seating Chart"<<endl;
    myFlight.displaySeatingChart();

    cout<<"\n5. Search for a Passenger"<<endl;
    string searchID;
    cout<<"Enter the Passenger ID you want to search (e.g.,100000): )"<<endl;
    cin>>searchID;
    searchID=cleanID(searchID);

    Passenger*p=myFlight.searchPassenger(searchID);

    if(p!=nullptr){
        cout<<"Passenger Found!"<<endl;
        cout<<"ID:"<<p->passengerID<<endl;
        cout<<"Name:"<<p->name<<endl;
        cout<<"Seat:"<<p->seatRow<<p->seatColumn<<endl;
        cout<<"Class:"<<p->pClass<<endl;
    }else{
        cout<<"Passenger ID "<<searchID<<" Not Found!!"<<endl;
    }


    return 0;

}*/


int main(){
    FlightLinkedList myFlight;
    int choice=0;
    string inputID;

    loadDataFromCSV("flight_passenger_data.csv",myFlight);

    while(choice!=6){
        cout<<"\n===== Flight Reservation System Menu ====="<<endl;
        cout<<"1. Display Passenger Manifest (List)"<<endl;
        cout<<"2. Display Seating Chart"<<endl;
        cout<<"3. Search Passenger"<<endl;  
        cout<<"4. Add Passenger"<<endl;
        cout<<"5. Remove Passenger"<<endl;
        cout<<"6. Exit"<<endl;
        cout<<"Please enter your choice (1-6): ";

        if(!(cin>>choice)){
            cout<<"\nError: Invalid Input!! Please only enter numbers 1-6."<<endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            continue;
        }

        switch(choice){
            case 1:
                myFlight.displayManifest();
                break;
            case 2:
                myFlight.displaySeatingChart();
                break;
            case 3:
                { 
                    string searchID;
                    cout<<"Enter the Passenger ID you want to search (e.g.,100000):";
                    cin>>searchID;
                    searchID=cleanID(searchID);
                
                    Passenger*p=myFlight.searchPassenger(searchID);

                    if(p!=nullptr){
                        cout<<"Passenger Found!"<<endl;
                        cout<<"ID:"<<p->passengerID<<endl;
                        cout<<"Name:"<<p->name<<endl;
                        cout<<"Seat:"<<p->seatRow<<p->seatColumn<<endl;
                        cout<<"Class:"<<p->pClass<<endl;
                    }else{
                        cout<<"Passenger ID "<<searchID<<" Not Found!!"<<endl;
                    }
                }
                break;
            case 4:        
                {
                    cout<<"\n4. Adding a new Passenger (Manual Input)"<<endl;

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
                    }
                }
                break;
            case 5:
                {
                    cout<<"\n3. Remove a Passenger"<<endl;
                    string removeID;
                    cout<<"Enter the Passenger ID to remove (e.g.,100000): ";
                    cin>>removeID;

                    cout<<"Processing removal of Passenger ID: "<<removeID<<endl;
                    bool isRemoved=myFlight.removePassenger(removeID);
                }
                break;
            case 6:
                cout<<"Exiting the system. ByeBye!"<<endl;
                break;
            default:
                cout<<"\nError: Ivanlid Choice!! Please select a number between 1-6."<<endl;
        }
    }

    return 0;

}