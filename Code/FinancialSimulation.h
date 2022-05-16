#include <stdbool.h>
typedef struct financialIdentity {
  double savings;
  double checking;
  double debt;
  double mortgage;
  int yearsWithDebt;
  int yearsRented;
  double debtPaid;
  bool ownsHouse;
} financialIdentity;

/*
Takes in a person struct and a yearly interestRate, and it modifies that struct
to reflect that person’s savings after one year.
*/
void savingsPlacement(financialIdentity *person, double interestRate);

/*
Applies monthly payments and updates the amount of remaining debt a
person has and the amount of money they have left in their checking and savings
accounts after one year.
*/
void debt(financialIdentity *person, double interestRate, double addlPay);

/*
Updates the amount of money a person will have left in their checking
and savings accounts after one year of paying rent,
if they’re in the rented apartment.
*/
void rent(financialIdentity *person, double rentAmount);

/*
Updates the amount of money a person will have left in their checking and
savings accounts after one year of paying a mortage,
if they live in a house.
*/
void house(financialIdentity *person, double mortgageAmount, double interestRate, int mortgageTerm);

//Simulates 40 years in the life of a financially and non-financially literate person
int* simulate(financialIdentity *person, double yearlySalary, bool financiallyLiterate, char fileName[]);
