/*
########################################################################################################
FILE: ajsPolynomial.h
WRITTEN BY: Andrea Sitton
PURPOSE: Specification file for the Polynomial class
########################################################################################################
*/

#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H
using namespace std;

// Polynomial class definition
class Polynomial
{
    private:
        // Structure for each term in the polynomial
        struct polynomialTerm
        {
            double coefficient;         // Term's coefficient
            int exponent;               // Term's exponent
            polynomialTerm *prev;       // To point to the previous term
            polynomialTerm *next;       // To point to the next term
        };

        polynomialTerm *head;           // Pointer to head of linked list
        int termsCount = 0;             // To count the number of terms in the polynomial
    public:
        Polynomial();                                                 // Default constructor
        ~Polynomial();                                                // Default destructor
        // Mutator methods
        void addNewTerm(double, int);
        void modifyTerm(int, double, int);
        void deleteTerm(int);
        // Accessor methods
        void printTerms() const;
        void printPolynomial() const;
        void sumPolynomials(Polynomial) const;
        void subtractPolynomials(Polynomial) const;
        void multiplyPolynomial(double, int) const;
        int getTermsCount() const;
};

#endif