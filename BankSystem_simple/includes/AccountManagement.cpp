

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
  Should have designed this with semaphores to lock the XML file to be accessed when a user is performing an operation. Oh well, just doing this for the heck of it.

Test close account again for correct check for balance = 0 before closing and history to stamp the close date 

BankSystem_simple.cpp
  1. Then go back to Account select in BankSystem_simple.cpp ** to test
  2. operation to print all accounts with lock status ** to test

AccountManagement
  1. new operation to neatly print all history. Asc by date, just order of how it was saved. ** here
    printAccountHistory
      better print format

Make command sections clearer with divisions like 
====== account ======
====/ account ====



Test:
Bank System:
std::cout << "0: Exit" << std::endl;                    ** pass
std::cout << "1: Create account" << std::endl;          ** pass
std::cout << "2: Select account" << std::endl;          ** pass
std::cout << "3: Print all accounts" << std::endl;      ** pass

Account Management:
std::cout << "0: Exit" << std::endl;                    ** pass
std::cout << "1: Balance" << std::endl;                 ** pass
std::cout << "2: Withdraw" << std::endl;                ** pass
std::cout << "3: Deposit" << std::endl;                 ** pass
std::cout << "4: Transfer" << std::endl;                ** pass
std::cout << "5: Print account history" << std::endl;   ** pass
std::cout << "99: Close Account" << std::endl;          ** pass
*/

// Close account: ask to withdraw all money or transfer. If withdraw, then set to 0 and lock account (Locked = 3). If transfer, transfer to valid account then lock.






/*
Purpose: Constructor, save file name that will be used for the account. Calls function to prompt user for commands to interaction with the account.
Params:
  string strFileName: name of the XML file with account information

  string strAccountName: target account name

  string strAccountNumber: strAccountNumber

  int &retCode: assigns a value depending on the nature of the exiting of the account management. 
          0 for successful exit (user selected 'exit' command)
          1 for a failure in a command and had to exit account.
          99 for account closed.
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
  int accountClosed = 1;

  int operationStatus;

  std::cout << "========== Account Management for account: " << this->strAccountName << "::" << this->strAccountNumber << " ==========" << std::endl << std::endl;

  while(continueFlag == 1){ // 

    std::cout << "\n========== Options menu ==========\n" << std::endl;

    // display options:
    //std::cout << "Options menu:" << std::endl;
    std::cout << "0: Exit" << std::endl;
    std::cout << "1: Balance" << std::endl;
    std::cout << "2: Withdraw" << std::endl;
    std::cout << "3: Deposit" << std::endl;
    std::cout << "4: Transfer" << std::endl;
    std::cout << "5: Print account history" << std::endl;
    std::cout << "99: Close Account" << std::endl;
    std::cout << "-----------------" << std::endl;

    // get any valid int from user
    selectedOption = inputHandler::getUserValidInt();

    std::cout << "==========\\ Options menu ==========\n" << std::endl;

    // When an integer is entered is entered, perform action if a valid option or prompt user again to enter a valid option's integer.
    switch(selectedOption){
      case 0:
        
        continueFlag = 0;
        break;
      case 1:
        std::cout<< "========== Retrieving balance ==========" <<std::endl;
        operationStatus = printBalance();
        std::cout<< "==========\\ Retrieving balance ==========" <<std::endl;
        break;
      case 2:
        std::cout<< "========== Withdraw ==========" <<std::endl;
        operationStatus = withdraw();
        std::cout<< "==========\\ Withdraw ==========" <<std::endl;
        break;
      case 3:
        std::cout<< "========== Deposit ==========" <<std::endl;
        operationStatus = deposit();
        std::cout<< "==========\\ Deposit ==========" <<std::endl;
        break;
      case 4:
        std::cout<< "========== Transfer ==========" <<std::endl;
        operationStatus = transfer();
        std::cout<< "==========\\ Transfer ==========" <<std::endl;
        break;
      case 5: 
        std::cout<< "========== Account history ==========" <<std::endl;
        //std::cout << "printAccountHistory" << std::endl;
        operationStatus = fileOperations::printAccountHistory(this->strFileName, this->strAccountName, this->strAccountNumber);
        std::cout<< "==========\\ Account history ==========" <<std::endl;
        break;
      case 99:
        std::cout<< "========== Closing account ==========" <<std::endl;
        operationStatus = closeAccount(accountClosed);
        std::cout<< "==========\\ Closing account ==========" <<std::endl;
        if(accountClosed == 0){
          continueFlag = 0;
        }

        break;
      default:
        std::cout << ">> Error, user has entered a number for an option that does not exist." << std::endl << std::endl;
        break;
    }

    if(operationStatus == 1){
      std::cout << ">> Issue detected, ending session now." << std::endl;
      break;
    }


  }

  std::cout << "==========\\ Account Management for account: " << this->strAccountName << "::" << this->strAccountNumber << " ==========\n" << std::endl;
  std::cout << "Exited Account." << std::endl;

  if (accountClosed == 0){
    return 99;
  }
  return 0;
}

// 0 for true
// 1 for false
// 2 for error
int AccountManagement::checkIfAccountStillExists(){
  return fileOperations::getFlagValidAccount(this->strFileName, this->strAccountName, this->strAccountNumber, "1");
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
    std::cout << ">> Error accessing account." << std::endl;
    return 1;
  } else if (i == 1){
    std::cout << ">> Account could not be found at this time." << std::endl;
    return 1;
  } else {
    // get and print the balance
    i = fileOperations::getBalance(this->strFileName, this->strAccountName, this->strAccountNumber, balanceRet);

    if(i == 1){
      std::cout << ">> Error accessing account." << std::endl;
      return 1;
    } else {
      std::cout << "\n> Account balance: " << balanceRet << "\n" << std::endl;
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
    std::cout << ">> Error accessing account." << std::endl;
    return 1;
  } else {
    std::cout << "> Current account balance: " << strBalance << "\n" << std::endl;
  }
  
  ldBalance = std::stold(strBalance, &sz);

  std::cout << "> Enter 0 to exit." << std::endl;
  std::cout << "> Please enter the amount you wish to withdraw, up to 2 fractional digits (123.123456): "; 
  
  withdrawAmt = inputHandler::getUserValid6FracVal();

  while (true){

    if (withdrawAmt == 0){
      break;
    }
    else if (withdrawAmt < 0 ){
      std::cout << "> Please enter a value greater than 0." << std::endl;
    } else if (withdrawAmt > ldBalance){
      std::cout << "> Please enter a value that is not greater than the current balance." << std::endl;
    } else {
      break;
    }

    // query balance again in case it changed
    operationStatus = fileOperations::getBalance(this->strFileName, this->strAccountName, this->strAccountNumber, strBalance);

    if(operationStatus == 1){
      std::cout << ">> Error accessing account." << std::endl;
      return 1;
    } else {
      std::cout << "> Current account balance: " << strBalance << "\n" << std::endl;
    }
    std::cout << "> Enter 0 to exit." << std::endl;
    std::cout << "> Please enter the amount you wish to withdraw, up to 2 fractional digits (123.123456): "; 
    withdrawAmt = inputHandler::getUserValid6FracVal();

  }

  if (withdrawAmt != 0){
    operationStatus = 0;
    
    difference = ldBalance - withdrawAmt;

    std::stringstream ss;
    ss << std::fixed << std::setprecision(6) << difference;

    std::string strNewBal = ss.str();
    

    std::stringstream ssBal;
    ssBal << std::fixed << std::setprecision(6) << ldBalance;
    std::string strBal = ssBal.str();

    std::stringstream ssWithdraw;
    ssWithdraw << std::fixed << std::setprecision(6) << withdrawAmt;
    std::string strWithdraw = ssWithdraw.str();

    // update balance in the XML
    //operationStatus = fileOperations::setBalance(this->strFileName, this->strAccountName, this->strAccountNumber, strNewBal, strNewBal, strWithdraw, 1);
    // new function that will attempt to commit the change in balance and addition of the transaction history in one save to file, so that in the case of an error in one of the operations there won't be partial updates in the file.
    operationStatus = fileOperations::commitWithdraw(this->strFileName, this->strAccountName, this->strAccountNumber, strNewBal, strWithdraw, strBal);

    if(operationStatus == 1){
      std::cout << ">> Error committing withdraw." << std::endl;
      return 1;
    } else {
      std::cout << "> Amount withdrawn: " << strWithdraw << std::endl;
      operationStatus = printBalance();
      
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
    std::cout << ">> Error accessing account." << std::endl;
    return 1;
  } else {
    std::cout << "> Current account balance: " << strBalance << std::endl;
  }
  
  ldBalance = std::stold(strBalance, &sz);


  std::cout << "> Enter 0 to exit." << std::endl;
  std::cout << "> Please enter the amount you wish to deposit, up to 6 fractional digits (123.123456): "; 
  
  depositAmt = inputHandler::getUserValid6FracVal();

  while (true){

    if (depositAmt == 0){
      break;
    }
    else if (depositAmt < 0 ){
      std::cout << "> Please enter a value greater than 0." << std::endl;
    } else {
      break;
    }

    std::cout << "> Enter 0 to exit." << std::endl;
    std::cout << "> Please enter the amount you wish to deposit, up to 6 fractional digits (123.yy): "; 
    depositAmt = inputHandler::getUserValid6FracVal();

  }

  if (depositAmt != 0){
    operationStatus = 0;
    
    sum = ldBalance + depositAmt;

    std::stringstream ss;
    ss << std::fixed << std::setprecision(6) << sum; // 6 factional digits

    std::string strNewBal = ss.str();
    

    std::stringstream ssBal;
    ssBal << std::fixed << std::setprecision(6) << ldBalance;
    std::string strBal = ssBal.str();

    std::stringstream ssDepsit;
    ssDepsit << std::fixed << std::setprecision(6) << depositAmt;
    std::string strDeposit= ssDepsit.str();

    // update balance in the XML
    //operationStatus = fileOperations::setBalance(this->strFileName, this->strAccountName, this->strAccountNumber, strNewBal, strBal, strDeposit, 2);

    // new function that will attempt to commit the change in balance and addition of the transaction history in one save to file, so that in the case of an error in one of the operations there won't be partial updates in the file.
    operationStatus = fileOperations::commitDeposit(this->strFileName, this->strAccountName, this->strAccountNumber, strNewBal, strDeposit, strBal);

    if(operationStatus == 1){
      std::cout << ">> Error accessing account." << std::endl;
      return 1;
    } else {
      std::cout << "> Amount deposited: " << strDeposit << std::endl;
      operationStatus = printBalance();
     
      return operationStatus;
    }
  }

  return 0;
}

/*
Purpose:
  Prompt the user for the destination account and then the amount to transfer. If valid destination account and amount to withdraw to transfer, then attempt to commit transfer.
Params:
  N/A
Return:
  int
  0: Successful transfer
  1: Error
*/
int AccountManagement::transfer(){

  /*
  
  1. print all accounts, excluding itself. 
  2. Prompt the user to enter the destination account name, -1 will signal exit
  3. Prompt the user to enter the destination account number, -1 will signal exit
  4. Make sure it is not the same account as the one currently selected
  5. check if account exists and NOT locked = 3
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

    std::cout << "> Available accounts to transer to:" << std::endl;


    if(fileOperations::printAllAccountsExcludeCurr(this->strFileName, this->strAccountName, this->strAccountNumber) == 1){
        return 1;
    } 

    std::cout << "================================" << std::endl;
    std::cout << "> Enter \"-1\" to exit Account Selection. \n" << std::endl;

    // prompt user for account name
    std::cout << "> Enter desired destination Account Name (case sensative): ";
    inputHandler::getUserInput(strAccountNameDest);
    std::cout << "\n";

    //std::cout << "user entered: " << strAccountNameDest << std::endl;
    if(strAccountNameDest.compare("-1") == 0){
        return 0;
    }

    // prompt user for the associated account number
    std::cout << "> Enter associated destination Account Number: ";
    iInAccountNumber = inputHandler::getUserValidInt();
    strAccountNumberDest = std::to_string(iInAccountNumber);

    //std::cout << "user entered: " << strAccountNumber << std::endl;
    if(strAccountNumberDest.compare("-1") == 0){
        return 0;
    }

    inputHandler::padLeadingZeros(padLeading, strAccountNumberDest);

    


    // check if valid account name and number combination
      // 0 for true
    // 1 for false
    // 2 for error
    iDestAccExists = fileOperations::getFlagValidAccount(this->strFileName, strAccountNameDest, strAccountNumberDest, "1");
    //std::cout <<"ret: " << iDestAccExists << std::endl;
    if(iDestAccExists == 2){
      return 1;
    } else if (iDestAccExists == 0){
      break;
    } else {
      std::cout << "> Destination account does not exist, please try again.\n" << std::endl;
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

    std::cout << "> Current account name: " << this->strAccountName << ", account number: " << this->strAccountNumber << ". Balance: " << currBalanceSrc << std::endl;
    std::cout << "> Destination account name: " << strAccountNameDest << ", account number: " << strAccountNumberDest << std::endl;
    std::cout << "> Please enter the valid amount to transfer to destination, up to 6 fractional digits (123.123456), or 0 to exit: ";
    ldTransferAmt = inputHandler::getUserValid6FracVal();

    while (true){

      if (ldTransferAmt == 0){
        break;
      }
      else if (ldTransferAmt < 0 ){
        std::cout << "> Please enter a value greater than 0." << std::endl;
      } else if (ldTransferAmt > ldCurrBalanceSrc){
        std::cout << "> Please enter a value that is not greater than the current balance of the source account." << std::endl;
      } else {
        
        break;

      }


      std::cout << "> Enter 0 to exit." << std::endl;
      std::cout << "> Please enter the valid amount to transfer to destination, up to 6 fractional digits (123.123456), or 0 to exit: ";
      ldTransferAmt = inputHandler::getUserValid6FracVal();

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
      std::string strNewBalSrc = ssSrc.str(); // new balance to set in source acc
      
      std::stringstream ssBalSrc;
      ssBalSrc << std::fixed << std::setprecision(6) << ldCurrBalanceSrc;
      std::string strBalSrc = ssBalSrc.str(); // current balance in source acc

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
      std::string strNewBalDest = ssDest.str(); // new balance of destination account
      
      std::stringstream ssBalDest;
      ssBalDest << std::fixed << std::setprecision(6) << ldCurrBalanceDest;
      std::string strBalDest = ssBalDest.str(); // current balance of destination account


      // --

      std::stringstream ssTransfer;
      ssTransfer << std::fixed << std::setprecision(6) << ldTransferAmt;
      std::string strTransfer = ssTransfer.str(); // amount that is being transferred

      // call function to commit transfer
      // (const char* &fileName, std::string &strAccountNameSrc, std::string &strAccountNumberSrc, std::string &strCurrBalSrc, std::string &strNewBalSrc, std::string &strAccountNameDest, std::string &strAccountNumberDest, std::string &strCurrBalDest, std::string &strNewBalDest, std::string &strTransferAmt)
      operationStatus = fileOperations::commitTransfer(this->strFileName, this->strAccountName, this->strAccountNumber, strBalSrc, strNewBalSrc, strAccountNameDest, strAccountNumberDest, strBalDest, strNewBalDest, strTransfer);
      
      
      if(operationStatus == 0){
        std::cout << "> Transfer complete." << std::endl;
      } else {
        std::cout << ">> Error: Transfer issue."<< std::endl;
      }
      return operationStatus;
      
    }

  } 



  

  return 1;
}



// Close account: ask to withdraw all money or transfer. If withdraw, then set to 0 and lock account (Locked = 3). If transfer, transfer to valid account then lock.
int AccountManagement::closeAccount(int &acctClosed){

  int operationStatus = -1;
  int continueFlag = 1;
  int selectedOption = -1;
  std::string strBalanceRet;
  std::string::size_type sz;
  long double ldBalance;

  acctClosed = 1; // 1 means not closed

  std::cout << "========== Closing account: " << this->strAccountName << "::" << this->strAccountNumber << " ==========\n" << std::endl;

  while(continueFlag == 1){ // 

    std::cout << "========== Closing account: options menu ==========" << std::endl;
    std::cout << "> You may only close the account once the balance is 0. Once 0, please select option [99]. Please select from the following options." << std::endl << std::endl;

    // display options:
    
    std::cout << "0: Exit" << std::endl;
    std::cout << "1: Balance" << std::endl;
    std::cout << "2: Withdraw" << std::endl;
    std::cout << "3: Transfer" << std::endl;
    std::cout << "99: Close account" << std::endl;
    std::cout << "-----------------" << std::endl;

    // get any valid int from user
    selectedOption = inputHandler::getUserValidInt();

    std::cout << "\n==========\\ Closing account: options menu ==========\n" << std::endl;

    // When an integer is entered is entered, perform action if a valid option or prompt user again to enter a valid option's integer.
    switch(selectedOption){
      case 0:
        continueFlag = 0;
        break;
      case 1:
        std::cout<< "========== Closing account: Print balance ==========" <<std::endl;
        operationStatus = printBalance();
        std::cout<< "==========\\ Closing account: Print balance ==========" <<std::endl;
        break;
      case 2:
        std::cout<< "========== Closing account: Withdraw ==========" <<std::endl;
        operationStatus = withdraw();
        std::cout<< "==========\\ Closing account: Withdraw ==========" <<std::endl;
        break;
      case 3:
        std::cout<< "========== Closing account: Transfer ==========" <<std::endl;
        operationStatus = transfer();
        std::cout<< "==========\\ Closing account: Transfer ==========" <<std::endl;
        break;
      case 99:
        operationStatus = fileOperations::getBalance(this->strFileName, this->strAccountName, this->strAccountNumber, strBalanceRet);
        if (operationStatus == 1){
          break;
        }

        ldBalance = std::stold(strBalanceRet, &sz); // convert string to long double

        if (ldBalance == 0){
          // if current balance of accout is 0 then they are able to close the account
          // call function in fileOperations to set the history and lock the account
          operationStatus = fileOperations::closeAccount(this->strFileName, this->strAccountName, this->strAccountNumber);

          if (operationStatus == 0){
            acctClosed = 0; // successfully closed
            std::cout << "> Account successfully closed.\n" << std::endl;
          } else {
            std::cout << ">> Error: Closing account issue.\n" << std::endl;
            break;
          }

          continueFlag = 0;
        } else {
          std::cout << "> The balance of the account is: " << strBalanceRet << ". Cannot close account until the balance is 0." << std::endl;
        }


        break;
      default:
        std::cout << ">> Error, user has entered a number for an option that does not exist." << std::endl << std::endl;
        break;
    }

    if(operationStatus == 1){
      std::cout << ">> Issue detected, ending session now." << std::endl;
      break;
    }


  }

  std::cout << "==========\\ Closing account: " << this->strAccountName << "::" << this->strAccountNumber << " ==========" << std::endl;

  return 0;
}


/*
void AccountManagement::printFileName(){
  std::cout << this->strFileName << std::endl;
}
*/