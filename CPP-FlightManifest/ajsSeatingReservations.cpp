/*
########################################################################################################
PROGRAM: ajsSeatingReservations.cpp
WRITTEN BY: Andrea Sitton
FOR: CPS 161 - Assignment #10

PURPOSE: This program provides a seating reservation system for a single 12-seat airplane. Users can
   navigate through a menu to view empty seats, see the current seating chart, see the alphabetized
   passenger manifest, and modify or delete a reservation. This version loads initial passenger data
   from a text file.     
######################################################################################################## 
*/
#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <limits>       // For handling wrong data type input
#include <optional>
#include <string>
#include <vector>
#include <windows.h>
using namespace std;

// Global variable for the number of seats in the plane
const int NUM_SEATS = 12;

// Structure definition
struct Seat
{
   int seatID;                // Seat identification number (1 - 12)
   bool seatOccupied;         // Indicates whether seat is reserved
   string passengerFirstName; // First name of passenger
   string passengerLastName;  // Last name of passenger
};

// Function prototypes
void createSeatingChart(Seat []);
void userMenu(Seat [], int &);
int inputValidation(int, int);
void displaySeatingChart(Seat []);
void printSpaces(int, string);
void alphabeticalManifest(Seat []);
void swap(Seat &, Seat &);
void displayAvailableSeats(Seat [], int);
void newReservation(Seat []);
void modifyReservation(Seat [], int, optional<int>);
int reservationLookup(Seat []);
int passengerLookup(Seat [], string, string);
int selectSeat(Seat []);
string enterName();
void displaySeatInformation(Seat [], int);

int main()
{
   // Get console handle
   HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
   
   // Change to an white background with blue text
   system("color F9");
   
   // Output formatting
   CONSOLE_FONT_INFOEX cfi;
   cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);
   cfi.nFont = 0;
   cfi.dwFontSize.X = 0;
   cfi.dwFontSize.Y = 24;
   cfi.FontFamily = FF_SWISS;
   cfi.FontWeight = FW_MEDIUM;
   wcscpy(cfi.FaceName, L"Consolas");
   SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi); 
   
   // Declare variables
   int userChoice;      // Holds user's menu choice
   
   // Create array of structures
   Seat seatingChart[NUM_SEATS];
   
   // Fill array with default information
   createSeatingChart(seatingChart);
   
   do
   {
      userMenu(seatingChart, userChoice);
   }
   while (userChoice != 7);
   
   cout << "\nThank you for flying Podunk Airlines!\n" << endl;
   SetConsoleTextAttribute(hConsole, 245);
   cout << "                             |                             " << endl;
   cout << "                             |                             " << endl;
   cout << "                             |                             " << endl; 
   cout << "                           .-'-.                           " << endl;
   cout << "                          ' ___ '                          " << endl;
   cout << "                ---------'  .-.  '---------                " << endl;
   cout << "_________________________'  '-'  '_________________________" << endl;
   cout << " ''''''-|---|--/   *,==][^',_m_,'^][==/   *,--|---|-'''''' " << endl;
   cout << "               *._ /   ||/   H  *,||  *._ /                " << endl;
   cout << "                       OO   O|O   OO                       " << endl;
   SetConsoleTextAttribute(hConsole, 249);
   cout << "\nASCII Art by Jon Hyatt with minor modifications by Andrea Sitton." << endl;
   cout << "Taken from the ASCII Art Archive.\n" << endl;
   system("pause");
   
   // Terminate program execution 
   return 0;
}

/*
########################################################################################################
FUNCTION: createSeatingChart
PURPOSE: This function loads passenger data from a text file into the seating chart to more easily
   demonstrate the program's functionality.
@param  seatingChart     Array of Seats used to store passenger data.
######################################################################################################## 
*/

void createSeatingChart(Seat seatingChart[])
{
   // Load existing passenger manifest
   ifstream inFile("ajsPassengerManifest.txt");
   
   int index = 0;
   
   while(inFile >> seatingChart[index].seatID)
   {
      inFile >> seatingChart[index].seatOccupied;
      inFile >> seatingChart[index].passengerFirstName;
      inFile >> seatingChart[index].passengerLastName;
      index++;
   }
   inFile.close();  
}

/*
########################################################################################################
FUNCTION: userMenu
PURPOSE: This function provides the menu by which users can navigate the program's functionality.
@param  seatingChart     Array of Seats used to store passenger data.
        userChoice       Reference variable to store an integer associated with the user's menu choice.
######################################################################################################## 
*/

void userMenu(Seat seatingChart[], int &userChoice)
{
   do
   {
      // Menu text for user
      cout << "Podunk Airlines Seat Reservation System" << endl;
      cout << "---------------------------------------\n" << endl;
      cout << "Menu Options:" << endl;
      cout << "\t1 = Display current seating chart" << endl;
      cout << "\t2 = Display alphabetized passenger manifest" << endl;
      cout << "\t3 = Display count of empty seats" << endl;
      cout << "\t4 = Make a new reservation" << endl;
      cout << "\t5 = Modify an existing reservation" << endl;
      cout << "\t6 = Cancel a reservation" << endl;
      cout << "\t7 = Exit menu\n" << endl;
      cout << "Please enter the numerical value associated with your choice: ";
      userChoice = inputValidation(1, 7);   
   }
   while (userChoice < 1 || userChoice > 7);
   
   // Navigate to the chosen menu function
   switch (userChoice)
   {
      case 1:
         displaySeatingChart(seatingChart);
         break;
         
      case 2:
         alphabeticalManifest(seatingChart);
         break;
         
      case 3:
         displayAvailableSeats(seatingChart, userChoice);
         break;
      
      case 4:
         newReservation(seatingChart);
         break;
         
      case 5:
      case 6:
         modifyReservation(seatingChart, userChoice, nullopt);
         break;
         
      case 7:
         return;         
   }
   cout << endl;
   system("pause");
   system("cls");
}

/*
########################################################################################################
FUNCTION: inputValidation
PURPOSE: This function validates input from the user, ensuring that it is an integer and falls within
   the accepted range of values.
@param  parameter1       The lower limit of the acceptable range of values.
        parameter2       The upper limit of the acceptable range of values.
@return                  The validated integer input of the user.
######################################################################################################## 
*/

int inputValidation(int parameter1, int parameter2)
{
   // Get console handle for manipulating output text color
   HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
   
   int value;
   
   SetConsoleTextAttribute(hConsole, 240);
   cin >> value;
   SetConsoleTextAttribute(hConsole, 249);
      
   // Catches non-integer input
   if (cin.fail())
   {
      SetConsoleTextAttribute(hConsole, 252);
      cout << "\nInvalid input. An integer is required.\n" << endl;
      SetConsoleTextAttribute(hConsole, 249);
      cin.clear();   // Clear the failbit
      // Ignore the remaining input
      cin.ignore(numeric_limits<streamsize>::max(),'\n');
   }
   else if (value < parameter1 || value > parameter2)
   {
      SetConsoleTextAttribute(hConsole, 252);
      cout << "\nInvalid selection. Please try again.\n" << endl;
      SetConsoleTextAttribute(hConsole, 249);
   }

   return value;
}

/*
########################################################################################################
FUNCTION: displaySeatingChart
PURPOSE: This function displays the current seating chart, formatted as a series of squares to
   represent the seats.
@param  seatingChart     Array of Seats used to store passenger data.
######################################################################################################## 
*/

void displaySeatingChart(Seat seatingChart[])
{
   // Get console handle for manipulating output text color
   HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
   
   int index = 0;
   
   cout << endl;
   cout << "    --------------------------------------------    " << endl;
   
   while (index < NUM_SEATS)
   {
      cout << "    ||                   ||                   ||    " << endl;
      // Different spacing requirements for single digit seat ID vs. double digit seat ID
      if (index < 8)
      {
         cout << "    || Seat ID: " << seatingChart[index].seatID << "        |";
         cout << "| Seat ID: " << seatingChart[index + 1].seatID << "        ||    " << endl;
      }   
      if (index == 8)
      {
         cout << "    || Seat ID: " << seatingChart[index].seatID << "        |";
         cout << "| Seat ID: " << seatingChart[index + 1].seatID << "       ||    " << endl;
      }
      if (index > 8)
      {
         cout << "    || Seat ID: " << seatingChart[index].seatID << "       |";
         cout << "| Seat ID: " << seatingChart[index + 1].seatID << "       ||    " << endl;
      }   
      cout << "    ||                   ||                   ||    " << endl;
      cout << "    || "; 
      
      // Colors the name field green if unoccupied, or purple if occupied
      if (seatingChart[index].seatOccupied == 0)
         SetConsoleTextAttribute(hConsole, 242);
      else
         SetConsoleTextAttribute(hConsole, 245);
      
      cout << seatingChart[index].passengerFirstName;
      SetConsoleTextAttribute(hConsole, 249);
      
      // Adds the right number of spaces in order to keep the seat divider lines straight
      printSpaces(18, seatingChart[index].passengerFirstName); 
      cout << "|| ";
      
      if (seatingChart[index + 1].seatOccupied == 0)
         SetConsoleTextAttribute(hConsole, 242);
      else
         SetConsoleTextAttribute(hConsole, 245);
      
      // Printing next passenger's first name because each loop cycle prints two columns
      cout << seatingChart[index + 1].passengerFirstName;
      SetConsoleTextAttribute(hConsole, 249);
      
      printSpaces(18, seatingChart[index + 1].passengerFirstName);
      cout << "||    " << endl;      
      cout << "    || ";
      
      if (seatingChart[index].seatOccupied == 0)
         SetConsoleTextAttribute(hConsole, 242);
      else
         SetConsoleTextAttribute(hConsole, 245);
      
      cout << seatingChart[index].passengerLastName;
      SetConsoleTextAttribute(hConsole, 249);
      
      printSpaces(18, seatingChart[index].passengerLastName); 
      cout << "|| ";
      
      if (seatingChart[index + 1].seatOccupied == 0)
         SetConsoleTextAttribute(hConsole, 242);
      else
         SetConsoleTextAttribute(hConsole, 245);
      
      cout << seatingChart[index + 1].passengerLastName;
      SetConsoleTextAttribute(hConsole, 249);
      
      printSpaces(18, seatingChart[index + 1].passengerLastName);
      cout << "||    " << endl;  
      cout << "    ||                   ||                   ||    " << endl;
      cout << "    --------------------------------------------    " << endl;
      // Increment by two since printing two passengers per cycle
      index += 2;    
   }
}

/*
########################################################################################################
FUNCTION: printSpaces
PURPOSE: This function prints the number of spaces needed to keep columns aligned.
@param  spaceSize     The number of characters that can fit in a column.
        word          The word that will be displayed in the column.
######################################################################################################## 
*/

void printSpaces(int spaceSize, string word)
{
   // Print spaces enough to fill the space after the word has been printed 
   for (int i = word.length(); i < spaceSize; i++)
      cout << " ";
}

/*
########################################################################################################
FUNCTION: alphabeticalManifest
PURPOSE: This function prints an alphabetized list of ticketed passengers.
@param  seatingChart     Array of Seats used to store passenger data.
######################################################################################################## 
*/

void alphabeticalManifest(Seat seatingChart[])                 
{
   // Get console handle for manipulating output text color
   HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
   
   vector<Seat> alphabeticalList;  // To store seat information in alphabetical order
   int targetIndex;                // Index of Seat structure being compared
   string targetValue;             // Value in lastName
   
   // Copy array data into vector
   for (int index = 0; index < NUM_SEATS; index++)
      if (seatingChart[index].seatOccupied == true)
         alphabeticalList.push_back(seatingChart[index]);
   
   // If there are no seat reservations
   if (alphabeticalList.empty())
   {
      SetConsoleTextAttribute(hConsole, 242);
      cout << "\nManifest contains no passengers." << endl;
      SetConsoleTextAttribute(hConsole, 249);
      return;
   }

   // Cycle through each element in the vector
   for (int start = 0; start < alphabeticalList.size() - 1; start++)
   {
	   // Store the value and location of the starting element
      targetIndex = start;
      targetValue = alphabeticalList[start].passengerLastName;
         
      for (int index = start + 1; index < alphabeticalList.size(); index++)
      {
	      // If value in element is less than targetValue
         if (alphabeticalList[index].passengerLastName < targetValue)
         {
            // Assign value as new targetValue
            targetValue = alphabeticalList[index].passengerLastName;
            // Store location of new targetValue
            targetIndex = index;
         }
	      // If passengers have the same last name
         else if (alphabeticalList[index].passengerLastName == targetValue)
		   {
			   // Compare first names
            if (alphabeticalList[index].passengerFirstName < alphabeticalList[targetIndex].passengerFirstName) 
			      // Store location of new targetValue
               targetIndex = index;			
		   }
      }
      // Swap target value with starting value
      swap(alphabeticalList[targetIndex], alphabeticalList[start]);
   }
   
   // Display alphabetized list
   
   cout << endl;
   cout << "    ---------------------------------------------------------    " << endl;
   cout << "    || PASSENGER LAST NAME || PASSENGER FIRST NAME || SEAT ||    " << endl;
   cout << "    ---------------------------------------------------------    " << endl;
   
   for (int index = 0; index < alphabeticalList.size(); index++)
   { 
      cout << "    || " << alphabeticalList[index].passengerLastName;
      printSpaces(19, alphabeticalList[index].passengerLastName);
      cout << " || " << alphabeticalList[index].passengerFirstName;
      printSpaces(20, alphabeticalList[index].passengerFirstName);
      cout << " ||  " << alphabeticalList[index].seatID;
      if (alphabeticalList[index].seatID < 10)
         cout << "   ||    " << endl;
      else if (alphabeticalList[index].seatID >= 10)
         cout << "  ||    " << endl;
   } 
   cout << "    ---------------------------------------------------------    " << endl;  
}

/*
########################################################################################################
FUNCTION: swap
PURPOSE: This function swaps the Seat structures stored in two different vector elements.
@param seat1    Reference variable representing the first Seat structure being swapped.
       seat2    Reference variable representing the second Seat structure being swapped.
######################################################################################################## 
*/

void swap(Seat &seat1, Seat &seat2)
{
   Seat temp = seat1;
   seat1 = seat2;
   seat2 = temp;
}

/*
########################################################################################################
FUNCTION: displayAvailableSeats
PURPOSE: This function prints a list of seats available to the user or provides a count of empty
   seats, depending on where the function is called.
@param  seatingChart     Array of Seats used to store passenger data.
        option           Integer representing what information the user is requesting.
######################################################################################################## 
*/

void displayAvailableSeats(Seat seatingChart[], int option)
{
   // Get console handle for manipulating output text color
   HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
   
   int counter = 0;     // To hold the number of empty seats
   
   if (option == 0)
      cout << "\nThe following seat numbers are available: ";
   
   for (int index = 0; index < NUM_SEATS; index++)
   {
      // If seat is available AND user wants a list of available seats
      if (seatingChart[index].seatOccupied == 0 && option == 0)
         cout << seatingChart[index].seatID << " ";
      // If seat is available and user only wants a count of empty seats   
      else if (seatingChart[index].seatOccupied == 0)
         counter++;
   }
   
   SetConsoleTextAttribute(hConsole, 242);
   if (option == 3)
      cout << "\nCurrently there are " << counter << " empty seat(s) on today's flight." << endl;
   SetConsoleTextAttribute(hConsole, 249);
}

/*
########################################################################################################
FUNCTION: newReservation
PURPOSE: This function allows the user to make a new seat reservation.
@param  seatingChart     Array of Seats used to store passenger data.
######################################################################################################## 
*/

void newReservation(Seat seatingChart[])
{
   // Get console handle for manipulating output text color
   HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
   
   int selectedSeatID;     // To hold user provided seat ID number
   string firstName;       // To hold user provided first name information
   string lastName;        // To hold user provided last name information
   char letter;            // To hold user confirmation that the reservation is correct
   
   // User selects seat
   selectedSeatID = selectSeat(seatingChart);
      
   // First name of passenger
   cout << "Please enter the FIRST NAME of the passenger: ";
   firstName = enterName();
      
   // Last name of passenger
   cout << "Please enter the LAST NAME of the passenger: ";
   lastName = enterName();
          
   // Store passenger name in array
   seatingChart[selectedSeatID - 1].passengerFirstName = firstName;
   seatingChart[selectedSeatID - 1].passengerLastName = lastName;
      
   // Confirm new reservation
   cout << "\nHere is the reservation information you entered:" << endl;
   cout << endl;
   displaySeatInformation(seatingChart, selectedSeatID - 1);
   
   do
   {
      cout << "\nIs this information correct? (Y/N): ";
      SetConsoleTextAttribute(hConsole, 240);
      cin >> letter;
      SetConsoleTextAttribute(hConsole, 249);
   
      // If information is correct, return to menu
      if ((char)toupper(letter) == 'Y')
      {
         return;
      }
      // If information is incorrect, call modifyReservation function
      // Since modifyReservation has user confirmation built in, we can
      // return to menu once the user exits the modifyReservation function
      else if((char)toupper(letter) == 'N')
      {
         // Providing the optional parameter of the selectedSeatID means the user
         // does not need to manually enter the seat number for the reservation
         // they just made
         modifyReservation(seatingChart, 5, selectedSeatID);
         return;
      }
      else
      {
         SetConsoleTextAttribute(hConsole, 252);
         cout << "\nInvalid selection. Please try again." << endl;
         SetConsoleTextAttribute(hConsole, 249);
      }
   }
   while (letter != 'Y' && letter != 'N');
}

/*
########################################################################################################
FUNCTION: modifyReservation
PURPOSE: This function allows the user to make changes to or cancel an existing reservation.
@param  seatingChart     Array of Seats used to store passenger data.
        option           Integer representing what action the user wants to perform.
        reservedSeat     An optional integer passed from the newReservation function if the user needs
                           to immediately change information for a newReservation.
######################################################################################################## 
*/

void modifyReservation(Seat seatingChart[], int option, optional<int> reservedSeat)
{
   // Get console handle for manipulating output text color
   HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
   
   bool changesComplete = false,  // Flag to signal user has completed necessary modification
        reservationFound = false; // Flag to signal reservation has been found
   int selectedSeatID,            // To hold user provided seat ID number
       newSeatID,                 // To hold new seat selection
       choice;                    // To hold user choice in menus
   string firstName,              // To hold user provided first name information
          lastName;               // To hold user provided last name information
   char letter;                   // To hold user confirmation that the reservation is correct
   
   // If function was called from newReservation, save optional parameter into selectedSeatID
   if (reservedSeat.has_value())
      selectedSeatID = reservedSeat.value();
      
   // If function was not called from newReservation, get reservation seat ID
   else
      do
      {
         selectedSeatID = reservationLookup(seatingChart);
         
         do
         {
            // Confirm modified reservation
            cout << "\nIs this the correct reservation? (Y/N)" << endl;
            displaySeatInformation(seatingChart, selectedSeatID - 1);
            cout << "\n==>  ";
            SetConsoleTextAttribute(hConsole, 240);
            cin >> letter;
            SetConsoleTextAttribute(hConsole, 249);
      
            letter = (char)toupper(letter);  // Force character to be uppercase
               
            if (letter == 'Y')
            {
               reservationFound = true;
               break;
            }
            else if (letter == 'N')
            {
               continue;
            }
            else
            {
               SetConsoleTextAttribute(hConsole, 252);
               cout << "\nInvalid selection. Please try again." << endl;
               SetConsoleTextAttribute(hConsole, 249);
            }
         }
         while (letter != 'Y' && letter != 'N');
      }
      while (!reservationFound);

      // Modify an existing reservation  
      if (option == 5)
      {      
         do
         {
            cout << endl;
            do
            {
               cout << "Which field needs to be modified?" << endl;
               cout << "\t1 = seat number" << endl;
               cout << "\t2 = passenger first name" << endl;
               cout << "\t3 = passenger last name" << endl;
               cout << "==>  ";
               choice = inputValidation(1, 3);
            }
            while (choice < 1 || choice > 3);
            
            switch (choice)
            {
               case 1:
                  newSeatID = selectSeat(seatingChart);
                  // Copy information from old reservation
                  seatingChart[newSeatID - 1].passengerFirstName = seatingChart[selectedSeatID - 1].passengerFirstName;
                  seatingChart[newSeatID - 1].passengerLastName = seatingChart[selectedSeatID - 1].passengerLastName;
                  // Clear old reservation information
                  seatingChart[selectedSeatID - 1].seatOccupied = false;
                  seatingChart[selectedSeatID - 1].passengerFirstName = "SEAT";
                  seatingChart[selectedSeatID - 1].passengerLastName = "UNOCCUPIED";
                  // Save selected seat ID as the new seat ID
                  selectedSeatID = newSeatID;
                  break;
               case 2:
                  cout << "\nPlease enter the FIRST NAME of the passenger: ";
                  firstName = enterName();
                  // Correct passenger first name
                  seatingChart[selectedSeatID - 1].passengerFirstName = firstName;
                  break;
               case 3:
                  cout << "\nPlease enter the LAST NAME of the passenger: ";
                  lastName = enterName();
                  // Correct passenger last name
                  seatingChart[selectedSeatID - 1].passengerLastName = lastName;
                  break;               
            }
         
            // Confirm modified reservation
            cout << "\nHere is the reservation information you entered:" << endl;
            cout << endl;
            displaySeatInformation(seatingChart, selectedSeatID - 1);

            do
            {
               cout << "\nIs this information correct? (Y/N): ";
               SetConsoleTextAttribute(hConsole, 240);
               cin >> letter;
               SetConsoleTextAttribute(hConsole, 249);
      
               letter = (char)toupper(letter);  // Force character to be uppercase
               
               if (letter == 'Y')
               {
                  changesComplete = true;
               }
               else if (letter == 'N')
               {
                  break;
               }
               else
               {
                  SetConsoleTextAttribute(hConsole, 252);
                  cout << "\nInvalid selection. Please try again." << endl;
                  SetConsoleTextAttribute(hConsole, 249);
               }
            }
            while (letter != 'Y' && letter != 'N');
         }
         while (!changesComplete);
      }
   
      // Cancel a reservation
      else if (option == 6)
      {
         seatingChart[selectedSeatID - 1].seatOccupied = false;
         seatingChart[selectedSeatID - 1].passengerFirstName = "SEAT";
         seatingChart[selectedSeatID - 1].passengerLastName = "UNOCCUPIED";
         changesComplete = true;
         SetConsoleTextAttribute(hConsole, 242);
         cout << "\nReservation deleted." << endl;
         SetConsoleTextAttribute(hConsole, 249);
      }
}

/*
########################################################################################################
FUNCTION: reservationLookup
PURPOSE: This function allows the user to look up a reservation based on the seat ID or the passenger's
   first and last name.
@param  seatingChart     Array of Seats used to store passenger data.
@return                  The seatID associated with the reservation the user wants to change or cancel.
######################################################################################################## 
*/

int reservationLookup(Seat seatingChart[])
{
   // Get console handle for manipulating output text color
   HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
   
   bool reservationFound = false; // Flag to signal reservation has been found
   int selectedSeatID,            // To hold user provided seat ID number
       choice;                    // To hold user choice in menus
   string firstName,              // To hold user provided first name information
          lastName;               // To hold user provided last name information
  
   do
   {         
      cout << endl;
      do
      {
         cout << "How would you like to search for the reservation?" << endl;
         cout << "\t1 = search by seat number" << endl;
         cout << "\t2 = search by passenger name" << endl;
         cout << "==>  ";
         choice = inputValidation(1, 2);
      }
      while (choice < 1 || choice > 2);
      
      switch (choice)
      {
         case 1:
            cout << endl;
            do
            {
               cout << "Please enter the seat ID of the reservation you would like to modify or cancel: ";
               selectedSeatID = inputValidation(1, 12);
            }
            while (selectedSeatID < 1 || selectedSeatID > 12);
                  
            if (seatingChart[selectedSeatID - 1].seatOccupied == 0)
            {
               SetConsoleTextAttribute(hConsole, 252);
               cout << "\nNo reservation exists for that seat number. Please try again." << endl;
               SetConsoleTextAttribute(hConsole, 249);
               // If entered seatID is unoccupied, set seatID as invalid value to re-trigger the 
               // search-by prompt
               selectedSeatID = -1;
               break;
            }              
            break;
         case 2:
            cout << "\nPlease enter the FIRST NAME of the passenger: ";
            firstName = enterName();
            cout << "Please enter the LAST NAME of the passenger: ";
            lastName = enterName();
            selectedSeatID = passengerLookup(seatingChart, firstName, lastName);  
            break;         
      }
         
      // Invalid seat ID triggers the loop to run again
      if (selectedSeatID == -1)
         continue;
      else
      {  
         reservationFound = true;   
      }
   }
   while (!reservationFound);
   
   return selectedSeatID; 
}

/*
########################################################################################################
FUNCTION: passengerLookup
PURPOSE: This function allows the user to look up a reservation based on the seat ID or the passenger's
   first and last name.
@param  seatingChart     Array of Seats used to store passenger data.
        firstName        The passenger's first name.
        lastName         The passenger's last name.
@return                  The seatID associated with the reservation the user wants to change or cancel.
######################################################################################################## 
*/

int passengerLookup(Seat seatingChart[], string firstName, string lastName)
{
   // Get console handle for manipulating output text color
   HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
   
   int seatID;
   
   // Cycle through array
   for (int index = 0; index < NUM_SEATS; index++)
   {
      // If first and last name are a match, return associated seatID
      if (seatingChart[index].passengerFirstName == firstName
         && seatingChart[index].passengerLastName == lastName)
      {
         seatID = index + 1;
         return seatID;
      }        
   }
   
   // If passenger name could not be matched, set seatID as invalid value to
   // re-trigger the search-by prompt
   SetConsoleTextAttribute(hConsole, 252);
   cout << "\nPassenger name not found. Please try again." << endl;
   SetConsoleTextAttribute(hConsole, 249);
   seatID = -1;
   return seatID;
}

/*
########################################################################################################
FUNCTION: selectSeat
PURPOSE: This function allows the user to select an available seat when making or changing a reservation.
   Selection is validated to make sure the seat is not occupied.
@param  seatingChart     Array of Seats used to store passenger data.
@return                  The seatID of the seat selected by the user.
######################################################################################################## 
*/

int selectSeat(Seat seatingChart[])
{
   // Get console handle for manipulating output text color
   HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
   
   int selectedSeat;             // To hold user provided seat ID number
   bool seatSelected = false;    // Flag to signal a valid seat has been selected
   
   // Get seat selection
   do
   {      
      displayAvailableSeats(seatingChart, 0);
      cout << "\n" << endl;
         
      do
      {
         cout << "Please enter the seat ID for you would like to reserve: ";
         selectedSeat = inputValidation(1, 12);    
      }
      while (selectedSeat < 1 || selectedSeat > 12);
      
      // Selected seat is already occupied
      if (seatingChart[selectedSeat - 1].seatOccupied == 1)
      {
         SetConsoleTextAttribute(hConsole, 252);
         cout << "\nThis seat is currently occupied. Please select from the available seats." << endl;
         SetConsoleTextAttribute(hConsole, 249);
      }
      else
         seatSelected = true;
   }
   while (!seatSelected);
   
   // Set seat to occupied
   seatingChart[selectedSeat - 1].seatOccupied = true;
   
   return selectedSeat;
}

/*
########################################################################################################
FUNCTION: enterName
PURPOSE: This function collects passenger name information and standardizes it to uppercase.
@return                  The user-entered name in all capital letters.
######################################################################################################## 
*/

string enterName()
{
   // Get console handle for manipulating output text color
   HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
   
   string input;
   SetConsoleTextAttribute(hConsole, 240);   
   cin >> input;
   SetConsoleTextAttribute(hConsole, 249);
      
   // Standardizes input to all upper case
   transform(input.begin(), input.end(), input.begin(), ::toupper);
   
   return input;
}

/*
########################################################################################################
FUNCTION: displaySeatInformation
PURPOSE: This function displays information for a specified seat.
@param  seatingChart     Array of Seats used to store passenger data.
        index            The element index of the Seat information to be displayed.
######################################################################################################## 
*/

void displaySeatInformation(Seat seatingChart[], int index)
{
   // Get console handle for manipulating output text color
   HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
   
   cout << "Seat ID: ";
   SetConsoleTextAttribute(hConsole, 245);
   cout << seatingChart[index].seatID << endl;
   SetConsoleTextAttribute(hConsole, 249);
   cout << "Passenger First Name: ";
   SetConsoleTextAttribute(hConsole, 245);
   cout << seatingChart[index].passengerFirstName << endl;
   SetConsoleTextAttribute(hConsole, 249);
   cout << "Passenger Last Name: ";
   SetConsoleTextAttribute(hConsole, 245);
   cout << seatingChart[index].passengerLastName << endl;
   SetConsoleTextAttribute(hConsole, 249);
}