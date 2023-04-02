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

  //void printFileName();



};

#endif