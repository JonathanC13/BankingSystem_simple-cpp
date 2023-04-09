

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
  

  Need a better locking mechanism? 
    So, if the account is accessible for the owner through online banking, it's ok since they log in and can do whatever
    In this app, it is for like an advisor or teller. Only 1 "Bank staff" can be active on the account.
      Since selecting an account only shows unlocked "Locked" = 0 accounts to choose from and they can only choose "Locked" = 0 accounts. The operations on the account do not have to check the "Locked" status.
        1. getBalance -> getTargetNodeAttrLevel1
        2. setBalance -> updateTargetNodeAttrLevel1
                      -> addTransactionHistory
        

  Then go back to Account select in BankSystem_simple.cpp
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
        operationStatus = withdraw();
        break;
      case 3:
        operationStatus = deposit();
        break;
      case 4:
        operationStatus = transfer();
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
int AccountManagement::withdraw(){

  int operationStatus;

  long double difference;

  std::string strBalance;
  long double withdrawAmt;
  long double ldBalance;

  std::string::size_type sz;
 

  //Get the balance:
  operationStatus = fileOperations::getBalance(this->strFileName, this->strAccountName, this->strAccountNumber, strBalance);

  if(operationStatus == 1){
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

    // query balance again in case it changed
    operationStatus = fileOperations::getBalance(this->strFileName, this->strAccountName, this->strAccountNumber, strBalance);

    if(operationStatus == 1){
      std::cout << "Error accessing account." << std::endl;
      return 1;
    } else {
      std::cout << "Account balance: " << strBalance << std::endl;
    }
    std::cout << "Enter 0 to exit." << std::endl;
    withdrawAmt = inputHandler::getUserValidMoneyVal();

  }

  if (withdrawAmt != 0){
    operationStatus = 0;
    
    difference = ldBalance - withdrawAmt;

    std::stringstream ss;
    ss << std::fixed << std::setprecision(6) << difference;

    std::string strNewBal = ss.str();
    

    std::stringstream ssBal;
    ssBal << ldBalance;
    std::string strBal = ssBal.str();

    std::stringstream ssWithdraw;
    ssWithdraw << withdrawAmt;
    std::string strWithdraw = ssWithdraw.str();

    // update balance in the XML
    operationStatus = fileOperations::setBalance(this->strFileName, this->strAccountName, this->strAccountNumber, strNewBal, strNewBal, strWithdraw, 1);

    if(operationStatus == 1){
      std::cout << "Error accessing account." << std::endl;
      return 1;
    } else {
      operationStatus = printBalance();
      if(operationStatus == 0){
        std::cout << "Amount withdrawn and balance updated.";
      }
      return operationStatus;
    }
  }
 
  return 0;


}

/*
  Purpose: Prompt the user the amount to be deposited and if valid amount, add to the balance and then update the Balance's node data
  Params:
    N/A
  Return:
    int: For status for deposit command
      0; success to update the balance for deposit
      1; failed to update the balance for deposit
  */
int AccountManagement::deposit(){

  int operationStatus;

  long double sum;

  std::string strBalance;
  long double depositAmt;
  long double ldBalance;

  std::string::size_type sz;
 

  //Get the balance:
  operationStatus = fileOperations::getBalance(this->strFileName, this->strAccountName, this->strAccountNumber, strBalance);

  if(operationStatus == 1){
    std::cout << "Error accessing account." << std::endl;
    return 1;
  } else {
    std::cout << "Account balance: " << strBalance << std::endl;
  }
  
  ldBalance = std::stold(strBalance, &sz);

  std::cout << "Current balance: " << strBalance << std::endl;

  std::cout << "Enter 0 to exit." << std::endl;
  std::cout << "Please enter the amount you wish to deposit, up to 2 fractional digits (123.yy): " << std::endl; 
  
  depositAmt = inputHandler::getUserValidMoneyVal();

  while (true){

    if (depositAmt == 0){
      break;
    }
    else if (depositAmt < 0 ){
      std::cout << "Please enter a value greater than 0." << std::endl;
    } else {
      break;
    }

    std::cout << "Enter 0 to exit." << std::endl;
    depositAmt = inputHandler::getUserValidMoneyVal();

  }

  if (depositAmt != 0){
    operationStatus = 0;
    
    sum = ldBalance + depositAmt;

    std::stringstream ss;
    ss << std::fixed << std::setprecision(6) << sum; // 6 factional digits

    std::string strNewBal = ss.str();
    

    std::stringstream ssBal;
    ssBal << ldBalance;
    std::string strBal = ssBal.str();

    std::stringstream ssDepsit;
    ssDepsit << depositAmt;
    std::string strDeposit= ssDepsit.str();

    // update balance in the XML
    operationStatus = fileOperations::setBalance(this->strFileName, this->strAccountName, this->strAccountNumber, strNewBal, strBal, strDeposit, 2);

    if(operationStatus == 1){
      std::cout << "Error accessing account." << std::endl;
      return 1;
    } else {
      operationStatus = printBalance();
      if(operationStatus == 0){
        std::cout << "Amount deposited and balance updated.";
      }
      return operationStatus;
    }
  }

  return 0;
}

/*
Purpose:
Params:
  
Return:
  
*/
int AccountManagement::transfer(){

  /*
  
  1. print all accounts, excluding itself. 
  2. Prompt the user to enter the destination account name, 0 will signal exit
  3. Prompt the user to enter the destination account number, 0 will signal exit
  4. Make sure it is not the same account as the one currently selected
  5. check if account exists and NOT locked
    5.1. If yes, Ask for transfer amount and check if valid to withdraw from source. 0 will signal exit
      5.1.1. Call fileOperation function that performs the transfer, give source info and destination info. The function will update the balances and add transaction history
    5.2. No, re-ask destination account info
                    
  */
  int operationStatus;

  

  std::string strAccountNameDest;
  int iInAccountNumber;
  std::string strAccountNumberDest;

  int iDestAccExists = -1;

  size_t padLeading = 9;

  while(true){
    strAccountNameDest = "";
    iInAccountNumber = -1;
    strAccountNumberDest = "";

    std::cout << "Available accounts to transer to:" << std::endl;


    if(fileOperations::printAllUnlockedAccounts(this->strFileName) == 1){
        return 1;
    } 

    std::cout <<  "================================" << std::endl;
    std::cout << "Enter \"exit\" to exit Account Selection. \n" << std::endl;

    // prompt user for account name
    std::cout << "Enter desired destination Account Name (case sensative): ";
    inputHandler::getUserInput(strAccountNameDest);
    std::cout << "\n";

    //std::cout << "user entered: " << strAccountNameDest << std::endl;
    if(strAccountNameDest.compare("exit") == 0){
        break;
    }

    // prompt user for the associated account number
    std::cout << "Enter associated destination Account Number: ";
    iInAccountNumber = inputHandler::getUserValidInt();
    strAccountNumberDest = std::to_string(iInAccountNumber);
    inputHandler::padLeadingZeros(padLeading, strAccountNumberDest);

    std::cout << "\n\n";

    //std::cout << "user entered: " << strAccountNumber << std::endl;
    if(strAccountNumberDest.compare("exit") == 0){
        break;
    }


    // check if valid account name and number combination
      // 0 for true
    // 1 for false
    // 2 for error
    iDestAccExists = fileOperations::getFlagValidAccount(this->strFileName, strAccountNameDest, strAccountNumberDest);
    //std::cout <<"ret: " << iDestAccExists << std::endl;
    if(iDestAccExists == 2){
      return 1;
    } else if (iDestAccExists == 0){
      break;
    } else {
      std::cout << "Destination account does not exist, please try again.\n" << std::endl;
    }

  }

  

  if (iDestAccExists == 0){
    
    std::string currBalanceSrc;
    long double ldTransferAmt;
    long double ldCurrBalanceSrc;

    operationStatus = fileOperations::getBalance(this->strFileName, this->strAccountName, this->strAccountNumber, currBalanceSrc);
    if (operationStatus == 1){
      return 1;
    }


    std::string::size_type sz;
    ldCurrBalanceSrc = std::stold(currBalanceSrc, &sz);


    std::cout << "Current account name: " << this->strAccountName << ", account number: " << this->strAccountNumber << ". Balance: " << currBalanceSrc << std::endl;
    std::cout << "Destination account name: " << strAccountNameDest << ", account number: " << strAccountNumberDest << std::endl;
    std::cout << "Please enter the valid amount to transfer to destination, up to 2 fractional digits (123.yy), or 0 to exit: ";
    ldTransferAmt = inputHandler::getUserValidMoneyVal();

    while (true){

      if (ldTransferAmt == 0){
        break;
      }
      else if (ldTransferAmt < 0 ){
        std::cout << "Please enter a value greater than 0." << std::endl;
      } else if (ldTransferAmt > ldCurrBalanceSrc){
        std::cout << "Please enter a value that is not greater than the current balance of the source account." << std::endl;
      } else {
        
        break;

      }


      std::cout << "Enter 0 to exit." << std::endl;
      ldTransferAmt = inputHandler::getUserValidMoneyVal();

    }

    if (ldTransferAmt != 0){
      std::string currBalanceDest;
      long double ldCurrBalanceDest;
      std::string::size_type szDest;
      

      long double sum;
      long double diff;

      operationStatus = 0;
    
      // withdraw from source
      diff = ldCurrBalanceSrc - ldTransferAmt;

      std::stringstream ssSrc;
      ssSrc << std::fixed << std::setprecision(6) << diff; // 6 factional digits
      std::string strNewBalSrc = ssSrc.str();
      
      std::stringstream ssBalSrc;
      ssBalSrc << ldCurrBalanceSrc;
      std::string strBalSrc = ssBalSrc.str();

      // --

      // deposit info
      operationStatus = fileOperations::getBalance(this->strFileName, strAccountNameDest, strAccountNumberDest, currBalanceDest);
      if (operationStatus == 1){
        return 1;
      }

      ldCurrBalanceDest = std::stold(currBalanceDest, &szDest);

      sum = ldCurrBalanceDest + ldTransferAmt;

      std::stringstream ssDest;
      ssDest << std::fixed << std::setprecision(6) << sum; // 6 factional digits
      std::string strNewBalDest = ssDest.str();
      
      std::stringstream ssBalDest;
      ssBalDest << ldCurrBalanceDest;
      std::string strBalDest = ssBalDest.str();


      // --

      std::stringstream ssTransfer;
      ssTransfer << ldTransferAmt;
      std::string strTransfer = ssTransfer.str();

      // update Source balance in the XML
      std::cout << "with: " << strNewBalSrc << std::endl;
      operationStatus = fileOperations::setBalance(this->strFileName, this->strAccountName, this->strAccountNumber, strNewBalSrc, strBalSrc, strTransfer, 3);

      if(operationStatus == 1){
        std::cout << "Transfer error: Could not withdraw from source account." << std::endl;
        // check if withdraw occurred, if yes. Then revert.
        std::string currBalanceSrcErr;

        operationStatus = fileOperations::getBalance(this->strFileName, this->strAccountName, this->strAccountNumber, currBalanceSrcErr);
        if (operationStatus == 1){
          std::cout << "Transfer error: Could not get current balance from source account." << std::endl;
          return 1;
        }

        // compare if balance before withdraw initiated is different than now. If not the same then update to the original balance.
        if(currBalanceSrcErr.compare(currBalanceSrc) != 0){
          operationStatus = fileOperations::setBalance(this->strFileName, this->strAccountName, this->strAccountNumber, strBalSrc, strBalSrc, strTransfer, 3);
          if (operationStatus == 1){
            std::cout << "Transfer error: Could not update balance of source account, need manual adjustment. Current balance: " << currBalanceSrcErr << ", Correct balance: " << currBalanceSrc << std::endl;
            return 1;
          }
        }

        return 1;
      }

      // update Destination balance in the XML
      std::cout << "dep: " << strNewBalDest << std::endl;
      operationStatus = fileOperations::setBalance(this->strFileName, strAccountNameDest, strAccountNumberDest, strNewBalDest, strBalDest, strTransfer, 3);

      //std::cout << "operationStatus: " << operationStatus << std::endl;
      if(operationStatus == 1){
        std::cout << "Transfer error: Could not deposit to destination account." << std::endl;
        // check if deposit occurred, if yes. Then revert.
        std::string currBalanceDestErr;

        operationStatus = fileOperations::getBalance(this->strFileName, strAccountNameDest, strAccountNumberDest, currBalanceDestErr);
        if (operationStatus == 1){
          std::cout << "Transfer error: Could not get current balance from destination account." << std::endl;
          return 1;
        }

        // compare if balance before deposit initiated is different than now. If not the same then update to the original balance.
        if(currBalanceDestErr.compare(currBalanceDest) != 0){
          operationStatus = fileOperations::setBalance(this->strFileName, strAccountNameDest, strAccountNumberDest, strBalDest, strBalDest, strTransfer, 3);
          if (operationStatus == 1){
            std::cout << "Transfer error: Could not update balance of destination account, need manual adjustment. Current balance: " << currBalanceDestErr << ", Correct balance: " << currBalanceDest << std::endl;
            return 1;
          }
        }

        // need to revert withdraw from source account
        std::string currBalanceSrcErr;

        operationStatus = fileOperations::getBalance(this->strFileName, this->strAccountName, this->strAccountNumber, currBalanceSrcErr);
        if (operationStatus == 1){
          std::cout << "Transfer error: Could not get current balance from source account." << std::endl;
          return 1;
        }

        // compare if balance before withdraw initiated is different than now. If not the same then update to the original balance.
        if(currBalanceSrcErr.compare(currBalanceSrc) != 0){
          operationStatus = fileOperations::setBalance(this->strFileName, this->strAccountName, this->strAccountNumber, strBalSrc, strBalSrc, strTransfer, 3);
          if (operationStatus == 1){
            std::cout << "Transfer error: Could not update balance of source account, need manual adjustment. Current balance: " << currBalanceSrcErr << ", Correct balance: " << currBalanceSrc << std::endl;
            return 1;
          }
        }

        return 1;
      }

      // update transaction history for both accounts
      operationStatus = fileOperations::addTransactionHistory(this->strFileName, strAccountName, strAccountNumber, strBalSrc, currBalanceSrc, strTransfer, 3, strAccountNameDest, strAccountNumberDest);

      if(operationStatus == 1){
        std::cout << "Adding transaction history for transfer errored." << std::endl;
        return 1;
      }

      
      
      if(operationStatus == 0){
        std::cout << "Transfer complete.";
      }
      return operationStatus;
      
    }

  }



  

  return 0;
}



/*
void AccountManagement::printFileName(){
  std::cout << this->strFileName << std::endl;
}
*/