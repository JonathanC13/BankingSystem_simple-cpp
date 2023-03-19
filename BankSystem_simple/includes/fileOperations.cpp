#include<iostream>
#include<stdio.h>
#include<string.h>
#include<time.h>

// XML Parser
#include "..\Libraries\pugixml-1.13\src\pugixml.hpp"

namespace fileOperations {

    std::string strRootElem = "BankAccounts";

    
    int addRootElementXML(const char* fileName, std::string rootElemName);
    void getLocalTime(char* timeRet, int sizeTimeRet);
    int loadBankFileXML(const char* fileName, pugi::xml_document& loadedDocRef);


    // if file exists check correctness, simply the root element is "BankAccounts", if not, recreate
    // if file does not exist, create XML file and add root element
    int createBankFileXML(const char* fileName){

        FILE * pFile;
        pFile = fopen (fileName,"r");


        if (pFile!=NULL) // if file exists
        {
            std::cout << "File exists." << std::endl;
            fclose (pFile);

            //std::cout << "Checking file root element for correctness." << std::endl;
            pugi::xml_document doc;

            loadBankFileXML(fileName, doc);
       
            pugi::xml_node rootElem = doc.first_child();


            if(strcmp(rootElem.name(), &strRootElem[0]) != 0){ // if the root element is not the expected one, just create new file.
                
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

    // load the XML information from [file] to [memory] into the XML document object for use.
    int loadBankFileXML(const char* fileName, pugi::xml_document& loadedDocRef){

        pugi::xml_parse_result result = loadedDocRef.load_file(fileName); // store result of the XML load


        //std::cout << "Load result: " << result.description() << std::endl;

        if (strcmp(result.description(), "No error") !=0 && strcmp(result.description(), "No document element found")!=0){

            return 1;
        } else {

            //pugi::xml_node project = loadedDocRef.child("bankAccounts");

            //std::cout << "bankAccounts: " << project.text().get() << std::endl;

            //loadedDocRef.print(std::cout);

            //std::cout << std::endl;

            return 0;
        }
        
        
    }

    // commit the XML wholly to the XML file
    int saveToBankFileXML(const char* fileName, pugi::xml_document& doc){

        if(doc.save_file(fileName)){ // true if success;
            //std::cout << "XML committed." << std::endl;
            return 0;
        } else {
            //std::cout << "XML not committed." << std::endl;
            return 1;
        }
        
    }

    // Upon new XML file creation, add the root element where all child, "Accounts,"  will be added within
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


    // takes the file name and account name that desires to be created. Load the XML, add the "account" as child to the root element, "BankAccounts".
    int addAccount(const char* fileName, std::string& accountName, std::string& accountNumber){
        std::cout << "Adding account." << std::endl;
        pugi::xml_document doc;

        char* cAccountName = &accountName[0];
        char* cAccountNumber = &accountNumber[0];

        std::cout << "a name: " << accountName << std::endl;

        for (int i = 0; i < 9; i++){
            std::cout << accountNumber[i] << "|"; 
        }
        std::cout<< std::endl;

        if(loadBankFileXML(fileName, doc) == 1){
            //std::cout << "Could not load or parse XML file." << std::endl;
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

            pugi::xml_node balanceElement = nodeAccount.append_child("Balance");
            balanceElement.append_attribute("lastUpdatedDate") = strDate;
            balanceElement.append_child(pugi::node_pcdata).set_value("");

            pugi::xml_node transactionHistory = nodeAccount.append_child("TransactionHistory");
            transactionHistory.append_attribute("count") = 1;

            pugi::xml_node History = transactionHistory.append_child("History");
            History.append_attribute("Order") = 1;
            History.append_attribute("Date") = strDate;
            History.append_attribute("Desc") = "Account created.";

            pugi::xml_node transaction = History.append_child("Transaction");
            transaction.append_attribute("Desc") = "none"; // usually withdraw or deposit

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

    int printAllAccounts(const char* fileName){

        pugi::xml_document doc;

        int printFlag = 0;

        if(loadBankFileXML(fileName, doc) == 1){
            std::cout << "printAllAccounts: Could not load or parse XML file." << std::endl;
            return 1; // some error

        } else {

             pugi::xml_node bankAccounts = doc.child("BankAccounts");

            // check all account numbers to see if already used.
            for(pugi::xml_node xnAccount : bankAccounts.children("Account")){ // for each Element "Account"
                //Attributes that belongs to the Element "Account"

                if(xnAccount.attribute("Locked").as_int() == 0){ // accounts that are not locked, 0 = not locked.

                    std::cout << "> " << "Account Name: " << xnAccount.attribute("AccountName").value() << ", Account Number: " << xnAccount.attribute("AccountNumber").value() << std::endl;

                }
                    
                
            }

            return 0;
        }
    }

    int ifExistsAccountNumber(const char* cAccountNumber, const char* fileName){

        pugi::xml_document doc;

        if(loadBankFileXML(fileName, doc) == 1){
            std::cout << "ifExistsAccountNumber: Could not load or parse XML file." << std::endl;
            return 2; // some error

        } else {

            //doc.print(std::cout);

            //std::cout << "New Account Number to check: " << cAccountNumber << std::endl;

            pugi::xml_node bankAccounts = doc.child("BankAccounts");

            // check all account numbers to see if already used.
            for(pugi::xml_node xnAccount : bankAccounts.children("Account")){ // for each Element "Account"
                for(pugi::xml_attribute xaAccountNumber : xnAccount.attributes()){ // for each Attribute that belongs to the Element "Account"
                    //std::cout << "attr name: " << xaAccountNumber.name() << ", attr value: " << xaAccountNumber.value() << std::endl;
                    if (strcmp(xaAccountNumber.name(), "AccountNumber") == 0 && strcmp(xaAccountNumber.value(), cAccountNumber) == 0){ // if the attribute is "AccountNumber" and the value of "AccountNumber" matches the value of "iAccountNumber", return false to indicate that the value is already in use.

                        return 0;
                    }
                }
            }
        }
        return 1;
    }

    void printXML(const char* fileName){
        std::cout << "Printing entire XML." << std::endl;
        pugi::xml_document doc;

        loadBankFileXML(fileName, doc);

        doc.print(std::cout);
    }

    // EST, my time
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
};