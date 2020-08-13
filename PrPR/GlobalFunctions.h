#include <iostream>

#define TableNames "C:\\Users\\kostadin.markov\\Desktop\\Database\\TableNames\\"
#define TableRows "C:\\Users\\kostadin.markov\\Desktop\\Database\\TableRows\\"
#define TableTree "C:\\Users\\kostadin.markov\\Desktop\\Database\\TreeNodeAndTreeSerialization.txt"

void writeStringToFile(std::ostream& os, const std::string& val);

void readStringFromFile(std::istream& is, std::string& val);

bool isOdd(int number);

std::string convertNumberToString(int rowsNumber);

bool isString(std::string& currRow);

std::vector<std::string> getArgumeters(std::string& originalStr, std::string delimator);