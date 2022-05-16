#include "FinancialSimulation.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

//Applies annual interest rate to savings.
void savingsPlacement(financialIdentity *person, double interestRate){
  double preAmount = person->savings;
  double postAmount = preAmount + (preAmount * interestRate);
  person->savings = postAmount;
}

//Pays one year's worth of rent.
//If checkings is empty the difference will be subtracted from savings.
void rent(financialIdentity *person, double rentAmount) {
  person->yearsRented += 1;
  double yearlyPayment = rentAmount * 12;
  if(yearlyPayment <= person->checking) {
    person->checking = person->checking - yearlyPayment;
  }
  else {
    double save = yearlyPayment - person->checking;
    person->checking = 0;
    person->savings = person->savings - save;
  }
}

//Pays one year's worth of monthly debt considering accrued interest
//If checkings is empty the difference will be subtracted from savings.
void debt(financialIdentity *person, double interestRate, double addlPay) {
  //Calculates annual interest considering monthly rate
  for(int i = 0; i < 12; i++) {
    if(person->debt > 0) {
      double payment = (person->debt * 0.03) + addlPay;
      if(payment <= person->checking) {
        person->checking = person->checking - payment;
        person->debt = person->debt - payment;
        person->debtPaid = person->debtPaid + payment;
      }
      else {
        double save = payment - person->checking;
        person->checking = 0;
        person->savings = person->savings - save;
        person->debt = person->debt - payment;
        person->debtPaid = person->debtPaid + payment;
      }
    }
  }

  //Applies annual interest to remaining debt
  if(person->debt > 0){
    person->yearsWithDebt += 1;
    person->debt = person->debt + (person->debt * interestRate);
  }

  //Reimburses if below zero
  if(person->debt < 0){
    person->checking = person->checking - person->debt;
    person->debt = 0;
  }
}

//Pays one year's worth of mortgage.
//If checkings is empty the difference will be subtracted from savings.
void house(financialIdentity *person, double mortgageAmount, double interestRate, int mortgageTerm){
  //Initialize local variables
  int numPayments = mortgageTerm * 12;
  double monthlyRate = interestRate / 12;
  double num = (pow(1 + monthlyRate, numPayments) - 1);
  double denom = (monthlyRate * pow(1 + monthlyRate, numPayments));
  double discount = num / denom;
  double monthlyPayment = mortgageAmount / discount;

  //Simulating 12 months of mortgate payments.
  for(int i = 0; i < 12; i++){
    if(person->mortgage > 0){
      //Calculates monthly interst to be added
      double monthlyInterest = monthlyRate * person->mortgage;

      if(monthlyPayment <= person->checking){
        person->checking = person->checking - monthlyPayment;
        person->mortgage = person->mortgage - monthlyPayment;
        person->mortgage = person->mortgage + monthlyInterest;
      }
      else{
        double savingsPayment = monthlyPayment - person->checking;
        person->checking = 0;
        person->savings = person->savings - savingsPayment;
        person->mortgage = person->mortgage - monthlyPayment;
        person->mortgage = person->mortgage + monthlyInterest;
      }
    }
  }

  //Reimburses if below zero
  if(person->mortgage < 0){
    person->checking = person->checking - person->mortgage;
    person->mortgage = 0;
  }
}

//"Simulates" the lifetime of a financially literate and non-financially literate person.
//Displays results as a table to both terminal and output .txt file.
int* simulate(financialIdentity *person, double yearlySalary, bool financiallyLiterate, char fileName[]){
  //Initialize local variables.
  double savSalary = 0.2;
  double checkSalary = 0.3;
  double homeValue = 210000;
  int wealth = person->savings + person->checking - person->debt - person->mortgage;

  //Initilize file writing.
  FILE *fptr;
  fptr = fopen(fileName, "w");
  if (fptr == NULL){
       printf("Error! opening file");
       exit(1);
  }
  if(financiallyLiterate){
    fprintf(fptr, "SIMULATED WEALTH PREDICTION FROM FINANCIALLY LITERATE DECISIONS\n");
  }
  else{
    fprintf(fptr, "SIMULATED WEALTH PREDICTION FROM NON-FINANCIALLY LITERATE DECISIONS \n");
  }
  fprintf(fptr, "Years   Checkings   Savings   Debt   DebtPaid   Mortgage   yearsWithDebt   YearsRented   OwnsHouse   Wealth\n");

  //Simulated 40 yearly cycles.
  for(int i = 0; i <= 40; i++){
    person->savings = person->savings + (yearlySalary * savSalary);
    person->checking = person->checking + (yearlySalary * checkSalary);

    //Performs yearly actions of either a financially literate or non-financially literate person based on input.
    if(financiallyLiterate){
      savingsPlacement(person, 0.07);
      debt(person, 0.2, 100);
      double downpayment = homeValue * 0.2;
      if(person->ownsHouse){
        house(person, homeValue, 0.045, 30);
      }
      else if(person->savings >= downpayment){
        person->ownsHouse = true;
        person->savings = person->savings - downpayment;
        person->mortgage = homeValue - downpayment;
      }
      else{
        rent(person, 950);
      }
    }
    else{
      savingsPlacement(person, 0.01);
      debt(person, 0.2, 0);
      double downpayment = homeValue * 0.05;
      if(person->ownsHouse){
        house(person, homeValue, 0.05, 30);
      }
      else if(person->savings >= downpayment){
        person->ownsHouse = true;
        person->savings = person->savings - downpayment;
        person->mortgage = homeValue - downpayment;
      }
      else{
        rent(person, 950);
      }
    }

    //Calculates wealth.
    wealth = person->savings + person->checking - person->debt - person->mortgage;

    //Writes to the desired file.
    fprintf(fptr, "%2d\t", i);
    fprintf(fptr, "%13.0f\t", person->checking);
    fprintf(fptr, "%9.0f\t", person->savings);
    fprintf(fptr, "%6.0f\t", person->debt);
    fprintf(fptr, "%9.0f\t", person->debtPaid);
    fprintf(fptr, "%10.0f\t", person->mortgage);
    fprintf(fptr, "%14d\t", person->yearsWithDebt);
    fprintf(fptr, "%12d\t", person->yearsRented);
    if(person->ownsHouse){
      fprintf(fptr, "\t\t\t Yes\t");
    }
    else{
      fprintf(fptr, "\t\t\t\tNo\t");
    }
    fprintf(fptr, "%3d\n", wealth);
  }
  return wealth;
}

//Main method
int main(int argc ,char* argv[]){
  //Initialize a financially literate person
  double savingsFL = 5000;
  double checkingFL = 0;
  double debtFL = 52500;
  double mortgageFL = 0;
  int yearsWithDebtFL = 0;
  int yearsRentedFL = 0;
  double debtPaidFL = 0;
  bool ownsHouseFL = false;
  bool financiallyLiterateFL = true;
  struct financialIdentity fl = {savingsFL, checkingFL, debtFL, mortgageFL, yearsWithDebtFL, yearsRentedFL, debtPaidFL, ownsHouseFL};

  //Initialize a non-financially literate person
  double savingsNFL = 5000;
  double checkingNFL = 0;
  double debtNFL = 52500;
  double mortgageNFL = 0;
  int yearsWithDebtNFL = 0;
  int yearsRentedNFL = 0;
  double debtPaidNFL = 0;
  bool ownsHouseNFL = false;
  bool financiallyLiterateNFL = false;
  struct financialIdentity nfl = {savingsNFL, checkingNFL, debtNFL, mortgageNFL, yearsWithDebtNFL, yearsRentedNFL, debtPaidNFL, ownsHouseNFL};

  //Simulates 40 years in the life of a financially and non-financially literate person
  simulate(&fl, 69000, financiallyLiterateFL, argv[1]);
  simulate(&nfl, 69000, financiallyLiterateNFL, argv[2]);
  return 0;
}
