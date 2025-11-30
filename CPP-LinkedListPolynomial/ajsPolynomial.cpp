/*
########################################################################################################
FILE: ajsPolynomial.cpp
WRITTEN BY: Andrea Sitton
PURPOSE: Implementation file for the Polynomial class
########################################################################################################
*/

#include "ajsPolynomial.h"
#include <iomanip>
#include <iostream>
using namespace std;

/*
########################################################################################################
FUNCTION: Polynomial
PURPOSE: Default constructor
########################################################################################################
*/

Polynomial::Polynomial()
{
    head = nullptr;
}

/*
########################################################################################################
FUNCTION: Polynomial
PURPOSE: Default destructor
########################################################################################################
*/

Polynomial::~Polynomial()
{

}

/*
########################################################################################################
FUNCTION: addNewTerm
PURPOSE: This mutator function adds a new polynomial term to the Polynomial object calling the function.
@param   coe          The coefficient of the term being added.
         exp          The exponent of the term being added.
########################################################################################################
*/

void Polynomial::addNewTerm(double coe, int exp)
{
    // Declare and assign a pointer to a new polynomial term
    polynomialTerm *newTerm = new polynomialTerm;
    // Declare a pointer to move through the polynomial
    polynomialTerm *termPtr;

    // Store values in new term
    newTerm->coefficient = coe;
    newTerm->exponent = exp;
    newTerm->prev = nullptr;
    newTerm->next = nullptr;

    // Increase term count
    termsCount++;

    // Check for other terms in polynomial
    if (!head)
    {
        head = newTerm;
    }
    else
    {
        termPtr = head;
        while (termPtr)
        {
            // If the new term has the biggest exponent, insert term after head
            // but before any other terms (i.e., at the very beginning of the polynomial)
            if (termPtr->exponent < newTerm->exponent && termPtr == head)    // Ex: inserting the term 7x^6 into 5x^4 + 4x^2
            {                                                // Since x^4 < x^6, termPtr is pointing to 5x^4
                newTerm->next = termPtr;                     // Make 7x^6 point down to 5x^4
                termPtr->prev = newTerm;                     // Make 5x^4 point up to 7x^6
                head = newTerm;                              // Make head pointer point to first term in polynomial (7x^6)
                break;
            }
            // Insert somewhere in the middle of the polynomial
            else if (termPtr->exponent < newTerm->exponent)     // Ex. inserting the term 8x^3 into 5x^4 + 4x^2
            {                                                   // Since x^2 < x^3, termPtr is pointing to 4x^2
                newTerm->prev = termPtr->prev;                  // Make 8x^3 point up to 5x^4
                newTerm->next = termPtr;                        // Make 8x^3 point down to 4x^2
                termPtr->prev->next = newTerm;                  // Make 5x^4 point down to 8x^3 (referenced through 4x^2)
                termPtr->prev = newTerm;                        // Make 4x^2 point up to 8x^3
                break;
            }
            // Add to the end of the polynomial
            else if (termPtr->exponent > newTerm->exponent && !termPtr->next)    // Ex. inserting the term 12x into 5x^4 + 4x^2
            {                                                               // Since x^2 > x^1, termPtr is pointing to 4x^2
                newTerm->prev = termPtr;                                    // Make 12x point up to 4x^2
                termPtr->next = newTerm;                                    // Make 4x^2 point down to 12x
                break;
            }
            // If the exponent of the new term matches an exponent of an existing term, combine them
            else if (termPtr->exponent == newTerm->exponent)
            {
                // Combining isn't adding a new term, so remove the count we added at the beginning of the function
                termsCount--;
                // If the two terms cancel each other out, we need to delete the term
                if (termPtr->coefficient + newTerm->coefficient == 0)
                {
                    // We need to know the position of the term being deleted, so start at the beginning of the list again
                    polynomialTerm *temp = head;
                    for (int count = 1; count <= termsCount; count++)
                    {
                        // Check to see if the coefficient and exponent of the to-be-deleted term match the temporary pointer
                        if (temp->coefficient == termPtr->coefficient && temp->exponent == termPtr->exponent)
                        {
                            // Delete the term at the designated position
                            this->deleteTerm(count);
                            break;
                        }
                        else
                            temp = temp->next;
                    }
                }
                // If the terms don't cancel out, add the coefficients together
                else
                    termPtr->coefficient += newTerm->coefficient;
                break;
            }
            else
            {
                // Advance to the next term
                termPtr = termPtr->next;
            }
        }
    }
}

/*
########################################################################################################
FUNCTION: modifyTerm
PURPOSE: This mutator function modifies an existing polynomial term of the Polynomial object calling
    the function.
@param   term         The number of the term to be modified.
         coe          The coefficient of the term being added.
         exp          The exponent of the term being added.
########################################################################################################
*/

void Polynomial::modifyTerm(int term, double coe, int exp)
{
    // Move through the polynomial, starting from the beginning
    polynomialTerm *termPtr = head;

    // Find the values stored in the desired term
    for (int i = 1; i < term; i++)
    {
        // Advance to the next term
        termPtr = termPtr->next;
    }

    // If the exponent is changing, delete the old term and
    // add a new term with the desired coefficient/exponent
    if (termPtr->exponent != exp)
    {
        deleteTerm(term);
        addNewTerm(coe, exp);
    }
    // If the exponent is not changing, adjust the coefficient
    else
    {
        termPtr->coefficient = coe;
    }
}

/*
########################################################################################################
FUNCTION: deleteTerm
PURPOSE: This mutator function removes an existing polynomial term from the Polynomial object calling
    the function.
@param   term         The number of the term to be removed.
########################################################################################################
*/

void Polynomial::deleteTerm(int term)
{
    // Move through the polynomial, starting from the beginning
    polynomialTerm *termPtr = head;

    // Find the values stored in the desired term
    for (int i = 1; i < term; i++)
    {
        // Advance to the next term
        termPtr = termPtr->next;
    }

    // If deleting the only term in the polynomial
    if (termsCount == 1)
    {
        head = nullptr;
    }
    // The first term in the polynomial
    else if (term == 1)
    {
        head = termPtr->next;
        termPtr->next->prev = nullptr;
    }
    // The last term in the polynomial
    else if (term == termsCount)
    {
        termPtr->prev->next = nullptr;
    }
    //  Somewhere in the middle
    else
    {
        termPtr->prev->next = termPtr->next;
        termPtr->next->prev = termPtr->prev;
    }
    delete termPtr;
    termsCount--;
}

/*
########################################################################################################
FUNCTION: printTerms
PURPOSE: This accessor function displays all the individual polynomial terms from the Polynomial object
    calling the function so that the user can choose one to modify or delete.
########################################################################################################
*/

void Polynomial::printTerms() const
{
    // Check for empty Polynomial object
    if (!head)
    {
        cout << "No terms in the polynomial.";;
        return;
    }

    // Move through the polynomial, starting from the beginning
    polynomialTerm *termPtr = head;

    // Display each term individually for user to select term to modify or remove
    for (int i = 0; i < termsCount; i++)
    {
        cout << "Term " << i + 1 << ": ";
        if ((termPtr->coefficient == 1 || termPtr->coefficient == -1) && termPtr->exponent > 0)
            cout << "";
        else
            cout << fixed << setprecision(1) << termPtr->coefficient;

        if (termPtr->exponent > 1)
            // Display x^ when exponent is 2 or higher
            cout << "x^" << termPtr->exponent << endl;
        else if (termPtr->exponent == 1)
            // Display x when exponent is exactly 1
            cout << "x" << endl;
        else
            cout << endl;
        // Advance to the next term
        termPtr = termPtr->next;
    }
}

/*
########################################################################################################
FUNCTION: printPolynomial
PURPOSE: This accessor function displays the entirety of the Polynomial object in the standard
    polynomial format (e.g., descending order of exponents, not showing coefficients of 1 or exponents
    of 0).
########################################################################################################
*/

void Polynomial::printPolynomial() const
{
    // Check for empty Polynomial object
    if (!head)
    {
        cout << "No terms in the polynomial.";;
        return;
    }

    // Move through the polynomial, starting from the beginning
    polynomialTerm *termPtr = head;

    while (termPtr)     // While termPtr points to a term and not nullptr
    {
        // If the coefficient is 1 or -1 with an exponent, don't display the redundant '1' -> x^2 instead of 1x^2
        if ((termPtr->coefficient == 1 || termPtr->coefficient == -1) && termPtr->exponent > 0)
            cout << "";
        else if (termPtr->coefficient >= 0 || termPtr == head)
            // Print the positive coefficient
            cout << fixed << setprecision(1) << termPtr->coefficient;
        else
            // If coefficient is negative, multiply by -1 to remove negative sign
            cout << fixed << setprecision(1) << termPtr->coefficient * -1;

        if (termPtr->exponent > 1)
            // Display x^ when exponent is 2 or higher
             cout << "x^" << termPtr->exponent;
        else if (termPtr->exponent == 1)
            // Display x when exponent is exactly 1
            cout << "x";

        // If there is another term and that term's coefficient is positive, print +
        if (termPtr->next && termPtr->next->coefficient >= 0)
            cout << " + ";
        // If there is another term and that term's coefficient is negative, print -
        // This is why negative coefficients are multiplied by -1 above
        else if (termPtr->next && termPtr->next->coefficient < 0)
            cout << " - ";

        // Advance to the next term
        termPtr = termPtr->next;
    }
}

/*
########################################################################################################
FUNCTION: sumPolynomials
PURPOSE: This accessor function creates a new, temporary Polynomial object and adds all terms from
    both polynomials to it.
@param   p2         The second polynomial object. The first one is calling the function.
########################################################################################################
*/

void Polynomial::sumPolynomials(Polynomial p2) const
{
    Polynomial sum;         // Temporary object used to store terms from both polynomials
    // Move through the polynomials, starting from the beginning
    polynomialTerm *p1Ptr = head;
    polynomialTerm *p2Ptr = p2.head;

    // Add all terms from the first polynomial
    while (p1Ptr)
    {
        sum.addNewTerm(p1Ptr->coefficient, p1Ptr->exponent);
        p1Ptr = p1Ptr->next;
    }
    // Add all terms from the second polynomial
    while (p2Ptr)
    {
        sum.addNewTerm(p2Ptr->coefficient, p2Ptr->exponent);
        p2Ptr = p2Ptr->next;
    }
    // Print results
    sum.printPolynomial();
}

/*
########################################################################################################
FUNCTION: subtractPolynomials
PURPOSE: This accessor function creates a new, temporary Polynomial object and adds all terms from
    both polynomials to it. The signs of the second polynomial are flipped to achieve subtraction.
@param   p2         The second polynomial object. The first one is calling the function.
########################################################################################################
*/

void Polynomial::subtractPolynomials(Polynomial p2) const
{
    Polynomial difference;          // Temporary object used to store terms from both polynomials
    // Move through the polynomials, starting from the beginning
    polynomialTerm *p1Ptr = head;
    polynomialTerm *p2Ptr = p2.head;

    // Add all terms from the first polynomial
    while (p1Ptr)
    {
        difference.addNewTerm(p1Ptr->coefficient, p1Ptr->exponent);
        p1Ptr = p1Ptr->next;
    }
    // Subtract all terms from the second polynomial
    while (p2Ptr)
    {
        // Multiplying the coefficient by -1 reverses the sign of the term
        difference.addNewTerm(-1 * p2Ptr->coefficient, p2Ptr->exponent);
        p2Ptr = p2Ptr->next;
    }
    // Print results
    difference.printPolynomial();
}

/*
########################################################################################################
FUNCTION: multiplyPolynomials
PURPOSE: This accessor function creates a new, temporary Polynomial object to hold the product of each
    term in the polynomial multiplied by a user-entered monomial.
@param   coe          The coefficient of the monomial.
         exp          The exponent of the monomial.
########################################################################################################
*/

void Polynomial::multiplyPolynomial(double coe, int exp) const
{
    Polynomial product;             // Temporary object used to store results of multiplication
    // Move through the polynomial, starting from the beginning
    polynomialTerm *p1Ptr = head;

    while (p1Ptr)
    {
        // To multiply a polynomial by a monomial, multiply the coefficients and add the exponents
        product.addNewTerm(p1Ptr->coefficient * coe, p1Ptr->exponent + exp);
        p1Ptr = p1Ptr->next;
    }
    // Print results
    product.printPolynomial();
}

/*
########################################################################################################
FUNCTION: getTermsCount
PURPOSE: This accessor function retrieves the number of terms in the Polynomial object that called the
    function.
@return                 The number of terms in the Polynomial object
########################################################################################################
*/

int Polynomial::getTermsCount() const
{
    return termsCount;
}