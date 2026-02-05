// #include "ArrayFlight.hpp"

// ArrayFlight::ArrayFlight(){
//     for(int r=0;r<MAX_ROWS;r++){
//         for(int c=0;c<MAX_COLS;c++){
//             seatMap[r][c].passengerID="";
//         }
//     }

//     for(int i=0;i<MAX_PASSENGERS;i++){
//         passengerList[i].passengerID="";
//     }
// }

// bool ArrayFlight::addPassenger(string id,string name, string rowStr,string colStr,string pClass){
//     int r=stoi(rowStr)-1;
//     char colChar=colStr[0];
//     int c=colChar-'A';

//     if(r<0||r>=MAX_ROWS||c<0||c>=MAX_COLS){
//         cout<<"Error: Invalid seat selection."<<endl;
//         return false;
//     }

//     if(seatMap[r][c].passengerID!=""){
//         cout<<"Error: Seat "<<rowStr<<colChar<<" is already occupied."<<endl;
//         return false;
//     }

//     seatMap[r][c].passengerID=id;
//     seatMap[r][c].name=name;
//     seatMap[r][c].seatRow=rowStr;
//     seatMap[r][c].seatColumn=colStr;
//     seatMap[r][c].pClass=pClass;

//     for(int i=0;i<MAX_PASSENGERS;i++){
//         if(passengerList[i].passengerID==""){
//             passengerList[i]=seatMap[r][c];
//             return true;
//         }
//     }
//     return false;
    
// }

// Passenger*ArrayFlight::searchPassenger(string id){
//     for(int i=0;i<MAX_PASSENGERS;i++){
//         if(passengerList[i].passengerID!=""&&passengerList[i].passengerID==id){
//             return &passengerList[i];
//         }
//     }
//     return nullptr;
// }

// bool ArrayFlight::removePassenger(string id){
//     for(int i=0;i<MAX_PASSENGERS;i=i++){
//         if (passengerList[i].passengerID==id)
//         {
//             string rowStr=passengerList[i].seatRow;
//             string colStr=passengerList[i].seatColumn;

//             passengerList[i].passengerID="";
//             passengerList[i].name="";
            
//             int r=stoi(rowStr)-1;
//             int c=colStr[0]-'A';

//             if(r>=0&&r<MAX_ROWS&&c>=0&&c<MAX_COLS){
//                 seatMap[r][c].passengerID="";
//                 seatMap[r][c].name="";
//             }

//             cout<<"Passenger ID "<<id<<" removed successfully."<<endl;
//             return true;

//         }
        
//     }
//     cout<<"Error: Passenger ID"<<id<<"not found."<<endl;
//     return false;
// }

// void ArrayFlight::displayManifest(){
//     cout << "\n==========================================" << endl;
//     cout << "       FLIGHT PASSENGER MANIFEST          " << endl;
//     cout << "==========================================" << endl;
//     cout << "ID\t | Name\t\t | Seat\t | Class" << endl;
//     cout << "------------------------------------------" << endl;

//     int count=0;
//     for (int i = 0; i < MAX_PASSENGERS; i++) {
//         // Only print slots that have a passenger
//         if (passengerList[i].passengerID != "") {
//             cout << passengerList[i].passengerID << "\t | " 
//                  << passengerList[i].name << "\t | " 
//                  << passengerList[i].seatRow << passengerList[i].seatColumn << "\t | " 
//                  << passengerList[i].pClass << endl;
//             count++;
//         }
//     }

//     cout << "------------------------------------------" << endl;
//     cout << "Total Passengers: " << count << endl;
//     cout << "==========================================\n" << endl;
// }


// void ArrayFlight::displaySeatMap(){
//     cout << "\n       [ SEATING CONFIGURATION ]       " << endl;
//     cout << "      A   B   C      D   E   F" << endl; // Header

//     for (int r = 0; r < MAX_ROWS; r++) {
//         // Print Row Number (padding single digits like " 1" vs "10")
//         if (r + 1 < 10) cout << " " << (r + 1) << "  ";
//         else cout << (r + 1) << "  ";

//         for (int c = 0; c < MAX_COLS; c++) {
//             // Check if seat is occupied
//             if (seatMap[r][c].passengerID != "") {
//                 cout << "[X] "; // Occupied
//             } else {
//                 cout << "[_] "; // Empty
//             }

//             // Add a visual aisle gap after column C (index 2)
//             if (c == 2) cout << "   "; 
//         }
//         cout << endl; // New line after each row
//     }
//     cout << "\nLegend: [X] = Occupied, [_] = Available\n" << endl;
// }

// // This is the EXACT same logic as addPassenger, but with NO cout statements
// bool ArrayFlight::addPassengerSilent(string id, string name, string rowStr, string colStr, string pClass) {
    
//     // 1. Convert Row/Col to indices (Copy this part from your main addPassenger)
//     int r = stoi(rowStr) - 1; 
//     int c = colStr[0] - 'A'; 

//     // Safety Check
//     if (r < 0 || r >= MAX_ROWS || c < 0 || c >= MAX_COLS) {
//         return false; // Silent failure
//     }

//     // 2. Check if seat is taken
//     if (seatMap[r][c].passengerID != "") {
//         return false; // Silent failure (The seat is full, just skip it)
//     }

//     // 3. Add to 2D Array
//     seatMap[r][c].passengerID = id;
//     seatMap[r][c].name = name;
//     seatMap[r][c].seatRow = rowStr;
//     seatMap[r][c].seatColumn = colStr;
//     seatMap[r][c].pClass = pClass;

//     // 4. Add to 1D Array
//     for (int i = 0; i < MAX_PASSENGERS; i++) {
//         if (passengerList[i].passengerID == "") { 
//             passengerList[i] = seatMap[r][c]; 
//             return true;
//         }
//     }

//     return false;
// }

// void ArrayFlight::filterByRow(string rowStr) {
//     // 1. Convert user input "1" to index 0
//     int r = stoi(rowStr) - 1; 

//     // 2. Safety Check
//     if (r < 0 || r >= MAX_ROWS) {
//         cout << "Error: Invalid Row (Must be 1-30)." << endl;
//         return;
//     }

//     cout << "\n--- PASSENGERS IN ROW " << rowStr << " ---" << endl;
//     bool found = false;

//     // 3. Loop through columns A to F (0 to 5)
//     for (int c = 0; c < MAX_COLS; c++) {
//         // If passengerID is NOT empty, someone is sitting here
//         if (seatMap[r][c].passengerID != "") {
//             cout << "[Seat " << rowStr << seatMap[r][c].seatColumn << "] " 
//                  << seatMap[r][c].name 
//                  << " (ID: " << seatMap[r][c].passengerID << ")" << endl;
//             found = true;
//         }
//     }

//     if (!found) cout << "(This row is completely empty)" << endl;
//     cout << "-----------------------------" << endl;
// }

// void ArrayFlight::filterByColumn(string colStr) {
//     // 1. Convert user input "A" to index 0
//     char colChar = toupper(colStr[0]);
//     int c = colChar - 'A'; 

//     // 2. Safety Check
//     if (c < 0 || c >= MAX_COLS) {
//         cout << "Error: Invalid Column (Must be A-F)." << endl;
//         return;
//     }

//     cout << "\n--- PASSENGERS IN COLUMN " << colChar << " ---" << endl;
//     bool found = false;

//     // 3. Loop through rows 1 to 30 (0 to 29)
//     for (int r = 0; r < MAX_ROWS; r++) {
//         if (seatMap[r][c].passengerID != "") {
//             cout << "[Seat " << (r + 1) << colChar << "] " 
//                  << seatMap[r][c].name 
//                  << " (ID: " << seatMap[r][c].passengerID << ")" << endl;
//             found = true;
//         }
//     }

//     if (!found) cout << "(This column is completely empty)" << endl;
//     cout << "--------------------------------" << endl;
// }