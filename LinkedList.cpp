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

// TODO: Implement Reservation (Add Passenger)
// Task: Create a new node, fill it with data, and link it to the head
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
// Task: Find the node with matching ID and remove it from the chain
bool FlightLinkedList::removePassenger(string id) {
    // 1. 检查空链表
    if(head == nullptr){
        cout << "Error: The list is empty." << endl;
        return false; 
    }

    // 2. 检查头节点 (Head)
    if(head->data.passengerID == id){
        Node* nodeToDelete = head;
        head = head->next;
        delete nodeToDelete;
        cout << "Success: Passenger " << id << " is removed." << endl;
        return true;
    }

    Node* current = head;
    Node* previous = nullptr;

    // 3. 循环查找 (标准写法)
    while(current != nullptr && current->data.passengerID != id){
        previous = current;       
        current = current->next;  
    }

    // 4. 检查是否找到
    if(current == nullptr){
        cout << "Error: Passenger ID " << id << " not found." << endl;
        return false;
    }

    // 5. 执行删除
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

// TODO: Implement Manifest
void FlightLinkedList::displayManifest() {
    Node* current = head;
    cout<<"\n========================================================" <<endl;
    cout<<"               Flight Passenger Manifest                   "<<endl;
    cout<<"\n========================================================" <<endl;
    cout<< "ID      | Name                      | Seat  | Class" << endl;    
    cout<<"--------------------------------------------------------" <<endl;
    
    while (current != nullptr) {
        string fullSeat=current->data.seatRow+current->data.seatColumn;
        cout<<left<<setw(12)<<current->data.passengerID<<setw(25)<<current->data.name<<setw(8)<<fullSeat<<setw(15)<<current->data.pClass<<endl;
        current=current->next;
    }
    cout<<"========================================================\n" <<endl;
}

void FlightLinkedList::displaySeatingChart() {
    int totalRows = 60; // Let's display 15 rows
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