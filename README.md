<h1>Very simple disgustingly amateurish "banking system"</h1>

**To compile**
1. Change to projects root folder directory
2. In command line: g++ -o BankSystem_simple.exe BankSystem_simple.cpp includes/inputHandler.cpp includes/fileOperations.cpp includes/AccountManagement.cpp Libraries/pugixml-1.13/src/pugixml.cpp

I probably was never past the novice level with regards to C++, so I now return by trying my hand at a beginner program.

This "banking system" is programmed with C++ language and the system's information storage apparatus is an XML document to store accounts' information, in which I use **[pugixml](https://pugixml.org/)** library to interface with XML files. 

The only reason I chose to use XML is because I used XML for a brief time in one of my co-op placements. I am aware that using an XML document to hold account information does not enitrely conform to the security standards, multi-user access capability, a real Banking system's ACID standard, and etc. I wrote it with single user in mind and wanted to perform simple XML operations and save them to the file. Due to it being single user and the operation to update a XML file it does somewhat meet some of the requirements of ACID, it doesn't mean much because what bank system would operate with a singular user.

**[https://globalbanks.com/how-are-databases-used-in-banking/](https://globalbanks.com/how-are-databases-used-in-banking/)**
<h1>ACID</h1>
<h2>Atomicity: Transaction process takes place at once or never happens at all.</h2>
The XML file's contents are loaded to memory and then the transaction information is updated to that local data to finally be saved, essentially overwriting, the XML file. At any point in modifying the XML in memory or attempting to overwrite the XML file, if there is an error the file does not get updated.

<h2>Consistency: Transactions follow the rules laid down by the business</h2>
I set the rules for withdraw, despost, transfer, and close account, so I'm cheating.

<h2>Isolation: Transactions occur independently without interference</h2>
It's a single user application; I'm also cheating here.

<h2>Durability: Transaction changes are completed even if the system fails.</h2>
For sure it doesn't meet this requirement.

