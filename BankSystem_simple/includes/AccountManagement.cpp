

#include<string>
#include<iostream>

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



int AccountCommandCenter();



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
        // here withdraw
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
  int: For status of account creation
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
    i = fileOperations::printTargetNodeDataLevel1(this->strFileName, this->strAccountName, this->strAccountNumber, "Balance", balanceRet);

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
void AccountManagement::printFileName(){
  std::cout << this->strFileName << std::endl;
}
*/