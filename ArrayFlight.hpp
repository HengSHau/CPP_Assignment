#ifndef ARRAYFLIGHT_HPP
#define ARRAYFLIGHT_HPP

#include <string>
#include <iostream>
#include "Passenger.hpp" 

using namespace std;

class ArrayFlight {
private:
    // Constants
    static const int MAX_ROWS = 30; 
    static const int MAX_COLS = 6;  
    static const int MAX_PASSENGERS = 180;

    // The Data Structures
    Passenger seatMap[MAX_ROWS][MAX_COLS];
    Passenger passengerList[MAX_PASSENGERS];

public:
    ArrayFlight(); // Constructor

    // Core Functions
    bool addPassenger(string id, string name, string row, string col, string pClass);
    bool addPassengerSilent(string id, string name, string row, string col, string pClass);
    bool removePassenger(string id);
    Passenger* searchPassenger(string id);
    void displayManifest();
    void displaySeatMap();
    void filterByRow(string row); 
    void filterByColumn(string col);
};

#endif