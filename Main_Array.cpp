#include <iostream>
#include <fstream>  // Required for file handling
#include <sstream>  // Required for splitting CSV strings
#include <string>
#include "ArrayFlight.hpp" // Links your array logic

using namespace std;

// Function to split a CSV line and add it to the system
void loadData(ArrayFlight& flightSystem, string filename) {
    ifstream file(filename);
    
    if (!file.is_open()) {
        cout << "Error: Could not open file " << filename << endl;
        return;
    }

    string line;
    // Skip the header line (assuming first row is headers)
    getline(file, line); 

    int count = 0;
    while (getline(file, line)) {
        stringstream ss(line);
        string id, name, row, col, pClass;

        // Parse CSV line: ID, Name, Row, Column, Class
        // Note: This assumes the CSV format matches the assignment
        getline(ss, id, ',');
        getline(ss, name, ',');
        getline(ss, row, ',');
        getline(ss, col, ',');
        getline(ss, pClass, ',');

        // Attempt to add to the system
        // Note: You might need to trim whitespace depending on your CSV format
        if (!flightSystem.addPassengerSilent(id, name, row, col, pClass)) {
            //cout << "Warning: Could not add " << name << " (Duplicate or Full)" << endl;
        } else {
            count++;
        }
    }
    
    cout << "\nData Loading Complete. " << count << " passengers loaded." << endl;
    file.close();
}

void printMenu() {
    cout << "\n--- FLIGHT RESERVATION SYSTEM (Array Implementation) ---" << endl;
    cout << "1. View Seating Map" << endl;
    cout << "2. View Passenger Manifest" << endl;
    cout << "3. Search for Passenger" << endl;
    cout << "4. Add New Reservation" << endl;
    cout << "5. Remove Reservation" << endl;
    cout << "6. Exit" << endl;
    cout << "Enter choice: ";
}

int main() {
    // 1. Initialize your System
    ArrayFlight flight; 

    // 2. Load Initial Data [Required by Assignment]
    cout << "Initializing System..." << endl;
    loadData(flight, "flight_passenger_data.csv");

    int choice;
    string id, name, row, col, pClass;

    // 3. Main Loop
    do {
        printMenu();
        if (!(cin >> choice)) { // Error handling for non-integer input
            cout << "Invalid input. Please enter a number." << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        switch (choice) {
        case 1: // View Map
            flight.displaySeatMap();
            break;

        case 2: // View List
            flight.displayManifest();
            break;

        case 3: // Search
            cout << "Enter Passenger ID to Search: ";
            cin >> id;
            {
                Passenger* p = flight.searchPassenger(id);
                if (p != nullptr) {
                    cout << "\n[RESULT FOUND]" << endl;
                    cout << "Name: " << p->name << endl;
                    cout << "Seat: " << p->seatRow << p->seatColumn << endl;
                    cout << "Class: " << p->pClass << endl;
                } else {
                    cout << "\n[RESULT NOT FOUND] Passenger ID does not exist." << endl;
                }
            }
            break;

        case 4: // Add Reservation
            cout << "Enter New ID: "; cin >> id;
            cout << "Enter Name: "; cin.ignore(); getline(cin, name);
            cout << "Enter Row (1-20): "; cin >> row;
            cout << "Enter Column (A-F): "; cin >> col;
            cout << "Enter Class: "; cin >> pClass;

            if (flight.addPassenger(id, name, row, col, pClass)) {
                cout << "Reservation Successful." << endl;
            } else {
                cout << "Reservation Failed: Seat occupied or invalid." << endl;
            }
            break;

        case 5: // Cancel
            cout << "Enter Passenger ID to Cancel: ";
            cin >> id;
            if (flight.removePassenger(id)) {
                cout << "Cancellation processed successfully." << endl;
            } else {
                cout << "Cancellation Failed: ID not found." << endl;
            }
            break;

        case 6:
            cout << "Exiting the system. ByeBye!" << endl;
            choice=0;
            break;

        default:
            cout << "Invalid option. Try again." << endl;
        }

    } while (choice != 0);

    return 0;
}