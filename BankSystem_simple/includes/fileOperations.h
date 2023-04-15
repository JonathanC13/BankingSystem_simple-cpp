#ifndef fileOperations_H
#define fileOperations_H

// fileOperations.h file acts as an interface for function as API to hide implementation of fileOperations.cpp



namespace fileOperations {

    /*
    Purpose:
        Check if XML file exists, if not then create with initial XML node.
    Params:
        char* fileName: name of the XML file
    Return:
        0: success
        1: failure
    */
    int createBankFileXML(const char* fileName);

    void printXML(const char* fileName);

    /*
    Purpose:
        Save the account name and account number to the XML file that contains the bank accounts. Load the XML and then add the "account" as child to the root element "BankAccounts".
    Params:
        const char* fileName: file name of the XML file that contains the bank accounts.
        std::string& accountName: The desired account name for the new account.
        std::string& accountNumber: The desired account number for the new account.
    Return:
        int
        0: XML document successfully saved with changes.
        1: Error: (1) When loading the XML file. (2) When attempting to save changes.
    */
    int addAccount(const char* fileName, std::string& accountName, std::string& accountNumber);

    /*
    Purpose:
        Check through accounts to see that the specified 9 digit account number already exists
    Params:
        const char* cAccountNumber: 9 digit account number to check existence.
        const char* fileName: the file name of the XML file that contains the accounts
    Return:
        0: The 9 digit account number exists and is in use.
        1: The account number is not in use.
        2: When attempting to load the XML file, an error occurred.
    */
    int ifExistsAccountNumber(const char* iAccountNumber, const char* fileName);

    int traverse_rangefor();

    int printAllAccounts(const char* fileName);

    int printAllUnlockedAccounts(const char* fileName);

    int printAllTransferableAccounts(const char* fileName);

    int printAllAccountsExcludeCurr(const char* fileName, std::string &strAccountName, std::string &strAccountNumber);

    /*
    Purpose:
        Check if specified bank account exists and is not "Locked" = 3.
    Params:
        const char* c_fileName: XML file that contains the bank accounts.
        std::string strAccountName: Specified account name.
        std::string strAccountNumber: Specified account number.
    Return:
        int
        0: Specified account valid to be chosen
        1: Specified account not valid to be chosen
        2: for error
    */
    int getFlagValidAccount(const char* c_fileName, std::string strAccountName, std::string strAccountNumber);

    int getBalance(const char* &fileName, std::string &strAccountName, std::string &strAccountNumber, std::string &strBalanceRet);

    int setBalance(const char* &fileName, std::string &strAccountName, std::string &strAccountNumber, std::string &strBalanceSet, std::string &strOrgBal, std::string &strChangeAmt, int iCommand);

    //int getTargetNodeAttrLevel1(const char* c_fileName, std::string strAccountName, std::string strAccountNum, std::string strNode, std::string &strNodeDataRet);

    //int updateTargetNodeAttrLevel1(const char* c_fileName, std::string strAccountName, std::string strAccountNumber, std::string strNode, std::string strNodeDataSet);

    int addTransactionHistory(const char* &fileName, std::string &strAccountName, std::string &strAccountNumber, std::string strBalanceSet, std::string strOrgBal, std::string strChangeAmt, int iCommand, std::string strAccountNameDest, std::string strAccountNumberDest);


    /*
    Purpose:
    Params:
        const char* c_fileName:
        std::string strAccountName:
        std::string strAccountNumber:
        std::string strLockVal
    Return:
        int
        0: Successfully updated "Locked" status and added history if strLockVal == 3, closing account.
        1: Error
    */
    int setAccountLockStatus(const char* c_fileName, std::string strAccountName, std::string strAccountNumber, std::string strLockVal);

    int printAccountHistory(const char* c_fileName, std::string strAccountName, std::string strAccountNumber);

    int modify_base();
};

#endif