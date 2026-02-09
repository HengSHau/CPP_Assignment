#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono> 
#include <string>
#include <cctype>

#include "Passenger.hpp"
#include "ArrayFlight.hpp"   
#include "LinkedList.hpp"    

using namespace std;
using namespace std::chrono;

// --- GLOBALS ---
Passenger* fileBuffer = nullptr; 
int dataSize = 0;                
ArrayFlight globalArray;
FlightLinkedList globalList;
bool seatRegistry[101][30][6] = {false}; 

// =============================================================
// HELPER: LOAD DATA
// =============================================================
void loadFileToBuffer(string filename) {
    ifstream file(filename);
    if (!file.is_open()) { cout << "Error: File not found.\n"; return; }

    string line;
    getline(file, line); // Skip header
    int lineCount = 0;
    while (getline(file, line)) lineCount++;

    dataSize = lineCount;
    fileBuffer = new Passenger[dataSize]; 

    file.clear(); 
    file.seekg(0); 
    getline(file, line); 

    cout << "System: Loading " << dataSize << " records..." << endl;
    
    int i = 0;
    while (getline(file, line)) {
        stringstream ss(line);
        string id, name, rowStr, colStr, pClass;
        
        getline(ss, id, ',');
        getline(ss, name, ',');
        getline(ss, rowStr, ',');
        getline(ss, colStr, ',');
        getline(ss, pClass, ',');

        int r = stoi(rowStr) - 1;
        int c = toupper(colStr[0]) - 'A';
        int flight = 1;

        while (flight <= 100) {
            if (!seatRegistry[flight][r][c]) {
                seatRegistry[flight][r][c] = true;
                break;
            }
            flight++;
        }

        Passenger p = {id, name, rowStr, colStr, pClass, flight};
        fileBuffer[i] = p; 
        globalArray.addPassenger(p);
        globalList.addPassenger(p);
        i++;
    }
    // Sort initially
    globalArray.insertionSort();
    globalList.insertionSort();
    file.close();
    cout << "System: Data Loaded & Sorted.\n";
}

// =============================================================
// HELPER: CREATE PASSENGER INPUT (With Duplicate Check)
// =============================================================
Passenger addNewPassenger(bool useArray) {
    string input;
    string id, name, row, col, pClass;

    cout << "\n--- Enter Passenger Details (Enter 'q' to Cancel) ---\n";
    
    // 1. Get Unique ID
    while(true){
        cout << "ID (Numbers only): "; 
        cin >> input;
        
        if (input == "q" || input == "Q") return {"QUIT", "", "", "", "", 0}; 

        // --- NEW CHECK: Is it a number? ---
        bool isNumber = true;
        for (char c : input) {
            if (!isdigit(c)) { // Check if character is NOT a digit (0-9)
                isNumber = false;
                break;
            }
        }

        if (!isNumber) {
            cout << "Error: ID must contain digits only (0-9). Please try again.\n";
            continue; // Restart loop
        }
        // ----------------------------------

        id = input; 

        bool exists = false;
        if (useArray) {
            if (globalArray.binarySearch(id) != nullptr) exists = true;
        } else {
            if (globalList.binarySearch(id) != nullptr) exists = true;
        }

        if (exists) cout << "Error: ID already exists. Try again.\n";
        else break;
    }
    
    // 2. Get Name
    cout << "Name: "; 
    cin.ignore(); 
    getline(cin, input);
    if (input == "q" || input == "Q") return {"QUIT", "", "", "", "", 0};
    name = input;
    
    // 3. Row & Auto-Class
    while(true) {
        cout << "Row (1-30): "; cin >> input;
        if (input == "q" || input == "Q") return {"QUIT", "", "", "", "", 0};

        try {
            int r = stoi(input);
            if (r >= 1 && r <= 30) {
                if (r <= 3) pClass = "First";
                else if (r <= 10) pClass = "Business";
                else pClass = "Economy";
                cout << ">> System Auto-Detected Class: " << pClass << endl;
                row = input;
                break;
            }
            cout << "Error: 1-30 only.\n";
        } catch(...) { cout << "Invalid number.\n"; }
    }

    // 4. Column
    while(true) {
        cout << "Col (A-F): "; cin >> input;
        if (input == "q" || input == "Q") return {"QUIT", "", "", "", "", 0};

        if (input.length() == 1 && toupper(input[0]) >= 'A' && toupper(input[0]) <= 'F') {
            col = string(1, (char)toupper(input[0]));
            break;
        }
        cout << "Error: A-F only.\n";
    }

    // --- 5. AUTO-ASSIGN LOGIC (Check Flight 1, then 2, then 3...) ---
    int r = stoi(row) - 1;
    int c = toupper(col[0]) - 'A';
    int flight = 1; 
    bool found = false;

    while (flight <= 100) {
        // If seat is empty on this flight, take it!
        if (!seatRegistry[flight][r][c]) {
            seatRegistry[flight][r][c] = true; // Mark as taken
            found = true;
            break; // Stop looking
        }
        flight++; // Try next flight
    }

    if (!found) {
        cout << "Error: This seat is fully booked on ALL 100 flights!\n";
        return {"QUIT", "", "", "", "", 0};
    }

    cout << ">> Seat Available! Auto-Assigned to Flight " << flight << endl;

    return {id, name, row, col, pClass, flight};
}


// =============================================================
// SUB-MENU: SESSION LOOP
// =============================================================
void runSession(bool useArray) {
    int choice;
    string structName = useArray ? "ARRAY" : "LINKED LIST";

    do {
        cout << "\n========================================\n";
        cout << "      OPERATING ON: " << structName << "\n";
        cout << "========================================\n";
        cout << "1. Add Passenger\n";
        cout << "2. Remove Passenger\n";
        cout << "3. Search Passenger\n";
        cout << "4. Display Manifest (List All)\n";
        cout << "5. Show Seat Map\n";
        cout << "6. Sort Comparizon\n";
        cout << "7. Back to Main Menu\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: { 

                Passenger p = addNewPassenger(useArray);
                
                if (p.passengerID == "QUIT") {
                    cout << ">> Operation Cancelled.\n";
                } 
                else {
                    cout << "\nInserting into " << structName << "..." << endl;
                    
                    // 1. START TIMER
                    auto start = high_resolution_clock::now();
                    
                    // 2. ONLY ADD (Do not sort yet)
                    if (useArray) {
                        globalArray.addPassenger(p);
                    } else {
                        globalList.addPassenger(p);
                    }
                    
                    // 3. STOP TIMER (Pure Add Time)
                    auto stop = high_resolution_clock::now();
                    auto duration = duration_cast<nanoseconds>(stop - start).count();
                    
                    // 4. Sort Afterwards (Not Timed)
                    if (useArray) globalArray.insertionSort(); 
                    else globalList.insertionSort(); 

                    cout << "[Success] Added in " << duration << " ns.\n";
                }
                break; 
            }
            
            // ... (The rest of the cases remain the same) ...
            
            case 2: {
                string id;
                cout << "Enter ID to remove: "; cin >> id;
                cout << "\nRemoving from " << structName << "..." << endl;
                auto start = high_resolution_clock::now();
                bool success = useArray ? globalArray.removePassenger(id) : globalList.removePassenger(id);
                auto stop = high_resolution_clock::now();
                auto duration = duration_cast<nanoseconds>(stop - start).count();

                if (success) cout << "[Success] Removed in " << duration << " ns.\n";
                else cout << "[Error] Passenger ID not found.\n";
                break;
            }

            case 3: {
                int searchType;
                cout<<"\n   --- SEARCH OPTIONS ---"<<endl;
                cout<<"1. Search by ID (Binary Search)"<<endl;
                cout<<"2. Search by Row (Linear Search)"<<endl;
                cout<<"3. Search by Column (Linear Search)"<<endl;
                cout<<"Enter search type: ";
                cin>>searchType;

                if (searchType == 1) {
                    string id;
                    cout<<"Enter ID: "; cin>>id;
                    cout<<"\nSearching for ID "<<id<<" in "<<structName<<"..."<<endl;

                    auto start = high_resolution_clock::now();
                    Passenger* p = useArray ? globalArray.binarySearch(id) : globalList.binarySearch(id);
                    auto stop = high_resolution_clock::now();
                    auto duration = duration_cast<nanoseconds>(stop - start).count();

                    if (p) cout<<">> Found: "<<p->name<<" (Flight "<<p->flightNumber<<")"<<endl;
                    else cout<<">> Not Found.\n";
                    cout<<"Binary Search Time: "<<duration<<" ns."<<endl;
                }
                else if (searchType == 2) {
                    // --- ROW SEARCH (LINEAR) ---
                    string row;
                    cout << "Enter Row (1-30): "; cin >> row;
                    cout << "\nFinding all passengers in Row " << row << "..." << endl;

                    auto start = high_resolution_clock::now();
                    if (useArray) globalArray.searchByRow(row);
                    else globalList.searchByRow(row);
                    auto stop = high_resolution_clock::now();
                    
                    auto duration = duration_cast<nanoseconds>(stop - start).count();
                    cout << "Linear Search Time: " << duration << " ns.\n";
                }
                else if (searchType == 3) {
                    // --- COLUMN SEARCH (LINEAR) ---
                    string col;
                    cout << "Enter Column (A-F): "; cin >> col;
                    
                    // Force Uppercase for better matching
                    if (col.length() > 0) col[0] = toupper(col[0]);

                    cout << "\nFinding all passengers in Column " << col << "..." << endl;

                    auto start = high_resolution_clock::now();
                    if (useArray) globalArray.searchByColumn(col);
                    else globalList.searchByColumn(col);
                    auto stop = high_resolution_clock::now();

                    auto duration = duration_cast<nanoseconds>(stop - start).count();
                    cout << "Linear Search Time: " << duration << " ns.\n";
                }
                else {
                    cout << "Invalid search type.\n";
                }
                break;
            }

            case 4: {
                auto start = high_resolution_clock::now();
                if (useArray) globalArray.displayAll();
                else globalList.displayAll();
                auto stop = high_resolution_clock::now();
                auto duration = duration_cast<nanoseconds>(stop - start).count();
                cout << "Display Time: " << duration << " ns.\n";
                break;
            }

            case 5: {
                int fNum;
                cout << "Enter Flight Number (1-100): "; cin >> fNum;
                if (useArray) globalArray.displaySeatingChart(fNum);
                else globalList.displaySeatingChart(fNum);
                break;
            }

            case 6:{
                int sortChoice;
                cout<<"\n   --- SORT OPTIONS ---"<<endl;
                cout<<"1. Insertion Sort"<<endl;
                cout<<"2. Bubble Sort"<<endl;
                cout<<"3. Selection Sort"<<endl;
                cout<<"4. Merge Sort"<<endl;
                cout<<"5. Back to Menu"<<endl;
                cout<<"Enter sort type: ";

                if (!(cin >> sortChoice)) {
                    cout << "Error: Invalid input! Please enter a number (1-5)." << endl;
                    cin.clear(); 
                    cin.ignore(10000, '\n');
                    continue; 
                }
                
                if(sortChoice>=1&&sortChoice<=4){
                    cout<<"\nSorting "<<structName<<" using selected algorithm..."<<endl;

                }

                auto start = high_resolution_clock::now();
                switch(sortChoice){
                    case 1:
                        if(useArray) globalArray.insertionSort();
                        else globalList.insertionSort();
                        break;
                    case 2:
                        if(useArray) globalArray.bubbleSort();
                        else globalList.bubbleSort();
                        break;
                    case 3:
                        if(useArray) globalArray.selectionSort();
                        else globalList.selectionSort();
                        break;
                    case 4:
                        if(useArray) globalArray.mergeSort();
                        else globalList.mergeSort();
                        break;
                    case 5:
                        break;
                    default:
                        cout<<"Invalid sort type."<<endl;
                        continue;
                }
                
                if(sortChoice>=1&&sortChoice<=4){
                    auto stop = high_resolution_clock::now();
                    auto duration = duration_cast<nanoseconds>(stop - start).count();
                    cout<<"Sorting completed in "<<duration<<" ns."<<endl;
                }
                break;
                
            }

            case 7:
                cout << "Returning to Main Menu...\n";
                choice=0;
                break;

            default:
                cout << "Invalid choice. Please try again.\n";
                break;
        }

    } while (choice != 0);
}

// =============================================================
// OPTION: BULK IMPORT BENCHMARK (Keep for Assignment)
// =============================================================
void runImportBenchmark() {
    cout << "\n[Data Import Benchmark]" << endl;
    ArrayFlight* testArray = new ArrayFlight();
    FlightLinkedList* testList = new FlightLinkedList();

    // Array
    auto start = high_resolution_clock::now();
    for (int i = 0; i < dataSize; i++) testArray->addPassenger(fileBuffer[i]);
    auto stop = high_resolution_clock::now();
    auto arrayTime = duration_cast<milliseconds>(stop - start).count();

    // List
    start = high_resolution_clock::now();
    for (int i = 0; i < dataSize; i++) testList->addPassenger(fileBuffer[i]);
    stop = high_resolution_clock::now();
    auto listTime = duration_cast<milliseconds>(stop - start).count();

    cout << "Array Import: " << arrayTime << " ms\n";
    cout << "List Import:  " << listTime << " ms\n";
    delete testArray; delete testList;
}

// =============================================================
// OPTION: MEMORY COMPARISON
// =============================================================
void compareMemory(){
    size_t arrayMem = globalArray.getMemoryUsage();
    size_t listMem = globalList.getMemoryUsage();
    cout<<"\nArray Memory:      "<<arrayMem<<" bytes\n";
    cout<<"Linked List Memory: "<<listMem<<" bytes\n";
}

// =============================================================
// MAIN MENU
// =============================================================
int main() {
    loadFileToBuffer("flight_passenger_data.csv");

    int mainChoice;
    do {
        cout << "\n########################################\n";
        cout << "      AIRLINE SYSTEM - MAIN MENU\n";
        cout << "########################################\n";
        cout << "1. ARRAY"<<endl;
        cout << "2. LINKED LIST"<<endl;
        cout << "3. Run Bulk Import Benchmark"<<endl;
        cout << "4. Compare Memory Usage"<<endl;
        cout << "5. Exit Program"<<endl;
        cout << "Enter choice: ";
        cin >> mainChoice;

        switch(mainChoice) {
            case 1: runSession(true); break;  // Use Array
            case 2: runSession(false); break; // Use List
            case 3: runImportBenchmark(); break;
            case 4: compareMemory(); break;
            case 5: 
                cout << "Exiting the system. Bye Bye!!"<<endl;
                mainChoice=0; 
                break;
            default: cout << "Invalid choice.\n";
        }
    } while (mainChoice != 0);

    if (fileBuffer) delete[] fileBuffer;
    return 0;
}