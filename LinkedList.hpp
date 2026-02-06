#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

#include "Passenger.hpp"

struct Node {
    Passenger data;
    Node* next;
};

class FlightLinkedList {
private:
    Node* head;
    
public:
    FlightLinkedList();
    
    void addPassenger(Passenger p);
    
    // MATCHING ALGORITHM
    void insertionSort(); 

    void searchByRow(string row);
    void searchByColumn(string col);

    Passenger* binarySearch(string id);
    void displayAll(); 
    void displaySeatingChart(int flightNum);

    size_t getMemoryUsage();
};

#endif