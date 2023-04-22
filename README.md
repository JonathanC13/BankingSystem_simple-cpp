<h1>Very simple "banking system"</h1>

I probably was never past the novice level with regards to C++, so I now return by trying my hand at a beginner program.

This "banking system" is programed with C++ language and the system's information storage apparatus is an XML document to store accounts' information, in which I use **[pugixml](https://pugixml.org/)** library to interface with XML files.

-- todo
g++ -o BankSystem_simple.exe BankSystem_simple.cpp includes/inputHandler.cpp includes/fileOperations.cpp includes/AccountManagement.cpp Libraries/pugixml-1.13/src/pugixml.cpp