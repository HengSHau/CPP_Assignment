#ifndef FLIGHT_LINKED_LIST_HPP
#define FLIGHT_LINKED_LIST_HPP

#include "Passenger.hpp"

// The Node represents one link in the chain
struct Node {
    Passenger data; // The passenger info
    Node* next;     // Pointer to the next node in the list
};

class FlightLinkedList {
private:
    Node* head; // The start of the list
    void sortedInsert(Node*& head_ref, Node* newNode); // Helper for binary search insertion

public:
    // Constructor
    FlightLinkedList();

    // Destructor (to clean up memory)
    ~FlightLinkedList();

    // Core Functionalities (Required by Assignment)
    bool addPassenger(Passenger p);          // Reservation (Insertion)
    bool removePassenger(string id);         // Cancellation (Deletion)
    Passenger* searchPassenger(string id);   // Seat Lookup (Search)
    void displayManifest();                  // Manifest Report (Traversal)
    void displaySeatingChart();              // Seating Chart Display
    Passenger* searchPassengerBinary(const std::string& id); // Optional: Binary Search
    void filterByRow(string row);
    void filterByColumn(string col);
    
    // Helper to check if a seat is already taken
    bool isSeatOccupied(string row, string col);

    void sortPassengers();
};

#endif