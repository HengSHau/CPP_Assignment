#include "LinkedList.hpp"
#include <iostream>
#include <iomanip>

using namespace std;

// Constructor: Start with an empty list
FlightLinkedList::FlightLinkedList() {
    head = nullptr;
}

// Destructor: Clean up all nodes when program ends
FlightLinkedList::~FlightLinkedList() {
    Node* current = head;
    while (current != nullptr) {
        Node* nextNode = current->next;
        delete current;
        current = nextNode;
    }
}

void FlightLinkedList::sortPassengers() {
    Node* current = head;
    Node* sorted = nullptr; // This will become our new sorted list

    while (current != nullptr) {
        // Save the next node so we don't lose track of the old list
        Node* next = current->next;

        // Insert 'current' into the 'sorted' list
        sortedInsert(sorted, current);

        // Move to the next item
        current = next;
    }

    // Update the real head to point to the new sorted list
    head = sorted;
    cout << "--> System: Linked List has been sorted by ID (Insertion Sort)." << endl;
}

// 2. HELPER FUNCTION (Finds the right spot)
void FlightLinkedList::sortedInsert(Node*& head_ref, Node* newNode) {
    Node* current;

    // Case 1: The list is empty OR the new node goes at the very FRONT (Smallest ID)
    if (head_ref == nullptr || newNode->data.passengerID < head_ref->data.passengerID) {
        newNode->next = head_ref;
        head_ref = newNode;
    }
    else {
        // Case 2: Traverse the list to find the insertion point
        current = head_ref;
        while (current->next != nullptr && 
               current->next->data.passengerID < newNode->data.passengerID) {
            current = current->next;
        }
        
        // Insert newNode after 'current'
        newNode->next = current->next;
        current->next = newNode;
    }
}

// TODO: Implement Reservation (Add Passenger)
bool FlightLinkedList::addPassenger(Passenger p) {
    if(isSeatOccupied(p.seatRow, p.seatColumn)) {
       // cout<<"Error: Seat "<<p.seatRow<<p.seatColumn<<" is already occupied!"<<endl;

        return false; // Seat already taken
    }

    if(searchPassenger(p.passengerID) != nullptr) {
        cout<<"Error: Passenger ID "<<p.passengerID<<" already exists!"<<endl;
        return false; // Duplicate ID
    }

    Node* newNode=new Node();
    newNode->data=p;
    newNode->next=head;
    head=newNode;

    return true; 
}

// TODO: Implement Cancellation (Delete Passenger)
bool FlightLinkedList::removePassenger(string id) {
    // 1. check for empty list
    if(head == nullptr){
        cout << "Error: The list is empty." << endl;
        return false; 
    }

    // 2. check head node
    if(head->data.passengerID == id){
        Node* nodeToDelete = head;
        head = head->next;
        delete nodeToDelete;
        cout << "Success: Passenger " << id << " is removed." << endl;
        return true;
    }

    Node* current = head;
    Node* previous = nullptr;

    // 3. loop to find passenger
    while(current != nullptr && current->data.passengerID != id){
        previous = current;       
        current = current->next;  
    }

    // 4. check if passenger found
    if(current == nullptr){
        cout << "Error: Passenger ID " << id << " not found." << endl;
        return false;
    }

    // 5. perform delete
    previous->next = current->next; 
    delete current;                 

    cout << "Success: Passenger " << id << " is removed." << endl;
    return true;
}

// TODO: Implement Search
Passenger* FlightLinkedList::searchPassenger(string id) {
    Node* current = head;
    while (current != nullptr) {
        if (current->data.passengerID == id) {
            return &(current->data); // Found it!
        }
        current = current->next;
    }
    return nullptr; // Not found
}

Passenger* FlightLinkedList::searchPassengerBinary(const std::string& id) {
    Node* start = head;
    Node* end = nullptr; // end is exclusive: search range is [start, end)

    while (start != end) {
        // find middle between start and end
        Node* slow = start;
        Node* fast = start;
        while (fast != end && fast->next != end) {
            fast = fast->next->next;
            slow = slow->next;
        }

        if (slow->data.passengerID == id) return &slow->data;
        if (slow->data.passengerID < id) start = slow->next;
        else end = slow;
    }
    return nullptr;
}

// TODO: Implement Manifest
void FlightLinkedList::displayManifest() {

    if(head == nullptr) {
        cout << "\nFlightpassenger manifest is empty.\n" << endl;
        return;
    }

    Node* current = head;

    int count=0;

    cout<<"\n========================================================" <<endl;
    cout<<"               Flight Passenger Manifest                   "<<endl;
    cout<<"\n========================================================" <<endl;
    cout<<left<<setw(12)<<"PassengerID"<<setw(25)<<"Name"<<setw(8)<<"Seat"<<setw(15)<<"Class"<<endl;
    cout<<"--------------------------------------------------------" <<endl;
    
    while (current != nullptr) {
        count++;
        string fullSeat=current->data.seatRow+current->data.seatColumn;
        cout<<left<<setw(12)<<current->data.passengerID<<setw(25)<<current->data.name<<setw(8)<<fullSeat<<setw(15)<<current->data.pClass<<endl;
        current=current->next;
    }
    cout<<"========================================================" <<endl;
    cout<<"Total Passengers on board: "<<count<<endl;
    cout<<"========================================================" <<endl;
}

void FlightLinkedList::displaySeatingChart() {
    int totalRows = 30; 
    char cols[] = {'A', 'B', 'C', 'D', 'E', 'F'};

    cout << "\n============= SEATING CHART =============" << endl;
    cout << "         A   B   C      D   E   F" << endl; 

    for (int r = 1; r <= totalRows; r++) {
        // Formatting row numbers (e.g., "Row 1 " vs "Row 10")
        if(r < 10) cout << "Row " << r << " : ";
        else       cout << "Row " << r << ": ";

        // Loop through 6 columns (A-F)
        for (int c = 0; c < 6; c++) {
            string rowStr = to_string(r);      // Convert int 1 to string "1"
            string colStr(1, cols[c]);         // Convert char 'A' to string "A"

            // Add an aisle gap between C and D
            if (c == 3) cout << "   "; 

            // Check if someone is sitting here
            if (isSeatOccupied(rowStr, colStr)) {
                cout << "[X] "; // Occupied
            } else {
                cout << "[_] "; // Empty
            }
        }
        cout << endl; // Move to next row
    }
    cout << "=========================================\n" << endl;
}

bool FlightLinkedList::isSeatOccupied(string row, string col) {
    Node* current = head; // Start at the front
    
    // Loop through every node in the list
    while (current != nullptr) {
        // If row AND column match, the seat is taken
        if (current->data.seatRow == row && current->data.seatColumn == col) {
            return true; 
        }
        current = current->next; // Move to the next node
    }
    
    return false; // We checked everyone, and the seat is free
}

void FlightLinkedList::filterByRow(string targetRow) {
    if (head == nullptr) {
        cout << "List is empty." << endl;
        return;
    }

    cout << "\n--- PASSENGERS IN ROW " << targetRow << " ---" << endl;
    Node* current = head;
    bool found = false;

    // We must traverse the ENTIRE list because passengers are scattered
    while (current != nullptr) {
        if (current->data.seatRow == targetRow) {
            cout << "[Seat " << current->data.seatRow << current->data.seatColumn << "] " 
                 << current->data.name 
                 << " (ID: " << current->data.passengerID << ")" << endl;
            found = true;
        }
        current = current->next;
    }

    if (!found) cout << "(No passengers found in this row)" << endl;
    cout << "-----------------------------" << endl;
}

void FlightLinkedList::filterByColumn(string targetCol) {
    if (head == nullptr) {
        cout << "List is empty." << endl;
        return;
    }

    // Ensure target is uppercase for comparison
    char targetChar = toupper(targetCol[0]);
    string colStr(1, targetChar);

    cout << "\n--- PASSENGERS IN COLUMN " << colStr << " ---" << endl;
    Node* current = head;
    bool found = false;

    while (current != nullptr) {
        // Compare the first character of the column string
        char seatColChar = toupper(current->data.seatColumn[0]);
        
        if (seatColChar == targetChar) {
            cout << "[Seat " << current->data.seatRow << current->data.seatColumn << "] " 
                 << current->data.name 
                 << " (ID: " << current->data.passengerID << ")" << endl;
            found = true;
        }
        current = current->next;
    }

    if (!found) cout << "(No passengers found in this column)" << endl;
    cout << "--------------------------------" << endl;
}