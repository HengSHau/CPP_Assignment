#include "LinkedList.hpp"

FlightLinkedList::FlightLinkedList() { 
    head = nullptr; 
}

// Simple Add (We sort LATER to measure time)
void FlightLinkedList::addPassenger(Passenger p) {
    Node* newNode = new Node();
    newNode->data = p;
    newNode->next = head;
    head = newNode;
}

// === MATCHING ALGORITHM: INSERTION SORT FOR LINKED LIST ===
void FlightLinkedList::insertionSort() {
    if(!head || !head->next) return; // 0 or 1 element

    Node* sorted = nullptr;
    Node* current = head;
    
    while (current != nullptr) {
        Node* next = current->next;
        
        // Insert current into sorted list
        if (sorted == nullptr || sorted->data.passengerID >= current->data.passengerID) {
            current->next = sorted;
            sorted = current;
        } else {
            Node* temp = sorted;
            while (temp->next != nullptr && temp->next->data.passengerID < current->data.passengerID) {
                temp = temp->next;
            }
            current->next = temp->next;
            temp->next = current;
        }
        
        current = next;
    }
    head = sorted;
}

// Helper for Binary Search
Node* getMiddle(Node* start, Node* last) {
    if (start == nullptr) return nullptr;
    
    Node* slow = start;
    Node* fast = start->next;
    
    while (fast != last) {
        fast = fast->next;
        if (fast != last) {
            slow = slow->next;
            fast = fast->next;
        }
    }
    return slow;
}

// === BINARY SEARCH (For ID) ===
Passenger* FlightLinkedList::binarySearch(string id) {
    Node* start = head;
    Node* last = nullptr; // 'last' acts like the 'end' index
    
    do {
        // Find middle node
        Node* mid = getMiddle(start, last);
        
        if (mid == nullptr) return nullptr;
        
        // 1. Check Middle
        if (mid->data.passengerID == id) {
            return &mid->data; // Found
        }
        
        // 2. If middle is smaller, ignore left half -> start from mid->next
        else if (mid->data.passengerID < id) {
            start = mid->next;
        }
        
        // 3. If middle is larger, ignore right half -> end at mid
        else {
            last = mid;
        }
        
    } while (last == nullptr || last != start);
    
    return nullptr; // Not Found
}

// === LINEAR SEARCH (For Row) ===
void FlightLinkedList::searchByRow(string row) {
    Node* current = head;
    bool found = false;
    cout << "   [List Results for Row " << row << "]:\n";
    while (current != nullptr) {
        if (current->data.seatRow == row) {
            // ADDED: Flight Number display
            cout << "   - Found: " << current->data.name 
                 << " (ID: " << current->data.passengerID 
                 << ", Flight: " << current->data.flightNumber << ")\n";
            found = true;
        }
        current = current->next;
    }
    if (!found) cout << "   - No passengers found.\n";
}

// === LINEAR SEARCH (For Column) ===
void FlightLinkedList::searchByColumn(string col) {
    Node* current = head;
    bool found = false;
    cout << "   [List Results for Col " << col << "]:\n";
    while (current != nullptr) {
        if (current->data.seatColumn == col) {
            // ADDED: Flight Number display
            cout << "   - Found: " << current->data.name 
                 << " (ID: " << current->data.passengerID 
                 << ", Flight: " << current->data.flightNumber << ")\n";
            found = true;
        }
        current = current->next;
    }
    if (!found) cout << "   - No passengers found.\n";
}