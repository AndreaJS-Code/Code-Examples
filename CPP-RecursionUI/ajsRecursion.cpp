/*
########################################################################################################
PROGRAM: ajsRecusion.cpp
WRITTEN BY: Andrea Sitton
FOR: CPS 161 - Assignment #12

PURPOSE: This program solves the following second-order linear homogeneous recurrance function:
   f(n) = a * f(n-2) + b * f(n-1)
   The user enters values for a, b, n, f(0), and f(1).  
######################################################################################################## 
*/
#include <cctype>
#include <iostream>
#include <limits>
#include <windows.h>
using namespace std;

// Function prototypes
void getUserInput();
int inputValidation();
int solveFunction(int);
void solveAgain(bool &);

// Global variables
int a, b, f0, f1, n;    // Necessary values for solving the function
bool details;           // If the user wants to see the function solved step-by-step
int functionCounter;    // For displaying each function evaluation at n one time

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
   int fn;        // Stores the function's solution
   bool again;    // If the user wants to solve another function 
        
   // Introductory message to user
   cout << "This program solves the following second-order linear homogeneous recurrance function: ";
   cout << "f(n) = a * f(n-2) + b * f(n-1)" << endl;
   cout << "You will be prompted to enter integer values that will be plugged into the equation.\n" << endl;
   
   do
   {
      functionCounter = 2;    // Since f(0) and f(1) are known, detailed solution steps start at f(2)
      getUserInput();
      fn = solveFunction(n);
      
      cout << "The value of the function when ";
      SetConsoleTextAttribute(hConsole, 245);
      cout << "n = " << n;
      SetConsoleTextAttribute(hConsole, 249);
      cout << " is ";
      SetConsoleTextAttribute(hConsole, 242);
      cout << fn;
      SetConsoleTextAttribute(hConsole, 249);
      cout << "." << endl;
      solveAgain(again);
      if (again)
         system("cls");
   }
   while (again);
   
   // Terminate program execution
   system("pause");   
   return 0;
}

/*
########################################################################################################
FUNCTION: getUserInput
PURPOSE: This function prompts the user for values to be used in solving the function. The values are
   stored in previously defined global variables. 
######################################################################################################## 
*/

void getUserInput()
{
   char letter;      // For user's choice on seeing function solving details
   
   // Get console handle
   HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
   
   // Get value for a
   cout << "f(n) = ";
   SetConsoleTextAttribute(hConsole, 245);
   cout << "a";
   SetConsoleTextAttribute(hConsole, 249);
   cout << " * f(n-2) + b * f(n-1)" << endl;
   cout << "Please enter an integer value for the coefficient a: ";
   a = inputValidation();
   
   // Get value for b
   cout << endl;
   cout << "f(n) = a * f(n-2) + ";
   SetConsoleTextAttribute(hConsole, 245);
   cout << "b";
   SetConsoleTextAttribute(hConsole, 249);
   cout << " * f(n-1)" << endl;
   cout << "Please enter an integer value for the coefficient b: ";
   b = inputValidation();
   
   // Get value for f(0)
   cout << "\n" << endl;
   cout << "The next two prompts are requesting the value of the function when n is a given value." << endl;
   SetConsoleTextAttribute(hConsole, 245);
   cout << "Please note that this does not require you to do any mathematics." << endl;
   SetConsoleTextAttribute(hConsole, 249);
   cout << "Any integer value will suffice.\n" << endl;
   cout << "Please enter an integer value for the function when ";
   SetConsoleTextAttribute(hConsole, 245);
   cout << "n = 0";
   SetConsoleTextAttribute(hConsole, 249);
   cout << ": ";
   f0 = inputValidation();
   
   // Get value for f(1)
   cout << endl;
   cout << "Please enter an integer value for the function when ";
   SetConsoleTextAttribute(hConsole, 245);
   cout << "n = 1";
   SetConsoleTextAttribute(hConsole, 249);
   cout << ": ";
   f1 = inputValidation();
   
   // Get value for n
   cout << "\n" << endl;
   cout << "Since we are determining the value of the function when n is a given value, we need a value for n." << endl;
   cout << "Please enter a positive integer (>= 0) value for ";
   SetConsoleTextAttribute(hConsole, 245);
   cout << "n";
   SetConsoleTextAttribute(hConsole, 249);
   cout << ": ";
   
   // Make sure n is GTE to 0
   do
   {
      n = inputValidation();
      if (n < 0)
      {
         SetConsoleTextAttribute(hConsole, 252);
         cout << "\nInvalid input. A positive integer (>= 0) is required.\n" << endl;
         SetConsoleTextAttribute(hConsole, 249);
         cout << "\t==> ";
      } 
   }
   while (n < 0);

   cout << "\n" << endl;
   
   do
   {
      // Get user's choice on seeing solution step-by-step
      cout << "Would you like to see the function solved step-by-step? (Y/N): ";
      SetConsoleTextAttribute(hConsole, 240);
      cin >> letter;
      SetConsoleTextAttribute(hConsole, 249);
      
      letter = (char)toupper(letter);  // Force character to be uppercase
   
      if(letter == 'Y')
      {
         details = true;
      }
      else if(letter == 'N')
      {
         details = false;
      }
      else
      {
         SetConsoleTextAttribute(hConsole, 252);
         cout << "\nInvalid selection. Please try again.\n" << endl;
         SetConsoleTextAttribute(hConsole, 249);
      }
   }
   while (letter != 'Y' && letter != 'N');
   
   // If user wants to see solution details, clear the screen
   if (details)
      system("cls");
   // Otherwise, add a space. That way the user can see the entered parameters alongside
   // the answer.
   else
      cout << endl;
}

/*
########################################################################################################
FUNCTION: inputValidation
PURPOSE: This function validates input from the user, ensuring that it is an integer.
@return        The validated integer input of the user.
######################################################################################################## 
*/

int inputValidation()
{
   // Get console handle for manipulating output text color
   HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
   
   int value;
   bool validValue = false;
   
   do
   {
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
         cout << "\t==> ";
      }
      else
         validValue = true;
   }
   while (!validValue);

   return value;
}

/*
########################################################################################################
FUNCTION: solveFunction
PURPOSE: This function solves the second-order homogeneous linear recurrence function by recursively
   calling itself until the base cases - f(0) and f(1) - are reached and the function's value can be
   calculated.
@param  x      Integer representing the value of n for which the function solution - f(n) - is being
   sought
@return        The value of f(n).
######################################################################################################## 
*/

int solveFunction(int x)
{
   
   // Get console handle for manipulating output text color
   HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
   
   int firstHalf,    // Stores the integer value of the first half of the function -> a * f(n-2)
       secondHalf,   // Stores the integer value of the second half of the function -> b * f(n-1)
       fn;           // Stores the value of the function's solution, f(n)
   
   // Return value stored in f0 variable when x is LTE to 0
   if (x <= 0)
      return f0;
   // Return value stored in f1 variable when x is equal to 1
   else if (x == 1)
      return f1;
   else
   {
      // While spliting the function into two parts isn't necessary, it makes it easier to display the
      // breakdown of how the function works if details are requested.
      firstHalf = a * solveFunction(x-2);
      secondHalf = b * solveFunction(x-1);
      fn = firstHalf + secondHalf; 
   }
   
   // If the user wants to see the function's solution performed step-by-step 
   if (details)
   {
      // For the first instance of x that equals the functionCounter, display step-by-step solution
      // then increment functionCounter so that the same information - e.g., f(2) - does not get
      // displayed to the user multiple times.
      if (x == functionCounter)
      {
         // Display color-coded function equation
         cout << "f(";
         SetConsoleTextAttribute(hConsole, 240);
         cout << "n";
         SetConsoleTextAttribute(hConsole, 249);
         cout << ") = ";
         SetConsoleTextAttribute(hConsole, 241);
         cout << "a";
         SetConsoleTextAttribute(hConsole, 249);
         cout << " * f(";
         SetConsoleTextAttribute(hConsole, 246);
         cout << "n-2";
         SetConsoleTextAttribute(hConsole, 249);
         cout << ") + ";
         SetConsoleTextAttribute(hConsole, 241);
         cout << "b";
         SetConsoleTextAttribute(hConsole, 249);
         cout << " * f(";
         SetConsoleTextAttribute(hConsole, 252);
         cout << "n-1";
         SetConsoleTextAttribute(hConsole, 249);
         cout << ")" << endl;
         
         // Display color-coded function equation where coefficients (a, b) and the
         // value of n being used are displayed - e.g., f(3), f(2). 
         cout << "f(";
         SetConsoleTextAttribute(hConsole, 240);
         cout << x;
         SetConsoleTextAttribute(hConsole, 249);
         cout << ") = ";
         SetConsoleTextAttribute(hConsole, 241);
         cout << a;
         SetConsoleTextAttribute(hConsole, 249);
         cout << " * f(";
         SetConsoleTextAttribute(hConsole, 246);
         cout << x-2;
         SetConsoleTextAttribute(hConsole, 249);
         cout << ") + ";
         SetConsoleTextAttribute(hConsole, 241);
         cout << b;
         SetConsoleTextAttribute(hConsole, 249);
         cout << " * f(";
         SetConsoleTextAttribute(hConsole, 252);
         cout << x-1;
         SetConsoleTextAttribute(hConsole, 249);
         cout << ")" << endl;
         
         // If displaying f(2), display value in f0 for f(n-2) and value in f1 for f(n-1)
         if (x - 2 == 0)
         {
            // Display color-coded function equation with the numerical values of f(n-2) and f(n-1)
            cout << "f(";
            SetConsoleTextAttribute(hConsole, 240);
            cout << x;
            SetConsoleTextAttribute(hConsole, 249);
            cout << ") = ";
            SetConsoleTextAttribute(hConsole, 241);
            cout << a;
            SetConsoleTextAttribute(hConsole, 249);
            cout << " * (";
            SetConsoleTextAttribute(hConsole, 246);
            cout << f0;
            SetConsoleTextAttribute(hConsole, 249);
            cout << ") + ";
            SetConsoleTextAttribute(hConsole, 241);
            cout << b;
            SetConsoleTextAttribute(hConsole, 249);
            cout << " * (";
            SetConsoleTextAttribute(hConsole, 252);
            cout << f1;
            SetConsoleTextAttribute(hConsole, 249);
            cout << ")" << endl;
         }
         // If displaying f(3), display value in f1 for f(n-2) and calculated value of f(2)
         else if (x - 2 == 1)
         {
            // Display color-coded function equation with the numerical values of f(n-2) and f(n-1)            
            cout << "f(";
            SetConsoleTextAttribute(hConsole, 240);
            cout << x;
            SetConsoleTextAttribute(hConsole, 249);
            cout << ") = ";
            SetConsoleTextAttribute(hConsole, 241);
            cout << a;
            SetConsoleTextAttribute(hConsole, 249);
            cout << " * (";
            SetConsoleTextAttribute(hConsole, 246);
            cout << f1;
            SetConsoleTextAttribute(hConsole, 249);
            cout << ") + ";
            SetConsoleTextAttribute(hConsole, 241);
            cout << b;
            SetConsoleTextAttribute(hConsole, 249);
            cout << " * (";
            SetConsoleTextAttribute(hConsole, 252);
            cout << secondHalf / b;
            SetConsoleTextAttribute(hConsole, 249);
            cout << ")" << endl;
         }
         // If displaying f(n) where n >= 4, display calculated value of f(n-2) and f(n-1)
         else
         {
            // Display color-coded function equation with the numerical values of f(n-2) and f(n-1) 
            cout << "f(";
            SetConsoleTextAttribute(hConsole, 240);
            cout << x;
            SetConsoleTextAttribute(hConsole, 249);
            cout << ") = ";
            SetConsoleTextAttribute(hConsole, 241);
            cout << a;
            SetConsoleTextAttribute(hConsole, 249);
            cout << " * (";
            SetConsoleTextAttribute(hConsole, 246);
            cout << firstHalf / a;
            SetConsoleTextAttribute(hConsole, 249);
            cout << ") + ";
            SetConsoleTextAttribute(hConsole, 241);
            cout << b;
            SetConsoleTextAttribute(hConsole, 249);
            cout << " * (";
            SetConsoleTextAttribute(hConsole, 252);
            cout << secondHalf / b;
            SetConsoleTextAttribute(hConsole, 249);
            cout << ")" << endl;
         }
         
         // Display color-coded function equation with the numerical values of a * f(n-2) as the
         // first half and b * f(n-1) as the second half 
         cout << "f(";
         SetConsoleTextAttribute(hConsole, 240);
         cout << x;
         SetConsoleTextAttribute(hConsole, 249);
         cout << ") = ";
         SetConsoleTextAttribute(hConsole, 242);
         cout << firstHalf;
         SetConsoleTextAttribute(hConsole, 249);
         cout << " + ";
         SetConsoleTextAttribute(hConsole, 245);
         cout << secondHalf << endl;
         SetConsoleTextAttribute(hConsole, 249);
         
         // Display the result of a * f(n-2) + b * f(n-1) for a given value of n
         cout << "f(";
         SetConsoleTextAttribute(hConsole, 240);
         cout << x;
         SetConsoleTextAttribute(hConsole, 249);
         cout << ") = ";
         SetConsoleTextAttribute(hConsole, 253);
         cout << fn << endl;
         SetConsoleTextAttribute(hConsole, 249);
         cout << endl;
         
         functionCounter++;
      }
   }
   return fn; 
}

/*
########################################################################################################
FUNCTION: solveAgain
PURPOSE: This function accepts and validates the user's choice to solve the function again with another
   set of values.
@param again     Reference variable that will store true if the user wants to solve again, false if not
######################################################################################################## 
*/

void solveAgain(bool &again)
{
   // Get console handle
   HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
   
   char letter;
      
   do
   {
      cout << "\nDo you want solve the function with another set of values? (Y/N): ";
      SetConsoleTextAttribute(hConsole, 240);
      cin >> letter;
      SetConsoleTextAttribute(hConsole, 249);
      
      letter = (char)toupper(letter);  // Force character to be uppercase
   
      if(letter == 'Y')
      {
         again = true;
      }
      else if(letter == 'N')
      {
         again = false;
         cout << "\nEnding program.\n" << endl;
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
