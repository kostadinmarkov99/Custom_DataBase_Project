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
#include "TableName.h"
#include "GlobalFunctions.h"


struct CellInfo
{
	bool isString;
	std::string string;
	int value;

	CellInfo();
	CellInfo(std::string str);
	CellInfo(int val);
	CellInfo& operator=(CellInfo& other);

	bool isEqual(CellInfo& other);
	bool isGreather(CellInfo& other);
	bool isSmaller(CellInfo& other);
	bool operator==(CellInfo& other);
};

struct node
{
	std::vector<CellInfo> data;
	node* next;

	node();
	node(std::vector<CellInfo> x);
	node& operator=(node& other);
	bool operator==(node& other);
	bool operator !=(node& other);
	bool operator>(node& other);
	bool operator<(node& other);
};

class List
{
	size_t sizeList;
	node* first, * last;

public:
	List();
	List(const List& ll);
	~List();
	bool isEmpty() const;
	List getAllTableArgs(std::string& originalSen, std::vector<TableName>& deserializedVector);
	void insertFront(std::vector<CellInfo> x);
	List& operator=(const List& ll);
	List  orderBy(int pos);
	void insertLast(std::vector<CellInfo> x);
	List distinctRows(List& other);
	bool existFisrtNode();
	bool existNode(node* other);
	List equalStatment(std::string value, int tablePossition);
	List notEqualStatment(std::string value, int tablePossition);
	List greatherStatment(std::string value, int tablePossition);
	List smallerStatment(std::string value, int tablePossition);
	List removeAll(List& other);
	void printNodes(std::vector<int> possitions, std::vector<std::string>& params);
	List& concatList(List& other);
	List& concatListNotEqual(List& other);
	List& concatListSmallerThen(List& other);
	List& concatListGreatherThen(List& other);
	bool removeFirst();
	int getSize() const;
	node* find(std::vector<CellInfo> key);
	node* find(std::string firstEl);
	std::vector<node*> findAllOccurances(std::string keyStr, int possition);
	node* findAnyOccurance(std::string keyStr, int possition);
	List pushList(List& other);
	bool remove(std::vector<CellInfo> key);
	bool isFirstDataAndKeyEqual(std::vector<CellInfo> first, std::vector<CellInfo> key);
	node* firstEl();
	bool remove(node* it);
};

bool writeToList(List& list, std::string& tableName);
size_t readOnlySizeForList(std::string tableName);
bool readForList(List& list, std::string& tableName);