/*
########################################################################################################
PROGRAM: ajsMain.cpp
WRITTEN BY: Andrea Sitton
FOR: CPS 162 - Assignment #3

PURPOSE: This program generates two somewhat randomized polynomial expressions and allows the user to
    add, modify, or delete terms as well as perform addition, subtraction, and multiplication (with
    a user-entered monomial) with the polynomials. It demonstrates the versatility of the linked list.
########################################################################################################
*/

#include "ajsPolynomial.h"
#include <cctype>
#include <cmath>
#include <iostream>
#include <limits>
#include <random>
#include <windows.h>
using namespace std;

// Function prototypes
void userMenu(HANDLE &, Polynomial &, Polynomial &, int &, int &);
void getRandomPoly(HANDLE &, Polynomial &, Polynomial &);
void overwritePoly(HANDLE &, Polynomial &, int);
void getPolyChoice(HANDLE &, const Polynomial &, const Polynomial &, int &);
double getCoefficient(HANDLE &);
int getExponent(HANDLE &);
int getTerm(HANDLE &, const Polynomial &, int);
double randCoefficient();
int randExponent();
int inputValidation(HANDLE &, int, int);
int inputValidation(HANDLE &, int);
char inputValidation(HANDLE &);
void printDashes(int, int);
void printDashes(int, int, int);

int main()
{
    // Get console handle
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Change to a white background with blue text
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
    int userChoice,          // Holds user's choice from the menu interface
        polyChoice;          // Holds user's choice for which polynomial to work with
    Polynomial poly1, poly2; // Create two new polynomials

    // Call menu
    do
    {
        userMenu(hConsole, poly1, poly2, userChoice, polyChoice);
    }
    while (userChoice != 8);

    // Terminate program execution
    system("pause");
    return 0;
}

/*
########################################################################################################
FUNCTION: userMenu
PURPOSE: This function provides the menu by which users can navigate the program's functionality.
@param   hConsole       For manipulating the text color displayed.
         poly1          The first Polynomial object.
         poly2          The second Polynomial object.
         userChoice     Reference variable to store an integer associated with the user's menu choice.
         polyChoice     Reference variable to store an integer associated with the user's polynomial
                            choice.
########################################################################################################
*/

void userMenu(HANDLE &hConsole, Polynomial &poly1, Polynomial &poly2, int &userChoice, int &polyChoice)
{
    // Local variables for holding new or modified polynomial term information
    double coefficient;
    int exponent, term;

    cout << endl;
    do
    {
        // Menu text for user
        cout << "Playing with Polynomials" << endl;
        // This adjusts the number of dividing dashes based on the length of the polynomials
        printDashes(14, poly1.getTermsCount(), poly2.getTermsCount());

        // Polynomials are automatically displayed on the menu screen for convenience
        SetConsoleTextAttribute(hConsole, 240);
        cout << "Polynomial 1: ";
        SetConsoleTextAttribute(hConsole, 245);
        poly1.printPolynomial();
        SetConsoleTextAttribute(hConsole, 240);
        cout << "\nPolynomial 2: ";
        SetConsoleTextAttribute(hConsole, 245);
        poly2.printPolynomial();
        SetConsoleTextAttribute(hConsole, 249);

        cout << endl;
        printDashes(14, poly1.getTermsCount(), poly2.getTermsCount());

        cout << "Menu Options:" << endl;
        cout << "\t1 = Generate a random polynomial" << endl;
        cout << "\t2 = Add a term to one of the polynomials" << endl;
        cout << "\t3 = Modify an existing term in one of the polynomials" << endl;
        cout << "\t4 = Delete a term from one of the polynomials" << endl;
        cout << "\t5 = Display the sum of the polynomials" << endl;
        cout << "\t6 = Display the difference between the polynomials" << endl;
        cout << "\t7 = Multiply one of the polynomials by a monomial" << endl;
        cout << "\t8 = Exit program\n" << endl;
        SetConsoleTextAttribute(hConsole, 242);
        cout << "Please enter the numerical value associated with your choice: ";
        userChoice = inputValidation(hConsole, 1, 8);
    }
    while (userChoice < 1 || userChoice > 8);

    // Navigate to the chosen menu function
    switch (userChoice)
    {
        // Generate a random polynomial
        case 1:
        {
            cout << "\nFor which polynomial do you want to randomly generate terms?" << endl;
            getRandomPoly(hConsole, poly1, poly2);
            break;
        }

        // Add a new term to a polynomial
        case 2:
        {
            cout << "\nWhich polynomial do you want to add a term to?" << endl;
            getPolyChoice(hConsole, poly1, poly2, polyChoice);
            // If user wants to return to the menu
            if (polyChoice == -1)
            {
                cout << "\nReturning to menu.";
                break;
            }
            // Otherwise, get the values for the coefficient and exponent of the new term
            coefficient = getCoefficient(hConsole);
            exponent = getExponent(hConsole);

            if (polyChoice == 1)
                poly1.addNewTerm(coefficient, exponent);
            else if (polyChoice == 2)
                poly2.addNewTerm(coefficient, exponent);

            SetConsoleTextAttribute(hConsole, 245);
            cout << "\nNew term added successfully.";
            SetConsoleTextAttribute(hConsole, 249);
            break;
        }

        // Modify existing polynomial term
        case 3:
        {
            if (poly1.getTermsCount() == 0 && poly2.getTermsCount() == 0)       // Check if both polynomials are empty
            {
                SetConsoleTextAttribute(hConsole, 252);
                cout << "\nNo terms in either polynomial.";
                SetConsoleTextAttribute(hConsole, 249);
                break;
            }
            else
            {
                cout << "\nWhich polynomial do you want to modify?" << endl;
                getPolyChoice(hConsole, poly1, poly2, polyChoice);
                if (polyChoice == 1)
                {
                    if (poly1.getTermsCount() == 0)             // Check if polynomial is empty
                    {

                        SetConsoleTextAttribute(hConsole, 252);
                        cout << "\nNo terms in the polynomial.";
                        SetConsoleTextAttribute(hConsole, 249);
                        break;
                    }
                    else
                    {
                        // Display terms available in polynomial 1.
                        term = getTerm(hConsole, poly1, 1);
                        // Get the modified values for the coefficient and exponent
                        coefficient = getCoefficient(hConsole);
                        exponent = getExponent(hConsole);
                        // Modify term
                        poly1.modifyTerm(term, coefficient, exponent);
                    }
                }
                else if (polyChoice == 2)
                {
                    if (poly2.getTermsCount() == 0)             // Check if polynomial is empty
                    {
                        SetConsoleTextAttribute(hConsole, 252);
                        cout << "\nNo terms in the polynomial.";
                        SetConsoleTextAttribute(hConsole, 249);
                        break;
                    }
                    else
                    {
                        // Display terms available in polynomial 2.
                        term = getTerm(hConsole, poly2, 1);
                        // Get the modified values for the coefficient and exponent
                        coefficient = getCoefficient(hConsole);
                        exponent = getExponent(hConsole);
                        // Modify term
                        poly2.modifyTerm(term, coefficient, exponent);
                    }
                }
                else
                {
                    cout << "\nReturning to menu.";
                    break;
                }

                SetConsoleTextAttribute(hConsole, 245);
                cout << "\nTerm modified successfully.";
                SetConsoleTextAttribute(hConsole, 249);
                break;
            }
        }

        // Delete term from polynomial
        case 4:
        {
            if (poly1.getTermsCount() == 0 && poly2.getTermsCount() == 0)       // Check if both polynomials are empty
            {
                SetConsoleTextAttribute(hConsole, 252);
                cout << "\nNo terms in either polynomial.";
                SetConsoleTextAttribute(hConsole, 249);
                break;
            }
            else
            {
                cout << "\nFrom which polynomial do you want to delete a term?" << endl;
                getPolyChoice(hConsole, poly1, poly2, polyChoice);
                if (polyChoice == 1)
                {
                    if (poly1.getTermsCount() == 0)         // Check if polynomial is empty
                    {
                        SetConsoleTextAttribute(hConsole, 252);
                        cout << "\nNo terms in the polynomial.";
                        SetConsoleTextAttribute(hConsole, 249);
                        break;
                    }
                    else
                    {
                        // Display terms available in polynomial 1.
                        term = getTerm(hConsole, poly1, 2);
                        poly1.deleteTerm(term);
                    }
                }
                else if (polyChoice == 2)
                {
                    if (poly2.getTermsCount() == 0)         // Check if polynomial is empty
                    {
                        SetConsoleTextAttribute(hConsole, 252);
                        cout << "\nNo terms in the polynomial.";
                        SetConsoleTextAttribute(hConsole, 249);
                        break;
                    }
                    else
                    {
                        // Display terms available in polynomial 2.
                        term = getTerm(hConsole, poly2, 2);
                        poly2.deleteTerm(term);
                    }
                }
                else
                {
                    cout << "\nReturning to menu.";
                    break;
                }

                SetConsoleTextAttribute(hConsole, 245);
                cout << "\nTerm deleted successfully.";
                SetConsoleTextAttribute(hConsole, 249);
                break;
            }
        }

        // Display sum of polynomials
        case 5:
        {
            cout << endl;
            printDashes(0, poly1.getTermsCount() + poly2.getTermsCount() - 1);

            SetConsoleTextAttribute(hConsole, 240);
            poly1.printPolynomial();
            SetConsoleTextAttribute(hConsole, 249);
            cout << "\n\n\t\t + \n" << endl;
            SetConsoleTextAttribute(hConsole, 245);
            poly2.printPolynomial();
            SetConsoleTextAttribute(hConsole, 249);
            cout << "\n\n\t\t = \n" << endl;
            SetConsoleTextAttribute(hConsole, 242);
            poly1.sumPolynomials(poly2);
            SetConsoleTextAttribute(hConsole, 249);

            cout << endl;
            printDashes(0, poly1.getTermsCount() + poly2.getTermsCount() - 1);
            break;
        }

        // Display difference between polynomials
        case 6:
        {
            cout << endl;
            printDashes(0, poly1.getTermsCount() + poly2.getTermsCount() - 1);

            SetConsoleTextAttribute(hConsole, 240);
            poly1.printPolynomial();
            SetConsoleTextAttribute(hConsole, 249);
            cout << "\n\n\t\t - \n" << endl;
            SetConsoleTextAttribute(hConsole, 245);
            poly2.printPolynomial();
            SetConsoleTextAttribute(hConsole, 249);
            cout << "\n\n\t\t = \n" << endl;
            SetConsoleTextAttribute(hConsole, 242);
            poly1.subtractPolynomials(poly2);
            SetConsoleTextAttribute(hConsole, 249);

            cout << endl;
            printDashes(0, poly1.getTermsCount() + poly2.getTermsCount() - 1);
            break;
        }

        // Multiply polynomial by monomial
        case 7:
        {
            cout << "\nWhich polynomial do you want to multiply?" << endl;
            getPolyChoice(hConsole, poly1, poly2, polyChoice);
            // If user wants to return to the menu
            if (polyChoice == -1)
            {
                cout << "\nReturning to menu.";
                break;
            }
            cout << "\nPlease enter a monomial (single term) to multiply the polynomial by.";
            // Get the coefficient and exponent of the monomial
            coefficient = getCoefficient(hConsole);
            exponent = getExponent(hConsole);
            if (polyChoice == 1)
            {
                cout << endl;
                printDashes(0, (poly1.getTermsCount() * 2) + 1);

                SetConsoleTextAttribute(hConsole, 240);
                poly1.printPolynomial();
                SetConsoleTextAttribute(hConsole, 249);
                cout << "  *  ";
                SetConsoleTextAttribute(hConsole, 245);

                // This controls how the monomial is displayed
                // If the coefficient is 1, print nothing
                if (coefficient == 1)
                    cout << "";
                // If the coefficient is -1, print only the minus sign
                else if (coefficient == -1)
                    cout << "-";
                // Otherwise, print the coefficient
                else
                    cout << coefficient;

                // If the exponent is greater than 1, display the ^ and value of the exponent
                if (exponent > 1)
                    cout << "x^" << exponent;
                // If the exponent is 1, just display x.
                // If the exponent is 0, display nothing -> 4.1x^0 = 4.1
                else if (exponent == 1)
                    cout << "x";

                SetConsoleTextAttribute(hConsole, 249);
                cout << "  =  ";
                SetConsoleTextAttribute(hConsole, 242);
                poly1.multiplyPolynomial(coefficient, exponent);
                SetConsoleTextAttribute(hConsole, 249);

                cout << endl;
                printDashes(0, (poly1.getTermsCount() * 2) + 1);
            }
            else if (polyChoice == 2)
            {
                cout << endl;
                printDashes(0, (poly2.getTermsCount() * 2) + 1);

                SetConsoleTextAttribute(hConsole, 240);
                poly2.printPolynomial();
                SetConsoleTextAttribute(hConsole, 249);
                cout << "  *  ";
                SetConsoleTextAttribute(hConsole, 245);

                // This controls how the monomial is displayed
                // If the coefficient is 1, print nothing
                if (coefficient == 1)
                    cout << "";
                // If the coefficient is -1, print only the minus sign
                else if (coefficient == -1)
                    cout << "-";
                // Otherwise, print the coefficient
                else
                    cout << coefficient;

                // If the exponent is greater than 1, display the ^ and value of the exponent
                if (exponent > 1)
                    cout << "x^" << exponent;
                // If the exponent is 1, just display x.
                // If the exponent is 0, display nothing -> 4.1x^0 = 4.1
                else if (exponent == 1)
                    cout << "x";

                SetConsoleTextAttribute(hConsole, 249);
                cout << "  =  ";
                SetConsoleTextAttribute(hConsole, 242);
                poly2.multiplyPolynomial(coefficient, exponent);
                SetConsoleTextAttribute(hConsole, 249);

                cout << endl;
                printDashes(0, (poly2.getTermsCount() * 2) + 1);
            }
            break;
        }

        // Exit menu
        case 8:
            cout << "\nEnding program." << endl;
            return;
    }
    cout << endl;
    system("pause");
    system("cls");
}

/*
########################################################################################################
FUNCTION: getRandomPoly
PURPOSE: This function allows the user to generate random polynomial terms for one or both Polynomial
    objects. This makes it easier to test the other mathematical functions of the program without
    requiring the user to manually enter terms.
@param   hConsole       For manipulating the text color displayed.
         poly1          The first Polynomial object.
         poly2          The second Polynomial object.
########################################################################################################
*/

void getRandomPoly(HANDLE &hConsole, Polynomial &poly1, Polynomial &poly2)
{
    int choice;

    do
    {
        SetConsoleTextAttribute(hConsole, 240);
        cout << "\tPolynomial 1: ";
        SetConsoleTextAttribute(hConsole, 245);
        poly1.printPolynomial();
        SetConsoleTextAttribute(hConsole, 240);
        cout << "\n\tPolynomial 2: ";
        SetConsoleTextAttribute(hConsole, 245);
        poly2.printPolynomial();
        SetConsoleTextAttribute(hConsole, 249);

        cout << "\nPlease select a polynomial by entering 1, 2, or 3 for both (";
        SetConsoleTextAttribute(hConsole, 252);
        cout << "or enter -1 to return to menu";
        SetConsoleTextAttribute(hConsole, 249);
        cout << "): ";
        choice = inputValidation(hConsole, 1, 3);
        // If user wants to return to menu
        if (choice == -1)
        {
            cout << "\nReturning to menu.";
            return;
        }

    }
    while (choice != 1 && choice != 2 && choice != 3);

    switch (choice)
    {
        // User wants to generate a random polynomial for Polynomial 1
        case 1:
        {
            // If Polynomial 1 already has terms in it
            if (poly1.getTermsCount() != 0)
            {
                overwritePoly(hConsole, poly1, 1);
            }
            // If Polynomial 1 is empty
            else
            {
                // Add terms to Polynomial 1
                for (int i = 0; i < 4; i++)
                    poly1.addNewTerm(randCoefficient(), randExponent());

                // Confirm successful polynomial generation
                SetConsoleTextAttribute(hConsole, 245);
                cout << "\nRandom terms added successfully to polynomial 1." << endl;
                SetConsoleTextAttribute(hConsole, 249);
            }
            break;
        }

        // User wants to generate a random polynomial for Polynomial 2
        case 2:
        {
            // If Polynomial 2 already has terms in it
            if (poly2.getTermsCount() != 0)
            {
                overwritePoly(hConsole, poly2, 2);
            }
            // If Polynomial 2 is empty
            else
            {
                // Add terms to Polynomial 2
                for (int i = 0; i < 4; i++)
                    poly2.addNewTerm(randCoefficient(), randExponent());

                // Confirm successful polynomial generation
                SetConsoleTextAttribute(hConsole, 245);
                cout << "\nRandom terms added successfully to polynomial 2." << endl;
                SetConsoleTextAttribute(hConsole, 249);
            }
            break;
        }

        // User wants to generate a random polynomial for Polynomial 1 AND Polynomial 2
        case 3:
        {
            // If Polynomial 1 already has terms in it but Polynomial 2 is empty
            if (poly1.getTermsCount() != 0 && poly2.getTermsCount() == 0)
            {
                overwritePoly(hConsole, poly1, 1);

                for (int i = 0; i < 4; i++)
                    poly2.addNewTerm(randCoefficient(), randExponent());

                SetConsoleTextAttribute(hConsole, 245);
                cout << "\nRandom terms added successfully to polynomial 2." << endl;
                SetConsoleTextAttribute(hConsole, 249);
            }
            // If Polynomial 2 already has terms in it but Polynomial 1 is empty
            else if (poly1.getTermsCount() == 0 && poly2.getTermsCount() != 0)
            {
                for (int i = 0; i < 4; i++)
                    poly1.addNewTerm(randCoefficient(), randExponent());

                SetConsoleTextAttribute(hConsole, 245);
                cout << "\nRandom terms added successfully to polynomial 1." << endl;
                SetConsoleTextAttribute(hConsole, 249);

                overwritePoly(hConsole, poly2, 2);
            }
            // If both Polynomial 1 AND Polynomial 2 have terms in them
            else if (poly1.getTermsCount() != 0 && poly2.getTermsCount() != 0)
            {
                overwritePoly(hConsole, poly1, 1);
                overwritePoly(hConsole, poly2, 2);
            }
            // Otherwise, both Polynomial 1 and Polynomial 2 are empty
            else
            {
                for (int i = 0; i < 4; i++)
                    poly1.addNewTerm(randCoefficient(), randExponent());
                SetConsoleTextAttribute(hConsole, 245);
                cout << "\nRandom terms added successfully to polynomial 1." << endl;
                SetConsoleTextAttribute(hConsole, 249);

                for (int i = 0; i < 4; i++)
                    poly2.addNewTerm(randCoefficient(), randExponent());
                SetConsoleTextAttribute(hConsole, 245);
                cout << "\nRandom terms added successfully to polynomial 2." << endl;
                SetConsoleTextAttribute(hConsole, 249);
            }
            break;
        }
    }
}

/*
########################################################################################################
FUNCTION: overwritePoly
PURPOSE: This function allows the user overwrite an existing polynomial with randomly generated terms.
@param   hConsole       For manipulating the text color displayed.
         poly           The Polynomial object.
         num            The polynomial number, for display purposes
########################################################################################################
*/

void overwritePoly(HANDLE &hConsole, Polynomial &poly, int num)
{
    char letter;

    // Confirm that the polynomial contains terms and ask the user if they want to overwrite them
    cout << "\nPolynomial " << num << " already contains terms." << endl;
    SetConsoleTextAttribute(hConsole, 252);
    cout << "Are you sure you want to overwrite the contents of polynomial " << num << " with randomly generated terms? (Y/N): ";
    letter = inputValidation(hConsole);

    // User does not want to overwrite existing terms
    if (letter == 'N')
    {
        SetConsoleTextAttribute(hConsole, 245);
        cout << "\nPolynomial " << num << " was not altered." << endl;
        SetConsoleTextAttribute(hConsole, 249);
    }
    // User did want want to overwrite existing terms
    else if (letter == 'Y')
    {
        // While there are still terms in the Polynomial object, delete term 1
        while (poly.getTermsCount() != 0)
        {
            poly.deleteTerm(1);
        }

        // Add four randomly generated terms to the Polynomial object
        for (int i = 0; i < 4; i++)
            poly.addNewTerm(randCoefficient(), randExponent());

        // Confirm successful overwrite
        SetConsoleTextAttribute(hConsole, 245);
        cout << "\nPolynomial " << num << " was successfully overwritten." << endl;
        SetConsoleTextAttribute(hConsole, 249);
    }
}

/*
########################################################################################################
FUNCTION: getPolyChoice
PURPOSE: This function allows the user to select one of the two polynomials to manipulate.
@param   hConsole       For manipulating the text color displayed.
         poly1          The first Polynomial object.
         poly2          The second Polynomial object.
         polyChoice     Reference variable to store an integer associated with the user's polynomial
                            choice.
########################################################################################################
*/

void getPolyChoice(HANDLE &hConsole, const Polynomial &poly1, const Polynomial &poly2, int &polyChoice)
{
    do
    {
        SetConsoleTextAttribute(hConsole, 240);
        cout << "\tPolynomial 1: ";
        SetConsoleTextAttribute(hConsole, 245);
        poly1.printPolynomial();
        SetConsoleTextAttribute(hConsole, 240);
        cout << "\n\tPolynomial 2: ";
        SetConsoleTextAttribute(hConsole, 245);
        poly2.printPolynomial();
        SetConsoleTextAttribute(hConsole, 249);
        cout << "\nPlease select a polynomial by entering 1 or 2 (";
        SetConsoleTextAttribute(hConsole, 252);
        cout << "or enter -1 to return to menu";
        SetConsoleTextAttribute(hConsole, 249);
        cout << "): ";
        polyChoice = inputValidation(hConsole, 1, 2);
        // If user wants to return to menu
        if (polyChoice == -1)
            return;
    }
    while (polyChoice != 1 && polyChoice != 2);
}

/*
########################################################################################################
FUNCTION: getCoefficient
PURPOSE: This function prompts the user to enter the value for a coefficient and returns the value to
    the main function
@param   hConsole       For manipulating the text color displayed.
@return                 The user-entered value of the new coefficient
########################################################################################################
*/

double getCoefficient(HANDLE &hConsole)
{
    double value;
    bool valid = false;

    cout << endl;
    do
    {
        cout << "Please enter a value for the ";
        SetConsoleTextAttribute(hConsole, 242);
        cout << "coefficient";
        SetConsoleTextAttribute(hConsole, 249);
        cout << " of the term: ";

        SetConsoleTextAttribute(hConsole, 240);
        cin >> value;
        SetConsoleTextAttribute(hConsole, 249);

        // Catches non-numeric input
        if (cin.fail())
        {
            SetConsoleTextAttribute(hConsole, 252);
            cout << "\nInvalid input. An numerical value is required.\n" << endl;
            SetConsoleTextAttribute(hConsole, 249);
            cin.clear(); // Clear the failbit
            // Ignore the remaining input
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else if (value == 0)
        {
            SetConsoleTextAttribute(hConsole, 252);
            cout << "\nInvalid coefficient. A non-zero numerical value is required.\n" << endl;
            SetConsoleTextAttribute(hConsole, 249);
        }
        else
            valid = true;
    }
    while (!valid);

    return value;
}

/*
########################################################################################################
FUNCTION: getExponent
PURPOSE: This function prompts the user to enter the value for an exponent and returns the value to
    the main function
@param   hConsole       For manipulating the text color displayed.
@return                 The user-entered value of the new exponent
########################################################################################################
*/

int getExponent(HANDLE &hConsole)
{
    int value;

    do
    {
        cout << "Please enter a value for the ";
        SetConsoleTextAttribute(hConsole, 242);
        cout << "exponent";
        SetConsoleTextAttribute(hConsole, 249);
        cout << " (GTE to 0) of the term: ";
        // For the purposes of this program, all exponents must be positive
        value = inputValidation(hConsole, 0);
    }
    while (value < 0);
    return value;
}

/*
########################################################################################################
FUNCTION: getTerm
PURPOSE: This function displays a list of the terms in the chosen polynomial and allows the user to
    select one to modify or delete.
@param   hConsole       For manipulating the text color displayed.
         poly           The chosen polynomial.
         textModifier   Depending on where the function was called, controls the wording provided to the
                            user.
@return                 The user-entered value representing the term they want to modify or delete.
########################################################################################################
*/

int getTerm(HANDLE &hConsole, const Polynomial &poly, int textModifier)
{
    int value;

    cout << endl;
    do
    {
        poly.printTerms();
        SetConsoleTextAttribute(hConsole, 242);
        cout << "\nPlease select the term you wish to ";
        if (textModifier == 1)
            cout << "modify: ";
        else if (textModifier == 2)
            cout << "remove: ";
        value = inputValidation(hConsole, 1, poly.getTermsCount());
    }
    while (value < 1 || value > poly.getTermsCount());  // Ensure that only valid terms can be chosen
    return value;
}

/*
########################################################################################################
FUNCTION: randCoefficient
PURPOSE: This function produces a random double between -50.0 and 50. It is used to generate starting
    polynomials for the user to interface with.
@return                 The random double to be used as a starting coefficient
########################################################################################################
*/

double randCoefficient()
{
    // Define range. The range is arbitrary, but I limited it to keep the numbers in a smaller starting pool.
    // Users can add or modify terms with coefficients outside of this range.
    const double MAX_COEFFICIENT = 50.0;
    const double MIN_COEFFICIENT = -50.0;

    // Create random number engine and distribution
    random_device rd;
    default_random_engine engine(rd());
    uniform_real_distribution<double> distribution(MIN_COEFFICIENT, MAX_COEFFICIENT);


    // Get a random coefficient between -50.0 and 50.0
    double coefficient = round(distribution(engine) * 10.0) / 10.0;
    return coefficient;
}

/*
########################################################################################################
FUNCTION: randExponent
PURPOSE: This function produces a random integer between 0 and 10. It is used to generate starting
    polynomials for the user to interface with.
@return                 The random integer to be used as a starting exponent
########################################################################################################
*/

int randExponent()
{
    // Define range. The range is arbitrary, but I limited it to keep the numbers in a smaller starting pool.
    // Users can add or modify terms with exponents greater than 10.
    const int MAX_EXPONENT = 10;
    const int MIN_EXPONENT = 0;

    // Create random number engine and distribution
    random_device rd;
    default_random_engine engine(rd());
    uniform_int_distribution<int> distribution(MIN_EXPONENT, MAX_EXPONENT);

    // Get a random exponent between 0 and 10
    int exponent = distribution(engine);
    return exponent;
}

/*
########################################################################################################
FUNCTION: inputValidation
PURPOSE: This function validates input from the user, ensuring that it is an integer that falls
    between two values.
@param   hConsole        For manipulating the text color displayed.
         parameter1      The lower limit of the acceptable range of values.
         parameter2      The upper limit of the acceptable range of values.
@return                  The validated integer input of the user.
########################################################################################################
*/

int inputValidation(HANDLE &hConsole, int parameter1, int parameter2)
{
    int value;

    SetConsoleTextAttribute(hConsole, 240);
    cin >> value;
    SetConsoleTextAttribute(hConsole, 249);

    // Catches non-numeric input
    if (cin.fail())
    {
        SetConsoleTextAttribute(hConsole, 252);
        cout << "\nInvalid input. An integer is required.\n" << endl;
        SetConsoleTextAttribute(hConsole, 249);
        cin.clear(); // Clear the failbit
        // Ignore the remaining input
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    // To allow user to return to the menu
    else if (value == -1)
        return value;
    else if (value < parameter1 || value > parameter2)
    {
        SetConsoleTextAttribute(hConsole, 252);
        cout << "\nValue must be GTE to " << parameter1 << " and LTE to " << parameter2 << ".\n" << endl;
        SetConsoleTextAttribute(hConsole, 249);
    }

    return value;
}

/*
########################################################################################################
FUNCTION: inputValidation
PURPOSE: This function validates input from the user, ensuring that it is a positive integer.
@param   hConsole        For manipulating the text color displayed.
         parameter1      The lower limit of the acceptable range of values.
@return                  The validated integer input of the user.
########################################################################################################
*/

int inputValidation(HANDLE &hConsole, int parameter1)
{
    int value; // To hold user input

    SetConsoleTextAttribute(hConsole, 240);
    cin >> value;
    SetConsoleTextAttribute(hConsole, 249);

    // Catches non-numeric input
    if (cin.fail())
    {
        SetConsoleTextAttribute(hConsole, 252);
        cout << "\nInvalid input. A numerical value is required.\n" << endl;
        SetConsoleTextAttribute(hConsole, 249);
        cin.clear(); // Clear the fail bit
        // Ignore the remaining input
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        // Force value to be a failing value
        value = -1;
    }
    else if (value < parameter1)
    {
        SetConsoleTextAttribute(hConsole, 252);
        cout << "\nValue must be greater than or equal to " << parameter1 << ".\n" << endl;
        SetConsoleTextAttribute(hConsole, 249);
    }

    return value;
}

/*
########################################################################################################
FUNCTION: inputValidation
PURPOSE: This function validates input from the user, ensuring that it is either Y or N.
@param   hConsole        For manipulating the text color displayed.
@return                  The validated character input of the user.
########################################################################################################
*/

char inputValidation(HANDLE &hConsole)
{
    char letter;
    do
    {
        SetConsoleTextAttribute(hConsole, 240);
        cin >> letter;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        SetConsoleTextAttribute(hConsole, 249);

        letter = static_cast<char>(toupper(letter)); // Force character to be uppercase

        if (letter != 'Y' && letter != 'N')
        {
            SetConsoleTextAttribute(hConsole, 252);
            cout << "\nInvalid input. Please enter 'Y' or 'N': ";
            SetConsoleTextAttribute(hConsole, 249);
        }
    }
    while (letter != 'Y' && letter != 'N');

    return letter;
}

/*
########################################################################################################
FUNCTION: printDashes
PURPOSE: This function prints dashes based on the number of polynomial terms being displayed.
@param   label          To accommodate the length of any labels (e.g., "Polynomial 1: ")
         terms          The number of terms being displayed horizontally
########################################################################################################
*/

void printDashes(int label, int terms)
{
    // If the multiplication function is called with an empty polynomial
    if (terms == 1)
        for (int i = 0; i < 70 + label; i++)
            cout << "-";
    // If the addition or subtraction functions are called with empty polynomials
    else if (terms <= 0)
        for (int i = 0; i < 30 + label; i++)
            cout << "-";
    // 11 is an estimate of the number of characters per term
    else
        for (int i = 0; i < (terms * 11) + label; i++)
            cout << "-";
    cout << endl;
}

/*
########################################################################################################
FUNCTION: printDashes
PURPOSE: This function prints dashes based on the number of polynomial terms being displayed. The number
    of terms from two different Polynomial objects are provided, and the number of dashes displayed are
    based on the larger polynomial.
@param   label          To accommodate the length of any labels (e.g., "Polynomial 1: ")
         poly1          The number of terms in the first polynomial
         poly2          The number of therms in the second polynomial
########################################################################################################
*/

void printDashes(int label, int poly1, int poly2)
{
    // If both objects are empty
    if (poly1 == 0 || poly2 == 0)
    {
        for (int i = 0; i < 30 + label; i++)
            cout << "-";
    }
    else if (poly1 >= poly2)
    {
        // 11 is an estimate of the number of characters per term
        for (int i = 0; i < (poly1 * 11) + label; i++)
            cout << "-";
    }
    else
        for (int i = 0; i < (poly2 * 11) + label; i++)
            cout << "-";
    cout << endl;
}