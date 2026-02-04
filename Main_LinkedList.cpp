// #include <iostream>
// #include<fstream>
// #include<sstream>
// #include<string>
// #include<limits>

// #include "LinkedList.hpp" 

// using namespace std;

// string cleanID(string input){
//     string output="";
//     for(char c:input){
//         if(isalnum(c)){
//             output+=c;
//         }
//     }
//     return output;
// }

// void loadDataFromCSV(string filename,FlightLinkedList&list){
//     ifstream file(filename);

//     if(!file.is_open()){
//         cout<<"Error: Cannot open file "<<filename<<"'"<<endl;
//         cout<<"Please check if the file is in the same folder as your.exe!!!"<<endl;
//         return;
//     }

//     string line;
//     if(file.good()){
//         getline(file, line);
//     }

//     int count=0;

//     while(getline(file,line)){
//         if (line.empty()) continue;

//         stringstream ss(line);
//         Passenger p;

//         string tempID;
//         getline(ss,tempID,',');        
//         p.passengerID=cleanID(tempID);

//         getline(ss,p.name,',');
//         getline(ss,p.seatRow,',');
//         getline(ss,p.seatColumn,',');
//         getline(ss,p.pClass);


//         list.addPassenger(p);
//         count++;
//     }
//     file.close();
//     cout<<">Successfully loaded "<<count<<" Passenger from file."<<endl;
// }




// int main(){
//     FlightLinkedList myFlight;
//     int choice=0;
//     string inputID;

//     loadDataFromCSV("flight_passenger_data.csv",myFlight);

//     while(choice!=6){
//         cout<<"\n===== Flight Reservation System Menu ====="<<endl;
//         cout<<"1. Display Passenger Manifest (List)"<<endl;
//         cout<<"2. Display Seating Chart"<<endl;
//         cout<<"3. Search Passenger"<<endl;  
//         cout<<"4. Add Passenger"<<endl;
//         cout<<"5. Remove Passenger"<<endl;
//         cout<<"6. Exit"<<endl;
//         cout<<"Please enter your choice (1-6): ";

//         if(!(cin>>choice)){
//             cout<<"\nError: Invalid Input!! Please only enter numbers 1-6."<<endl;
//             cin.clear();
//             cin.ignore(numeric_limits<streamsize>::max(),'\n');
//             continue;
//         }

//         switch(choice){
//             case 1:
//                 myFlight.displayManifest();
//                 break;
//             case 2:
//                 myFlight.displaySeatingChart();
//                 break;
//             case 3:
//                 { 
//                     string searchID;
//                     cout<<"Enter the Passenger ID you want to search (e.g.,100000):";
//                     cin>>searchID;
//                     searchID=cleanID(searchID);
                
//                     Passenger*p=myFlight.searchPassenger(searchID);

//                     if(p!=nullptr){
//                         cout<<"Passenger Found!"<<endl;
//                         cout<<"ID:"<<p->passengerID<<endl;
//                         cout<<"Name:"<<p->name<<endl;
//                         cout<<"Seat:"<<p->seatRow<<p->seatColumn<<endl;
//                         cout<<"Class:"<<p->pClass<<endl;
//                     }else{
//                         cout<<"Passenger ID "<<searchID<<" Not Found!!"<<endl;
//                     }
//                 }
//                 break;
//             case 4:        
//                 {
//                     cout<<"\n4. Adding a new Passenger (Manual Input)"<<endl;

//                     Passenger p;
//                     int tempRow;

//                     cout<<"Enter Passenger ID:";
//                     cin>>p.passengerID;
//                     p.passengerID=cleanID(p.passengerID);

//                     if (myFlight.searchPassenger(p.passengerID) != nullptr) {
//                         cout << "\n ERROR: Passenger ID " << p.passengerID << " already exists!" << endl;
//                         cout << "Duplicate IDs are not allowed. Please try again.\n" << endl;
//                         break; 
//                     }

//                     cout<<"Enter Name (No Spaces):";
//                     cin>>p.name;

//                     cout<<"Enter Seat Row (1-30):";
//                     if(!(cin>>tempRow)){
//                         cout<<"\nError: Invalid input for Seat Row. Must be a number between 1-30."<<endl;
//                         cin.clear();
//                         cin.ignore(numeric_limits<streamsize>::max(),'\n');
//                         break;
//                     }
//                     if(tempRow>=1&&tempRow<=3){
//                         p.pClass="First";
//                     }
//                     else if(tempRow>=4&&tempRow<=10){
//                         p.pClass="Business";
//                     }
//                     else if(tempRow>=11&&tempRow<=30){
//                         p.pClass="Economy";
//                     }else{
//                         cout<<"\nError: Seat Row must be between 1 and 30."<<endl;
//                         break;
//                     }

//                     p.seatRow=to_string(tempRow);
//                     cout << "--> System Auto-Assigned Class: " << p.pClass << endl;
                    
//                     cout<<"Enter Seat Column (A-F):";
//                     cin>>p.seatColumn;

//                     for (char &c : p.seatColumn) {
//                         c = toupper(c);
//                     }

//                     if(myFlight.addPassenger(p)){
//                         cout << "--> Success: Passenger added to " << p.pClass << "Class." << endl;
//                     }else{
//                         cout << "\n--------------------------------------------------" << endl;
//                         cout << "FAILED: Seat " << p.seatRow << p.seatColumn << " is currently OCCUPIED." << endl;
//                         cout << "ACTION REQUIRED: Please go to Menu Option 5 (Remove)" << endl;
//                         cout << "to remove the passenger at this seat first." << endl;
//                         cout << "--------------------------------------------------" << endl;
//                     }
//                 }
//                 break;

//             case 5:
//                 {
//                     cout<<"\n3. Remove a Passenger"<<endl;
//                     string removeID;
//                     cout<<"Enter the Passenger ID to remove (e.g.,100000): ";
//                     cin>>removeID;

//                     cout<<"Processing removal of Passenger ID: "<<removeID<<endl;
//                     bool isRemoved=myFlight.removePassenger(removeID);
//                 }
//                 break;
//             case 6:
//                 cout<<"Exiting the system. ByeBye!"<<endl;
//                 break;
//             default:
//                 cout<<"\nError: Ivalid Choice!! Please select a number between 1-6."<<endl;
//         }
//     }

//     return 0;

// }