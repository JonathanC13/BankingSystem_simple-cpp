#ifndef AccountManagement_H
#define AccountManagement_H

#include<string>

class AccountManagement
{

private:
  const char* strFileName;
  std::string strAccountName;
  std::string strAccountNumber;

public:

  /*
  Purpose: Constructor, save file name that will be used for the account. Calls function to prompt user for commands to interaction with the account.
  Params:
    const char* strFileName: name of the XML file with account information

    string strAccountName: target account name

    string strAccountNumber: strAccountNumber

    int &retCode: assigns a value depending on the nature of the exiting of the account management. 0 for successful exit (user selected 'exit' command), 1 for a failure in a command and had to exit account.
  Return:
    N/A
  */
  AccountManagement(const char* strFileName, std::string strAccountName, std::string strAccountNumber, int &retCode);

  /*
  Purpose: Prompts user for commands to interact with the account.
  Params:
    N/A
  Return:
    int: 
      0, for successful exit (user selected 'exit' command)
      1, for a failure in a command and had to exit account.
  */
  int AccountCommandCenter();

  /*
  Purpose:
      Check if specified bank account exists and is not "Locked" = 3.
  Params:
      N/A
  Return:
      int
      0: Specified account valid to be chosen
      1: Specified account not valid to be chosen
      2: for error
  */
  int checkIfAccountStillExists();

  /*
  Purpose: Access account information on file and return the balance.
  Params:
    N/A
  Return:
    int: For status of account creation
      0; success, could access the account and print the balance
      1; failed to access the account and print the balance
  */
  int printBalance();



  /*
  Purpose: Prompt the user the amount to be withdrawn and if valid amount, subtract from the balance and then update the Balance's node data
  Params:
    N/A
  Return:
    int: For status for withdraw command
      0; success to update the balance for withdraw
      1; failed to update the balance for withdraw
  */
  int withdraw();


  /*
  Purpose: Prompt the user the amount to be deposited and if valid amount, add to the balance and then update the Balance's node data
  Params:
    N/A
  Return:
    int: For status for deposit command
      0; success to update the balance for deposit
      1; failed to update the balance for deposit
  */
  int deposit();

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
  int transfer();

  /*
  Purpose:
      Options menu for account closure. Can only close the account, "Locked" = 3, when the balance is 0.
      The user can either withdraw or transfer to reduce the account's balance.
      Once the balance is 0, the user can enter 99 to process the account closure.
  Params:
    int &acctClosed: Reference variable to indicate if account was closed at the end of this process; 0 = account closed, 1 = not closed
  Return:
    int
    0: Successful account closure
    1: Error
  */
  int closeAccount(int &acctClosed);

  //void printFileName();

private:
  


};

#endif