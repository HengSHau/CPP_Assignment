#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

#include "Passenger.hpp"
#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

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
    bool removePassenger(string id);
    
    // MATCHING ALGORITHM
    void insertionSort(); 
    void bubbleSort();   
    void selectionSort(); 
    void mergeSort();

    void searchByRow(string row);
    void searchByColumn(string col);

    Passenger* binarySearch(string id);
    void displayAll(); 
    void displaySeatingChart(int flightNum);

    size_t getMemoryUsage();
};

#endif