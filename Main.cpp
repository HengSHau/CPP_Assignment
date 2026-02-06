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

// Registry to track seat assignments (Flight 1-100, Row 0-29, Col 0-5)
bool seatRegistry[101][30][6] = {false}; 


// =============================================================
// HELPER: LOAD FILE
// =============================================================
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
        while (flight <= 100) {
            if (!seatRegistry[flight][r][c]) {
                seatRegistry[flight][r][c] = true;
                break;
            }
            flight++;
        }

        // Create Passenger Object
        // Note: 'pClass' from file goes into 'passengerClass' struct member
        Passenger p = {id, name, rowStr, colStr, pClass, flight};
        
        // Store in Buffer
        fileBuffer[i] = p; 
        
        // Populate Global Systems
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
// OPTION 1: BULK IMPORT BENCHMARK
// =============================================================
void runImportBenchmark() {
    cout << "\n[ Starting Bulk Import Benchmark... ]" << endl;

    // Use Heap Allocation to prevent Stack Overflow
    ArrayFlight* testArray = new ArrayFlight();
    FlightLinkedList* testList = new FlightLinkedList();

    // 1. Measure Array Import
    auto start = high_resolution_clock::now();
    for (int i = 0; i < dataSize; i++) {
        testArray->addPassenger(fileBuffer[i]);
    }
    auto stop = high_resolution_clock::now();
    auto arrayImportTime = duration_cast<milliseconds>(stop - start).count();

    // 2. Measure Linked List Import
    start = high_resolution_clock::now();
    for (int i = 0; i < dataSize; i++) {
        testList->addPassenger(fileBuffer[i]);
    }
    stop = high_resolution_clock::now();
    auto listImportTime = duration_cast<milliseconds>(stop - start).count();

    // 3. Measure Array Sort
    start = high_resolution_clock::now();
    testArray->insertionSort();
    stop = high_resolution_clock::now();
    auto arraySortTime = duration_cast<milliseconds>(stop - start).count();

    // 4. Measure Linked List Sort
    start = high_resolution_clock::now();
    testList->insertionSort();
    stop = high_resolution_clock::now();
    auto listSortTime = duration_cast<milliseconds>(stop - start).count();

    cout << "\n=== BULK RESULTS ===" << endl;
    cout << "Array Import: " << arrayImportTime << " ms | Sort: " << arraySortTime << " ms" << endl;
    cout << "List Import:  " << listImportTime  << " ms | Sort: " << listSortTime  << " ms" << endl;

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
    
    // 1. Get Unique ID
    while(true){
        cout << "Enter Passenger ID: "; 
        cin >> id;
        if (globalArray.binarySearch(id) != nullptr) {
            cout << "Error: ID exists. Try again." << endl;
        } else {
            break; 
        }
    }

    // 2. Get Name
    cout << "Enter Name: "; 
    cin.ignore(); 
    getline(cin, name);
    
    // 3. Get Row
    while(true) {
        cout << "Enter Row (1-30): "; cin >> row;
        try {
            int rowNum = stoi(row);
            if (rowNum >= 1 && rowNum <= 30) {
                if (rowNum <= 3) pClass = "First";
                else if (rowNum <= 10) pClass = "Business";
                else pClass = "Economy";
                cout << ">> System detected Class: " << pClass << endl;
                break; 
            } else {
                cout << "Error: Row must be between 1 and 30.\n";
            }
        } catch (...) {
            cout << "Error: Please enter a valid number.\n";
        }
    }

    // 4. Get Column
    while (true) {
        cout << "Enter Col (A-F): "; cin >> col;
        char c = toupper(col[0]);
        if (col.length() == 1 && c >= 'A' && c <= 'F') {
            col = string(1, c); 
            break; 
        } else {
            cout << "Error: Invalid Seat. Only A-F allowed." << endl;
        }
    }

    // 5. Find Flight
    int r = stoi(row) - 1;
    int c = toupper(col[0]) - 'A';
    int flight = 1;

    while (flight <= 100) {
        if (!seatRegistry[flight][r][c]) {
            seatRegistry[flight][r][c] = true;
            break;
        }
        flight++;
    }
    cout << ">> Assigned to Flight " << flight << endl;

    Passenger p = {id, name, row, col, pClass, flight};

    cout << "\nAdding Passenger to System..." << endl;

    // --- MEASURE ARRAY ADD ---
    auto start = high_resolution_clock::now();
    globalArray.addPassenger(p); 
    auto stop = high_resolution_clock::now();
    auto arrayTime = duration_cast<nanoseconds>(stop - start).count();
    
    globalArray.insertionSort(); // Sort immediately

    // --- MEASURE LIST ADD ---
    start = high_resolution_clock::now();
    globalList.addPassenger(p);
    stop = high_resolution_clock::now();
    auto listTime = duration_cast<nanoseconds>(stop - start).count();

    globalList.insertionSort(); // Sort immediately

    cout << "\n=== ADD RESULTS ===" << endl;
    cout << "Array Add Time:       " << arrayTime << " nanoseconds" << endl;
    cout << "Linked List Add Time: " << listTime << " nanoseconds" << endl;
}

// =============================================================
// OPTION 3: SEARCH
// =============================================================
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
        cout << "Enter Passenger ID: "; cin >> query;
        cout << "\n[ Binary Search Benchmark ]" << endl;
        
        auto start = high_resolution_clock::now();
        Passenger* p1 = globalArray.binarySearch(query);
        auto stop = high_resolution_clock::now();
        auto t1 = duration_cast<nanoseconds>(stop - start).count();

        start = high_resolution_clock::now();
        Passenger* p2 = globalList.binarySearch(query);
        stop = high_resolution_clock::now();
        auto t2 = duration_cast<nanoseconds>(stop - start).count();

        if(p1) cout << "Found: " << p1->name << endl;
        else cout << "Not Found." << endl;

        cout << "Time: Array=" << t1 << "ns | List=" << t2 << "ns" << endl;
    }
    else if (choice == 2) {
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
        cout << "Enter Col (A-F): "; cin >> query;
        if(query.length() > 0) query[0] = toupper(query[0]);

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

// =============================================================
// OPTION 4: DISPLAY ALL
// =============================================================
void displayAllPassengers() {
    int choice;
    cout << "\n=== DISPLAY ALL PASSENGERS (TRAVERSAL) ===" << endl;
    cout << "1. Display using Array Traversal" << endl;
    cout << "2. Display using Linked List Traversal" << endl;
    cout << "Enter choice: ";
    cin >> choice;

    if (choice == 1) globalArray.displayAll();
    else if (choice == 2) globalList.displayAll();
    else cout << "Invalid choice.\n";
}

// =============================================================
// OPTION 6: MEMORY COMPARISON
// =============================================================
void compareMemory(){
    cout << "\n=== MEMORY USAGE COMPARISON ===\n";
    
    size_t arrayMem = globalArray.getMemoryUsage();
    size_t listMem = globalList.getMemoryUsage();

    double arrayKB = arrayMem / 1024.0;
    double listKB = listMem / 1024.0;

    cout << "1. Array Memory Usage:       " << arrayMem << " bytes (" << arrayKB << " KB)" << endl;
    cout << "   Reason: Array pre-allocates all 18,000 seats immediately.\n" << endl;
    
    cout << "2. Linked List Memory Usage: " << listMem << " bytes (" << listKB << " KB)" << endl;
    cout << "   Reason: List only allocates memory per added passenger.\n" << endl;
    
    cout << "------------------------------------------------" << endl;

    if(arrayMem > listMem){
        cout << ">> WINNER: Linked List is more memory efficient." << endl;
    } else {
        cout << ">> WINNER: Array is more memory efficient." << endl;
    }
}

// =============================================================
// MAIN EXECUTION
// =============================================================
int main() {
    // Load Data Once at Startup
    loadFileToBuffer("flight_passenger_data.csv");

    int choice;
    do {
        cout << "\n=== AIRLINE SYSTEM MENU ===" << endl;
        cout << "1. Run Bulk Benchmark (Import data)" << endl;
        cout << "2. Add New Passenger" << endl;
        cout << "3. Search Passenger (by Row/Column)" << endl;
        cout << "4. Display All Passengers" << endl;
        cout << "5. Display Seating Chart (by Flight Number)" << endl;
        cout << "6. Display Memory Usage" << endl;
        cout << "7. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch(choice) {
            case 1: runImportBenchmark(); break;
            case 2: addNewPassenger(); break;
            case 3: searchPassenger(); break;
            case 4: displayAllPassengers(); break;
            case 5: {
                int flightNum;
                cout << "Enter Flight Number (1-100): ";
                cin >> flightNum;
                globalArray.displaySeatingChart(flightNum);
                break;
            }
            case 6: compareMemory(); break;
            case 7: 
                choice = 0; // Exit loop
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