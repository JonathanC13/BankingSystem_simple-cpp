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

    int addAccount(const char* fileName, std::string& accountName, std::string& accountNumber);

    int ifExistsAccountNumber(const char* iAccountNumber, const char* fileName);

    int traverse_rangefor();

    int printAllAccounts(const char* fileName);

    int printAllUnlockedAccounts(const char* fileName);

    int printAllTransferableAccounts(const char* fileName);

    int printAllAccountsExcludeCurr(const char* fileName, std::string &strAccountName, std::string &strAccountNumber);

    int getFlagValidAccount(const char* c_fileName, std::string strAccountName, std::string strAccountNumber);

    int getBalance(const char* &fileName, std::string &strAccountName, std::string &strAccountNumber, std::string &strBalanceRet);

    int setBalance(const char* &fileName, std::string &strAccountName, std::string &strAccountNumber, std::string &strBalanceSet, std::string &strOrgBal, std::string &strChangeAmt, int iCommand);

    //int getTargetNodeAttrLevel1(const char* c_fileName, std::string strAccountName, std::string strAccountNum, std::string strNode, std::string &strNodeDataRet);

    //int updateTargetNodeAttrLevel1(const char* c_fileName, std::string strAccountName, std::string strAccountNumber, std::string strNode, std::string strNodeDataSet);

    int addTransactionHistory(const char* &fileName, std::string &strAccountName, std::string &strAccountNumber, std::string &strBalanceSet, std::string &strOrgBal, std::string &strChangeAmt, int iCommand, std::string strAccountNameDest, std::string strAccountNumberDest);

    int setAccountLockStatus(const char* c_fileName, std::string strAccountName, std::string strAccountNumber, std::string strLockVal);

    int modify_base();
};

#endif