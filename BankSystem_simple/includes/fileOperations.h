#ifndef fileOperations_H
#define fileOperations_H

// fileOperations.h file acts as an interface for function as API to hide implementation of fileOperations.cpp



namespace fileOperations {

    /*
    Purpose:
        Check if the specified file exists. 
            If the file exists: check the root element and if it is not what is expected then create a new file with Root element, "BankAccounts".
            else: create a new file with Root element.
    Params:
        const char* fileName: file name to check existence.
    Return:
        int:
        0: Success, File exists and correct
        1: Error when creating file.
    */
    int createBankFileXML(const char* fileName);

    /*
    Purpose:
        If the file does not exist and then create the XML file. Either the file already existed or newly created, attempt to load the XML file.
    Params:
        const char* fileName: file name of the XML file that contains the bank accounts
    Return:
        0: Successfully loaded the xml document for use
        1: Failure to either create the file or load the XML file
    */
    int createFileIfNotExists(const char* fileName);

    /*
    Purpose:
        Print the entire XML raw.
    Params:
        const char* fileName: File name of the XML file.
    Return:
        N/A
    */
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

    //int traverse_rangefor();

    /*
    Purpose:
        Print all the accounts with it's details: Account name, account number, Locked status.
    Params:
        const char* fileName: XML file that contains the accounts
    Return:
        int:
        0: Success
        1: Error
    */
    int printAllAccounts(const char* fileName);

    /*
    Purpose:
        Print all the accounts with the "Locked" = 0.
    Params:
        const char* fileName: XML file name with the accounts
    Return:
        int:
        0: Success
        1: Error
    */
    int printAllUnlockedAccounts(const char* fileName);

    /*
    Purpose:
        Print all accounts that have "Locked" = 0 || 1
    Params:
        const char* fileName: XML file name with the accounts
    Return:
        int:
        0: Success
        1: Error
    */
    int printAllTransferableAccounts(const char* fileName);

    /*
    Purpose:
        Print all accounts that are not "Locked" = 3 and is not the specified account name and account number passed in the parameter.
    Params:
        const char* fileName: XML file for the accounts
        std::string &strAccountName: Excluded account name with excluded account number
        std::string &strAccountNumber: Excluded account number with excluded account name
    Return:
        int:
        0: Success
        1: Error
    */
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
    int getFlagValidAccount(const char* c_fileName, std::string strAccountName, std::string strAccountNumber, std::string strFlagSel);

    /*
    Purpose:
        Execute funtion to obtain the balance of the specified account name and account number.
    Params:
        const char* &fileName: XML file for the accounts
        std::string &strAccountName: specified account name
        std::string &strAccountNumber: specified account number
        std::string &strBalanceRet: reference variable for the balance.
    Return:
        int:
        0: Success
        1: Error
    */
    int getBalance(const char* &fileName, std::string &strAccountName, std::string &strAccountNumber, std::string &strBalanceRet);

    //int setBalance(const char* &fileName, std::string &strAccountName, std::string &strAccountNumber, std::string &strBalanceSet, std::string &strOrgBal, std::string &strChangeAmt, int iCommand);

    //int getTargetNodeAttrLevel1(const char* c_fileName, std::string strAccountName, std::string strAccountNum, std::string strNode, std::string &strNodeDataRet);

    //int updateTargetNodeAttrLevel1(const char* c_fileName, std::string strAccountName, std::string strAccountNumber, std::string strNode, std::string strNodeDataSet);
    
    //int addTransactionHistory(const char* &fileName, std::string &strAccountName, std::string &strAccountNumber, std::string strBalanceSet, std::string strOrgBal, std::string strChangeAmt, int iCommand, std::string strAccountNameDest, std::string strAccountNumberDest);

    /*
    Purpose:
        For the target account, update the balance and add transaction history for the withdraw
    Params:
        const char* &fileName: XML file for the accounts
        std::string &strAccountName: Target account name
        std::string &strAccountNumber: Target account number
        std::string &strBalanceSet: Balance amount after the withdraw, used for current balance and transaction detail
        std::string &strChangeAmt: Withdraw amount, used for transaction detail
        std::string strBalanceOg: Balance value before transaction, used for transaction detail
    Return:
        int:
        0: Success in saving XML file with the changes
        1: Error  
    */
    int commitWithdraw(const char* &fileName, std::string &strAccountName, std::string &strAccountNumber, std::string &strBalanceSet, std::string &strChangeAmt, std::string strBalanceOg);

    /*
    Purpose:
        For the target account, update the balance and add transaction history for the deposit
    Params:
        const char* &fileName: XML file for the accounts
        std::string &strAccountName: Target account name
        std::string &strAccountNumber: Target account number
        std::string &strBalanceSet: Balance amount after the deposit, used for current balance and transaction detail
        std::string &strChangeAmt: Deposit amount, used for transaction detail
        std::string strBalanceOg: Balance value before transaction, used for transaction detail
    Return:
        int:
        0: Success in saving XML file with the changes
        1: Error  
    */ 
    int commitDeposit(const char* &fileName, std::string &strAccountName, std::string &strAccountNumber, std::string &strBalanceSet, std::string &strChangeAmt, std::string strBalanceOg);

    /*
    Purpose:
        Process the transfer of funds from source account to destination account, and add the transaction history.
    Params:
        const char* &fileName: XML for the accounts
        std::string &strAccountNameSrc: Source account name of the transfer
        std::string &strAccountNumberSrc: Source account number of the transfer
        std::string &strCurrBalSrc: Balance of the source account before the transfer
        std::string &strNewBalSrc: New balance of the source account after the transfer
        std::string &strAccountNameDest: Destination account name of the transfer
        std::string &strAccountNumberDest: Destination account number of the transfer
        std::string &strCurrBalDest: Balance of the destination account before the transfer
        std::string &strNewBalDest: New balance of the destination account after the transfer
        std::string &strTransferAmt: The amount being transferred
    Return:
        int:
        0: Success in saving XML file with the changes
        1: Error  
    */
    int commitTransfer(const char* &fileName, std::string &strAccountNameSrc, std::string &strAccountNumberSrc, std::string &strCurrBalSrc, std::string &strNewBalSrc, std::string &strAccountNameDest, std::string &strAccountNumberDest, std::string &strCurrBalDest, std::string &strNewBalDest, std::string &strTransferAmt);

    /*
    Purpose:
        Process account closure, set "Locked" = 3 and add transaction history for account closure.
    Params:
        const char* c_fileName: XML file for the accounts
        std::string strAccountName: Target account name
        std::string strAccountNumber: Target account number
    Return:
        int:
        0: Success in saving XML file with the changes
        1: Error  
    */
    int closeAccount(const char* c_fileName, std::string strAccountName, std::string strAccountNumber);
    
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

    /*
    Purpose:
        Print a formatted TransactionHistory for the target account.
    Params:
        const char* c_fileName: XML file for the accounts
        std::string strAccountName: Specified account name
        std::string strAccountNumber: Specified account number
    Return:
        int:
        0: Success
        1: Error
    */
    int printAccountHistory(const char* c_fileName, std::string strAccountName, std::string strAccountNumber);



    //int modify_base();
};

#endif