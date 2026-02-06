#include "ArrayFlight.hpp"
#include <iostream>
#include <iomanip>
#include <string> // Added for stoi

using namespace std;

ArrayFlight::ArrayFlight() {
    for(int i=0; i<TOTAL_ROWS; i++)
        for(int j=0; j<COLS; j++)
            seatMap[i][j].passengerID = "";
}

void ArrayFlight::addPassenger(Passenger p) {
    int r = stoi(p.seatRow) - 1;
    int c = toupper(p.seatColumn[0]) - 'A';
    int globalRow = ((p.flightNumber - 1) * 30) + r;

    if(globalRow >= 0 && globalRow < TOTAL_ROWS) {
        seatMap[globalRow][c] = p;
    }
}

bool ArrayFlight::removePassenger(string id) {
    for (int i = 0; i < TOTAL_ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (seatMap[i][j].passengerID == id) {
                seatMap[i][j].passengerID = "";
                seatMap[i][j].name = "";
                seatMap[i][j].flightNumber = 0;
                return true; 
            }
        }
    }
    return false; 
}

void ArrayFlight::insertionSort() {
    int n = TOTAL_ROWS * COLS;
    for (int i = 1; i < n; i++) {
        int r_curr = i / COLS; int c_curr = i % COLS;
        if (seatMap[r_curr][c_curr].passengerID == "") continue;

        Passenger key = seatMap[r_curr][c_curr];
        int j = i - 1;
        while (j >= 0) {
            int r_prev = j / COLS; int c_prev = j % COLS;
            if (seatMap[r_prev][c_prev].passengerID == "" || 
               (seatMap[r_prev][c_prev].passengerID > key.passengerID && seatMap[r_prev][c_prev].passengerID != "")) {
                int r_next = (j + 1) / COLS; int c_next = (j + 1) % COLS;
                seatMap[r_next][c_next] = seatMap[r_prev][c_prev];
                j = j - 1;
            } else { break; }
        }
        int r_dest = (j + 1) / COLS; int c_dest = (j + 1) % COLS;
        seatMap[r_dest][c_dest] = key;
    }
}

Passenger* ArrayFlight::binarySearch(string id) {
    int left = 0;
    int right = (TOTAL_ROWS * COLS) - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        int r = mid / COLS; int c = mid % COLS;

        if (seatMap[r][c].passengerID == "") right = mid - 1;
        else if (seatMap[r][c].passengerID == id) return &seatMap[r][c];
        else if (seatMap[r][c].passengerID < id) left = mid + 1;
        else right = mid - 1;
    }
    return nullptr;
}

void ArrayFlight::searchByRow(string row) {
    bool found = false;
    cout << "   [Array Results for Row " << row << "]:\n";
    for (int i = 0; i < TOTAL_ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (seatMap[i][j].passengerID != "" && seatMap[i][j].seatRow == row) {
                cout << "   - Found: " << seatMap[i][j].name 
                     << " (ID: " << seatMap[i][j].passengerID 
                     << ", Flight: " << seatMap[i][j].flightNumber << ")\n";
                found = true;
            }
        }
    }
    if (!found) cout << "   - No passengers found.\n";
}

void ArrayFlight::searchByColumn(string col) {
    bool found = false;
    cout << "   [Array Results for Col " << col << "]:\n";
    for (int i = 0; i < TOTAL_ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (seatMap[i][j].passengerID != "" && seatMap[i][j].seatColumn == col) {
                cout << "   - Found: " << seatMap[i][j].name 
                     << " (ID: " << seatMap[i][j].passengerID 
                     << ", Flight: " << seatMap[i][j].flightNumber << ")\n";
                found = true;
            }
        }
    }
    if (!found) cout << "   - No passengers found.\n";
}

void ArrayFlight::displayAll() {
    cout << "\n=== ALL PASSENGERS (Array Traversal) ===\n";
    cout << left << setw(10) << "ID" << setw(20) << "Name" << setw(10) << "Seat" << setw(12) << "Class" << setw(10) << "Flight" << endl;
    cout << "------------------------------------------------------------\n";
    int count = 0;
    for (int i = 0; i < TOTAL_ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (seatMap[i][j].passengerID != "") {
                // FIXED: Using pClass here
                cout << left << setw(10) << seatMap[i][j].passengerID 
                     << setw(20) << seatMap[i][j].name 
                     << setw(10) << (seatMap[i][j].seatRow + seatMap[i][j].seatColumn)
                     << setw(12) << seatMap[i][j].pClass  
                     << setw(10) << seatMap[i][j].flightNumber << endl;
                count++;
            }
        }
    }
    cout << "------------------------------------------------------------\n";
    cout << "Total Passengers: " << count << endl;
}

void ArrayFlight::displaySeatingChart(int flightNum) {
    if (flightNum < 1 || flightNum > 100) { cout << "Error: Invalid Flight.\n"; return; }

    bool tempGrid[30][6] = {false};
    for (int i = 0; i < TOTAL_ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (seatMap[i][j].passengerID != "" && seatMap[i][j].flightNumber == flightNum) {
                try {
                    int r = stoi(seatMap[i][j].seatRow) - 1;
                    int c = toupper(seatMap[i][j].seatColumn[0]) - 'A';
                    if (r >= 0 && r < 30 && c >= 0 && c < 6) tempGrid[r][c] = true;
                } catch(...) {}
            }
        }
    }

    cout << "\n=== SEATING CHART: FLIGHT " << flightNum << " ===\n";
    cout << "      A   B   C   D   E   F\n    -------------------------\n";
    for (int i = 0; i < 30; i++) {
        cout << left << setw(3) << (i + 1) << "|";
        for (int j = 0; j < 6; j++) cout << (tempGrid[i][j] ? "[X] " : "[ ] ");
        cout << endl;
    }
}

size_t ArrayFlight::getMemoryUsage() { 
    return sizeof(seatMap); 
}

void ArrayFlight::bubbleSort() {
    int n=TOTAL_ROWS*COLS;
    for(int i=0;i<n-1;i++){
        for(int j=0;j<n-1;j++){
            int r1=j/COLS; int c1=j%COLS;
            int r2=(j+1)/COLS; int c2=(j+1)%COLS;

            string id1=seatMap[r1][c1].passengerID;
            string id2=seatMap[r2][c2].passengerID;

            if(id1==""&&id2==""){
                continue;
            }
            if(id1==""){
                swap(seatMap[r1][c1],seatMap[r2][c2]);
            }
            else if(id2!=""&&id1>id2){
                swap(seatMap[r1][c1],seatMap[r2][c2]);
            }
        }
    }
}

void ArrayFlight::selectionSort(){
    int n=TOTAL_ROWS*COLS;
    for(int i=0;i<n-1;i++){
        int minIndex=i;
        int r_min=minIndex/COLS; int c_min=minIndex%COLS;
        for(int j=i+1;j<n;j++){
            int r_j=j/COLS; int c_j=j%COLS;

            string id_min=seatMap[r_min][c_min].passengerID;
            string id_j=seatMap[r_j][c_j].passengerID;

            if(id_j==""&&id_min==""){
                continue;
            }
            if(id_min==""){
                minIndex=j;
                r_min=minIndex/COLS; c_min=minIndex%COLS;
            }
            else if(id_j!=""&&id_j<id_min){
                minIndex=j;
                r_min=minIndex/COLS; c_min=minIndex%COLS;
            }
        }
        if(minIndex!=i){
            int r_i=i/COLS; int c_i=i%COLS;
            swap(seatMap[r_i][c_i],seatMap[r_min][c_min]);
        }
    }
}

void mergeArray(Passenger arr[],int left, int mid, int right){
    int n1=mid-left+1;
    int n2=right-mid;

    Passenger*L=new Passenger[n1];
    Passenger*R=new Passenger[n2];

    for(int i=0;i<n1;i++) L[i]=arr[left+i];
    for(int j=0;j<n2;j++) R[j]=arr[mid+1+j];

    int i=0,j=0,k=left;
    while(i<n1&&j<n2){
        if(L[i].passengerID==""){
            arr[k]=R[j];
            j++;
        }
        else if(R[j].passengerID==""){
            arr[k]=L[i];
            i++;
        }
        else if(L[i].passengerID<=R[j].passengerID){
            arr[k]=L[i];
            i++;
        }
        else{
            arr[k]=R[j];
            j++;
        }
    }
}

void mergeSortRec(Passenger arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSortRec(arr, left, mid);
        mergeSortRec(arr, mid + 1, right);
        mergeArray(arr, left, mid, right);
    }
}

void ArrayFlight::mergeSort() {
    // 1. Flatten 2D Map to 1D Array for easier sorting
    int total = TOTAL_ROWS * COLS;
    Passenger* flatArr = new Passenger[total];
    
    for(int i=0; i<total; i++) {
        flatArr[i] = seatMap[i/COLS][i%COLS];
    }

    // 2. Perform Sort
    mergeSortRec(flatArr, 0, total - 1);

    // 3. Put back into 2D Map
    for(int i=0; i<total; i++) {
        seatMap[i/COLS][i%COLS] = flatArr[i];
    }
    delete[] flatArr;
}