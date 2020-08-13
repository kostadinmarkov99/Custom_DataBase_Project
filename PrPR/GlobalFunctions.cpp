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

void writeStringToFile(std::ostream& os, const std::string& val)
{
	size_t valueLength = val.size();
	os.write((char*)&valueLength, sizeof(valueLength));
	if (valueLength != 0)
	{
		os.write(val.data(), valueLength);
	}
}

void readStringFromFile(std::istream& is, std::string& val)
{
	size_t valueLength = 0;
	val = {};
	is.read((char*)&valueLength, sizeof(valueLength));
	if (valueLength != 0)
	{
		std::vector<char> rawData;
		rawData.resize(valueLength);
		is.read(rawData.data(), valueLength);
		val.assign(rawData.data(), valueLength);
	}
}

bool isOdd(int number)
{
	if (number % 2 == 1) return true;
	return false;
}

std::string convertNumberToString(int rowsNumber)
{
	const char* smallNumbers[] = {
  "Zero", "One", "Two", "Three", "Four", "Five",
  "Six", "Seven", "Eight", "Nine", "Ten",
  "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen",
  "Sixteen", "Seventeen", "Eighteen", "Nineteen"
	};

	std::string res;
	if (rowsNumber > 99) {
		res = smallNumbers[rowsNumber / 100];
		res += " hundred";
		rowsNumber %= 100;
		if (rowsNumber) res += " and ";
	}
	if (rowsNumber >= 20) {
		static const char* Decades[] = {
		  "", "", "twenty", "thirty", "forty",
		  "fifty", "sixty", "seventy", "eighty", "ninety"
		};
		res += Decades[rowsNumber / 10];
		rowsNumber %= 10;
		if (rowsNumber) res += "-";
	}
	if (rowsNumber < 20 && rowsNumber > 0)
		res += smallNumbers[rowsNumber];
	return res;
}

bool isString(std::string& currRow)
{
	int quates = 0;
	int currRowSize = currRow.size();
	for (size_t i = 0; i < currRowSize; i++)
	{
		if (currRow[i] == '\"') quates++;
	}
	if (quates == 0) return false;
	return true;
}

std::vector<std::string> getArgumeters(std::string& originalStr, std::string delimator)
{
	std::vector<std::string>result;

	while (originalStr.size()) {
		int index = originalStr.find(delimator);
		if (index != std::string::npos) {
			result.push_back(originalStr.substr(0, index));
			originalStr = originalStr.substr(index + delimator.size());
			if (originalStr.size() == 0)result.push_back(originalStr);
		}
		else {
			result.push_back(originalStr);
			originalStr = "";
		}
	}

	return result;
}