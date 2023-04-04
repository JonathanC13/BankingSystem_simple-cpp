

#include<string>
#include<iostream>
#include<iomanip>
#include <sstream>

#include "AccountManagement.h"
#include "inputHandler.h"
#include "fileOperations.h"

/*
Purpose:
Params:
  
Return:
  
*/

/*
Todo:
  Withdraw
*/

// Close account: ask to withdraw all money or transfer. If withdraw, then set to 0 and lock account (Locked = 3). If transfer, transfer to valid account then lock.






/*
Purpose: Constructor, save file name that will be used for the account. Calls function to prompt user for commands to interaction with the account.
Params:
  string strFileName: name of the XML file with account information

  string strAccountName: target account name

  string strAccountNumber: strAccountNumber

  int &retCode: assigns a value depending on the nature of the exiting of the account management. 0 for successful exit (user selected 'exit' command), 1 for a failure in a command and had to exit account.
Return:
  N/A
*/
AccountManagement::AccountManagement(const char* strFileName, std::string strAccountName, std::string strAccountNumber, int &retCode)
{
  this->strFileName = strFileName;
  this->strAccountName = strAccountName;
  this->strAccountNumber = strAccountNumber;

  retCode = AccountCommandCenter();
}

/*
Purpose: Prompts user for commands to interact with the account.
Params:
  N/A
Return:
  int: 
    0, for successful exit (user selected 'exit' command)
    1, for a failure in a command and had to exit account.
*/
int AccountManagement::AccountCommandCenter(){

  int selectedOption;
  int continueFlag = 1;

  int operationStatus;

  while(continueFlag == 1){ // 

    std::cout << "===== Account Management for account: " << this->strAccountName << "::" << this->strAccountNumber << " =====" << std::endl << std::endl;

    // display options:
    std::cout << "Options menu:" << std::endl;
    std::cout << "0: Exit" << std::endl;
    std::cout << "1: Balance" << std::endl;
    std::cout << "2: Withdraw" << std::endl;
    std::cout << "3: Deposit" << std::endl;
    std::cout << "4: Transfer" << std::endl;
    std::cout << "99: Close Account" << std::endl;
    std::cout << "-----------------" << std::endl;

    // get any valid int from user
    selectedOption = inputHandler::getUserValidInt();

    // When an integer is entered is entered, perform action if a valid option or prompt user again to enter a valid option's integer.
    switch(selectedOption){
      case 0:
        continueFlag = 0;
        break;
      case 1:
        operationStatus = printBalance();
        break;
      case 2:
        operationStatus = Withdraw();
        break;
      case 3:

        break;
      case 99:
        break;
      default:
        std::cout << "Error, user has entered a number for an option that does not exist." << std::endl << std::endl;
        break;
    }

    std::cout << "\n";

    if(operationStatus == 1){
      std::cout << "Issue detected, ending session now." << std::endl;
      break;
    }


  }

  return 0;
}

// 0 for true
// 1 for false
// 2 for error
int AccountManagement::checkIfAccountStillExists(){
  return fileOperations::getFlagValidAccount(this->strFileName, this->strAccountName, this->strAccountNumber);
}


/*
Purpose: Access account information on file and return the balance.
Params:
  N/A
Return:
  int: For status of retrieving balance
    0; success, could access the account and print the balance
    1; failed to access the account and print the balance
*/
int AccountManagement::printBalance(){

  std::string balanceRet;

  // checking if account still exists
  int i = checkIfAccountStillExists();
  if(i == 2){
    std::cout << "Error accessing account." << std::endl;
    return 1;
  } else if (i == 1){
    std::cout << "Account could not be found at this time." << std::endl;
    return 1;
  } else {
    // get and print the balance
    i = fileOperations::getBalance(this->strFileName, this->strAccountName, this->strAccountNumber, balanceRet);

    if(i == 1){
      std::cout << "Error accessing account." << std::endl;
      return 1;
    } else {
      std::cout << "Account balance: " << balanceRet << std::endl;
      return 0;
    }

  }



  return 0;
}

/*
Purpose: Prompt the user the amount to be withdrawn and if valid amount, subtract from the balance and then update the Balance's node data
Params:
  N/A
Return:
  int: For status for withdraw command
    0; success to update the balance for withdraw
    1; failed to update the balance for withdraw
*/
int AccountManagement::Withdraw(){

  int opertationStatus;

  long double difference;

  std::string strBalance;
  long double withdrawAmt;
  long double ldBalance;

  std::string::size_type sz;
 

  //Get the balance:
  opertationStatus = fileOperations::getBalance(this->strFileName, this->strAccountName, this->strAccountNumber, strBalance);

  if(opertationStatus == 1){
    std::cout << "Error accessing account." << std::endl;
    return 1;
  } else {
    std::cout << "Account balance: " << strBalance << std::endl;
  }
  
  ldBalance = std::stold(strBalance, &sz);

  std::cout << "Current balance: " << strBalance << std::endl;

  std::cout << "Enter 0 to exit." << std::endl;
  std::cout << "Please enter the amount you wish to withdraw, up to 2 fractional digits (123.yy): " << std::endl; 
  
  withdrawAmt = inputHandler::getUserValidMoneyVal();

  while (true){

    if (withdrawAmt == 0){
      break;
    }
    else if (withdrawAmt < 0 ){
      std::cout << "Please enter a value greater than 0." << std::endl;
    } else if (withdrawAmt > ldBalance){
      std::cout << "Please enter a value that is not greater than the current balance." << std::endl;
    } else {
      break;
    }

    std::cout << "Enter 0 to exit." << std::endl;
    withdrawAmt = inputHandler::getUserValidMoneyVal();

  }

  if (withdrawAmt != 0){
    
    difference = ldBalance - withdrawAmt;

    std::stringstream ss;
    ss << std::fixed << std::setprecision(6) << difference;

    std::string strNewBal = ss.str();
    opertationStatus = 0;

    // update balance in the XML
    opertationStatus = fileOperations::setBalance(this->strFileName, this->strAccountName, this->strAccountNumber, strNewBal);

    if(opertationStatus == 1){
      std::cout << "Error accessing account." << std::endl;
      return 1;
    } else {
      opertationStatus = printBalance();
      if(opertationStatus == 0){
        std::cout << "Amount withdrawn and balance updated.";
      }
      return opertationStatus;
    }
  }
 
  return 0;


}

/*
void AccountManagement::printFileName(){
  std::cout << this->strFileName << std::endl;
}
*/