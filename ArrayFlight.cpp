#include "ArrayFlight.hpp"
#include<iomanip>


using namespace std;

ArrayFlight::ArrayFlight() {
    for(int i=0; i<TOTAL_ROWS; i++)
        for(int j=0; j<COLS; j++)
            seatMap[i][j].passengerID = "";
}

void ArrayFlight::addPassenger(Passenger p) {
    // 1. Convert "1" -> 0
    int r = stoi(p.seatRow) - 1;
    
    // 2. Convert "A" -> 0
    int c = toupper(p.seatColumn[0]) - 'A';
    
    // 3. Calculate Global Index based on Flight Number
    // Flight 1 starts at 0, Flight 2 starts at 30, etc.
    int globalRow = ((p.flightNumber - 1) * 30) + r;

    // 4. Write data to memory
    if(globalRow >= 0 && globalRow < TOTAL_ROWS) {
        seatMap[globalRow][c] = p;
    }
}


void ArrayFlight::insertionSort() {
    int n = TOTAL_ROWS * COLS;
    
    for (int i = 1; i < n; i++) {
        // Convert 'i' to 2D coords
        int r_curr = i / COLS;
        int c_curr = i % COLS;
        
        // Skip empty seats (optimization)
        if (seatMap[r_curr][c_curr].passengerID == "") continue;

        Passenger key = seatMap[r_curr][c_curr];
        int j = i - 1;

        // Move elements that are greater than key to one position ahead
        while (j >= 0) {
            int r_prev = j / COLS;
            int c_prev = j % COLS;
            
            // If prev is empty, or prev > key, we swap/move
            // Note: We want to pack empty seats to the end, or sort IDs
            if (seatMap[r_prev][c_prev].passengerID == "" || 
               (seatMap[r_prev][c_prev].passengerID > key.passengerID && seatMap[r_prev][c_prev].passengerID != "")) {
                
                // Shift forward
                int r_next = (j + 1) / COLS;
                int c_next = (j + 1) % COLS;
                seatMap[r_next][c_next] = seatMap[r_prev][c_prev];
                
                j = j - 1;
            } else {
                break;
            }
            int r_dest=(j+1)/COLS;
            int c_dest=(j+1)%COLS;
            seatMap[r_dest][c_dest]=key;
        }
        
        // Place key
        int r_dest = (j + 1) / COLS;
        int c_dest = (j + 1) % COLS;
        seatMap[r_dest][c_dest] = key;
    }
}

Passenger* ArrayFlight::binarySearch(string id) {
    int left = 0;
    int right = (TOTAL_ROWS * COLS) - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        int r = mid / COLS;
        int c = mid % COLS;

        // Optimization: Insertion sort pushes empty strings "" to the end.
        // If we hit "", we are too far right.
        if (seatMap[r][c].passengerID == "") {
            right = mid - 1;
        }
        else if (seatMap[r][c].passengerID == id) {
            return &seatMap[r][c]; // Found
        }
        else if (seatMap[r][c].passengerID < id) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }
    return nullptr; // Not Found
}

// === LINEAR SEARCH (For Row) ===
void ArrayFlight::searchByRow(string row) {
    bool found = false;
    cout << "   [Array Results for Row " << row << "]:\n";
    for (int i = 0; i < TOTAL_ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (seatMap[i][j].passengerID != "" && seatMap[i][j].seatRow == row) {
                // ADDED: Flight Number display
                cout << "   - Found: " << seatMap[i][j].name 
                     << " (ID: " << seatMap[i][j].passengerID 
                     << ", Flight: " << seatMap[i][j].flightNumber << ")\n";
                found = true;
            }
        }
    }
    if (!found) cout << "   - No passengers found.\n";
}

// === LINEAR SEARCH (For Column) ===
void ArrayFlight::searchByColumn(string col) {
    bool found = false;
    cout << "   [Array Results for Col " << col << "]:\n";
    for (int i = 0; i < TOTAL_ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (seatMap[i][j].passengerID != "" && seatMap[i][j].seatColumn == col) {
                // ADDED: Flight Number display
                cout << "   - Found: " << seatMap[i][j].name 
                     << " (ID: " << seatMap[i][j].passengerID 
                     << ", Flight: " << seatMap[i][j].flightNumber << ")\n";
                found = true;
            }
        }
    }
    if (!found) cout << "   - No passengers found.\n";
}

// === TRAVERSAL (Display All) ===
void ArrayFlight::displayAll() {
    cout << "\n=== ALL PASSENGERS (Array Traversal) ===\n";
    
    // Fixed Header Spacing
    cout << left<<setw(10)<<"ID"<<setw(20)<<"Name"<<setw(10)<<"Seat"<<setw(12)<<"Class"<<setw(10)<<"Flight"<<endl;
         
    cout << "------------------------------------------------------------\n";
    
    int count = 0;
    for (int i = 0; i < TOTAL_ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (seatMap[i][j].passengerID != "") {
                cout<<left<<setw(10)<<seatMap[i][j].passengerID<<setw(20)<<seatMap[i][j].name<<setw(10)<<(seatMap[i][j].seatRow + seatMap[i][j].seatColumn)<<setw(12) << seatMap[i][j].pClass<<setw(10) << seatMap[i][j].flightNumber << endl;
                count++;
            }
        }
    }
    cout << "------------------------------------------------------------\n";
    cout << "Total Passengers: " << count << endl;
}

// === SEATING CHART (Visual Grid) ===
void ArrayFlight::displaySeatingChart(int flightNum) {
    if (flightNum<1 || flightNum > 100) {
        cout << "Error: Invalid Flight Number (1-100).\n";
        return;
    }

    cout<<"\n=== SEATING CHART: FLIGHT "<<flightNum<<" ===\n";
    cout<<"     A   B   C   D   E   F\n"; // Header
    cout<<"    -------------------------\n";

    // Calculate where this flight starts in the big array
    // Flight 1 starts at row 0. Flight 2 starts at row 30.
    int startRow = (flightNum - 1) * 30;
    int endRow = startRow + 30;

    for (int i = startRow; i < endRow; i++) {
        // Print Row Number (e.g., "1 ", "10")
        int actualRow=(i % 30) + 1;
        cout<<left << setw(3)<<actualRow<<"|";

        for (int j = 0; j < COLS; j++) {
            if (seatMap[i][j].passengerID != "") {
                cout << "[X] "; // Occupied
            } else {
                cout << "[ ] "; // Empty
            }
        }
        cout << endl;
    }
    cout << "    -------------------------\n";
    cout << "    [X] = Occupied, [ ] = Empty\n";
}

size_t ArrayFlight::getMemoryUsage() {
    return sizeof(seatMap);
}

