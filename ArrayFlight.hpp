#ifndef ARRAYFLIGHT_HPP
#define ARRAYFLIGHT_HPP

#include "Passenger.hpp"
#include <iomanip>

// Capable of holding ~100 flights (100 * 30 rows = 3000 rows)
const int TOTAL_ROWS = 3000; 
const int COLS = 6;

class ArrayFlight {
private:
    Passenger seatMap[TOTAL_ROWS][COLS]; 

public:
    ArrayFlight();
    
    void addPassenger(Passenger p); // Finds the first free flight
    void insertionSort(); // THE MATCHING SORT ALGORITHM

    void searchByRow(string row);
    void searchByColumn(string col);
    
    Passenger* binarySearch(string id); 
    void displayAll(); 
    void displaySeatingChart(int flightNum);
};

#endif