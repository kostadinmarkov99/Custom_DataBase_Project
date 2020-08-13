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

struct TableName
{
	std::string name;
	std::string value;
	int size;
	TableName();
	TableName(std::string name, std::string value);
	TableName(std::string name, std::string value, int s);
};

bool pairToWrite(std::vector<TableName>& elements_to_write, std::string& tableName);
bool pairToRead(std::vector<TableName>& elements_to_read, std::string& tableName);