#include<iostream>
#include<time.h>
#include <cstring>
#include<string>
#include<algorithm>

#include ".\includes\inputHandler.h"
#include ".\includes\fileOperations.h"
#include ".\includes\AccountManagement.h"

/* complie:  
g++ -o BankSystem_simple.exe BankSystem_simple.cpp includes/inputHandler.cpp includes/fileOperations.cpp includes/AccountManagement.cpp Libraries/pugixml-1.13/src/pugixml.cpp
// if using includes/eg.h, in the compile then need to explicitly reference the cpp the .h file references.
*/

/* 
git
Branch from main
work on branch
to merge, switch to main and Merge from, then sync to git
*/

/*
'Account' attribute 'Locked':
    0: unlocked
    1: in use
    3: locked (archived)
*/

int createAccount(const char* fileName);
void randomAccountNumber(std::string& strAccountNumber);
int selectAccount(const char* fileName);


/*
Purpose:
    1. Present the user with basic banking options and ensure the user only can enter those options' integer value to continue
    2. Loop getting commands from the user until the user enters the command integer value for exit
*/
int main(){


    const char* fileName = "BankSystem.XML";

    /* 
        if return value is 1 then it indicates that, if the file did not exist, there was an error attempting to create the XML file.
     */
    if(fileOperations::createFileIfNotExists(fileName) == 1){
        std::cout << ">> Error with the file." << std::endl;
        return 1;
    }
    
    int continueFlag = 1;
    int selectedOption;

    int operationStatus = 0;
    int retCode;

    // test
    //continueFlag = 0;

    

    //inputHandler::getUserValid6FracVal();
    //return 0;
    //std::string strRet;
    //AccountManagement am = AccountManagement(fileName, "Bert", "000123112", retCode);
    //AccountManagement am = AccountManagement(fileName, "Jo", "000155554", retCode);
    //fileOperations::printTargetNodeDataLevel1(fileName, "Bert", "123112", "Balance", strRet);
    
    //am.printBalance();
    //fileOperations::modify_base();
    //inputHandler::getUserValidMoneyVal();

    //std::cout << strRet;

  
    // /test

    //operationStatus = createAccount("BankSystem.XML");

    std::cout << "==================== Banking system ====================\n" << std::endl;

    while(continueFlag == 1){ // 

        std::cout << "\n========== Options menu ==========\n" << std::endl;

        // display options:
        std::cout << "0: Exit" << std::endl;
        std::cout << "1: Create account" << std::endl;
        std::cout << "2: Select account" << std::endl;
        std::cout << "3: Print all accounts" << std::endl;
        std::cout << "-----------------" << std::endl;

        // get int from user
        selectedOption = inputHandler::getUserValidInt();

        std::cout << "==========\\ Options menu ==========\n" << std::endl;

        // When an integer is entered is entered, perform action if a valid option or prompt user again to enter a valid option's integer.
        switch(selectedOption){
            case 0:
                std::cout << "> Exited." << std::endl;
                operationStatus = 0;
                continueFlag = 0;
                break;
            case 1:
                // create account
                std::cout << "========== Initializing account creation ==========\n" << std::endl;
                operationStatus = createAccount(fileName);

                if (operationStatus == 0){
                    std::cout << "> Account created successfully." << std::endl;
                } else if (operationStatus == 2){
                    std::cout << "> Exited." << std::endl;
                }else {
                    std::cout << "> Account creation error." << std::endl;
                }

                std::cout << "\n==========\\ Initializing account creation ==========" << std::endl;

                break;
            case 2:
                // select account
                std::cout << "========== Account selection ==========\n" << std::endl;

                operationStatus = selectAccount(fileName);

                std::cout << "\n==========\\ Account selection ==========" << std::endl;
                break;
            case 3:
                // Print all accounts
                std::cout << "========== Print all accounts ==========\n" << std::endl;

                operationStatus = fileOperations::printAllAccounts(fileName);

                std::cout << "\n==========\\ Print all accounts ==========" << std::endl;
                break;
            default:
                std::cout << ">> Error, user has entered a number for an option that does not exist." << std::endl;
                break;
        }


        if(operationStatus == 1){
            std::cout << ">> main: Issue detected, ending session now." << std::endl;
            break;
        }

        std::cout << std::endl;
    }
    
        
    std::cout << "====================\\ Banking system ====================" << std::endl;

    return 0;
}


/*
Purpose:
    Generate a random 9 digit account number and store into reference variable
Params:
  std::string& strAccountNumber: reference variable for the 9 digit account number
Return:
  N/A
*/
void randomAccountNumber(std::string& strAccountNumber){


    std::srand(time(NULL)); // time(NULL) gets seconds since 00:00 hours, Jan 1, 1970 UTC

    int randNum = rand() % 999999999 + 1; // rand number between 1 and 999999999

    std::string sRandNum = std::to_string(randNum);

    //std::cout << "Rand 1: " << sRandNum << std::endl;

    //pad leading 0s
    //sRandNum = std::string(9 - sRandNum.size(), '0') + sRandNum;
    size_t n_zero = 9;

    strAccountNumber = std::string(n_zero - std::min(n_zero, sRandNum.length()), '0') + sRandNum;

}

/*
void padLeadingZeros(size_t numberOfDigits, std::string &strPadTarget){
    strPadTarget = std::string(numberOfDigits - std::min(numberOfDigits, strPadTarget.length()), '0') + strPadTarget;
}
*/
/*
Purpose:
    Display all accounts' names and account numbers that are not "Locked" = 3. To select an account, have the user type the account name and then account number.
    After valid account selected, an AccountManagement object is created and all account operations are handled within that object.
Parameters:
    N/A
Return:
    int
    0: successfully exited
    1: Error
*/
int selectAccount(const char* fileName){

    std::string strInAccountName;
    int iInAccountNumber;
    std::string strAccountNumber;

    int operationStatus = -1;
    int iValidAcc = 1;
    int retCode = 1;

    size_t padLeading = 9;

    while(true){
        strInAccountName = "";
        iInAccountNumber = -1;
        strAccountNumber = "";

        std::cout << "> Select from the current active accounts:" << std::endl;
        

        if(fileOperations::printAllUnlockedAccounts(fileName) == 1){
            return 1;
        } 

        std::cout << "------------------------" << std::endl;
        std::cout << "> Enter \"-1\" to exit Account Selection. \n" << std::endl;

        // prompt user for account name
        std::cout << "> Enter desired Account Name (case sensative): ";
        inputHandler::getUserInput(strInAccountName);
        std::cout << "\n";

        //std::cout << "user entered: " << strInAccountName << std::endl;
        if(strInAccountName.compare("-1") == 0){
            return 0;
        }

        // prompt user for the associated account number
        std::cout << "> Enter associated Account Number: ";
        iInAccountNumber = inputHandler::getUserValidInt();

        strAccountNumber = std::to_string(iInAccountNumber);
        //std::cout << "user entered: " << strAccountNumber << std::endl;
        if(strAccountNumber.compare("-1") == 0){
            return 0;
        }

        inputHandler::padLeadingZeros(padLeading, strAccountNumber);

        //std::cout << "\n\n";

        // check if valid account name and number combination
        // 0 for true
        // 1 for false
        // 2 for error
        iValidAcc = fileOperations::getFlagValidAccount(fileName, strInAccountName, strAccountNumber, "0");
        //std::cout << "ret: " << i << std::endl;

        if(iValidAcc == 1){
            std::cout << "> Invalid account, please try again.\n" << std::endl;
        } else if(iValidAcc == 0){
            break;
        } else {
            return 1;
        }
        // else 1, ask for account again
 
    }

    if(iValidAcc == 0){
        // When selecting account, need to set "Locked" = 1 to indicate account is currently being used.
        operationStatus = fileOperations::setAccountLockStatus(fileName, strInAccountName, strAccountNumber, "1");
        if (operationStatus != 0){
            return operationStatus;
        } else {
            
            // if setting "Locked" == 1 successful, then create AccountManagment object.
            // Once a valid account has been chosen, then create an AccountManagement object that will loop internally to prompt the user for commands to interact with the account
            AccountManagement am = AccountManagement(fileName, strInAccountName, strAccountNumber, retCode);
        }

        if (retCode != 99){
            // When exit, need to set "Locked" = 0,
            return fileOperations::setAccountLockStatus(fileName, strInAccountName, strAccountNumber, "0");
        } else {
            return 0;
        }
    }

    return retCode;

}

/*
Purpose:
    Prompt the user for the desired information for the account name. A 9 digit account number is generated for the account and then the account is added to the XML file.
Parameters:
    N/A
Return:
    int: For status of account creation
        0; success, file created.
        1; failure to create the file for the account.
        2; exit.
*/
int createAccount(const char* fileName){

    int accNumChk = 0;

    // Ask for account name
    std::string strAccountName;

    std::cout << "> Enter \"-1\" to exit account creation.\n" << std::endl;
    std::cout << "> Enter desired Account Name: ";
    inputHandler::getUserInput(strAccountName);

    if (strAccountName.compare("-1") == 0){
        return 2;
    }

    // randomly generate account number
    std::string ac;

    while (accNumChk == 0){
        randomAccountNumber(ac);

        // loop while account number already exists
        accNumChk = fileOperations::ifExistsAccountNumber(&ac[0], fileName);

    }

    std::cout << "> Generated account number: " << ac << std::endl;

    //std::cout << "Acc Number check value: " << accNumChk << std::endl;

    /*
    0: The 9 digit account number exists and is in use.
    1: The account number is not in use.
    2: When attempting to load the XML file, an error occurred.
    */
    if (accNumChk == 2){
        return 1;
    } else {
        // Populate file with XML and row elements to structure the internals of the file.
        return fileOperations::addAccount(fileName, strAccountName, ac);
    }
}


/*
Purpose:
    Print all accounts that exist within the XML file.
Params:
  const char* fileName: file name of the XML file
Return:
  0: success
  1: error
*/
int printAllAccounts(const char* fileName){
    return fileOperations::printAllAccounts(fileName);
}

/*
int reactivateAccount(){

    return 0;
}
*/
