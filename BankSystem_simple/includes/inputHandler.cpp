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

    /*
    Purpose:
        Remove all spaces from string
    Params:
        std::string &strLine: String to operate on.
    Return:
        N/A
    */
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

    /*
    Purpose:
        Get an integer value from the user, prompt references "Selection"
    Parameters:
        N/A
    Return:
        valid integer from user.
    */
    int getUserValidSelectionInt(){

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

    /*
    Purpose:
        Get an integer value from the user.
    Parameters:
        N/A
    Return:
        valid integer from user.
    */
    int getUserValidInt(){

        std::string strIn;
        int intVal;
        
        std::getline(std::cin,strIn);

        while(!isInteger(strIn, intVal)){

            std::cout << "Please enter a number: ";
            std::getline(std::cin,strIn); // get new value
        }
        std::cout << std::endl;

        return intVal;
    }

  

    

    int getIndexOfDecimal(std::string &s){
        return s.find(".");

    }


    bool isLongDouble(std::string & s, long double &ldOut)
    {
        removeStringSpaces(s);

        if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;

        std::string::size_type sz;
        ldOut = std::stold(s, &sz); // the *p is set to the next character in the string AFTER the numerical value.

        //std::cout << "a: " << s[sz] << std::endl;
        return (s[sz] == 0);   // if value of p is NOT the null terminating value, then the string is not wholly a long double.
    }

    /*
    Purpose:
        get valid money value, 2 fractional digits. xx.yy
    Parameters: 
        N/A
    Return:
        long double
    */
    long double getUserValidMoneyVal(){

        std::string strIn;
        long double ldVal;
        
        std::getline(std::cin,strIn);

        while(!isLongDouble(strIn, ldVal) || ((strIn.length() -1) - getIndexOfDecimal(strIn) > 2)){

            //std::cout << ldVal << std::endl;
            //std::cout << strIn.length()-1 << '-' << getIndexOfDecimal(strIn)  << std::endl;
            //std::cout << isLongDouble(strIn, ldVal) <<std::endl;

            std::cout << "Please enter a number with maximum 2 fractional digits: ";
            std::getline(std::cin,strIn); // get new value
        }
        std::cout << std::endl;

        return ldVal;
    }

    /*
    Purpose:
        Get numeric value that has max inclusive 6 fractional digits. Requires 1 digit before the decimal place. E.g: 5.
    Parameters: 
        N/A
    Return:
        long double
    */
    long double getUserValid6FracVal(){

        std::string strIn;
        long double ldVal;
        
        std::getline(std::cin,strIn);

        //std::cout << "a:" << (strIn.length() -1) << std::endl;
        //std::cout << "b:" << getIndexOfDecimal(strIn) << std::endl;
        //std::cout << "c:" << (strIn.length() -1) - getIndexOfDecimal(strIn) << std::endl;

        while(!isLongDouble(strIn, ldVal) || ((strIn.length() -1) - getIndexOfDecimal(strIn) > 6)){

            //std::cout << ldVal << std::endl;
            //std::cout << strIn.length()-1 << '-' << getIndexOfDecimal(strIn)  << std::endl;
            //std::cout << isLongDouble(strIn, ldVal) <<std::endl;

            std::cout << "Please enter a number with maximum 6 fractional digits: ";
            std::getline(std::cin,strIn); // get new value

            //std::cout << "a:" << (strIn.length() -1) << std::endl;
            //std::cout << "b:" << getIndexOfDecimal(strIn) << std::endl;
            //std::cout << "c:" << (strIn.length() -1) - getIndexOfDecimal(strIn) << std::endl;
        }
        std::cout << std::endl;

        return ldVal;
    }

    /*
    Purpose:
        Prompt user to enter characters
    Parameters: 
        string strUserIn; Reference variable for user input
    Return:
        N/A
    */
    void getUserInput(std::string& strUserIn){

        std::getline(std::cin,strUserIn);

    }

    void padLeadingZeros(size_t numberOfDigits, std::string &strPadTarget){
        strPadTarget = std::string(numberOfDigits - std::min(numberOfDigits, strPadTarget.length()), '0') + strPadTarget;
    }
    

};


