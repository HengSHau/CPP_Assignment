#include "LinkedList.hpp"
#include <iostream>

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

// TODO: Implement Reservation
// Task: Create a new node, fill it with data, and link it to the head
bool FlightLinkedList::addPassenger(Passenger p) {
    if(isSeatOccupied(p.seatRow, p.seatColumn)) {
        cout<<"Error: Seat "<<p.seatRow<<p.seatColumn<<" is already occupied!"<<endl;
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

// TODO: Implement Cancellation
// Task: Find the node with matching ID and remove it from the chain
bool FlightLinkedList::removePassenger(string id) {
    if(head==nullptr){
        cout<<"Error: The list is empty. Cannot remove passenger."<<endl;
        return false; // List is empty
    }

    Node*current=head;
    Node*previous=nullptr;

    if(head->data.passengerID==id){
        Node*nodeToDelete=head;
        head=head->next;
        delete nodeToDelete;
        cout<<"Success: Passenger"<<id<<"is removed"<<endl;
        return true;
    }

    while(current !=nullptr&&current->data.passengerID!=id){
        previous=current;
        current=current->next;
    }

    if(current==nullptr){
        cout<<"Error: Passenger ID"<<id<<"not found."<<endl;
        return false;
    }
    previous->next=current->next;
    delete current;

    cout<<"Success: Passenger "<<id<<" is removed"<<endl;

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
    cout << "--- Passenger Manifest ---" << endl;
    while (current != nullptr) {
        // Print details here: ID, Name, Seat
        cout << current->data.passengerID << " : " << current->data.name << endl;
        current = current->next;
    }
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