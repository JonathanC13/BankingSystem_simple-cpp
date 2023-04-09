/*

#ifndef inputHandler_H
#define inputHandler_H

Those are called #include guards.

Once the header is included, it checks if a unique value (in this case HEADERFILE_H) is defined. Then if it's not defined, it defines it and continues to the rest of the page.

When the code is included again, the first ifndef fails, resulting in a blank file.

That prevents double declaration of any identifiers such as types, enums and static variables.

*/

#ifndef inputHandler_H
#define inputHandler_H

// inputHandler.h file acts as an interface for function as API to hide implementation of inputHandler.cpp


namespace inputHandler {

    /*
    Purpose:
        Get an integer value from the user.
    Parameters:
        N/A
    Return:
        valid integer from user.
    */
    int getUserValidSelectionInt();

    /*
    Purpose:
        Get an integer value from the user.
    Parameters:
        N/A
    Return:
        valid integer from user.
    */
    int getUserValidInt();

    void removeStringSpaces(std::string &strLine);

    /*
    Purpose:
        Prompt user to enter characters
    Parameters: 
        string strUserIn; Reference variable for user input
    Return:
        N/A
    */
    void getUserInput(std::string& strUserIn);

    long double getUserValidMoneyVal();

    void padLeadingZeros(size_t numberOfDigits, std::string &strPadTarget);

};

#endif