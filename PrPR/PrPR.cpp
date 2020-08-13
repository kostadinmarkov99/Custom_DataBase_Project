#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <stdio.h>
#include <string>
#include<stdio.h>
#include <cstdio>
#include <algorithm>
#include <queue>
#include "GlobalFunctions.h"
#include "List.h"
#include "Tree.h"

std::string removeSpaces(std::string& word) {
	std::string newWord;
	for (int i = 0; i < word.length(); i++) {
		if (word[i] != ' ') {
			newWord += word[i];
		}
	}

	return newWord;
}

char asciitolower(char in) {
	if (in <= 'Z' && in >= 'A')
		return in - ('Z' - 'z');
	return in;
}

void ToLowerCase(std::string& sen, size_t& size)
{
	for (size_t i = 0; i < size; i++)
	{
		sen[i] = asciitolower(sen[i]);
	}
}

bool findCommandWordByWord(std::string commandLine, std::string commandCandidat)
{
	int pos;
	bool flag = false;

	for (int i = 1; (pos = commandLine.find(commandCandidat)) != -1; i++)
	{
		commandLine = commandLine.substr(++pos);
		//std::cout << "Found = " << commandCandidat << " " << i << " Times" << std::endl;
		flag = true;
	}

	return flag;
}

std::string findCommand(std::string sentance)
{
	std::string commandToBeReturned;
	if (findCommandWordByWord(sentance, "createtable"))
	{
		commandToBeReturned = "CreateTable";
		return commandToBeReturned;
	}
	else if (findCommandWordByWord(sentance, "droptable"))
	{
		commandToBeReturned = "DropTable";
		return commandToBeReturned;
	}
	else if (findCommandWordByWord(sentance, "listtables"))
	{
		commandToBeReturned = "ListTables";
		return commandToBeReturned;
	}
	else if (findCommandWordByWord(sentance, "select"))
	{
		commandToBeReturned = "Select";
		return commandToBeReturned;
	}
	else if (findCommandWordByWord(sentance, "tableinfo"))
	{
		commandToBeReturned = "TableInfo";
		return commandToBeReturned;
	}
	else if (findCommandWordByWord(sentance, "insert"))
	{
		commandToBeReturned = "Insert";
		return commandToBeReturned;
	}
	else if (findCommandWordByWord(sentance, "remove"))
	{
		commandToBeReturned = "Remove";
		return commandToBeReturned;
	}
	else if (findCommandWordByWord(sentance, "quit"))
	{
		commandToBeReturned = "Quit";
		return commandToBeReturned;
	}
	else
	{
		commandToBeReturned = "Unlegal";
		return commandToBeReturned;
	}
}

int findTablesName(std::string& originalSen, std::string& key)
{
	//originalSen = originalSen.substr(11, originalSen.size());
	originalSen = removeSpaces(originalSen);
	std::string breketStartPosition = key;
	int found = originalSen.find(breketStartPosition);
	if (found != std::string::npos)
		return found;
	return -1;
}

size_t countOfOcc(std::string& line, const char& ch)
{
	size_t count = 0;
	for (int i = 0; i < line.size(); i++)
	{
		if (line.at(i) == ch)
			count++;
	}
	return count;
}

bool isLineLegit(std::string& line)
{
	// number of - (
	int countOfOpenBrecket = countOfOcc(line, '(');
	int countOfClosedBrecket = countOfOcc(line, ')');

	if (countOfOpenBrecket != countOfClosedBrecket) return false;
}
bool containsString(const std::string& sentence, const std::string& word)
{
	size_t pos = 0;
	bool flag;
	sentence.find(word) ? flag = true : flag = false;

	return flag;
}


std::string returnOnlyRealString(std::string value)
{
	size_t valueSize = value.size();
	char* returnValue = new char[valueSize];
	for (size_t i = 0; i < valueSize; i++)
	{
		char valueChar = value[i];
		if (valueChar >= 'A' && valueChar <= 'Z') returnValue[i] = valueChar;
	}

	return returnValue;
}

std::vector<std::string> getArgumeters(std::vector<TableName>& destableNames)
{
	std::vector<std::string> result;
	int sizeOfTableNames = destableNames.size();

	for (size_t i = 0; i < sizeOfTableNames; i++)
	{
		TableName currTableName = destableNames[i];
		std::string currName = currTableName.name;

		result.push_back(currName);
	}

	return result;
}

int Get_Size(std::string tableName)
{
	std::string path = TableNames + tableName;
	FILE* pFile = NULL;

	fopen_s(&pFile, path.c_str(), "rb");

	fseek(pFile, 0, SEEK_END);

	int Size = ftell(pFile);

	fclose(pFile);

	return Size;
}

std::string getParams(std::string originalStr, std::string delimator)
{
	std::string result;
	size_t delimatorSize = delimator.size();
	ToLowerCase(delimator, delimatorSize);
	while (originalStr.size()) {
		int index = originalStr.find(delimator);
		if (index != std::string::npos) {
			result = originalStr.substr(0, index);
			return result;
		}
		else return originalStr;
	}

	return result;
}

std::string getResultParams(std::string originalStr, std::string delimator)
{
	std::string result;
	while (originalStr.size()) {
		int index = originalStr.find(delimator);
		if (index != std::string::npos) {
			result = originalStr.substr(0, index);
			return result;
		}
		else return "";
	}

	return result;
}

std::vector<std::string> findAndStats(std::string line)
{
	std::vector<std::string> result;
	std::string firstConditionAND = "Hello World";

	while (line.size() > 0)
	{
		firstConditionAND = getResultParams(line, "AND");
		//There are no AND conditions
		if (firstConditionAND == "") {
			firstConditionAND = line;
			line = "";
		}
		else line = line.substr(firstConditionAND.size() + 3, line.size());
		firstConditionAND = removeSpaces(firstConditionAND);
		result.push_back(firstConditionAND);
	}

	return result;
}

std::vector<std::string> getOperationResult(std::string& line)
{
	std::vector<std::string> result;
	std::string rowName;
	std::string operation;
	std::string value;

	std::string equal = getResultParams(line, "==");
	std::string notEqual = getResultParams(line, "!=");
	std::string  greatherThan = getResultParams(line, ">");
	std::string smallerThan = getResultParams(line, "<");
	std::string greatherOREqual = getResultParams(line, ">=");
	std::string smallerOrEqual = getResultParams(line, "<=");
	std::string somthingElse;

	if (equal != "")
	{
		rowName = equal;
		operation = "==";
		line = line.substr(equal.length(), line.length());
		line = removeSpaces(line);
		line = line.substr(2, line.length());
		value = line;
	}
	else if (notEqual != "")
	{
		rowName = notEqual;
		operation = "!=";
		line = line.substr(notEqual.length(), line.length());
		line = removeSpaces(line);
		line = line.substr(2, line.length());
		value = line;
	}
	else if (greatherThan != "")
	{
		rowName = greatherThan;
		operation = ">";
		line = line.substr(greatherThan.length(), line.length());
		line = removeSpaces(line);
		line = line.substr(1, line.length());
		value = line;
	}
	else if (smallerThan != "")
	{
		rowName = smallerThan;
		operation = "<";
		line = line.substr(smallerThan.length(), line.length());
		line = removeSpaces(line);
		line = line.substr(1, line.length());
		value = line;
	}
	else if (greatherOREqual != "")
	{
		rowName = greatherOREqual;
		operation = ">=";
		line = line.substr(greatherOREqual.length(), line.length());
		line = removeSpaces(line);
		line = line.substr(2, line.length());
		value = line;
	}
	else if (smallerOrEqual != "")
	{
		rowName = smallerOrEqual;
		operation = "<=";
		line = line.substr(smallerOrEqual.length(), line.length());
		line = removeSpaces(line);
		line = line.substr(2, line.length());
		value = line;
	}
	else
	{
		operation = "";
	}

	result.push_back(rowName);
	result.push_back(operation);
	result.push_back(value);

	return result;
}

std::vector<std::string> findORStats(std::string line)
{
	std::vector<std::string> result;
	std::string firstConditionOR = "Hello World";

	while (line.size() > 0)
	{
		firstConditionOR = getParams(line, "OR");
		//There are no AND conditions
		if (firstConditionOR == line) {
			firstConditionOR = line;
			line = "";
		}
		else line = line.substr(firstConditionOR.size() + 2, line.size());
		firstConditionOR = removeSpaces(firstConditionOR);
		result.push_back(firstConditionOR);
	}

	return result;
}

int returnPossition(std::vector<TableName> tableNames, std::string param)
{
	int tablePossition = 0;
	for (size_t k = 0; k < tableNames.size(); k++)
	{
		std::string currTbNm = tableNames[k].name;
		if (currTbNm != param)
		{
			tablePossition++;
		}
		else  break;
	}

	return tablePossition;
}

std::vector<std::string> sortStatmentsAND(std::vector<std::string>& statmentsAnd)
{

	std::vector<std::string> result;
	std::vector<std::string> resultEqual;
	std::vector<std::string> resultNotEqual;
	std::vector<std::string> resultGreather;
	std::vector<std::string> resultSmaller;

	int statsAndSize = statmentsAnd.size();

	for (size_t i = 0; i < statsAndSize; i++)
	{
		std::string firstStat = statmentsAnd[i];
		std::string firstStatCopy = firstStat;
		std::vector<std::string> operations = getOperationResult(firstStat);

		std::string condition = operations[1];

		if (condition == "==")
			resultEqual.push_back(firstStatCopy);
		else if (condition == "!=")
			resultNotEqual.push_back(firstStatCopy);
		else if (condition == ">")
			resultGreather.push_back(firstStatCopy);
		else if (condition == "<")
			resultSmaller.push_back(firstStatCopy);
	}

	if (resultNotEqual.size() != 0)
		result.insert(result.end(), resultNotEqual.begin(), resultNotEqual.end());
	if (resultGreather.size() != 0)
		result.insert(result.end(), resultGreather.begin(), resultGreather.end());
	if (resultSmaller.size() != 0)
		result.insert(result.end(), resultSmaller.begin(), resultSmaller.end());
	if (resultEqual.size() != 0)
		result.insert(result.end(), resultEqual.begin(), resultEqual.end());

	return result;
}

int main()
{
	Tree helperTree;

	std::string serializatedTreeString;
	std::ifstream treeFile(TableTree);

	if (treeFile.is_open())
	{
		while (treeFile.good())
		{
			std::getline(treeFile, serializatedTreeString);
			//if (serializatedTreeString == "") flag = false;
		}
		treeFile.close();
	}
	else std::cout << "You did not entry right path to the folders of the database. Unable to open serialization tree file!";

	TreeNode* deserializedTree = helperTree.deserialize(serializatedTreeString);
	Tree tree(deserializedTree);

	do
	{
		std::string sen, command, originalSen;
		std::cout << "FMISql>";
		getline(std::cin, sen);
		originalSen = sen;
		size_t senLength = sen.size();
		ToLowerCase(sen, senLength);

		command = findCommand(sen);

		if (command == "CreateTable")
		{
			//TODO: Finishing the legit function
			if (isLineLegit(originalSen))
				originalSen = originalSen.substr(11, originalSen.size());
			std::string key = "(";
			int found = findTablesName(originalSen, key);
			std::string tableName;

			if (found != -1) {
				tableName = originalSen.substr(0, found);
				if (tableName.size() > 20)
				{
					std::cout << "The maximum size of the table's name is 20\n";
					continue;
				}
				originalSen = originalSen.substr(found, originalSen.size());
			}
			else {
				std::cout << "Invalid command, Please try again!\n";
				continue;
			}

			TreeNode* currNode = tree.find(tableName);

			if (currNode != nullptr)
			{
				std::cout << "There is already an existing table with that name!\n";
				continue;
			}

			//create node with the name of the table
			tree.insertNode(tableName);
			std::string serializationString = tree.serialize(tree.getRoot());
			std::ofstream treeFile;
			treeFile.open(TableTree);
			if (treeFile.is_open())
			{
				treeFile << serializationString;
				treeFile.close();
			}
			else
			{
				std::cout << "Problem with the file, where the tree is serialized!";
				continue;
			}

			originalSen = originalSen.substr(1, originalSen.size() - 2);

			std::vector<std::string> tableArguments = getArgumeters(originalSen, ",");

			std::vector<TableName> newTableName;
			bool flag = true;
			size_t tableArgumentsSize = tableArguments.size();
			for (size_t i = 0; i < tableArgumentsSize; i++)
			{
				std::string tableArgumentElement = tableArguments[i];
				std::vector<std::string> tabelParamElement = getArgumeters(tableArgumentElement, ":");
				std::string elementName = tabelParamElement[0];
				if (elementName.length() > 20)
				{
					std::cout << "The maximum size of the parameter's names of the tables are 20 characters!" << std::endl;
					flag = false;
					break;
				}
				std::string elementType = tabelParamElement[1];
				if (elementType.length() > 6)
				{
					std::cout << "The maximum size of the paramether's types   are 6 characters!" << std::endl;
					flag = false;
					break;
				}
				TableName elementPair(elementName, elementType);
				newTableName.push_back(elementPair);
			}

			if (!flag)
			{
				TreeNode* deletedNode = tree.removeNode(tableName);
				std::string filePathTableName = TableNames + tableName;
				const char* filePathTableNameChar = &filePathTableName[0];

				std::string filePathRows = TableRows + tableName;
				const char* flPthRw = &filePathRows[0];

				const int result = remove(filePathTableNameChar);
				const int result1 = remove(flPthRw);

				std::string serializationString = tree.serialize(tree.getRoot());
				std::ofstream treeFile;
				treeFile.open(TableTree);
				if (treeFile.is_open())
				{
					treeFile << serializationString;
					treeFile.close();
				}
				else
				{
					std::cout << "Problem with the file, where the tree is serialized!";
					continue;
				}
				continue;
			}

			if (!pairToWrite(newTableName, tableName))
			{
				std::cout << "Error writing out of vector.\n";
				return 1;
			}

			std::cout << "Table " << tableName << " is created!" << std::endl;
		}

		else if (command == "DropTable")
		{
			originalSen = originalSen.substr(9, originalSen.size());
			originalSen = removeSpaces(originalSen);
			std::string tableName = originalSen;

			if (tableName == "") {
				std::cout << "Invalid command, Please try again!";
				continue;
			}

			TreeNode* node = tree.find(tableName);
			if (node == nullptr)
			{
				std::cout << "There is no such a table!";
				continue;
			}
			TreeNode* deletedNode = tree.removeNode(tableName);

			if (deletedNode != nullptr) std::cout << "Table " + tableName + " successfully removed!\n";
			else std::cout << "No such a table!";

			std::string filePathTableName = TableNames + tableName;
			const char* filePathTableNameChar = &filePathTableName[0];

			std::string filePathRows = TableRows + tableName;
			const char* flPthRw = &filePathRows[0];

			const int result = remove(filePathTableNameChar);
			const int result1 = remove(flPthRw);

			std::string serializationString = tree.serialize(tree.getRoot());
			std::ofstream treeFile;
			treeFile.open(TableTree);
			if (treeFile.is_open())
			{
				treeFile << serializationString;
				treeFile.close();
			}
			else
			{
				std::cout << "Problem with the file, where the tree is serialized!";
				continue;
			}
		}

		else if (command == "ListTables")
		{
			size_t numberOfTables = tree.getfullCount();

			std::cout << "There";
			if (numberOfTables == 0)
			{
				std::cout << "There are no tables in the database\n";
				break;
			}
			if (numberOfTables == 1) std::cout << " is 1 Table\n";
			else std::cout << " are " << numberOfTables << " Tables in the database:\n";
			//TODO padding 

			tree.printLevel();
			std::cout << "\n";
		}

		else if (command == "Insert")
		{
			if (isLineLegit(originalSen))
				originalSen = originalSen.substr(6, originalSen.size());
			std::string key = "INTO";
			int found = findTablesName(originalSen, key);
			std::string tableName;
			bool areMoreThanOneRowInserted = false;

			if (found != -1) {
				if (found == 0)
				{
					originalSen = originalSen.substr(4, originalSen.length());
					std::string key = "(";
					int foundBrecket = findTablesName(originalSen, key);
					if (foundBrecket != -1)
					{
						if (originalSen[0] == '{') areMoreThanOneRowInserted = true;
						tableName = originalSen.substr(0, foundBrecket);
						originalSen = originalSen.substr(foundBrecket + 1, originalSen.length() - tableName.length() - 2);
					}
				}
				else {
					std::cout << "Invalid command, Please try again!";
					continue;
				}
			}
			else {
				std::cout << "Invalid command, Please try again!";
				continue;
			}

			TreeNode* insertionTreeNode = tree.find(tableName);

			if (insertionTreeNode != nullptr)
			{
				List deserializedList;

				if (!readForList(deserializedList, tableName))
				{
					std::cout << "Error reading of vector!\n";
					return 1;
				}

				std::vector<TableName> deserializedVector;

				if (!pairToRead(deserializedVector, tableName))
				{
					std::cout << "Error reading of vector!\n";
					return 1;
				}

				int deserializedVectorSize = deserializedVector.size();

				//In for construction for looping all the insertion arguments

				originalSen = originalSen.substr(0, originalSen.length());
				List allTableArguments = allTableArguments.getAllTableArgs(originalSen, deserializedVector);

				deserializedList.pushList(allTableArguments);

				if (!writeToList(deserializedList, tableName))
				{
					std::cout << "Error reading of vector!\n";
					return 1;
				}

				int numRowsNumber = allTableArguments.getSize();
				std::string numRowsString = convertNumberToString(numRowsNumber);

				std::cout << numRowsString << " row(s) inserted." << std::endl;
			}
			else
			{
				std::cout << "There is no such a table!" << std::endl;
				continue;
			}
		}

		else if (command == "TableInfo")
		{
			originalSen = originalSen.substr(10, originalSen.size());
			originalSen = removeSpaces(originalSen);
			std::string tableName = originalSen;

			if (tableName == "") {
				std::cout << "Invalid command, Please try again!";
				continue;
			}

			TreeNode* tableInformation = tree.find(tableName);

			if (tableInformation == nullptr)
			{
				std::cout << "There is no such table!\n";
				continue;
			}

			std::vector<TableName> deserializedVector;

			if (!pairToRead(deserializedVector, tableName))
			{
				std::cout << "Error reading of vector!\n";
				continue;
			}

			size_t sizeOfTbl = readOnlySizeForList(tableName);

			int fileSize = Get_Size(tableName);

			std::cout << "Table " << tableName << " : (";

			int sizeOfTableNames = deserializedVector.size();

			for (size_t i = 0; i < sizeOfTableNames; i++)
			{
				TableName currTableName = deserializedVector[i];
				std::string currName = currTableName.name;
				std::string currValue = currTableName.value;

				if (i == sizeOfTableNames - 1)
					std::cout << currName << ":" << currValue << ")" << std::endl;
				else
					std::cout << currName << ":" << currValue << ", ";
			}

			std::cout << "Total " << sizeOfTbl << " in the table (" << fileSize << "KB data)" << " in the table" << std::endl;
		}

		else if (command == "Select")
		{

			originalSen = originalSen.substr(6, originalSen.length());

			std::string paramsAsString = getParams(originalSen, "FROM");
			std::vector<std::string> params;
			originalSen = originalSen.substr(paramsAsString.length(), originalSen.length());
			paramsAsString = removeSpaces(paramsAsString);

			params = getArgumeters(paramsAsString, ",");

			int paramsSize = params.size();

			std::string tableName = getParams(originalSen, "WHERE");
			originalSen = originalSen.substr(tableName.length(), originalSen.length());
			tableName = removeSpaces(tableName);
			tableName = tableName.substr(4, tableName.length());
			if (findCommandWordByWord(originalSen, "WHERE"))
				originalSen = originalSen.substr(5, originalSen.length());

			std::vector<TableName> tableNames;

			if (!pairToRead(tableNames, tableName))
			{
				std::cout << "Problem\n";
				continue;
			}

			List desList;

			if (!readForList(desList, tableName))
			{
				std::cout << "Problem\n";
				continue;
			}
			int desListSize = desList.getSize();

			TreeNode* currNode = tree.find(tableName);

			if (currNode == nullptr)
			{
				std::cout << "There is no such a table" << std::endl;
				continue;
			}

			int flag = 0;

			if (params.size() == 0)
			{
				std::cout << "There is a problem with the wanted parameters!\n";
				continue;
			}

			if (params[0] == "*")
			{
				params = getArgumeters(tableNames);
			}

			paramsSize = params.size();

			bool distinctFlag = false;

			if (findCommandWordByWord(originalSen, "DISTINCT"))
			{
				int index = originalSen.find("DISTINCT");
				distinctFlag = true;
				std::string stringAfterDICTINCT = originalSen.substr(index + 8, originalSen.length());
				originalSen = originalSen.substr(0, index) + stringAfterDICTINCT;

			}

			std::string line = getParams(originalSen, "ORDER BY");
			originalSen = originalSen.substr(line.length(), originalSen.length());
			std::string andSt = "AND";
			bool orORAndflag = false;
			if (strstr(line.c_str(), andSt.c_str()))
			{
				orORAndflag = true;
			}

			std::string orderByParam = "";
			if (findCommandWordByWord(originalSen, "ORDER BY"))
			{
				originalSen = removeSpaces(originalSen);
				orderByParam = originalSen.substr(7);
			}
			List resutlForPrint;

			//if there are no where clause
			if (line.size() == 0)
			{
				resutlForPrint = desList;
			}

			std::vector<std::string> statmentAND;
			std::vector<std::string> statmentOR;

			if (orORAndflag) {
				statmentAND = findAndStats(line);
				statmentAND = sortStatmentsAND(statmentAND);
			}
			else
				statmentOR = findORStats(line);

			int statmentsANDSize = statmentAND.size();
			int statmentsORSize = statmentOR.size();

			std::vector<int> possitionForPrint;

			int statmets = statmentAND.size() + statmentOR.size();
			int tablePossition = 0;
			bool conditionFlag = true;

			if (statmentAND.size() > 0)
			{
				for (int s = 0; s < statmentsANDSize; s++)
				{
					std::string currentSatment = statmentAND[s];
					std::vector<std::string> operations = getOperationResult(currentSatment);

					std::string rowName = operations[0];
					std::string condition = operations[1];
					std::string value = operations[2];


					if (tablePossition == params.size())
					{
						conditionFlag = false;
						break;
					}

					if (condition != "==" && condition != ">" && condition != "<" && condition != "!=")
					{
						conditionFlag = false;
						std::cout << "Uncorect operation " << condition << ".\n";
						break;
					}

					tablePossition = returnPossition(tableNames, rowName);

					if (tablePossition == params.size())
					{
						conditionFlag = false;
						break;
					}

					if (condition == "==")
					{
						List equalList = desList.equalStatment(value, tablePossition);
						node* equalListNode = equalList.firstEl();
						std::vector<CellInfo> equalVec = equalListNode->data;
						if (resutlForPrint.find(equalVec))
							continue;
						resutlForPrint.concatList(equalList);

						std::cout << "Uncorect operation " << condition << ".\n";
						std::cout << "Uncorect operation " << condition << ".\n";

					}

					else if (condition == "!=")
					{
						List notEqualList = desList.notEqualStatment(value, tablePossition);
						if (!notEqualList.existFisrtNode()) continue;
						resutlForPrint.concatListNotEqual(notEqualList);
						//std::cout << "Hello World" << std::endl;
					}

					else if (condition == ">")
					{
						List greatherThanList = desList.greatherStatment(value, tablePossition);
						if (greatherThanList.firstEl()->data.size() == 0) continue;
						resutlForPrint = resutlForPrint.concatListGreatherThen(greatherThanList);
						if (s == 0)
						{
							if (resutlForPrint.getSize() == 0)
								continue;
							//else break;
						}
						else if (s != 0)
						{
							if (resutlForPrint.getSize() == 0) continue;
							else break;
						}
					}
					else if (condition == "<")
					{
						List smallerThanList = desList.smallerStatment(value, tablePossition);
						if (!smallerThanList.firstEl()->data.size() == 0)
						{
							resutlForPrint.concatListSmallerThen(smallerThanList);
						}
						if (s == 0)
						{
							if (resutlForPrint.getSize() == 0)
								continue;
							//else break;
						}
						else if (s != 0)
						{
							if (resutlForPrint.getSize() == 0) continue;
							//else break;
						}
					}
				}
			}
			else
			{
				for (int s = 0; s < statmentsORSize; s++)
				{
					std::string currentSatment = statmentOR[s];
					std::vector<std::string> operations = getOperationResult(currentSatment);

					std::string rowName = operations[0];
					std::string condition = operations[1];
					std::string value = operations[2];

					if (condition != "==" && condition != ">" && condition != "<" && condition != "!=")
					{
						conditionFlag = false;
						std::cout << "Uncorect operation " << condition << ".\n";
						break;
					}

					tablePossition = returnPossition(tableNames, rowName);

					if (tablePossition == params.size())
					{
						conditionFlag = false;
						break;
					}

					if (condition == "==")
					{
						List equalList = desList.equalStatment(value, tablePossition);
						if (!equalList.existFisrtNode())
							continue;
						resutlForPrint.concatList(equalList);
						if (s == 0)
						{
							if (resutlForPrint.getSize() == 0)
								continue;
							else break;
						}
						else if (s != 0)
						{
							if (resutlForPrint.getSize() == 0) continue;
							else break;
						}
					}

					else if (condition == "!=")
					{
						List notEqualList = desList.notEqualStatment(value, tablePossition);
						resutlForPrint.concatListNotEqual(notEqualList);
						if (s == 0)
						{
							if (resutlForPrint.getSize() == 0)
								continue;
							else break;
						}
						else if (s != 0)
						{
							if (resutlForPrint.getSize() == 0) continue;
							else break;
						}
					}

					else if (condition == ">")
					{
						List greatherThanList = desList.greatherStatment(value, tablePossition);
						if (greatherThanList.firstEl()->data.size() == 0) continue;
						resutlForPrint = resutlForPrint.concatListGreatherThen(greatherThanList);
						if (s == 0)
						{
							if (resutlForPrint.getSize() == 0)
								continue;
							else break;
						}
						else if (s != 0)
						{
							if (resutlForPrint.getSize() == 0) continue;
							else break;
						}
					}
					else if (condition == "<")
					{
						List smallerThanList = desList.smallerStatment(value, tablePossition);
						if (!smallerThanList.getSize() == 0)
						{
							resutlForPrint = resutlForPrint.concatListSmallerThen(smallerThanList);
						}
						if (s == 0)
						{
							if (resutlForPrint.getSize() == 0)
								continue;
							else break;
						}
						else if (s != 0)
						{
							if (resutlForPrint.getSize() == 0) continue;
							else break;
						}
					}
				}
			}

			if (!conditionFlag)
			{
				std::cout << "Unvalid operation or table's name!.\n";
				continue;
			}

			int allPositions = 0;
			for (size_t i = 0; i < paramsSize; i++)
			{
				//List wantedTableRows;
				std::string param = params[i];
				int beginDifference = (20 - param.size()) / 2;
				int endDifference = 0;
				allPositions += param.size();
				int tablePossition = returnPossition(tableNames, param);
				possitionForPrint.push_back(tablePossition);
				int sdd = param.length();
				if (isOdd(sdd)) endDifference = beginDifference + 1;
				else endDifference = beginDifference;

				if (i == 0)
				{
					std::cout << "|";
					for (size_t s = 0; s < beginDifference; s++)
					{
						std::cout << " ";
					}
					std::cout << param;
					for (size_t s = 0; s < endDifference; s++)
					{
						std::cout << " ";
					}
					std::cout << "|";
				}
				else {
					for (size_t s = 0; s < beginDifference; s++)
					{
						std::cout << " ";
					}
					std::cout << param;
					for (size_t s = 0; s < endDifference; s++)
					{
						std::cout << " ";
					}
					std::cout << "|";
				}
			}

			std::cout << "\n";

			int dashes = (20 * params.size()) + (params.size()) + 1;
			for (size_t i = 0; i < dashes; i++)
			{
				std::cout << "-";
			}

			int printPossitionsSize = possitionForPrint.size();
			std::cout << "\n";

			if (distinctFlag)
			{
				resutlForPrint = resutlForPrint.distinctRows(resutlForPrint);
			}

			if (orderByParam != "")
			{
				tablePossition = returnPossition(tableNames, orderByParam);
				resutlForPrint = resutlForPrint.orderBy(tablePossition);
			}

			resutlForPrint.printNodes(possitionForPrint, params);

		}

		else if (command == "Remove")
		{
			originalSen = originalSen.substr(6, originalSen.length());

			std::string paramsAsString = getParams(originalSen, "FROM");
			std::vector<std::string> params;
			originalSen = originalSen.substr(paramsAsString.length(), originalSen.length());
			paramsAsString = removeSpaces(paramsAsString);

			params = getArgumeters(paramsAsString, ",");

			int paramsSize = params.size();

			std::string tableName = getParams(originalSen, "WHERE");
			originalSen = originalSen.substr(tableName.length(), originalSen.length());
			tableName = removeSpaces(tableName);
			tableName = tableName.substr(4, tableName.length());

			if (!findCommandWordByWord(originalSen, "WHERE"))
			{
				std::cout << "You have to specify wich row(s) yoi want to remove with where clause.\n";
				continue;
			}

			originalSen = originalSen.substr(5, originalSen.length());

			if (originalSen.size() == 0)
			{
				std::cout << "You have to specify the arguments of the where clouse\n";
				continue;
			}

			std::vector<TableName> tableNames;

			if (!pairToRead(tableNames, tableName))
			{
				std::cout << "Problem\n";
				continue;
			}

			List desList;

			if (!readForList(desList, tableName))
			{
				std::cout << "Problem\n";
				continue;
			}
			int desListSize = desList.getSize();

			TreeNode* currNode = tree.find(tableName);

			if (currNode == nullptr)
			{
				std::cout << "There is no such a table\n";
				continue;
			}

			int flag = 0;

			if (params.size() > 0)
			{
				std::cout << "You can not put any table's parameters here.\n";
				continue;
			}

			paramsSize = params.size();

			std::string line = getParams(originalSen, "ORDER BY");
			originalSen = originalSen.substr(line.length(), originalSen.length());
			std::string andSt = "AND";
			bool orORAndflag = false;
			if (strstr(line.c_str(), andSt.c_str()))
			{
				orORAndflag = true;
			}

			List resutlForPrint;

			//if there are no where clause
			if (line.size() == 0)
			{
				resutlForPrint = desList;
			}

			std::vector<std::string> statmentAND;
			std::vector<std::string> statmentOR;

			if (orORAndflag) {
				statmentAND = findAndStats(line);
				statmentAND = sortStatmentsAND(statmentAND);
			}
			else
				statmentOR = findORStats(line);

			int statmentsANDSize = statmentAND.size();
			int statmentsORSize = statmentOR.size();

			std::vector<int> possitionForPrint;

			int statmets = statmentAND.size() + statmentOR.size();
			int tablePossition = 0;


			bool conditionFlag = true;
			if (statmentAND.size() > 0)
			{
				for (int s = 0; s < statmentsANDSize; s++)
				{
					std::string currentSatment = statmentAND[s];
					std::vector<std::string> operations = getOperationResult(currentSatment);

					std::string rowName = operations[0];
					std::string condition = operations[1];
					std::string value = operations[2];

					tablePossition = returnPossition(tableNames, rowName);
					if (tablePossition == tableNames.size())
					{
						std::cout << "Uncorrect parameters. " << rowName << " doesn't exist in the table.\n";

						break;
					}

					if (condition == "==")
					{
						List equalList = desList.equalStatment(value, tablePossition);
						resutlForPrint.concatList(equalList);
						if (s == 0)
						{
							if (resutlForPrint.getSize() == 0)
								continue;
							else break;
						}
					}

					else if (condition == "!=")
					{
						List notEqualList = desList.notEqualStatment(value, tablePossition);
						resutlForPrint.concatListNotEqual(notEqualList);
						//std::cout << "Hello World" << std::endl;
					}

					else if (condition == ">")
					{
						List greatherThanList = desList.greatherStatment(value, tablePossition);
						resutlForPrint = resutlForPrint.concatListGreatherThen(greatherThanList);
					}
					else if (condition == "<")
					{
						List smallerThanList = desList.smallerStatment(value, tablePossition);
						resutlForPrint = resutlForPrint.concatListSmallerThen(smallerThanList);
					}
				}
			}
			else
			{
				for (int s = 0; s < statmentsORSize; s++)
				{
					std::string currentSatment = statmentOR[s];
					std::vector<std::string> operations = getOperationResult(currentSatment);

					std::string rowName = operations[0];
					std::string condition = operations[1];
					std::string value = operations[2];

					if (condition != "==" && condition != ">" && condition != "<" && condition != "!=")
					{
						conditionFlag = false;
						std::cout << "Uncorect operation " << condition << ".\n";
						break;
					}

					tablePossition = returnPossition(tableNames, rowName);

					if (tablePossition == params.size())
					{
						conditionFlag = false;
						break;
					}

					if (tablePossition == tableNames.size())
					{
						std::cout << "Uncorrect parameters. " << rowName << " doesn't exist in the table.\n";

						break;
					}

					if (condition == "==")
					{
						List equalList = desList.equalStatment(value, tablePossition);
						if (!equalList.existFisrtNode())
							continue;
						resutlForPrint.concatList(equalList);
						if (s == 0)
						{
							if (resutlForPrint.getSize() == 0)
								continue;
							else break;
						}
						else if (s != 0)
						{
							if (resutlForPrint.getSize() == 0) continue;
							else break;
						}
					}

					else if (condition == "!=")
					{
						List notEqualList = desList.notEqualStatment(value, tablePossition);
						resutlForPrint.concatListNotEqual(notEqualList);
						if (s == 0)
						{
							if (resutlForPrint.getSize() == 0)
								continue;
							else break;
						}
						else if (s != 0)
						{
							if (resutlForPrint.getSize() == 0) continue;
							else break;
						}
					}

					else if (condition == ">")
					{
						List greatherThanList = desList.greatherStatment(value, tablePossition);
						resutlForPrint = resutlForPrint.concatListGreatherThen(greatherThanList);
						if (s == 0)
						{
							if (resutlForPrint.getSize() == 0)
								continue;
							else break;
						}
						else if (s != 0)
						{
							if (resutlForPrint.getSize() == 0) continue;
							else break;
						}
					}
					else if (condition == "<")
					{
						List smallerThanList = desList.smallerStatment(value, tablePossition);
						resutlForPrint = resutlForPrint.concatListSmallerThen(smallerThanList);
						if (s == 0)
						{
							if (resutlForPrint.getSize() == 0)
								continue;
							else break;
						}
						else if (s != 0)
						{
							if (resutlForPrint.getSize() == 0) continue;
							else break;
						}
					}
				}
			}

			if (!conditionFlag)
			{
				std::cout << "Unvalid operation or table's name!.\n";
				continue;
			}

			//the rows for deletion
			int resutlForPrintSize = resutlForPrint.getSize();

			desList.removeAll(resutlForPrint);

			if (!writeToList(desList, tableName))
			{
				std::cout << "Error reading of vector!\n";
				return 1;
			}

			std::string numRowsString = convertNumberToString(resutlForPrintSize);

			std::cout << numRowsString << " row(s) removed." << std::endl;

		}

		else if (command == "Quit")
		{
			std::cout << "The command is Quit" << std::endl;
			std::cout << "Goodbye, master ;(" << std::endl;
			break;
		}

		else if (command == "Unlegal\n") continue;
	} while (true);
}
