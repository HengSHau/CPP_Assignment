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

// --- GLOBAL STORAGE (Dynamic Array) ---
Passenger* fileBuffer = nullptr; 
int dataSize = 0;                
bool isDataLoaded = false;

// --- GLOBAL SYSTEMS (For Manual Operations) ---
// These persist so you can add passengers one by one and keep the data.
ArrayFlight globalArray;
FlightLinkedList globalList;

// Registry to track seat assignments
bool seatRegistry[100][30][6] = {false}; 


void loadFileToBuffer(string filename) {
    if (isDataLoaded) return; 

    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: CSV file not found." << endl;
        return;
    }

    // 1. Count Lines first to allocate exact memory
    string line;
    getline(file, line); // Skip header
    int lineCount = 0;
    while (getline(file, line)) lineCount++;

    // 2. Allocate Memory (Dynamic Array)
    dataSize = lineCount;
    fileBuffer = new Passenger[dataSize]; 

    // 3. Reset File and Read Data
    file.clear(); 
    file.seekg(0); 
    getline(file, line); // Skip header

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

        // Auto-assign flight logic
        while (true) {
            if (!seatRegistry[flight][r][c]) {
                seatRegistry[flight][r][c] = true;
                break;
            }
            flight++;
        }

        Passenger p = {id, name, rowStr, colStr, pClass, flight};
        
        // Store in Buffer
        fileBuffer[i] = p; 
        
        // Also populate Global Systems (So Manual Add works on full data)
        globalArray.addPassenger(p);
        globalList.addPassenger(p);
        
        i++;
    }
    
    // Sort global systems initially
    globalArray.insertionSort();
    globalList.insertionSort();
    
    file.close();
    isDataLoaded = true;
    cout << "System: Data Loaded & Sorted. Ready for operations.\n";
}

// =============================================================
// OPTION 1: BULK IMPORT BENCHMARK (0 -> 10,000)
// =============================================================
void runImportBenchmark() {
    cout << "\n[ Starting Bulk Import Benchmark... ]" << endl;

    // FIX: Use Heap Allocation (new) to prevent Stack Overflow crash
    ArrayFlight* testArray = new ArrayFlight();
    FlightLinkedList* testList = new FlightLinkedList();


    // Measure Array Import
    auto start = high_resolution_clock::now();
    for (int i = 0; i < dataSize; i++) {
        testArray->addPassenger(fileBuffer[i]);
    }
    auto stop = high_resolution_clock::now();
    auto arrayImportTime = duration_cast<milliseconds>(stop - start).count();

    // Measure Linked List Import
    start = high_resolution_clock::now();
    for (int i = 0; i < dataSize; i++) {
        testList->addPassenger(fileBuffer[i]);
    }
    stop = high_resolution_clock::now();
    auto listImportTime = duration_cast<milliseconds>(stop - start).count();


    // Measure Array Sort
    start = high_resolution_clock::now();
    testArray->insertionSort();
    stop = high_resolution_clock::now();
    auto arraySortTime = duration_cast<milliseconds>(stop - start).count();

    // Measure Linked List Sort
    start = high_resolution_clock::now();
    testList->insertionSort();
    stop = high_resolution_clock::now();
    // Use Microseconds for List because it is very fast
    auto listSortTime = duration_cast<microseconds>(stop - start).count();

    cout << "\n=== BULK RESULTS ===" << endl;
    cout << "Array Import: " << arrayImportTime << " ms | Sort: " << arraySortTime << " ms" << endl;
    cout << "List Import:  " << listImportTime << " ms | Sort: " << listSortTime << " us" << endl;

    // Clean up heap memory
    delete testArray;
    delete testList;
}

// =============================================================
// OPTION 2: ADD SINGLE PASSENGER (Manual Time Test)
// =============================================================
void addNewPassenger() {
    string id, name, row, col, pClass;
    
    cout << "\n=== ADD NEW PASSENGER ===" << endl;
    while(true){
        cout<<"Enter Passenger ID: "; 
        cin>>id;

        if (globalArray.binarySearch(id) != nullptr) {
            cout << "Error: ID exists." << endl;
        } 
        else {
            break; // Unique
        }
    }

    cout << "Enter Name: "; cin.ignore(); getline(cin, name);
    
    while(true) {
        cout << "Enter Row (1-30): "; cin >> row;
        try {
            int rowNum = stoi(row);
            if (rowNum >= 1 && rowNum <= 30) {
                // Determine Class
                if (rowNum >= 1 && rowNum <= 3) pClass = "First";
                else if (rowNum >= 4 && rowNum <= 10) pClass = "Business";
                else pClass = "Economy";
                
                cout << ">> System detected Class: " << pClass << endl;
                break; // Valid Row
            } else {
                cout << "Error: Row must be between 1 and 30.\n";
            }
        } catch (...) {
            cout << "Error: Please enter a valid number.\n";
        }
    }

    // 3. Get Column Input
    while (true) {
        cout << "Enter Col (A-F): "; cin >> col;
        
        // Normalize input
        char c = toupper(col[0]);
        
        // Check if single character AND between A-F
        if (col.length() == 1 && c >= 'A' && c <= 'F') {
            col = string(1, c); // Ensure it's uppercase stored
            break; // Valid!
        } else {
            cout << "Error: Invalid Seat. Only columns A, B, C, D, E, F are allowed." << endl;
        }
    }
    int r=stoi(row)-1;
    int c=toupper(col[0])-'A';
    int flight = 1;

    while (true) {
        if (!seatRegistry[flight][r][c]) {
            // Found it!
            seatRegistry[flight][r][c] = true; // Mark as taken
            break;
        }
        flight++; // Try next flight
    }

    
    
    cout << ">> Assigned to Flight " << flight << endl;

    Passenger p = {id, name, row, col, pClass, flight};

    cout << "\nAdding Passenger to System..." << endl;

    // --- MEASURE ARRAY ADD ---
    auto start = high_resolution_clock::now();
    globalArray.addPassenger(p); // RAW ADD
    auto stop = high_resolution_clock::now();
    auto arrayTime = duration_cast<nanoseconds>(stop - start).count();
    
    // Sort Array Immediately (Outside Timer)
    globalArray.insertionSort();

    // --- MEASURE LIST ADD ---
    start = high_resolution_clock::now();
    globalList.addPassenger(p); // RAW ADD
    stop = high_resolution_clock::now();
    auto listTime = duration_cast<nanoseconds>(stop - start).count();

    // Sort List Immediately (Outside Timer)
    globalList.insertionSort();

    cout << "\n=== ADD RESULTS ===" << endl;
    cout << "Array Add Time:      " << arrayTime << " nanoseconds" << endl;
    cout << "Linked List Add Time:" << listTime << " nanoseconds" << endl;
}

void searchPassenger() {
    int choice;
    string query;

    cout << "\n=== SEARCH MENU ===" << endl;
    cout << "1. Search by ID (Binary Search)" << endl;
    cout << "2. Search by Row (Linear Search)" << endl;
    cout << "3. Search by Column (Linear Search)" << endl;
    cout << "Enter choice: ";
    cin >> choice;

    if (choice == 1) {
        // --- 1. ID SEARCH (BINARY) ---
        cout << "Enter Passenger ID: "; cin >> query;
        
        cout << "\n[ Binary Search Benchmark ]" << endl;
        
        // Measure Array
        auto start = high_resolution_clock::now();
        Passenger* p1 = globalArray.binarySearch(query);
        auto stop = high_resolution_clock::now();
        auto t1 = duration_cast<nanoseconds>(stop - start).count();

        // Measure List
        start = high_resolution_clock::now();
        Passenger* p2 = globalList.binarySearch(query);
        stop = high_resolution_clock::now();
        auto t2 = duration_cast<nanoseconds>(stop - start).count();

        if(p1) cout << "Found: " << p1->name << endl;
        else cout << "Not Found." << endl;

        cout << "Time: Array=" << t1 << "ns | List=" << t2 << "ns" << endl;
    }
    else if (choice == 2) {
        // --- 2. ROW SEARCH (LINEAR) ---
        cout << "Enter Row (1-30): "; cin >> query;
        
        cout << "\n[ Linear Search Benchmark ]" << endl;

        auto start = high_resolution_clock::now();
        globalArray.searchByRow(query);
        auto stop = high_resolution_clock::now();
        auto t1 = duration_cast<nanoseconds>(stop - start).count();

        start = high_resolution_clock::now();
        globalList.searchByRow(query);
        stop = high_resolution_clock::now();
        auto t2 = duration_cast<nanoseconds>(stop - start).count();

        cout << "Time: Array=" << t1 << "ns | List=" << t2 << "ns" << endl;
    }
    else if (choice == 3) {
        // --- 3. COLUMN SEARCH (LINEAR) ---
        cout << "Enter Col (A-F): "; cin >> query;
        if(query.length()>0) query[0] = toupper(query[0]);

        cout << "\n[ Linear Search Benchmark ]" << endl;

        auto start = high_resolution_clock::now();
        globalArray.searchByColumn(query);
        auto stop = high_resolution_clock::now();
        auto t1 = duration_cast<nanoseconds>(stop - start).count();

        start = high_resolution_clock::now();
        globalList.searchByColumn(query);
        stop = high_resolution_clock::now();
        auto t2 = duration_cast<nanoseconds>(stop - start).count();

        cout << "Time: Array=" << t1 << "ns | List=" << t2 << "ns" << endl;
    }
}

void displayAllPassengers() {
    int choice;
    cout << "\n=== DISPLAY ALL PASSENGERS (TRAVERSAL) ===" << endl;
    cout << "1. Display using Array Traversal" << endl;
    cout << "2. Display using Linked List Traversal" << endl;
    cout << "Enter choice: ";
    cin >> choice;

    if (choice == 1) {
        globalArray.displayAll();
    } else if (choice == 2) {
        globalList.displayAll();
    } else {
        cout << "Invalid choice.\n";
    }
}

int main() {
    // Load Data Once at Startup
    loadFileToBuffer("flight_passenger_data.csv");

    int choice;
    do {
        cout<<"\n=== AIRLINE SYSTEM MENU ==="<< endl;
        cout<<"1. Run Bulk Benchmark (Import data)"<<endl;
        cout<<"2. Add New Passenger"<<endl;
        cout<<"3. Search Passenger (by Row/Column)"<<endl;
        cout<<"4. Display All Passengers\n";
        cout<< "0. Exit"<<endl;
        cout<< "Enter choice: ";
        cin>>choice;

        switch(choice) {
            case 1:   
                runImportBenchmark(); 
                break;
            case 2: 
                addNewPassenger(); 
                break;
            case 3: 
                searchPassenger(); 
                break;
            case 4:
                displayAllPassengers(); 
                break;
            case 0: 
                break;
            default: cout << "Invalid choice." << endl;
        }

    } while (choice != 0);

    // Clean up Global Buffer
    if (fileBuffer != nullptr) {
        delete[] fileBuffer;
    }
    
    return 0;
}