#include "LinkedList.hpp"
#include <iomanip>
#include <iostream>

using namespace std;

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

// === REMOVE PASSENGER ===
bool FlightLinkedList::removePassenger(string id) {
    if (head == nullptr) return false;

    // Case 1: Removing the Head (First Node)
    if (head->data.passengerID == id) {
        Node* temp = head;
        head = head->next; // Move head to next
        delete temp;       // Delete old head
        return true;
    }

    // Case 2: Removing a Middle or Last Node
    Node* current = head;
    Node* prev = nullptr;

    while (current != nullptr && current->data.passengerID != id) {
        prev = current;
        current = current->next;
    }

    if (current == nullptr) return false; // Not found

    // Unlink the node
    prev->next = current->next;
    delete current;
    return true;
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
    
    return nullptr; 
}

// LINEAR SEARCH 
void FlightLinkedList::searchByRow(string row) {
    Node* current = head;
    bool found = false;
    cout << "   [List Results for Row " << row << "]:\n";
    while (current != nullptr) {
        if (current->data.seatRow == row) {
            cout << "   - Found: " << current->data.name 
                 << " (ID: " << current->data.passengerID 
                 << ", Flight: " << current->data.flightNumber << ")\n";
            found = true;
        }
        current = current->next;
    }
    if (!found) cout << "   - No passengers found.\n";
}

void FlightLinkedList::searchByColumn(string col) {
    Node* current = head;
    bool found = false;
    cout << "   [List Results for Col " << col << "]:\n";
    while (current != nullptr) {
        if (current->data.seatColumn == col) {
            cout << "   - Found: " << current->data.name 
                 << " (ID: " << current->data.passengerID 
                 << ", Flight: " << current->data.flightNumber << ")\n";
            found = true;
        }
        current = current->next;
    }
    if (!found) cout << "   - No passengers found.\n";
}

// === TRAVERSAL (Display All) ===
void FlightLinkedList::displayAll() {
    cout << "\n=== ALL PASSENGERS (Linked List Traversal) ===\n";
    
    // Header
    cout << left << setw(10) << "ID" 
         << setw(20) << "Name" 
         << setw(10) << "Seat" 
         << setw(12) << "Class" 
         << setw(10) << "Flight" << endl;

    cout << "------------------------------------------------------------\n";

    Node* current = head; 
    int count = 0;

    while (current != nullptr) {
        // NOTE: I used 'passengerClass' to match ArrayFlight. 
        // If your struct uses 'pClass', change this back to 'pClass'.
        cout << left << setw(10) << current->data.passengerID 
             << setw(20) << current->data.name 
             << setw(10) << (current->data.seatRow + current->data.seatColumn)
             << setw(12) << current->data.pClass 
             << setw(10) << current->data.flightNumber << endl;
        
        current = current->next; 
        count++;
    }
    cout << "------------------------------------------------------------\n";
    cout << "Total Passengers: " << count << endl;
}


// === SEATING CHART (Visual Grid) ===
void FlightLinkedList::displaySeatingChart(int flightNum) {
    if (flightNum < 1 || flightNum > 100) {
        cout << "Error: Invalid Flight Number (1-100).\n";
        return;
    }

    // 1. Create a temporary mini-grid for just this flight (30 rows, 6 cols)
    // Initialize all to false (Empty)
    bool tempGrid[30][6] = {false};

    // 2. Scan the list and mark occupied seats
    Node* current = head;
    while (current != nullptr) {
        if (current->data.flightNumber == flightNum) {
            // Parse Row (String to Index)
            int r = stoi(current->data.seatRow) - 1;
            // Parse Col (Char to Index)
            int c = toupper(current->data.seatColumn[0]) - 'A';
            
            if (r >= 0 && r < 30 && c >= 0 && c < 6) {
                tempGrid[r][c] = true; // Mark as occupied
            }
        }
        current = current->next;
    }

    // 3. Print the Grid
    cout << "\n=== SEATING CHART: FLIGHT " << flightNum << " ===\n";
    cout << "      A   B   C   D   E   F\n";
    cout << "    -------------------------\n";

    for (int i = 0; i < 30; i++) {
        cout << left << setw(3) << (i + 1) << "|"; // Row Number
        for (int j = 0; j < 6; j++) {
            if (tempGrid[i][j]) {
                cout << "[X] ";
            } else {
                cout << "[ ] ";
            }
        }
        cout << endl;
    }
    cout << "    -------------------------\n";
    cout << "    [X] = Occupied, [ ] = Empty\n";
}

size_t FlightLinkedList::getMemoryUsage() {
    size_t total = 0;
    Node* current = head;
    while(current != nullptr) {
        total += sizeof(Node);
        current = current->next;
    }
    return total;
}

void FlightLinkedList::bubbleSort() {
    if (!head) return;
    bool swapped;
    Node* ptr1;
    Node* lptr = nullptr;

    do {
        swapped = false;
        ptr1 = head;

        while (ptr1->next != lptr) {
            if (ptr1->data.passengerID > ptr1->next->data.passengerID) {
                // Swap Data
                Passenger temp = ptr1->data;
                ptr1->data = ptr1->next->data;
                ptr1->next->data = temp;
                swapped = true;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
}

void FlightLinkedList::selectionSort() {
    Node* temp=head;
    while(temp){
        Node*min=temp;
        Node*r=temp->next;
        while(r){
            if(min->data.passengerID> r->data.passengerID){
                min=r;
            }
            r=r->next;
        }
        Passenger p=temp->data;
        temp->data=min->data;
        min->data=p;
        temp=temp->next;
    }
}


//MergeSort
void splitList(Node* source, Node** frontRef, Node** backRef) {
    Node* fast;
    Node* slow;
    slow = source;
    fast = source->next;

    while (fast != nullptr) {
        fast = fast->next;
        if (fast != nullptr) {
            slow = slow->next;
            fast = fast->next;
        }
    }
    *frontRef = source;
    *backRef = slow->next;
    slow->next = nullptr;
}

Node* sortedMerge(Node* a, Node* b) {
    Node* result = nullptr;

    if (a == nullptr) return b;
    if (b == nullptr) return a;

    if (a->data.passengerID <= b->data.passengerID) {
        result = a;
        result->next = sortedMerge(a->next, b);
    } else {
        result = b;
        result->next = sortedMerge(a, b->next);
    }
    return result;
}

void mergeSortList(Node** headRef) {
    Node* head = *headRef;
    Node* a;
    Node* b;

    if ((head == nullptr) || (head->next == nullptr)) return;

    splitList(head, &a, &b);
    mergeSortList(&a);
    mergeSortList(&b);
    *headRef = sortedMerge(a, b);
}

// Public Function
void FlightLinkedList::mergeSort() {
    mergeSortList(&head);
}