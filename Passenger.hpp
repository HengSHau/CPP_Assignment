#ifndef PASSENGER_HPP
#define PASSENGER_HPP

#include <string>
using namespace std;

struct Passenger {
    string passengerID;
    string name;
    string seatRow;
    string seatColumn;
    string pClass; // We used this name in ArrayFlight.cpp and LinkedList.cpp
    int flightNumber;
};

#endif