#include<iostream>
#include<string.h>
#include <cstring>
#include<time.h>
#include <fstream>      // std::fstream

// XML Parser
#include "..\Libraries\pugixml-1.13\src\pugixml.hpp"

namespace fileOperations {

    std::string strRootElem = "BankAccounts";

    
    int addRootElementXML(const char* fileName, std::string rootElemName);
    void getLocalTime(char* timeRet, int sizeTimeRet);
    int loadBankFileXML(const char* fileName, pugi::xml_document& loadedDocRef);


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
    int createBankFileXML(const char* fileName){

        FILE * pFile;
        pFile = fopen (fileName,"r");


        if (pFile!=NULL) // if file exists
        {
            //std::cout << "File exists." << std::endl;
            fclose (pFile);

            //std::cout << "Checking file root element for correctness." << std::endl;
            pugi::xml_document doc;

            loadBankFileXML(fileName, doc);
       
            pugi::xml_node rootElem = doc.first_child();


            //if(strcmp(rootElem.name(), &strRootElem[0]) != 0){ // if the root element is not the expected one, just create new file.
            if(strRootElem.compare(rootElem.name()) != 0){ //
                pFile = fopen (fileName,"w");
                fclose (pFile);

                // Add root element
                return addRootElementXML(fileName, strRootElem);
            }

            return 0;
        } else{
            // file does not exist, create file
            //std::cout << "File does not exists, creating now." << std::endl;
            pFile = fopen (fileName,"w");
            fclose (pFile);

            // Add root element
            return addRootElementXML(fileName, strRootElem);
            
        }

        return 1; // could not create file.
    }

    /*
    Purpose:
        Load the XML information from [file] to [memory] into the XML document object for use.
    Params:
        const char* fileName: XML file that contains the accounts
        pugi::xml_document& loadedDocRef: Reference variable for the loaded XML contents
    Return:
        int:
        0: Successfully able to load the XML file's contents into a xml_document
        1: Error when loading the XML file contents
    */
    int loadBankFileXML(const char* fileName, pugi::xml_document& loadedDocRef){

        pugi::xml_parse_result result = loadedDocRef.load_file(fileName); // store result of the XML load


        //std::cout << "Load result: " << result.description() << std::endl;

        std::string res = std::string(result.description());

        //if (strcmp(result.description(), "No error") !=0 && strcmp(result.description(), "No document element found")!=0){
        if (res.compare("No error") !=0 && res.compare("No document element found")!=0){

            return 1;
        } else {

            //pugi::xml_node project = loadedDocRef.child("bankAccounts");

            //std::cout << "bankAccounts: " << project.text().get() << std::endl;

            //loadedDocRef.print(std::cout);

            //std::cout << std::endl;

            return 0;
        }
        
        
    }

    /*
    Purpose:
        If the file does not exist and then create the XML file. Either the file already existed or newly created, attempt to load the XML file.
    Params:
        const char* fileName: file name of the XML file that contains the bank accounts
    Return:
        0: Successfully loaded the xml document for use
        1: Failure to either create the file or load the XML file
    */
    int createFileIfNotExists(const char* fileName){
        pugi::xml_document doc;

        std::fstream fs;

        fs.open(fileName);
        if (fs.is_open() == false) {
            // file could not be opened

            // attempt to create the file
            if(createBankFileXML(fileName) != 0){
                std::cout << ">> Could not create the file." << std::endl;
                return 1;   
            }
        } else {
            fs.close();

        }

        // attempt to load the file
        return loadBankFileXML(fileName, doc);

    }

    // commit the XML wholly to the XML file
    /*
    Purpose:
        Save the xml_document contents to the XML file.
    Params:
        const char* fileName: XML file that contains the accounts
        pugi::xml_document& doc: xml_document contents to save
    Return:
        int:
        0: Successfully saved xml_document to the file
        1: Error when attempting to save to the file
    */
    int saveToBankFileXML(const char* fileName, pugi::xml_document& doc){

        if(doc.save_file(fileName)){ // true if success;
            //std::cout << "XML committed." << std::endl;
            return 0;
        } else {
            std::cout << ">> saveToBankFileXML: XML not committed." << std::endl;
            return 1;
        }
        
    }

    // Upon new XML file creation, add the root element where all child, "Accounts,"  will be added within
    /*
    Purpose:
        Add a root node to the xml_document.
    Params:
        const char* fileName: XML file that contains the accounts
        std::string rootElemName: node to be added as the root element node
    Return:
        int:
        0: Successfully added node and saved to the XML file
        1: Error
    */
    int addRootElementXML(const char* fileName, std::string rootElemName){

        //std::cout << "Adding root element." << std::endl;
        pugi::xml_document doc;

        char* cRootElemName = &rootElemName[0];

        if(loadBankFileXML(fileName, doc) == 1){
            std::cout << "Could not load or parse XML file." << std::endl;
            return 1; // some error

        } else {
            // continue to add node
            //std::cout << "Adding root element." << std::endl;
            pugi::xml_node node = doc.append_child(cRootElemName);

            //doc.print(std::cout);

            // commit to file
            return saveToBankFileXML(fileName, doc);
        }
    }


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
    int addAccount(const char* fileName, std::string& accountName, std::string& accountNumber){
        //std::cout << "Adding account." << std::endl;
        pugi::xml_document doc;

        char* cAccountName = &accountName[0];
        char* cAccountNumber = &accountNumber[0];

        //std::cout << "a name: " << accountName << std::endl;

        /*
        for (int i = 0; i < 9; i++){
            std::cout << accountNumber[i] << "|"; 
        }
        std::cout<< std::endl;
        */

        if(loadBankFileXML(fileName, doc) == 1){
            std::cout << ">> addAccount: Could not load or parse XML file." << std::endl;
            return 1; // some error

        } else {


            char strDate[50];
            getLocalTime(&strDate[0], sizeof(strDate)-1);

            // continue to add node
            pugi::xml_node rootElem = doc.first_child();

            //std::cout << "Root: " << rootElem.name() << std::endl;

            
            pugi::xml_node nodeAccount = rootElem.append_child("Account");
            nodeAccount.append_attribute("AccountName") = cAccountName;
            nodeAccount.append_attribute("AccountNumber") = cAccountNumber;
            nodeAccount.append_attribute("Locked") = 0; // default 0 for unlocked
            nodeAccount.append_attribute("lastUpdatedDate") = strDate;

            pugi::xml_node balanceElement = nodeAccount.append_child("Balance");
            balanceElement.append_attribute("currentBalance") = 0; // Inital balance = 0
            //balanceElement.append_child(pugi::node_pcdata).set_value("");

            pugi::xml_node transactionHistory = nodeAccount.append_child("TransactionHistory");
            transactionHistory.append_attribute("count") = 1;

            pugi::xml_node History = transactionHistory.append_child("History");
            History.append_attribute("Order") = 1;
            History.append_attribute("Date") = strDate;
            History.append_attribute("Desc") = "Account created.";

            pugi::xml_node transaction = History.append_child("Transaction");
            transaction.append_attribute("Desc") = "none"; // usually withdraw or deposit
            transaction.append_attribute("BalanceBefore") = 0; // The balance before this transaction has taken place
            transaction.append_attribute("BalanceAfter") = 0; // The balance after this transaction has taken place

            pugi::xml_node transferFrom = transaction.append_child("TransferFrom");
            pugi::xml_node fromAccountName = transferFrom.append_child("AccountName");
            fromAccountName.append_child(pugi::node_pcdata).set_value("");
            pugi::xml_node fromAccountNumber = transferFrom.append_child("AccountNumber");
            fromAccountNumber.append_child(pugi::node_pcdata).set_value("");
            pugi::xml_node amountFrom = transferFrom.append_child("AmountFrom");
            amountFrom.append_child(pugi::node_pcdata).set_value("");


            pugi::xml_node transferTo = transaction.append_child("TransferTo");
            pugi::xml_node toAccountName = transferTo.append_child("AccountName");
            toAccountName.append_child(pugi::node_pcdata).set_value("");
            pugi::xml_node toAccountNumber = transferTo.append_child("AccountNumber");
            toAccountNumber.append_child(pugi::node_pcdata).set_value("");
            pugi::xml_node amountTo = transferTo.append_child("AmountTo");
            amountTo.append_child(pugi::node_pcdata).set_value("");



            //doc.print(std::cout);

            // commit to file
            return saveToBankFileXML(fileName, doc);
        }

    }

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
    int printAllAccounts(const char* fileName){

        std::string strLockedDesc;

        pugi::xml_document doc;

        int printFlag = 0;

        if(loadBankFileXML(fileName, doc) == 1){
            std::cout << ">> printAllAccounts: Could not load or parse XML file." << std::endl;
            return 1; // some error

        } else {

             pugi::xml_node bankAccounts = doc.child("BankAccounts");

            // check all account numbers to see if already used.
            for(pugi::xml_node xnAccount : bankAccounts.children("Account")){ // for each Element "Account"
                //Attributes that belongs to the Element "Account"
                strLockedDesc = xnAccount.attribute("Locked").value();
                
                std::cout << "- " << "Account Name: " << xnAccount.attribute("AccountName").value() << ", Account Number: " << xnAccount.attribute("AccountNumber").value() << ". Locked status: " << strLockedDesc << std::endl;

                
                    
                
            }

            return 0;
        }
    }

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
    int printAllUnlockedAccounts(const char* fileName){

        pugi::xml_document doc;

        int printFlag = 0;

        if(loadBankFileXML(fileName, doc) == 1){
            std::cout << "printAllUnlockedAccounts: Could not load or parse XML file." << std::endl;
            return 1; // some error

        } else {

             pugi::xml_node bankAccounts = doc.child("BankAccounts");

            // check all account numbers to see if already used.
            for(pugi::xml_node xnAccount : bankAccounts.children("Account")){ // for each Element "Account"
                //Attributes that belongs to the Element "Account"

                if(xnAccount.attribute("Locked").as_int() == 0){ // accounts that are not locked, 0 = not locked.

                    std::cout << "- " << "Account Name: " << xnAccount.attribute("AccountName").value() << ", Account Number: " << xnAccount.attribute("AccountNumber").value() << std::endl;

                }
                    
                
            }

            return 0;
        }
    }

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
    int printAllTransferableAccounts(const char* fileName){

        pugi::xml_document doc;

        int printFlag = 0;

        if(loadBankFileXML(fileName, doc) == 1){
            std::cout << "printAllUnlockedAccounts: Could not load or parse XML file." << std::endl;
            return 1; // some error

        } else {

             pugi::xml_node bankAccounts = doc.child("BankAccounts");

            // check all account numbers to see if already used.
            for(pugi::xml_node xnAccount : bankAccounts.children("Account")){ // for each Element "Account"
                //Attributes that belongs to the Element "Account"

                if(xnAccount.attribute("Locked").as_int() == 0 || xnAccount.attribute("Locked").as_int() == 1){ // accounts that are not locked, 0 = not locked, 1 = a user is currently using, but can still transfer into

                    std::cout << "- " << "Account Name: " << xnAccount.attribute("AccountName").value() << ", Account Number: " << xnAccount.attribute("AccountNumber").value() << std::endl;

                }
                    
                
            }

            return 0;
        }
    }

    /*
    Purpose:
        Print all accounts that are not "Locked" = 3 and is not the specified account name and account number passed in the parameter.
    Params:
        const char* fileName: XML file for the accounts
        std::string &strAccountName: Excluded account name 
        std::string &strAccountNumber: Excluded account number
    Return:
        int:
        0: Success
        1: Error
    */
    int printAllAccountsExcludeCurr(const char* fileName, std::string &strAccountName, std::string &strAccountNumber){

        pugi::xml_document doc;

        int printFlag = 0;

        if(loadBankFileXML(fileName, doc) == 1){
            std::cout << ">> printAllAccountsExcludeCurr: Could not load or parse XML file." << std::endl;
            return 1; // some error

        } else {

             pugi::xml_node bankAccounts = doc.child("BankAccounts");

            // check all account numbers to see if already used.
            for(pugi::xml_node xnAccount : bankAccounts.children("Account")){ // for each Element "Account"
                //Attributes that belongs to the Element "Account"

                if(xnAccount.attribute("Locked").as_int() != 3 && (strAccountName.compare(std::string(xnAccount.attribute("AccountName").value())) != 0 && strAccountNumber.compare(std::string(xnAccount.attribute("AccountNumber").value())) != 0)){ // accounts that are not locked, 0 = not locked.

                    std::cout << "- " << "Account Name: " << xnAccount.attribute("AccountName").value() << ", Account Number: " << xnAccount.attribute("AccountNumber").value() << std::endl;

                }
                    
                
            }

            return 0;
        }
    }

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
    int ifExistsAccountNumber(const char* cAccountNumber, const char* fileName){

        std::string strAccountNumber_attr = "AccountNumber";

        pugi::xml_document doc;

        if(loadBankFileXML(fileName, doc) == 1){
            std::cout << ">> ifExistsAccountNumber: Could not load or parse XML file." << std::endl;
            return 2; // some error

        } else {

            //doc.print(std::cout);

            //std::cout << "New Account Number to check: " << cAccountNumber << std::endl;

            pugi::xml_node bankAccounts = doc.child("BankAccounts");

            // check all account numbers to see if already used.
            for(pugi::xml_node xnAccount : bankAccounts.children("Account")){ // for each Element "Account"
                for(pugi::xml_attribute xaAccountNumber : xnAccount.attributes()){ // for each Attribute that belongs to the Element "Account"
                    //std::cout << "attr name: " << xaAccountNumber.name() << ", attr value: " << xaAccountNumber.value() << std::endl;
                    //if (strcmp(xaAccountNumber.name(), "AccountNumber") == 0 && strcmp(xaAccountNumber.value(), cAccountNumber) == 0){ // if the attribute is "AccountNumber" and the value of "AccountNumber" matches the value of "iAccountNumber", return false to indicate that the value is already in use.
                    if (strAccountNumber_attr.compare(std::string(xaAccountNumber.name())) == 0 && std::string(cAccountNumber).compare(std::string(xaAccountNumber.value())) == 0){ // if the attribute is "AccountNumber" and the value of "AccountNumber" matches the value of "iAccountNumber", return false to indicate that the value is already in use.
                        return 0;
                    }
                }
            }
        }
        return 1;
    }

    /*
    Purpose:
        Print the entire XML raw.
    Params:
        const char* fileName: File name of the XML file.
    Return:
        N/A
    */
    void printXML(const char* fileName){
        std::cout << "Printing entire XML." << std::endl;
        pugi::xml_document doc;

        loadBankFileXML(fileName, doc);

        doc.print(std::cout);
    }

    /*
    Purpose:
        Gets the local time, EST. It checks my system's timezone to determine the localtime
    Params:
        char* timeRet: char array pointer for the generated time
        int sizeTimeRet: size of the char array pointer
    Return:
        N/A
    */
    void getLocalTime(char* timeRet, int sizeTimeRet){


        //char dest[50]; // formatted time destination
        size_t size;

        struct tm* structTimeLocal;

        time_t rawtime = time(NULL); //number of seconds since 00:00 hours, Jan 1, 1970 UTC 

        // struct tm * localtime (const time_t * timer);
        structTimeLocal = localtime(&rawtime);

        //std::cout << "Year:" << 1900 + structTimeLocal->tm_year<< std::endl;

        //size_t strftime(char * __restrict__ dest, size_t maxsize, const char * __restrict__ format, const struct tm * __restrict__ timeptr);
        //size = strftime(dest, sizeof(dest)-1, "%F %R %Z",structTimeLocal);

        size = strftime(timeRet, sizeTimeRet, "%Y-%m-%d %H:%M %Z",structTimeLocal);

    }

    /*
    Purpose:
        Check if specified bank account exists and is not "Locked" = 3.
    Params:
        const char* c_fileName: XML file that contains the bank accounts.
        std::string strAccountName: Specified account name.
        std::string strAccountNumber: Specified account number.
        std::string strFlagSel: 0 = For only "Locked" = 0
                                1 = For only "Locked" != 3
    Return:
        int
        0: Specified account valid to be chosen
        1: Specified account not valid to be chosen
        2: for error
    */
    int getFlagValidAccount(const char* c_fileName, std::string strAccountName, std::string strAccountNumber, std::string strFlagSel){

        if (strFlagSel.compare("0") != 0 && strFlagSel.compare("1") != 0){
            return 1;
        }

        char* c_accountName = &strAccountName[0];
        char* c_accountNumber = &strAccountNumber[0];

        pugi::xml_document doc;

        if(loadBankFileXML(c_fileName, doc) == 1){
            std::cout << ">> ifExistsAccountNumber: Could not load or parse XML file." << std::endl;
            return 2; // some error

        } else {

            //std::cout << "Account name in: " << strAccountName << std::endl;
            //std::cout << "Account num in: " << strAccountNumber << std::endl;

            pugi::xml_node bankAccounts = doc.child("BankAccounts");

            for(pugi::xml_node xnAccount : bankAccounts.children("Account")){ // for each Element "Account"
                //Attributes that belongs to the Element "Account"

                //if(xnAccount.attribute("Locked").as_int() == 0 && strcmp(xnAccount.attribute("AccountName").value(), c_accountName) == 0 && strcmp(xnAccount.attribute("AccountNumber").value(), c_accountNumber) == 0){ // accounts that are not locked, 0 = not locked, with matching account name and number.
                //std::cout << "> " << "Account Name: " << xnAccount.attribute("AccountName").value() << ", Account Number: " << xnAccount.attribute("AccountNumber").value() << ". Locked: " << xnAccount.attribute("Locked").as_int() << std::endl;

                if (strFlagSel.compare("0") == 0){
                    if(xnAccount.attribute("Locked").as_int() == 0 && strAccountName.compare(std::string(xnAccount.attribute("AccountName").value())) == 0 && strAccountNumber.compare(std::string(xnAccount.attribute("AccountNumber").value())) == 0){ 

                        //std::cout << "> " << "Account Name: " << xnAccount.attribute("AccountName").value() << ", Account Number: " << xnAccount.attribute("AccountNumber").value() << std::endl;
                        

                        return 0;

                    }
                } else if (strFlagSel.compare("1") == 0){
                    if(xnAccount.attribute("Locked").as_int() != 3 && strAccountName.compare(std::string(xnAccount.attribute("AccountName").value())) == 0 && strAccountNumber.compare(std::string(xnAccount.attribute("AccountNumber").value())) == 0){ 

                        //std::cout << "> " << "Account Name: " << xnAccount.attribute("AccountName").value() << ", Account Number: " << xnAccount.attribute("AccountNumber").value() << std::endl;
                        

                        return 0;

                    }
                }
            }
        }


        return 1;
    }


    /*
    Purpose:
        Get the strNode value that is 1 level lower from the "Account" node
    Params:
        const char* c_fileName: XML file that contains the accounts
        std::string strAccountName: Target account name
        std::string strAccountNumber: Target account number
        std::string strNode: Target node 1 level lower from the "Account" node.
        std::string &strNodeDataRet: Reference variable for the node value of the strNode
    Return:
        int:
        0: Success
        1: Error loading XML the file
    */
    int getTargetNodeDataLevel1(const char* c_fileName, std::string strAccountName, std::string strAccountNumber, std::string strNode, std::string &strNodeDataRet){

        pugi::xml_document doc;

        const char* c_Node = &strNode[0];

        int printFlag = 0;

        if(loadBankFileXML(c_fileName, doc) == 1){
            std::cout << "printAllAccounts: Could not load or parse XML file." << std::endl;
            return 1; // some error

        } else {

             pugi::xml_node bankAccounts = doc.child("BankAccounts");

            // check all account numbers to see if already used.
            for(pugi::xml_node xnAccount : bankAccounts.children("Account")){ // for each Element "Account"
                //Attributes that belongs to the Element "Account"

                if(strAccountName.compare(std::string(xnAccount.attribute("AccountName").value())) == 0 && strAccountNumber.compare(std::string(xnAccount.attribute("AccountNumber").value())) == 0){ // accounts that are not locked, 0 = not locked, with matching account name and number.

                    // Check only 1 level deeper
                    strNodeDataRet = std::string(xnAccount.child(c_Node).child_value());

                    return 0;

                }
                    
                
            }

            return 0;
        }

    }

    /*
    Purpose:
        For the target node "Account," with the specified account name and number, search 1 level lower for the target strNode and get the target attribute's value.
    Params:
        const char* c_fileName: XML file for the accounts
        std::string strAccountName: Specified account name
        std::string strAccountNumber: Specified account number
        std::string strNode: Name of the node to search for 1 level lower that the "Accounts" node.
        std::string strAttrName: Name of the attribute within the strNode
        std::string &strNodeAttrRet: Reference variable to store the value of strAttrName
    Return:
        int:
        0: Success
        1: Error
    */
    int getTargetNodeAttrLevel1(const char* c_fileName, std::string strAccountName, std::string strAccountNumber, std::string strNode, std::string strAttrName, std::string &strNodeAttrRet){

        pugi::xml_document doc;

        const char* c_Node = &strNode[0];
        const char* c_strAttrName = &strAttrName[0];

        int printFlag = 0;

        if(loadBankFileXML(c_fileName, doc) == 1){
            std::cout << "getTargetNodeAttrLevel1: Could not load or parse XML file." << std::endl;
            return 1; // some error

        } else {

             pugi::xml_node bankAccounts = doc.child("BankAccounts");

            // check all account numbers to see if already used.
            for(pugi::xml_node xnAccount : bankAccounts.children("Account")){ // for each Element "Account"
                //Attributes that belongs to the Element "Account"

                //std::cout << xnAccount.attribute("AccountName").value() << " " << xnAccount.attribute("AccountNumber").value() << std::endl;

                if(strAccountName.compare(std::string(xnAccount.attribute("AccountName").value())) == 0 && strAccountNumber.compare(std::string(xnAccount.attribute("AccountNumber").value())) == 0){ // accounts that are not locked, 0 = not locked, with matching account name and number.

                    
                    // Check only 1 level deeper
                    strNodeAttrRet =  xnAccount.child(c_Node).attribute(c_strAttrName).value();                    

                    return 0;

                }
                    
                
            }

            return 0;
        }

    }

    /*
    Purpose:
        Update the value of strNode's strAttrName, where the strNode is 1 level lower from the "Account" node.
    Params:
        const char* c_fileName: XML file that contains the accounts
        std::string strAccountName: Target account name
        std::string strAccountNumber: Target account number
        std::string strNode: Target node 1 level lower from the "Account" node.
        std::string strAttrName: Target attribute within the strNode
        std::string &strNodeAttrSet: Value to update the value of attribute strAttrName
    Return:
        int:
        0: Successfully saved to the XML file
        1: Error
    */
    int updateTargetNodeAttrLevel1(const char* c_fileName, std::string strAccountName, std::string strAccountNumber, std::string strNode, std::string strAttrName, std::string &strNodeAttrSet){

        pugi::xml_document doc;

        const char* c_Node = &strNode[0];
        const char* c_strAttrName = &strAttrName[0];
        const char* c_strNodeDataSet = &strNodeAttrSet[0];

        //std::cout << "strNodeAttrSet: " << strNodeAttrSet << std::endl;

        int printFlag = 0;

        if(loadBankFileXML(c_fileName, doc) == 1){
            std::cout << ">> updateTargetNodeAttrLevel1: Could not load or parse XML file." << std::endl;
            return 1; // some error

        } else {

            pugi::xml_node bankAccounts = doc.child("BankAccounts");

            // check all account numbers to see if already used.
            for(pugi::xml_node xnAccount : bankAccounts.children("Account")){ // for each Element "Account"
                //Attributes that belongs to the Element "Account"

                //std::cout << "2. " << xnAccount.attribute("AccountName").value() << " " << xnAccount.attribute("AccountNumber").value() << std::endl;
                if(strAccountName.compare(std::string(xnAccount.attribute("AccountName").value())) == 0 && strAccountNumber.compare(std::string(xnAccount.attribute("AccountNumber").value())) == 0){ // accounts that are not locked, 0 = not locked, with matching account name and number.
                    
                    xnAccount.child(c_Node).attribute(c_strAttrName).set_value(c_strNodeDataSet);     

                    //doc.print(std::cout);
                    //std::cout << "3. " << xnAccount.attribute("AccountName").value() << " " << xnAccount.attribute("AccountNumber").value() << std::endl;

                    return saveToBankFileXML(c_fileName, doc);             

                }
                    
                
            }

            return 0;
        }

    }

    /*
    iCommand:
        1: withdraw
        2: deposit
        3: transfer
    */
    int addTransactionHistory(const char* &fileName, std::string &strAccountName, std::string &strAccountNumber, std::string strBalanceSet, std::string strOrgBal, std::string strChangeAmt, int iCommand, std::string strAccountNameDest, std::string strAccountNumberDest, std::string strSetBalDest, std::string strOrgBalDest){

        pugi::xml_document doc;

        std::string strCommandDesc = "";
        std::string strCommandDestDesc = "";
        const char* c_Node = "TransactionHistory";

        int transferCompleteFlag = 0;

        char strDate[50];
        getLocalTime(&strDate[0], sizeof(strDate)-1);

        if(iCommand == 1){
            strCommandDesc = "Withdraw";
        } else if (iCommand == 2){
            strCommandDesc = "Deposit";
        } else if (iCommand == 3){
            strCommandDesc = "Transfer to; account name: " + strAccountNameDest + ", account number: " + strAccountNumberDest;
            strCommandDestDesc = "Transfer from; account name: " + strAccountName + ", account number: " + strAccountNumber;
        } else if (iCommand == 99){
            strCommandDesc = "Account closed.";
        } else {
            std::cout << "addTransactionHistory: No such command: " << iCommand << std::endl;
            return 1;
        }

        //std::cout << "history strCommandDesc: " << strCommandDesc << std::endl;

        int printFlag = 0;

        if(loadBankFileXML(fileName, doc) == 1){
            std::cout << ">> addTransactionHistory: Could not load or parse XML file." << std::endl;
            return 1; // some error

        } else {

            pugi::xml_node bankAccounts = doc.child("BankAccounts");

            
            for(pugi::xml_node xnAccount : bankAccounts.children("Account")){ // for each Element "Account"
                //Attributes that belongs to the Element "Account"

                // find dest account, if transfer
                if (iCommand == 3){
                    if(strAccountNameDest.compare(std::string(xnAccount.attribute("AccountName").value())) == 0 && strAccountNumberDest.compare(std::string(xnAccount.attribute("AccountNumber").value())) == 0){

                        //std::cout << "Found dest account: " << strAccountNameDest << std::endl;
                        // get TransactionHistory Node
                        pugi::xml_node transactionHistory = xnAccount.child(c_Node);

                        // update attribute count
                        char * p1;
                        transactionHistory.attribute("count").set_value(strtol(transactionHistory.attribute("count").value(), &p1,10) + 1);

                        // Add new tranaction history section
                        pugi::xml_node History = transactionHistory.append_child("History");

                        char * p;

                        History.append_attribute("Order") = transactionHistory.attribute("count").value();
                        History.append_attribute("Date") = strDate;

                        History.append_attribute("Desc") = &strCommandDestDesc[0];
                        
                        pugi::xml_node transaction = History.append_child("Transaction");
                        transaction.append_attribute("Desc") = &strCommandDestDesc[0]; 
                        transaction.append_attribute("BalanceBefore") = &strOrgBalDest[0]; 
                        transaction.append_attribute("BalanceAfter") = &strSetBalDest[0]; 

                        pugi::xml_node transferFrom = transaction.append_child("TransferFrom");
                        pugi::xml_node fromAccountName = transferFrom.append_child("AccountName");
                        fromAccountName.text() = &strAccountName[0];

                        pugi::xml_node fromAccountNumber = transferFrom.append_child("AccountNumber");
                        fromAccountNumber.text() = &strAccountNumber[0];

                        pugi::xml_node amountFrom = transferFrom.append_child("AmountFrom");
                        amountFrom.text() = &strChangeAmt[0];


                        pugi::xml_node transferTo = transaction.append_child("TransferTo");
                        
                        pugi::xml_node toAccountName = transferTo.append_child("AccountName");
                        toAccountName.text() = &strAccountNameDest[0];

                        pugi::xml_node toAccountNumber = transferTo.append_child("AccountNumber");
                        toAccountNumber.text() = &strAccountNumberDest[0];

                        pugi::xml_node amountTo = transferTo.append_child("AmountTo");
                        amountTo.text() = &strChangeAmt[0];

                        transferCompleteFlag++;
                    }
                }

                // find source account
                if(strAccountName.compare(std::string(xnAccount.attribute("AccountName").value())) == 0 && strAccountNumber.compare(std::string(xnAccount.attribute("AccountNumber").value())) == 0){ // accounts that are not locked, 0 = not locked, with matching account name and number.

                    // get TransactionHistory Node
                    pugi::xml_node transactionHistory = xnAccount.child(c_Node);

                    // update attribute count
                    char * p1;
                    transactionHistory.attribute("count").set_value(strtol(transactionHistory.attribute("count").value(), &p1,10) + 1);

                    // Add new tranaction history section
                    pugi::xml_node History = transactionHistory.append_child("History");

                    char * p;

                    History.append_attribute("Order") = transactionHistory.attribute("count").value();
                    History.append_attribute("Date") = strDate;

                    History.append_attribute("Desc") = &strCommandDesc[0];
                    
                    pugi::xml_node transaction = History.append_child("Transaction");
                    transaction.append_attribute("Desc") = &strCommandDesc[0]; 
                    transaction.append_attribute("BalanceBefore") = &strOrgBal[0]; 
                    transaction.append_attribute("BalanceAfter") = &strBalanceSet[0]; 

                    if (iCommand == 1){
                        std::cout << "withdraw" << std::endl;
                        // withdraw
                        pugi::xml_node transferFrom = transaction.append_child("TransferFrom");
                        pugi::xml_node fromAccountName = transferFrom.append_child("AccountName");
                        fromAccountName.text() = &strAccountName[0];

                        pugi::xml_node fromAccountNumber = transferFrom.append_child("AccountNumber");
                        fromAccountNumber.text() = &strAccountNumber[0];

                        pugi::xml_node amountFrom = transferFrom.append_child("AmountFrom");
                        amountFrom.text() = &strChangeAmt[0];


                        pugi::xml_node transferTo = transaction.append_child("TransferTo");
                        
                        pugi::xml_node toAccountName = transferTo.append_child("AccountName");
                        toAccountName.text() = "";

                        pugi::xml_node toAccountNumber = transferTo.append_child("AccountNumber");
                        toAccountNumber.text() = "";

                        pugi::xml_node amountTo = transferTo.append_child("AmountTo");
                        toAccountNumber.text() = "";
                        std::cout << "withdraw end" << std::endl;
                        break;
                    }
                    else if(iCommand == 2){
                        // deposit
                        pugi::xml_node transferFrom = transaction.append_child("TransferFrom");
                        pugi::xml_node fromAccountName = transferFrom.append_child("AccountName");
                        fromAccountName.text() = "";

                        pugi::xml_node fromAccountNumber = transferFrom.append_child("AccountNumber");
                        fromAccountNumber.text() = "";

                        pugi::xml_node amountFrom = transferFrom.append_child("AmountFrom");
                        amountFrom.text() = "";


                        pugi::xml_node transferTo = transaction.append_child("TransferTo");
                        
                        pugi::xml_node toAccountName = transferTo.append_child("AccountName");
                        toAccountName.text() = &strAccountName[0];

                        pugi::xml_node toAccountNumber = transferTo.append_child("AccountNumber");
                        toAccountNumber.text() = &strAccountNumber[0];

                        pugi::xml_node amountTo = transferTo.append_child("AmountTo");
                        amountTo.text() = &strChangeAmt[0];

                        break;
                    } else if (iCommand == 3){
                        // transfer. Source account

                        pugi::xml_node transferFrom = transaction.append_child("TransferFrom");
                        pugi::xml_node fromAccountName = transferFrom.append_child("AccountName");
                        fromAccountName.text() = &strAccountName[0];

                        pugi::xml_node fromAccountNumber = transferFrom.append_child("AccountNumber");
                        fromAccountNumber.text() = &strAccountNumber[0];

                        pugi::xml_node amountFrom = transferFrom.append_child("AmountFrom");
                        amountFrom.text() = &strChangeAmt[0];


                        pugi::xml_node transferTo = transaction.append_child("TransferTo");
                        
                        pugi::xml_node toAccountName = transferTo.append_child("AccountName");
                        toAccountName.text() = &strAccountNameDest[0];

                        pugi::xml_node toAccountNumber = transferTo.append_child("AccountNumber");
                        toAccountNumber.text() = &strAccountNumberDest[0];

                        pugi::xml_node amountTo = transferTo.append_child("AmountTo");
                        toAccountNumber.text() = &strChangeAmt[0];

                        transferCompleteFlag++;

                    } else if (iCommand == 99){
                        pugi::xml_node transferFrom = transaction.append_child("TransferFrom");

                        pugi::xml_node fromAccountName = transferFrom.append_child("AccountName");                       
                        pugi::xml_node fromAccountNumber = transferFrom.append_child("AccountNumber");
                        pugi::xml_node amountFrom = transferFrom.append_child("AmountFrom");


                        pugi::xml_node transferTo = transaction.append_child("TransferTo");
                        
                        pugi::xml_node toAccountName = transferTo.append_child("AccountName");
                        pugi::xml_node toAccountNumber = transferTo.append_child("AccountNumber");
                        pugi::xml_node amountTo = transferTo.append_child("AmountTo");

                        break;
                    } 

                    

                }
                
                if(transferCompleteFlag == 2){
                    break;
                }
                
            }

            return saveToBankFileXML(fileName, doc);
        }



        

        return 0;
    }

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
    int getBalance(const char* &fileName, std::string &strAccountName, std::string &strAccountNumber, std::string &strBalanceRet){
        //std::cout << strAccountName << " " << strAccountNumber << std::endl;
        return getTargetNodeAttrLevel1(fileName, strAccountName, strAccountNumber, "Balance", "currentBalance", strBalanceRet);
    }

    

    /*
    Purpose:
        Update the value of strAttrName which is an attribute of the "Account" node
    Params:
        const char* c_fileName: XML file that contains the accounts
        std::string strAccountName: Target account name
        std::string strAccountNumber: Target account number
        std::string strAttrName: Attribute name within the node "Account"
        std::string strAttrVal: Value to update the value of strAttrName
    Return:
        int:
        0: Successfully saved to XML file
        1: Error
    */
    int updateAccountNode(const char* c_fileName, std::string strAccountName, std::string strAccountNumber, std::string strAttrName, std::string strAttrVal){
        pugi::xml_document doc;

        const char* c_strAttrName = &strAttrName[0];
        const char* c_strAttrVal = &strAttrVal[0];

        //std::cout << "strNodeAttrSet: " << strNodeAttrSet << std::endl;

        int printFlag = 0;

        if(loadBankFileXML(c_fileName, doc) == 1){
            std::cout << ">> updateAccountNode: Could not load or parse XML file." << std::endl;
            return 1; // some error

        } else {

            pugi::xml_node bankAccounts = doc.child("BankAccounts");

            // check all account numbers to see if already used.
            for(pugi::xml_node xnAccount : bankAccounts.children("Account")){ // for each Element "Account"
                //Attributes that belongs to the Element "Account"

                //std::cout << "2. " << xnAccount.attribute("AccountName").value() << " " << xnAccount.attribute("AccountNumber").value() << std::endl;
                if(strAccountName.compare(std::string(xnAccount.attribute("AccountName").value())) == 0 && strAccountNumber.compare(std::string(xnAccount.attribute("AccountNumber").value())) == 0){ 
                    
                    xnAccount.attribute(c_strAttrName).set_value(c_strAttrVal);

                    return saveToBankFileXML(c_fileName, doc);             

                }
                    
                
            }

            return 0;
        }
    }

    /*
    iCommand:
        1: withdraw
        2: deposit
        3: transfer
    */
    int setBalance(const char* &fileName, std::string &strAccountName, std::string &strAccountNumber, std::string &strBalanceSet, std::string &strOrgBal, std::string &strChangeAmt, int iCommand){

        if (iCommand != 1 && iCommand != 2 && iCommand != 3){
            return 1;
        }

        int operationStatus = 0;
        char cDate[50];
        getLocalTime(&cDate[0], sizeof(cDate)-1);
        std::string strDate = std::string(cDate);

        //std::cout << "1. " << strAccountName << " " << strAccountNumber << std::endl;
        operationStatus = updateTargetNodeAttrLevel1(fileName, strAccountName, strAccountNumber, "Balance", "currentBalance", strBalanceSet);

        //std::cout << "operationStatus1: " << operationStatus << std::endl;

        if (operationStatus == 0){
            // update time stamp    
            operationStatus = updateAccountNode(fileName, strAccountName, strAccountNumber, "lastUpdatedDate", strDate);
        }

        if (operationStatus == 0 && iCommand != 3 ){
            
            //std::cout << "history" << std::endl;
            // add transaction history
            operationStatus = addTransactionHistory(fileName, strAccountName, strAccountNumber, strBalanceSet, strOrgBal, strChangeAmt, iCommand, "", "","0","0");
            
        }


        return operationStatus;
    }

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
    int setAccountLockStatus(const char* c_fileName, std::string strAccountName, std::string strAccountNumber, std::string strLockVal){
        int operationStatus = 0;
        //updateAccountNode(const char* c_fileName, std::string strAccountName, std::string strAccountNumber, std::string strAttrName, std::string strAttrVal)
        operationStatus = updateAccountNode(c_fileName, strAccountName, strAccountNumber, "Locked", strLockVal);

        if(operationStatus != 0){
            return operationStatus;
        }

        // update history
        if (strLockVal == "3"){ // 3 is locked
            return addTransactionHistory(c_fileName, strAccountName, strAccountName, "0", "0", "0", 99, "", "","0","0");
        } else {
            return operationStatus;
        }
    }

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
    int closeAccount(const char* c_fileName, std::string strAccountName, std::string strAccountNumber){

        char cDate[50];
        getLocalTime(&cDate[0], sizeof(cDate)-1);
        std::string strDate = std::string(cDate);

        std::string strBalanceRet;
        if(getBalance(c_fileName, strAccountName, strAccountNumber, strBalanceRet) == 1){
            return 1;
        }

        pugi::xml_document doc;

        if(loadBankFileXML(c_fileName, doc) == 1){
            std::cout << ">> closeAccount: Could not load or parse XML file." << std::endl;
            return 1; // some error

        } else {

            pugi::xml_node bankAccounts = doc.child("BankAccounts");

            for(pugi::xml_node xnAccount : bankAccounts.children("Account")){ // for each Element "Account"
                //Attributes that belongs to the Element "Account"

                if(strAccountName.compare(std::string(xnAccount.attribute("AccountName").value())) == 0 && strAccountNumber.compare(std::string(xnAccount.attribute("AccountNumber").value())) == 0){ 
                    
                    xnAccount.attribute("Locked").set_value("3");

                    // add transaction history
                    pugi::xml_node transactionHistory = xnAccount.child("TransactionHistory");

                    // update attribute count
                    char * p1;
                    transactionHistory.attribute("count").set_value(strtol(transactionHistory.attribute("count").value(), &p1,10) + 1);

                    // Add new tranaction history section
                    pugi::xml_node History = transactionHistory.append_child("History");

                    char * p;

                    History.append_attribute("Order") = transactionHistory.attribute("count").value();
                    History.append_attribute("Date") = cDate;

                    History.append_attribute("Desc") = "Close Account";
                    
                    pugi::xml_node transaction = History.append_child("Transaction");
                    transaction.append_attribute("Desc") = "Close Account"; 
                    transaction.append_attribute("BalanceBefore") = &strBalanceRet[0]; 
                    transaction.append_attribute("BalanceAfter") = &strBalanceRet[0]; 

                    pugi::xml_node transferFrom = transaction.append_child("TransferFrom");

                    pugi::xml_node fromAccountName = transferFrom.append_child("AccountName");                       
                    pugi::xml_node fromAccountNumber = transferFrom.append_child("AccountNumber");
                    pugi::xml_node amountFrom = transferFrom.append_child("AmountFrom");


                    pugi::xml_node transferTo = transaction.append_child("TransferTo");
                    
                    pugi::xml_node toAccountName = transferTo.append_child("AccountName");
                    pugi::xml_node toAccountNumber = transferTo.append_child("AccountNumber");
                    pugi::xml_node amountTo = transferTo.append_child("AmountTo");

                    return saveToBankFileXML(c_fileName, doc);             

                }
                    
                
            }

            return 1;
        }
    }

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
    int printAccountHistory(const char* c_fileName, std::string strAccountName, std::string strAccountNumber){

        pugi::xml_document doc;

        const char* c_strAccountName = &strAccountName[0];
        const char* c_strAccountNumber = &strAccountNumber[0];

        //std::cout << "strAccountName: " << strAccountName << ". strAccountNumber: " << strAccountNumber << "\n" << std::endl;

        int printFlag = 0;

        if(loadBankFileXML(c_fileName, doc) == 1){
            std::cout << ">> printAccountHistory: Could not load or parse XML file." << std::endl;
            return 1; // some error

        } else {

            pugi::xml_node bankAccounts = doc.child("BankAccounts");

            // check all account numbers to see if already used.
            for(pugi::xml_node xnAccount : bankAccounts.children("Account")){ // for each Element "Account"
                //Attributes that belongs to the Element "Account"

                //std::cout << "0. " << xnAccount.attribute("AccountName").value() << " " << xnAccount.attribute("AccountNumber").value() << std::endl;
                if(strAccountName.compare(std::string(xnAccount.attribute("AccountName").value())) == 0 && strAccountNumber.compare(std::string(xnAccount.attribute("AccountNumber").value())) == 0){ 

                    //std::cout << "in" << std::endl;
                    
                    pugi::xml_node xnAccountTransactionHist = xnAccount.child("TransactionHistory");

                    for(pugi::xml_node xnAccountHistory : xnAccountTransactionHist.children("History")){
                        std::cout << "------ History ------" << std::endl;
                        std::cout << "Order: " << xnAccountHistory.attribute("Order").value() << "\n" << "Date: " << xnAccountHistory.attribute("Date").value() << "\n" << "Desc: " << xnAccountHistory.attribute("Desc").value() << "\n" << std::endl;
                        

                        pugi::xml_node xnTransaction = xnAccountHistory.child("Transaction");
                        pugi::xml_node xnTransFrom = xnTransaction.child("TransferFrom");
                        pugi::xml_node xnTransTo = xnTransaction.child("TransferTo");

                        std::cout << "\t" << "------ Transaction details ------" << std::endl;
                        std::cout << "\t" << "Transaction: " << xnTransaction.attribute("Desc").value() << std::endl;
                        std::cout << "\t" << "BalanceBefore: " << xnTransaction.attribute("BalanceBefore").value() << std::endl;
                        std::cout << "\t" << "BalanceAfter: " << xnTransaction.attribute("BalanceAfter").value() << std::endl;

                        std::cout << "\t\t" << "--- TransferFrom ---" << std::endl;
                        std::cout << "\t\t\t" << "AccountName: " << xnTransFrom.child("AccountName").text().get() << std::endl;
                        std::cout << "\t\t\t" << "AccountNumber: " << xnTransFrom.child("AccountNumber").text().get() << std::endl;
                        std::cout << "\t\t\t" << "AmountFrom: " << xnTransFrom.child("AmountFrom").text().get() << std::endl;
                        std::cout << "\t\t" << "---\\ TransferFrom ---" << std::endl;

                        std::cout << "\t\t" << "--- TransferTo ---" << std::endl;
                        std::cout << "\t\t\t" << "AccountName: " << xnTransTo.child("AccountName").text().get() << std::endl;
                        std::cout << "\t\t\t" << "AccountNumber: " << xnTransTo.child("AccountNumber").text().get() << std::endl;
                        std::cout << "\t\t\t" << "AmountTo: " << xnTransTo.child("AmountTo").text().get() << std::endl;
                        std::cout << "\t\t" << "---\\ TransferFrom ---" << std::endl;
                        std::cout << "\t" << "------\\ Transaction details ------\n" << std::endl;

                        std::cout << "------\\ History ------\n" << std::endl;

                        std::cout << "-----------------------------------------\n" << std::endl;
                        

                    }

                           

                }
                    
                
            }

            return 0;
        }
    }

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
    int commitWithdraw(const char* &fileName, std::string &strAccountName, std::string &strAccountNumber, std::string &strBalanceSet, std::string &strChangeAmt, std::string strBalanceOg){
        char cDate[50];
        getLocalTime(&cDate[0], sizeof(cDate)-1);
        //std::string strDate = std::string(cDate);



        const char* cBal = &strBalanceSet[0];

        pugi::xml_document doc;

        if(loadBankFileXML(fileName, doc) == 1){
            std::cout << ">> commitWithdraw: Could not load or parse XML file." << std::endl;
            return 1; // some error

        } else {

            pugi::xml_node bankAccounts = doc.child("BankAccounts");

            for(pugi::xml_node xnAccount : bankAccounts.children("Account")){ // for each Element "Account"
                //Attributes that belongs to the Element "Account"

                // find the target account
                if(strAccountName.compare(std::string(xnAccount.attribute("AccountName").value())) == 0 && strAccountNumber.compare(std::string(xnAccount.attribute("AccountNumber").value())) == 0){ 

                    xnAccount.attribute("lastUpdatedDate").set_value(cDate);

                    // change balance
                    xnAccount.child("Balance").attribute("currentBalance").set_value(cBal);     

                    // add history
                    // get TransactionHistory Node
                    pugi::xml_node transactionHistory = xnAccount.child("TransactionHistory");

                    // update attribute count
                    char * p1;
                    transactionHistory.attribute("count").set_value(strtol(transactionHistory.attribute("count").value(), &p1,10) + 1);

                    // Add new tranaction history section
                    pugi::xml_node History = transactionHistory.append_child("History");

                    char * p;

                    History.append_attribute("Order") = transactionHistory.attribute("count").value();
                    History.append_attribute("Date") = cDate;

                    History.append_attribute("Desc") = "Withdraw";
                    
                    pugi::xml_node transaction = History.append_child("Transaction");
                    transaction.append_attribute("Desc") = "Withdraw"; 
                    transaction.append_attribute("BalanceBefore") = &strBalanceOg[0]; 
                    transaction.append_attribute("BalanceAfter") = &strBalanceSet[0]; 

                    // withdraw
                    pugi::xml_node transferFrom = transaction.append_child("TransferFrom");
                    pugi::xml_node fromAccountName = transferFrom.append_child("AccountName");
                    fromAccountName.text() = &strAccountName[0];

                    pugi::xml_node fromAccountNumber = transferFrom.append_child("AccountNumber");
                    fromAccountNumber.text() = &strAccountNumber[0];

                    pugi::xml_node amountFrom = transferFrom.append_child("AmountFrom");
                    amountFrom.text() = &strChangeAmt[0];


                    pugi::xml_node transferTo = transaction.append_child("TransferTo");
                    
                    pugi::xml_node toAccountName = transferTo.append_child("AccountName");
                    toAccountName.text() = "";

                    pugi::xml_node toAccountNumber = transferTo.append_child("AccountNumber");
                    toAccountNumber.text() = "";

                    pugi::xml_node amountTo = transferTo.append_child("AmountTo");
                    toAccountNumber.text() = "";
                    //std::cout << "withdraw end" << std::endl;
                    
                          
                    return saveToBankFileXML(fileName, doc); 
                }
                    
                
            }


        }
        return 1;
           
    }

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
    int commitDeposit(const char* &fileName, std::string &strAccountName, std::string &strAccountNumber, std::string &strBalanceSet, std::string &strChangeAmt, std::string strBalanceOg){
        char cDate[50];
        getLocalTime(&cDate[0], sizeof(cDate)-1);
        //std::string strDate = std::string(cDate);


        const char* cBal = &strBalanceSet[0];

        pugi::xml_document doc;

        if(loadBankFileXML(fileName, doc) == 1){
            std::cout << "commitWithdraw: Could not load or parse XML file." << std::endl;
            return 1; // some error

        } else {

            pugi::xml_node bankAccounts = doc.child("BankAccounts");

            for(pugi::xml_node xnAccount : bankAccounts.children("Account")){ // for each Element "Account"
                //Attributes that belongs to the Element "Account"

                // find the target account
                if(strAccountName.compare(std::string(xnAccount.attribute("AccountName").value())) == 0 && strAccountNumber.compare(std::string(xnAccount.attribute("AccountNumber").value())) == 0){ 

                    xnAccount.attribute("lastUpdatedDate").set_value(cDate);

                    // change balance
                    xnAccount.child("Balance").attribute("currentBalance").set_value(cBal);     

                    // add history
                    // get TransactionHistory Node
                    pugi::xml_node transactionHistory = xnAccount.child("TransactionHistory");

                    // update attribute count
                    char * p1;
                    transactionHistory.attribute("count").set_value(strtol(transactionHistory.attribute("count").value(), &p1,10) + 1);

                    // Add new tranaction history section
                    pugi::xml_node History = transactionHistory.append_child("History");

                    char * p;

                    History.append_attribute("Order") = transactionHistory.attribute("count").value();
                    History.append_attribute("Date") = cDate;

                    History.append_attribute("Desc") = "Deposit";
                    
                    pugi::xml_node transaction = History.append_child("Transaction");
                    transaction.append_attribute("Desc") = "Deposit"; 
                    transaction.append_attribute("BalanceBefore") = &strBalanceOg[0]; 
                    transaction.append_attribute("BalanceAfter") = &strBalanceSet[0]; 

                    //deposit
                    pugi::xml_node transferFrom = transaction.append_child("TransferFrom");
                    pugi::xml_node fromAccountName = transferFrom.append_child("AccountName");
                    fromAccountName.text() = "";

                    pugi::xml_node fromAccountNumber = transferFrom.append_child("AccountNumber");
                    fromAccountNumber.text() = "";

                    pugi::xml_node amountFrom = transferFrom.append_child("AmountFrom");
                    amountFrom.text() = "";


                    pugi::xml_node transferTo = transaction.append_child("TransferTo");
                    
                    pugi::xml_node toAccountName = transferTo.append_child("AccountName");
                    toAccountName.text() = &strAccountName[0];

                    pugi::xml_node toAccountNumber = transferTo.append_child("AccountNumber");
                    toAccountNumber.text() = &strAccountNumber[0];

                    pugi::xml_node amountTo = transferTo.append_child("AmountTo");
                    amountTo.text() = &strChangeAmt[0];

                    return saveToBankFileXML(fileName, doc); 
                }
            }
        }
        return 1;
    }

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
    int commitTransfer(const char* &fileName, std::string &strAccountNameSrc, std::string &strAccountNumberSrc, std::string &strCurrBalSrc, std::string &strNewBalSrc, std::string &strAccountNameDest, std::string &strAccountNumberDest, std::string &strCurrBalDest, std::string &strNewBalDest, std::string &strTransferAmt){
        char cDate[50];
        getLocalTime(&cDate[0], sizeof(cDate)-1);
        //std::string strDate = std::string(cDate);

        std::string strCommandSrcDesc = "";
        std::string strCommandDestDesc = "";

        strCommandSrcDesc = "Transfer to; account name: " + strAccountNameDest + ", account number: " + strAccountNumberDest;
        strCommandDestDesc = "Transfer from; account name: " + strAccountNameSrc + ", account number: " + strAccountNumberSrc;

        int iChangeCnt = 0;

        pugi::xml_document doc;

        if(loadBankFileXML(fileName, doc) == 1){
            std::cout << "commitWithdraw: Could not load or parse XML file." << std::endl;
            return 1; // some error

        } else {

            pugi::xml_node bankAccounts = doc.child("BankAccounts");

            for(pugi::xml_node xnAccount : bankAccounts.children("Account")){ // for each Element "Account"
                //Attributes that belongs to the Element "Account"

                // find the target source account
                if(strAccountNameSrc.compare(std::string(xnAccount.attribute("AccountName").value())) == 0 && strAccountNumberSrc.compare(std::string(xnAccount.attribute("AccountNumber").value())) == 0){ 

                    xnAccount.attribute("lastUpdatedDate").set_value(cDate);

                    // change balance
                    xnAccount.child("Balance").attribute("currentBalance").set_value(&strNewBalSrc[0]);

                    // add history
                    // get TransactionHistory Node
                    pugi::xml_node transactionHistory = xnAccount.child("TransactionHistory");

                    // update attribute count
                    char * p1;
                    transactionHistory.attribute("count").set_value(strtol(transactionHistory.attribute("count").value(), &p1,10) + 1);

                    // Add new tranaction history section
                    pugi::xml_node History = transactionHistory.append_child("History");

                    char * p;

                    History.append_attribute("Order") = transactionHistory.attribute("count").value();
                    History.append_attribute("Date") = cDate;

                    History.append_attribute("Desc") = &strAccountNameSrc[0];
                    
                    pugi::xml_node transaction = History.append_child("Transaction");
                    transaction.append_attribute("Desc") = &strCommandSrcDesc[0]; 
                    transaction.append_attribute("BalanceBefore") = &strCurrBalSrc[0]; 
                    transaction.append_attribute("BalanceAfter") = &strNewBalSrc[0]; 

                    pugi::xml_node transferFrom = transaction.append_child("TransferFrom");
                    pugi::xml_node fromAccountName = transferFrom.append_child("AccountName");
                    fromAccountName.text() = &strAccountNameSrc[0];

                    pugi::xml_node fromAccountNumber = transferFrom.append_child("AccountNumber");
                    fromAccountNumber.text() = &strAccountNumberSrc[0];

                    pugi::xml_node amountFrom = transferFrom.append_child("AmountFrom");
                    amountFrom.text() = &strTransferAmt[0];


                    pugi::xml_node transferTo = transaction.append_child("TransferTo");
                    
                    pugi::xml_node toAccountName = transferTo.append_child("AccountName");
                    toAccountName.text() = &strAccountNameDest[0];

                    pugi::xml_node toAccountNumber = transferTo.append_child("AccountNumber");
                    toAccountNumber.text() = &strAccountNumberDest[0];

                    pugi::xml_node amountTo = transferTo.append_child("AmountTo");
                    amountTo.text() = &strTransferAmt[0];

                    iChangeCnt ++;
                }

                // find the target destination account
                if(strAccountNameDest.compare(std::string(xnAccount.attribute("AccountName").value())) == 0 && strAccountNumberDest.compare(std::string(xnAccount.attribute("AccountNumber").value())) == 0){ 

                    xnAccount.attribute("lastUpdatedDate").set_value(cDate);

                    // change balance
                    xnAccount.child("Balance").attribute("currentBalance").set_value(&strNewBalDest[0]);

                    // add history
                    // get TransactionHistory Node
                    pugi::xml_node transactionHistory = xnAccount.child("TransactionHistory");

                    // update attribute count
                    char * p1;
                    transactionHistory.attribute("count").set_value(strtol(transactionHistory.attribute("count").value(), &p1,10) + 1);

                    // Add new tranaction history section
                    pugi::xml_node History = transactionHistory.append_child("History");

                    char * p;

                    History.append_attribute("Order") = transactionHistory.attribute("count").value();
                    History.append_attribute("Date") = cDate;

                    History.append_attribute("Desc") = &strCommandDestDesc[0];
                    
                    pugi::xml_node transaction = History.append_child("Transaction");
                    transaction.append_attribute("Desc") = &strCommandDestDesc[0]; 
                    transaction.append_attribute("BalanceBefore") = &strCurrBalDest[0]; 
                    transaction.append_attribute("BalanceAfter") = &strNewBalDest[0]; 

                    pugi::xml_node transferFrom = transaction.append_child("TransferFrom");
                    pugi::xml_node fromAccountName = transferFrom.append_child("AccountName");
                    fromAccountName.text() = &strAccountNameSrc[0];

                    pugi::xml_node fromAccountNumber = transferFrom.append_child("AccountNumber");
                    fromAccountNumber.text() = &strAccountNumberSrc[0];

                    pugi::xml_node amountFrom = transferFrom.append_child("AmountFrom");
                    amountFrom.text() = &strTransferAmt[0];


                    pugi::xml_node transferTo = transaction.append_child("TransferTo");
                    
                    pugi::xml_node toAccountName = transferTo.append_child("AccountName");
                    toAccountName.text() = &strAccountNameDest[0];

                    pugi::xml_node toAccountNumber = transferTo.append_child("AccountNumber");
                    toAccountNumber.text() = &strAccountNumberDest[0];

                    pugi::xml_node amountTo = transferTo.append_child("AmountTo");
                    amountTo.text() = &strTransferAmt[0];


                    iChangeCnt ++;
                }
            }
        }

        if (iChangeCnt == 2){
            return saveToBankFileXML(fileName, doc); 
        } else {
            return 1;
        }
    }

    
    void testAdd(){
        pugi::xml_document doc;

        // tag::code[]
        // add node with some name
        pugi::xml_node node = doc.append_child("node");

        // add description node with text child
        pugi::xml_node descr = node.append_child("description");
        descr.append_child(pugi::node_pcdata).set_value("Simple node");

        // add param node before the description
        pugi::xml_node param = node.insert_child_before("param", descr);

        // add attributes to param node
        param.append_attribute("name") = "version";
        param.append_attribute("value") = 1.1;
        param.insert_attribute_after("type", param.attribute("name")) = "float";
        // end::code[]

        //std::cout << "hi" << std::endl;

        doc.print(std::cout);
        /*
        <node>
            <param name="version" type="float" value="1.1000000000000001" />
            <description>Simple node</description>
        </node>
        */
    }

    int traverse_rangefor(){
        pugi::xml_document doc;
        if (!doc.load_file("xgconsole.xml")) return -1;

        pugi::xml_node tools = doc.child("Profile").child("Tools");

        // tag::code[]
        for (pugi::xml_node tool: tools.children("Tool"))
        {
            std::cout << "Tool:";

            for (pugi::xml_attribute attr: tool.attributes())
            {
                std::cout << " " << attr.name() << "=" << attr.value();
            }

            for (pugi::xml_node child: tool.children())
            {
                std::cout << ", child " << child.name();
            }

            std::cout << std::endl;
        }

        return 0;

    }


    int modify_base(){
        pugi::xml_document doc;
        if (!doc.load_string("<node id='123'>text</node><!-- comment -->", pugi::parse_default | pugi::parse_comments)) return -1;

        // tag::node[]
        pugi::xml_node node = doc.child("node");

        // change node name
        std::cout << node.set_name("notnode");
        std::cout << ", new node name: " << node.name() << std::endl;

        // change comment text
        std::cout << doc.last_child().set_value("useless comment");
        std::cout << ", new comment text: " << doc.last_child().value() << std::endl;

        // we can't change value of the element or name of the comment
        std::cout << node.set_value("1") << ", " << doc.last_child().set_name("2") << std::endl;
        // end::node[]

        // tag::attr[]
        pugi::xml_attribute attr = node.attribute("id");

        // change attribute name/value
        std::cout << attr.set_name("key") << ", " << attr.set_value("345");
        std::cout << ", new attribute: " << attr.name() << "=" << attr.value() << std::endl;

        // we can use numbers or booleans
        attr.set_value(1.234);
        std::cout << "new attribute value: " << attr.value() << std::endl;

        // we can also use assignment operators for more concise code
        attr = true;
        std::cout << "final attribute value: " << attr.value() << std::endl;
        // end::attr[]

        doc.print(std::cout);

        return 0;

        /*
        
        1, new node name: notnode
        1, new comment text: useless comment
        0, 0
        1, 1, new attribute: key=345
        new attribute value: 1.234
        final attribute value: true
        <notnode key="true">text</notnode>
        <!--useless comment-->
        
        */
    }
};