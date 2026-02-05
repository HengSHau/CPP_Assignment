/*#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono> 
#include <limits> // Required for numeric_limits
#include <cctype> // Required for toupper

// HEADER FILES
#include "ArrayFlight.hpp"
#include "LinkedList.hpp" 

using namespace std;
using namespace std::chrono;

// ==========================================
// 1. DATA LOADING FUNCTIONS
// ==========================================

// Loader for ARRAY
void loadDataArray(ArrayFlight& system, string filename) {
    ifstream file(filename);
    if (!file.is_open()) return;
    
    string line, id, name, row, col, pClass;
    getline(file, line); // Skip header

    while (getline(file, line)) {
        stringstream ss(line);
        getline(ss, id, ',');
        getline(ss, name, ',');
        getline(ss, row, ',');
        getline(ss, col, ',');
        getline(ss, pClass, ',');

        system.addPassengerSilent(id, name, row, col, pClass);
    }
    file.close();
}

// Loader for LINKED LIST
void loadDataLinkedList(FlightLinkedList& system, string filename) {
    ifstream file(filename);
    if (!file.is_open()) return;

    string line, id, name, row, col, pClass;
    getline(file, line); // Skip header

    while (getline(file, line)) {
        stringstream ss(line);
        getline(ss, id, ',');
        getline(ss, name, ',');
        getline(ss, row, ',');
        getline(ss, col, ',');
        getline(ss, pClass, ',');

        Passenger p;
        p.passengerID = id;
        p.name = name;
        p.seatRow = row;
        p.seatColumn = col;
        p.pClass = pClass;

        system.addPassenger(p);
    }
    file.close();
    system.sortPassengers();
}

// ==========================================
// 2. DRIVER MODES
// ==========================================

void runArrayMode() {
    ArrayFlight flight;
    cout << "\n[Loading Array System...]" << endl;
    loadDataArray(flight, "flight_passenger_data.csv");
    cout << "Data Loaded (180 Passengers Limit)." << endl;

    int choice;
    string id, name, row, col, pClass;
    
    do {
        cout << "\n--- ARRAY SYSTEM MENU ---" << endl;
        cout << "1. View Map" << endl;
        cout << "2. View Manifest" << endl;
        cout << "3. Search" << endl;
        cout << "4. Add Reservation" << endl;
        cout << "5. Remove Reservation" << endl;
        cout << "6. Back to Main Menu" << endl;
        cout << "Choice: ";
        
        if (!(cin >> choice)) { 
            cin.clear(); cin.ignore(10000, '\n'); 
            continue; 
        }

        // USING SWITCH CASE HERE
        switch (choice) {
            case 1:
                flight.displaySeatMap();
                break;

            case 2:
                flight.displayManifest();
                break;

            case 3: 
                int searchType;
                cout << "\nSearch/Filter Mode:" << endl;
                cout << "1. Search by ID" << endl;
                cout << "2. Filter by Row (1-30)" << endl;
                cout << "3. Filter by Column (A-F)" << endl;
                cout << "Choice: ";
                cin >> searchType;

                if (searchType == 1) {
                    // --- EXISTING ID SEARCH ---
                    cout << "Enter ID: "; cin >> id;
                    auto start = high_resolution_clock::now();
                    Passenger* p = flight.searchPassenger(id);
                    auto stop = high_resolution_clock::now();
                    
                    if (p != nullptr) cout << "Found: " << p->name << " at " << p->seatRow << p->seatColumn << endl;
                    else cout << "Not Found." << endl;
                    
                    auto duration = duration_cast<nanoseconds>(stop - start);
                    cout << "(Time: " << duration.count() << " ns)" << endl;

                } else if (searchType == 2) {
                    // --- ROW FILTER ---
                    cout << "Enter Row (1-30): "; cin >> row;
                    
                    auto start = high_resolution_clock::now();
                    flight.filterByRow(row);
                    auto stop = high_resolution_clock::now();
                    
                    auto duration = duration_cast<nanoseconds>(stop - start);
                    cout << "(Time: " << duration.count() << " ns)" << endl;

                } else if (searchType == 3) {
                    // --- COLUMN FILTER ---
                    cout << "Enter Column (A-F): "; cin >> col;
                    
                    auto start = high_resolution_clock::now();
                    flight.filterByColumn(col);
                    auto stop = high_resolution_clock::now();
                    
                    auto duration = duration_cast<nanoseconds>(stop - start);
                    cout << "(Time: " << duration.count() << " ns)" << endl;
                }
                break;

            case 4:
                cout << "Enter ID: "; cin >> id;
                cout << "Enter Name: "; cin.ignore(); getline(cin, name);
                cout << "Enter Row (1-30): "; cin >> row;
                cout << "Enter Column (A-F): "; cin >> col;
                cout << "Enter Class: "; cin >> pClass;
                
                {
                    auto start = high_resolution_clock::now();
                    
                    // 2. RUN LOGIC
                    bool success = flight.addPassenger(id, name, row, col, pClass);
                    
                    // 3. STOP TIMER
                    auto stop = high_resolution_clock::now();
                    auto duration = duration_cast<nanoseconds>(stop - start);
                    
                    // 4. PRINT
                    if(success) 
                        cout << "Success. ";
                    else 
                        cout << "Failed. ";

                    cout << "(Time taken: " << duration.count() << " ns)" << endl;
                }
                break;

            case 5:
                cout << "Enter ID to remove: ";
                cin >> id;
                {
                    auto start = high_resolution_clock::now();

                    // 2. EXECUTE REMOVAL
                    bool removed = flight.removePassenger(id);

                    // 3. STOP TIMER
                    auto stop = high_resolution_clock::now();
                    auto duration = duration_cast<nanoseconds>(stop - start);

                    // 4. PRINT RESULT + TIME
                    if (removed)
                        cout << "Success: Passenger removed. ";
                    else
                        cout << "Failed: Passenger not found. ";

                    cout << "(Time taken: " << duration.count() << " ns)" << endl;
                }
                break;

            case 6:
                choice=0;
                break; // Exits the loop

            default:
                cout << "Invalid option. Please try again." << endl;
        }

    } while (choice != 0);
}

void runLinkedListMode() {
    FlightLinkedList flight; // Variable name is 'flight'
    cout << "\n[Loading Linked List System...]" << endl;
    loadDataLinkedList(flight, "flight_passenger_data.csv");
    cout << "Data Loaded." << endl;

    int choice;
    string id; 
    
    do {
        cout << "\n--- LINKED LIST MENU ---" << endl;
        cout << "1. Display Passenger Manifest (List)" << endl;
        cout << "2. Display Seating Chart" << endl;
        cout << "3. Search Passenger" << endl;   
        cout << "4. Add Passenger" << endl;
        cout << "5. Remove Passenger" << endl;
        cout << "6. Exit to Main Menu" << endl; // NOTE: Case 6 exits loop, but main loop handles actual exit
        cout << "Please enter your choice: ";

        if (!(cin >> choice)) { cin.clear(); cin.ignore(10000, '\n'); continue; }

        switch(choice){
            case 1:
                flight.displayManifest(); // Fixed: changed myFlight to flight
                break;
            case 2:
                flight.displaySeatingChart(); // Fixed: changed myFlight to flight
                break;
            case 3:
                { 
                    string searchType;
                    cout<<"\nSearch/Filter Mode:"<<endl;
                    cout<<"1. Search by ID"<<endl;
                    cout<<"2. Filter by Row (1-30)"<<endl;
                    cout<<"3. Filter by Column (A-F)"<<endl;
                    cout<<"Choice: ";
                    cin>>searchType;
                    
                    if(searchType=="1"){
                        cout<<"Enter ID to search (e.g.,100000): ";
                        cin>>id;
                        auto start = high_resolution_clock::now();
                        Passenger* p = flight.searchPassengerBinary(id); 
                        auto stop = high_resolution_clock::now();
                        auto duration = duration_cast<nanoseconds>(stop - start);
                        if(p!=nullptr){
                            cout<<"Passenger Found!"<<endl;
                            cout<<"ID: "<<p->passengerID<<endl;
                            cout<<"Name: "<<p->name<<endl;
                            cout<<"Seat: "<<p->seatRow<<p->seatColumn<<endl;
                            cout<<"Class: "<<p->pClass<<endl;
                        } else {
                            cout<<"Passenger ID "<<id<<" Not Found!!"<<endl;
                        }
                        cout<<"(Search Time: "<<duration.count()<<" ns)"<<endl;

                    }else if(searchType=="2"){
                        string row;
                        cout<<"Enter Row to filter (1-30): ";
                        cin>>row;
                        auto start = high_resolution_clock::now();
                        flight.filterByRow(row);
                        auto stop = high_resolution_clock::now();
                        auto duration = duration_cast<nanoseconds>(stop - start);
                        cout<<"(Filter Time: "<<duration.count()<<" ns)"<<endl;

                    

                    } else if(searchType=="3"){
                        string col;
                        cout<<"Enter Column to filter (A-F): ";
                        cin>>col;
                        auto start = high_resolution_clock::now();
                        flight.filterByColumn(col);
                        auto stop = high_resolution_clock::now();
                        auto duration = duration_cast<nanoseconds>(stop - start);
                        cout<<"(Filter Time: "<<duration.count()<<" ns)"<<endl;
                    }
                    break;
                }    
                case 4:        
                {
                    cout << "\n4. Adding a new Passenger (Manual Input)" << endl;

                    Passenger p;
                    int tempRow;

                    cout << "Enter Passenger ID: ";
                    cin >> p.passengerID;
                    // Removed undefined cleanID function

                    // Fixed: changed myFlight to flight
                    if (flight.searchPassenger(p.passengerID) != nullptr) {
                        cout << "\n ERROR: Passenger ID " << p.passengerID << " already exists!" << endl;
                        cout << "Duplicate IDs are not allowed. Please try again.\n" << endl;
                        break; 
                    }

                    cout << "Enter Name (No Spaces): ";
                    cin >> p.name;

                    cout << "Enter Seat Row (1-30): ";
                    if(!(cin >> tempRow)){
                        cout << "\nError: Invalid input for Seat Row. Must be a number between 1-30." << endl;
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(),'\n');
                        break;
                    }
                    
                    // Logic to auto-assign class
                    if(tempRow >= 1 && tempRow <= 3) p.pClass = "First";
                    else if(tempRow >= 4 && tempRow <= 10) p.pClass = "Business";
                    else if(tempRow >= 11 && tempRow <= 30) p.pClass = "Economy";
                    else {
                        cout << "\nError: Seat Row must be between 1 and 30." << endl;
                        break;
                    }

                    p.seatRow = to_string(tempRow);
                    cout << "--> System Auto-Assigned Class: " << p.pClass << endl;
                    
                    cout << "Enter Seat Column (A-F): ";
                    cin >> p.seatColumn;

                    for (char &c : p.seatColumn) {
                        c = toupper(c);
                    }

                    auto start = high_resolution_clock::now();

                    bool success=flight.addPassenger(p);

                    auto stop = high_resolution_clock::now();
                    auto duration = duration_cast<nanoseconds>(stop - start);

                    // Fixed: changed myFlight to flight
                    if(success){
                        cout << "--> Success: Passenger added to " << p.pClass << " Class." << endl;
                    } else {
                        cout << "\n--------------------------------------------------" << endl;
                        cout << "FAILED: Seat " << p.seatRow << p.seatColumn << " is currently OCCUPIED." << endl;
                        cout << "--------------------------------------------------" << endl;
                    }
                    cout << "(Time taken: " << duration.count() << " ns)" << endl;
                }
                break;

            case 5:
                {
                    cout << "\n5. Remove a Passenger" << endl;
                    string removeID;
                    cout << "Enter the Passenger ID to remove: ";
                    cin >> removeID;

                    auto start = high_resolution_clock::now();

                    // Fixed: changed myFlight to flight
                    bool isRemoved = flight.removePassenger(removeID);

                    auto stop = high_resolution_clock::now();
                    auto duration = duration_cast<nanoseconds>(stop - start);

                    if(isRemoved){
                        cout << "Success: Passenger ID " << removeID << " has been removed." << endl;
                    } else {
                        cout << "Error: Passenger ID " << removeID << " not found." << endl;
                    }
                    cout << "(Time taken: " << duration.count() << " ns)" << endl;
                }
                break;
            case 6:
                choice=0;
                break;
            default:
                cout << "\nError: Invalid Choice!! Please select a number between 1-6." << endl;
        }
    } while (choice != 0);
}

void runBenchmark() {
    cout << "\n=== PERFORMANCE COMPARISON (LOAD DATA)===" << endl;
    
    ArrayFlight arr;
    FlightLinkedList list;

    // Measure Array
    auto start = high_resolution_clock::now();
    loadDataArray(arr, "flight_passenger_data.csv");
    auto stop = high_resolution_clock::now();
    auto durationArr = duration_cast<microseconds>(stop - start);

    // Measure Linked List
    start = high_resolution_clock::now();
    loadDataLinkedList(list, "flight_passenger_data.csv");
    stop = high_resolution_clock::now();
    auto durationList = duration_cast<microseconds>(stop - start);

    cout << "Array Load Time:       " << durationArr.count() << " microseconds" << endl;
    cout << "Linked List Load Time: " << durationList.count() << " microseconds" << endl;
    
    if (durationArr.count() < durationList.count())
        cout << ">> Array was faster." << endl;
    else
        cout << ">> Linked List was faster." << endl;
}

// ==========================================
// 3. MAIN ENTRY POINT
// ==========================================
int main() {
    int mode;
    do {
        cout << "\n==============================================" << endl;
        cout << "   FLIGHT SYSTEM MASTER DRIVER" << endl;
        cout << "==============================================" << endl;
        cout << "1. Run Array Implementation" << endl;
        cout << "2. Run Linked List Implementation" << endl;
        cout << "3. Run Performance Benchmark(Load Data)" << endl;
        cout << "4. Exit" << endl;
        cout << "Select Mode: ";
        
        if (!(cin >> mode)) {
            cin.clear(); cin.ignore(10000, '\n'); 
            continue;
        }

        switch (mode) {
            case 1: runArrayMode(); break;
            case 2: runLinkedListMode(); break;
            case 3: runBenchmark(); break;
            case 4: cout << "Exiting the System. Bye Bye!!" << endl; 
                mode=0;
                break;
            default: cout << "Invalid mode." << endl;
        }
    } while (mode != 0);

    return 0;
}*/