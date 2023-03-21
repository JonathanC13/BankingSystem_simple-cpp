#include<iostream>
#include<time.h>
#include <cstring>
#include<string>
#include<algorithm>

#include ".\includes\inputHandler.h"
#include ".\includes\fileOperations.h"

// // complie:  g++ -o BankSystem_simple.exe BankSystem_simple.cpp includes/inputHandler.cpp includes/fileOperations.cpp includes/AccountManagement.cpp Libraries/pugixml-1.13/src/pugixml.cpp  // if using includes/eg.h, in the compile then need to explicitly reference the cpp the .h file references.

/*
TODO:
test
2. Account Management class: - here
    Constructor:  
        only asks for file name
    Functions:
        Exit
        Request balance
        Deposit
        Withdraw
        Close account -> are you sure confirmation. Not deleting the file, just add a flag to the Account element that it is locked
3. Select account -- Need #2 completed first. Initialize object when valid account info entered by user
4. Re-activate account: removing the 'X' last character from the file name, so that selectAccount function will display it.

git
Branch from main
work on branch
to merge, switch to main and Merge from, then sync to git
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
    
    int continueFlag = 1;
    int selectedOption;

    int operationStatus = 0;



    //operationStatus = createAccount("BankSystem.XML");


    while(continueFlag == 1){ // 

        std::cout << "===== Banking system =====" << std::endl << std::endl;

        // display options:
        std::cout << "Options menu:" << std::endl;
        std::cout << "0: Exit" << std::endl;
        std::cout << "1: Create account" << std::endl;
        std::cout << "2: Select account" << std::endl;
        std::cout << "3: Re-activate account" << std::endl;
        std::cout << "-----------------" << std::endl;

        // get int from user
        selectedOption = inputHandler::getUserValidInt();

        // When an integer is entered is entered, perform action if a valid option or prompt user again to enter a valid option's integer.
        switch(selectedOption){
            case 0:
                continueFlag = 0;
                break;
            case 1:
                // create account
                std::cout << "Initializing account creation." << std::endl;
                operationStatus = createAccount(fileName);

                if (operationStatus == 0){
                    std::cout << "Account created successfully." << std::endl;
                } else {
                    std::cout << "Account creation error." << std::endl;
                }

                break;
            case 2:
                // select account
                operationStatus = selectAccount(fileName);
                break;
            case 3:
                // Re-activate account
                break;
            default:
                std::cout << "Error, user has entered a number for an option that does not exist." << std::endl << std::endl;
                break;
        }


        if(operationStatus == 1){
            std::cout << "Issue detected, ending session now." << std::endl;
            break;
        }

        std::cout << std::endl;
    }
    
        
    

    return 0;
}

// Generate a random int for the account number and store into reference variable [x]
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

void padLeadingZeros(size_t numberOfDigits, std::string &strPadTarget){
    strPadTarget = std::string(numberOfDigits - std::min(numberOfDigits, strPadTarget.length()), '0') + strPadTarget;
}

/*
Purpose:
    Display all accounts' names and account numbers for the user except file names with the last character 'X'. To select an account, have the user type the entire account name and number; accountName_19485
    After valid account selected, an Account object is created and all account operations are handled within that object.
Parameters:
    N/A
Return:
    int: 0 = 
*/ // TODO
int selectAccount(const char* fileName){

    std::string strInAccountName;
    int iInAccountNumber;
    std::string strAccountNumber;

    size_t padLeading = 9;

    while(true){
        strInAccountName = "";
        iInAccountNumber = -1;
        strAccountNumber = "";

        std::cout << "Current active accounts:" << std::endl;
        

        if(fileOperations::printAllAccounts(fileName) == 1){
            return 1;
        } 

        std::cout << "========================" << std::endl;
        std::cout << "Enter \"exit\" to exit Account Selection. \n" << std::endl;

        // prompt user for account name
        std::cout << "Enter desired Account Name (case sensative): ";
        inputHandler::getUserInput(strInAccountName);
        std::cout << "\n";

        //std::cout << "user entered: " << strInAccountName << std::endl;
        if(strInAccountName.compare("exit") == 0){
            break;
        }

        // prompt user for the associated account number
        std::cout << "Enter associated Account Number: ";
        iInAccountNumber = inputHandler::getUserValidInt();
        strAccountNumber = std::to_string(iInAccountNumber);
        padLeadingZeros(padLeading, strAccountNumber);

        std::cout << "\n\n";

        //std::cout << "user entered: " << strAccountNumber << std::endl;
        if(strAccountNumber.compare("exit") == 0){
            break;
        }


        // check if valid account name and number combination
        int i = fileOperations::getFlagValidAccount(fileName, strInAccountName, strAccountNumber);
        std::cout << "ret: " << i << std::endl;
        break;
    }

    return 0;

}

/*
Purpose:
    Prompt the user for the desired information for the account;
        - Account name
Parameters:
    N/A
Return:
    int: For status of account creation
        0; success, file created.
        1; failure to create the file for the account.
*/
int createAccount(const char* fileName){

    int accNumChk = 0;

    // Ask for account name
    std::string strAccountName;

    
    std::cout << "Enter desired Account Name: ";
    inputHandler::getUserInput(strAccountName);

    // randomly generate account number
    std::string ac;

    while (accNumChk == 0){
        randomAccountNumber(ac);

        // loop while account number already exists
        accNumChk = fileOperations::ifExistsAccountNumber(&ac[0], fileName);

    }

    std::cout << "Acc Number check value: " << accNumChk << std::endl;

    if (accNumChk == 2){
        return 1;
    }


    // Populate file with XML and row elements to structure the internals of the file.
    return fileOperations::addAccount(fileName, strAccountName, ac);
}



int reactivateAccount(){

    return 0;
}