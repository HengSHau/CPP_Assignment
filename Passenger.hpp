#ifndef PASSENGER_HPP
#define PASSENGER_HPP

#include <string>
#include <iostream>

using namespace std;

// This struct is the "payload" that holds the actual data
struct Passenger {
    string passengerID;
    string name;
    string seatRow;    // Keeping as string to handle inputs easily
    string seatColumn; // Keeping as string or char
    string pClass;     // "First", "Business", "Economy"
    int flightNumber;
};

#endif