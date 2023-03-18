// file to get user inputs

#include<iostream>
#include<string> // getline
#include <stdexcept> // exception handling

namespace inputHandler {

    void removeStringSpaces(std::string &strLine);


    bool isInteger(std::string & s, int &intOut)
    {
        removeStringSpaces(s);

        if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;

        char * p;
        intOut = strtol(s.c_str(), &p, 10); // the *p is set to the next character in the string AFTER the numerical value.

        return (*p == 0);   // if value of p is NOT the null terminating value, then the string is not wholly an integer.
    }


    // validate if string is an integer, then convert into int.
    /*
    bool checkIfValidInt(std::string &strIn, int &intOut){

        try{
            intOut = std::stoi(strIn);
        } catch (std::invalid_argument) {
            return false;
        }
        return true;
        
    }
    */

    // remove all spaces from string
    void removeStringSpaces(std::string &strLine){
        std::string newStr;

        int len = strLine.length();      
        

        char* cPtr = &strLine[0];

        while(len != 0){
            if(*cPtr != ' '){
                newStr.push_back(*cPtr);
                
            }
            cPtr++;

            len --;
        }

        strLine = newStr;
    }

    // Purely getting an integer from the user.
    int getUserValidInt(){

        std::string strIn;
        int intVal;

        std::cout << "Enter selection number from the options above: ";

        std::getline(std::cin,strIn);

        while(!isInteger(strIn, intVal)){

            std::cout << "Please enter a proper selection number from the options above: ";
            std::getline(std::cin,strIn); // get new value
        }
        std::cout << std::endl;

        return intVal;
    }

    void getUserInput(std::string& strUserIn){

        std::getline(std::cin,strUserIn);

    }

    

};


