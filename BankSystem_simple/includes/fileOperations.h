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
};

#endif