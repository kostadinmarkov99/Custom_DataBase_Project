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

#define TableNames "C:\\Users\\PC\\Desktop\\Database\\TableNames\\"
#define TableRows "C:\\Users\\PC\\Desktop\\Database\\TableRows\\"
#define TableTree "C:\\Users\\PC\\Desktop\\Database\\TreeNodeAndTreeSerialization.txt"
#define TableTreeSomething "C:\\Users\\PC\\Desktop\\Database\\TreeNodeAndTreeSerialization.txt"

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


#pragma region TableName
struct TableName
{
	std::string name;
	std::string value;
	int size;
	TableName() : name(), value(), size() {}
	TableName(std::string name, std::string value)
	{
		this->name = name;
		this->value = value;
		this->size = 0;
	}
	TableName(std::string name, std::string value, int s)
	{
		this->name = name;
		this->value = value;
		size = 0;
	}
};

bool pairToWrite(std::vector<TableName>& elements_to_write, std::string& tableName)
{
	std::string filePathTableName = TableNames + tableName;
	std::string filePathRows = TableRows + tableName;
	//std::string tableName(filePathTableName);
	//std::string tableRows(filePathRows);
	std::ofstream out(filePathTableName, std::ios::binary);
	std::ofstream outRows(filePathRows, std::ios::binary);
	if (!out || !outRows) return false;

	//std::vector<TableName> elements_to_write;

	size_t vec_size = elements_to_write.size();
	out.write(reinterpret_cast<char*>(&vec_size), sizeof(vec_size));

	for (int i = 0; i < vec_size; i++)
	{
		TableName& el = elements_to_write[i];
		writeStringToFile(out, el.name);
		writeStringToFile(out, el.value);
		out.write(reinterpret_cast<char*>(&el.size), sizeof(int));
	}

	return true;
}

bool pairToRead(std::vector<TableName>& elements_to_read, std::string& tableName)
{
	std::string filePath = TableNames + tableName;
	std::string plateNumber(filePath);
	std::ifstream in(plateNumber, std::ios::binary);
	if (!in) return false;
	//std::vector<Element> elements_to_read;

	size_t vec_size = 0;
	in.read(reinterpret_cast<char*>(&vec_size), sizeof(vec_size));

	elements_to_read.reserve(vec_size);

	for (int i = 0; i < vec_size; i++)
	{
		TableName el;

		readStringFromFile(in, el.name);
		readStringFromFile(in, el.value);

		in.read(reinterpret_cast<char*>(&el.size), sizeof(el.size));
		elements_to_read.push_back(el);
	}

	return true;
}

#pragma endregion

#pragma region ListNodeAndList
struct CellInfo
{
	bool isString;
	std::string string;
	int value;

	CellInfo()
	{
		isString = true;
		string = "No name";
		value = NULL;
	}
	CellInfo(std::string str)
	{
		isString = true;
		string = str;
		value = NULL;
	}
	CellInfo(int val)
	{
		isString = false;
		value = val;
		string = "";
	}

	CellInfo& operator=(CellInfo& other)
	{
		isString = other.isString;
		string = other.string;
		value = other.value;

		return *this;
	}

	bool isEqual(CellInfo& other)
	{
		if ((this->isString == other.isString) && (this->string == other.string) && (this->value == other.value)) return true;
		return false;
	}

	bool isGreather(CellInfo& other)
	{
		if ((this->string > other.string) && (this->value < other.value)) return true;
		return false;
	}

	bool isSmaller(CellInfo& other)
	{
		if ((this->string < other.string) && (this->value > other.value)) return true;
		return false;
	}

	bool operator==(CellInfo& other)
	{
		if ((this->isString == other.isString) && (this->string == other.string) && (this->value == other.value)) return true;
		return false;
	}
};

struct node
{
	std::vector<CellInfo> data;
	node* next;

	node()
	{
		std::vector<CellInfo> dataNode;
		this->data = dataNode;
		this->next = nullptr;
	}

	node(std::vector<CellInfo> x)
	{
		data = x;
	}

	node& operator=(node& other)
	{
		this->next = other.next;
		this->data = other.data;
		return*this;
	}

	bool operator==(node& other)
	{
		bool flag = false;

		size_t d = data.size();
		size_t a = other.data.size();
		if (d == a)
		{
			int count = 0;
			for (size_t i = 0; i < data.size(); i++)
			{
				if (data[i].isEqual(other.data[i])) count++;
			}

			if (count == data.size()) flag = true;
		}

		return flag;
	}

	bool operator !=(node& other)
	{
		bool flag = true;
		size_t d = data.size();
		size_t a = other.data.size();
		if (d == a)
		{
			int count = 0;
			for (size_t i = 0; i < data.size(); i++)
			{
				if (data[i].isEqual(other.data[i])) count++;
			}

			if (count == data.size()) flag = false;
		}

		return flag;
	}

	bool operator>(node& other)
	{
		bool flag = true;
		size_t d = data.size();
		size_t a = other.data.size();
		if (d == a)
		{
			int count = 0;
			for (size_t i = 0; i < data.size(); i++)
			{
				if (data[i].isGreather(other.data[i])) count++;
			}

			if (count == data.size()) flag = false;
		}

		return flag;
	}

	bool operator<(node& other)
	{
		bool flag = true;
		size_t d = data.size();
		size_t a = other.data.size();
		if (d == a)
		{
			int count = 0;
			for (size_t i = 0; i < data.size(); i++)
			{
				if (data[i].isSmaller(other.data[i])) count++;
			}

			if (count == data.size()) flag = false;
		}

		return flag;
	}
};

class List
{
	size_t sizeList;
	node* first, * last;

public:
	List() { first = last = nullptr; }

	List(const List& ll)
	{
		if (ll.first == nullptr)
			return;

		node* tmp = ll.first;

		first = new node;
		first->data = tmp->data;
		first->next = nullptr;
		node* current = first;

		tmp = tmp->next;

		while (tmp != nullptr)
		{
			current->next = new node;
			current = current->next;
			current->data = tmp->data;
			current->next = nullptr;
			tmp = tmp->next;
		}
	}

	~List()
	{
		node* current = first;
		node* next;

		while (current != NULL) {
			next = current->next;
			delete current;
			this->sizeList--;
			current = next;
		}
	}

	bool isEmpty() const
	{
		if (sizeList == 0) return true;
		return false;
	}

	List getAllTableArgs(std::string& originalSen, std::vector<TableName>& deserializedVector)
	{
		List result;
		while (originalSen.size())
		{
			std::vector<CellInfo> currResult;
			int index = originalSen.find(")");
			if (index != std::string::npos)
			{
				std::string currentArg = originalSen.substr(1, index - 1);
				std::vector<std::string> tableArguments = getArgumeters(currentArg, ",");

				int desVectorSize = deserializedVector.size();

				for (size_t ita = 0; ita < desVectorSize; ita++)
				{
					TableName curTab = deserializedVector[ita];
					//std::string curtabName = returnOnlyRealString(curTab.value);
					std::string currTabValue = curTab.value;
					std::string currentRow = tableArguments[ita];

					if (isString(currentRow))
					{
						CellInfo newListNode(currentRow);
						currResult.push_back(newListNode);
						//deserializedList.insertLast(valueType);
						continue;
					}
					else
					{
						int number = std::atoi(currentRow.c_str());
						CellInfo newListNode(number);
						//std::vector<ListNode> valueType;
						currResult.push_back(number);
						//deserializedList.insertLast(valueType);
						continue;
					}
				}
				result.insertLast(currResult);
				if (index >= originalSen.size() - 1)
					break;
				else { originalSen = originalSen.substr(index + 2, originalSen.length()); }
			}
			else
			{
				std::string withBrecks = "(" + originalSen + ")";
				return getAllTableArgs(withBrecks, deserializedVector);
			}
		}
		*this = result;
		return *this;
	}

	void insertFront(std::vector<CellInfo> x)
	{
		node* nodeToInsert = new node(x);
		//nodeToInsert->data = x;
		nodeToInsert->next = first;
		first = nodeToInsert;
		sizeList++;
	}

	List& operator=(const List& ll)
	{
		if (ll.first == nullptr)
		{
			List nList;
			return nList;
		}
		node* tmp = ll.first;

		first = new node;
		first->data = tmp->data;
		first->next = nullptr;
		node* current = first;

		tmp = tmp->next;

		while (tmp != nullptr)
		{
			current->next = new node;
			current = current->next;
			current->data = tmp->data;
			current->next = nullptr;
			tmp = tmp->next;
		}

		this->sizeList = ll.sizeList;

		return *this;
	}

	List  orderBy(int pos)
	{
		List resultList;
		List thisList = *this;

		node* it = this->first;

		while (it->data.size() != 0)
		{
			it = this->first;
			std::vector<CellInfo> itNode = it->data;
			std::vector<CellInfo> minListNode = itNode;
			node* iterator;
			if (it->next)
				iterator = first;
			else break;

			CellInfo currItNode = itNode[pos];
			std::string value = "";
			int val = INT_MIN;

			if (currItNode.isString) value = currItNode.string;
			else val = currItNode.value;
			std::string minCandidat = "";
			int minCandInt = INT_MIN;

			while (iterator)
			{
				std::vector<CellInfo> iteratorNode = iterator->data;
				CellInfo iterNode = iteratorNode[pos];


				if (iterNode.isString) minCandidat = iterNode.string;
				else minCandInt = iterNode.value;

				if (!iterNode.isString)
				{
					if (minCandInt < val)
					{
						minListNode = iteratorNode;
						minCandInt = iterNode.value;
						val = minCandInt;
					}
				}
				else
				{
					if (minCandidat < value) {
						minListNode = iteratorNode;
						minCandidat = iterNode.string;
					}
				}
				if (iterator->next)
					iterator = iterator->next;
				else {
					int c = 0;

					for (size_t i = 0; i < minListNode.size(); i++)
					{
						if (minListNode[i] == it->data[i]) c++;
						if (c == it->data.size())
						{
							this->first = this->first->next;
						}
					}
					resultList.insertLast(minListNode);
					this->remove(minListNode);
					if (it->data.size() != 0)
					{
						it = it->next;
						it = this->first;
					}
					break;
				}
			}
		}
		resultList.insertLast(it->data);
		return resultList;
	}

	void insertLast(std::vector<CellInfo> x)
	{
		if (first == nullptr && last == nullptr)
		{
			node* nodeToInsert = new node(x);
			first = last = nodeToInsert;
			last->next = nullptr;
			sizeList++;
			return;
		}
		node* nodeToInsert = new node(x);
		//nodeToInsert->data = x;
		nodeToInsert->next = nullptr;
		last->next = nodeToInsert;
		last = nodeToInsert;
		sizeList++;
	}

	List distinctRows(List& other)
	{
		List result;
		List thisList = *this;

		node* it = other.first;
		while (it)
		{
			std::vector<CellInfo> itVec = it->data;

			if (result.existNode(it))
			{
				it = it->next;
				continue;
			}

			result.insertLast(itVec);

			it = it->next;
		}

		return result;
	}

	bool existFisrtNode()
	{
		if (this->first) return true;
		else return false;
	}

	bool existNode(node* other)
	{
		bool flag = false;
		node* it = this->first;

		if (it)
		{
			std::vector<CellInfo> itLSTNd = it->data;
			std::vector<CellInfo> otherLSTNd = other->data;
			int counter = 0;
			for (size_t i = 0; i < itLSTNd.size(); i++)
			{
				CellInfo itListNode = itLSTNd[i];
				CellInfo otherListNode = otherLSTNd[i];

				if (itListNode == otherListNode) counter++;
			}

			if (counter == itLSTNd.size()) return true;

			if (it->next)
				it = it->next;
			else return flag;
		}

		return false;
	}

	List equalStatment(std::string value, int tablePossition)
	{
		List resultList;
		std::vector<node*> it = findAllOccurances(value, tablePossition);

		if (it.size() != 0)
		{
			for (size_t i = 0; i < it.size(); i++)
			{
				node* currNode = it[i];
				resultList.insertLast(currNode->data);
			}
		}
		if (it.size() == 0)
			return resultList;
		//std::vector<ListNode> itVector = it->data;
		return resultList;
	}

	List notEqualStatment(std::string value, int tablePossition)
	{
		List resultList;
		node* itOcc = findAnyOccurance(value, tablePossition);
		if (!itOcc) return resultList;
		node* it = first;
		while (it)
		{
			std::vector<CellInfo> itVec = it->data;

			if (!it->data[tablePossition].isEqual(itOcc->data[tablePossition])) resultList.insertLast(itVec);
			else
			{
				it = it->next;
				continue;
			}
			if (it->next) it = it->next;
			else it = it->next;
			continue;
		}

		return resultList;
	}

	List greatherStatment(std::string value, int tablePossition)
	{
		List resultList;
		//node* itOcc = findAnyOccurance(value, tablePossition);
		node* it = first;
		while (it)
		{
			std::vector<CellInfo> firstNodeData = it->data;

			std::string firstNodeDataValue = std::to_string(firstNodeData[tablePossition].value);
			std::string firstDataString = firstNodeData[tablePossition].string;

			//From string to int
			if (firstDataString == "") {
				int firstNodeDataValueInt = atoi(firstNodeDataValue.c_str());
				int valueInt = atoi(value.c_str());

				if (firstNodeDataValueInt > valueInt) resultList.insertLast(firstNodeData);
				if (it->next) it = it->next;
				else break;
				continue;
			}
			else if (firstNodeData[tablePossition].string > value) resultList.insertLast(firstNodeData);
			else continue;
			if (it->next) it = it->next;
			//if (!it->next) continue;
		}

		//*this = resultList;
		return resultList;
	}

	List smallerStatment(std::string value, int tablePossition)
	{
		List resultList;
		//node* itOcc = findAnyOccurance(value, tablePossition);
		node* it = first;
		while (it)
		{
			std::vector<CellInfo> firstNodeData = it->data;

			std::string firstNodeDataValue = std::to_string(firstNodeData[tablePossition].value);
			std::string firstDataString = firstNodeData[tablePossition].string;

			//From string to int
			if (firstDataString == "") {
				int firstNodeDataValueInt = atoi(firstNodeDataValue.c_str());
				int valueInt = atoi(value.c_str());

				if (firstNodeDataValueInt < valueInt) resultList.insertLast(firstNodeData);
				if (it->next) it = it->next;
				else break;
				continue;
			}
			else if (firstNodeData[tablePossition].string < value) resultList.insertLast(firstNodeData);
			else continue;
			if (it->next) it = it->next;
			//if (!it->next) continue;
		}

		return resultList;
	}

	List removeAll(List& other)
	{
		node* it = other.first;

		while (it)
		{
			std::vector<CellInfo> itData = it->data;
			this->remove(itData);

			it = it->next;
		}

		return *this;
	}

	void printNodes(std::vector<int> possitions, std::vector<std::string>& params)
	{
		List* thisList = this;
		std::string val = "";
		bool flag = true;
		node* it = thisList->first;

		int selectedRows = 0;
		while (it)
		{
			selectedRows++;

			for (size_t i = 0; i < possitions.size(); i++)
			{
				std::string firstParamArg = params[i];

				val += "|";
				int position = possitions[i];
				int strValueSize;
				int beginDifference = 0;
				int endDifference = 0;

				bool iSString = it->data[position].isString;
				if (iSString) {
					std::string strValue = it->data[position].string;
					strValueSize = strValue.size();
					beginDifference = (20 - strValueSize) / 2;
					if (isOdd(strValueSize)) endDifference = beginDifference + 1;
					else endDifference = beginDifference;

					for (size_t s = 0; s < beginDifference; s++)
					{

						val += " ";
					}
					val += strValue;
					for (size_t s = 0; s < endDifference; s++)
					{
						val += " ";
					}
				}
				else
				{
					std::string number = std::to_string(it->data[position].value);
					strValueSize = number.size();
					beginDifference = (20 - strValueSize) / 2;
					if (isOdd(strValueSize)) endDifference = beginDifference + 1;
					else endDifference = beginDifference;

					for (size_t s = 0; s < beginDifference; s++)
					{

						val += " ";
					}
					val += number;
					for (size_t s = 0; s < endDifference; s++)
					{
						val += " ";
					}
				}
			}

			val += "|\n";

			if (it->next) it = it->next;
			else
			{
				flag = false;
				break;
			}

			if (!flag) break;
		}


		std::string numRowsString = convertNumberToString(selectedRows);

		std::cout << val << std::endl;
		std::cout << "Total " << numRowsString;
		if (selectedRows == 1) std::cout << " row selected.\n";
		else std::cout << " rows selected.\n";

	}


	List& concatList(List& other)
	{
		List thisList = *this;
		node* otherNext = other.first;

		this->pushList(other);

		return *this;
	}

	List& concatListNotEqual(List& other)
	{
		List thisList = *this;
		node* otherNext = other.first;
		if (this->getSize() == 0)
		{
			this->pushList(other);
			return *this;
		}
		while (otherNext)
		{
			std::vector<CellInfo> otherCurrentNode = otherNext->data;
			node* findCurrNode = this->find(otherCurrentNode);
			if (findCurrNode) continue;
			this->last->next = otherNext;
			sizeList++;
			if (otherNext->next) otherNext = otherNext->next;
			else continue;
		}

		return *this;
	}

	List& concatListSmallerThen(List& other)
	{
		List resultList;
		List thisList = *this;
		node* otherNext = other.first;
		if (this->getSize() == 0)
		{
			this->pushList(other);
			return *this;
		}

		while (otherNext)
		{
			std::vector<CellInfo> otherCurrentNode = otherNext->data;
			node* findCurrNode = this->find(otherCurrentNode);
			if (!findCurrNode)
			{
				if (otherNext->next) otherNext = otherNext->next;
				else break;
				continue;
			}
			resultList.insertLast(otherCurrentNode);
			if (otherNext->next) otherNext = otherNext->next;
			else break;
		}

		*this = resultList;
		return resultList;
	}

	List& concatListGreatherThen(List& other)
	{
		List resultList;
		List thisList = *this;
		node* otherNext = other.first;
		if (this->getSize() == 0)
		{
			this->pushList(other);
			return *this;
		}

		while (otherNext)
		{
			std::vector<CellInfo> otherCurrentNode = otherNext->data;
			node* findCurrNode = this->find(otherCurrentNode);
			if (!findCurrNode)
			{
				if (otherNext->next) otherNext = otherNext->next;
				else break;
				continue;
			}
			resultList.insertLast(otherCurrentNode);
			if (otherNext->next) otherNext = otherNext->next;
			else break;
		}

		return resultList;
	}

	bool removeFirst()
	{
		if (isEmpty()) return false;
		node* firstNode = first;
		first = first->next;
		firstNode->next = nullptr;
		delete firstNode;
		sizeList--;
		return true;
	}

	int getSize() const
	{
		return sizeList;
	}

	node* find(std::vector<CellInfo> key)
	{
		node* it = first;
		while (it && !isFirstDataAndKeyEqual(it->data, key)) it = it->next;

		return it;
	}

	node* find(std::string firstEl)
	{
		node* it = first;
		while (it)
		{
			std::vector<CellInfo> firstNodeData = it->data;
			if (firstNodeData[0].string == firstEl) return it;
			if (it->next)
				it = it->next;
		}

		return nullptr;
	}

	std::vector<node*> findAllOccurances(std::string keyStr, int possition)
	{
		std::vector<node*> result;
		node* it = first;
		while (it)
		{
			std::vector<CellInfo> firstNodeData = it->data;
			int firstNodeSize = firstNodeData.size();

			std::string firstNodeDataValue = std::to_string(firstNodeData[possition].value);

			if (firstNodeData[possition].string == keyStr || firstNodeDataValue == keyStr) {
				result.push_back(it);
			}

			it = it->next;
		}

		return result;
	}

	node* findAnyOccurance(std::string keyStr, int possition)
	{
		node* it = first;
		while (it)
		{
			std::vector<CellInfo> firstNodeData = it->data;
			int firstNodeSize = firstNodeData.size();

			std::string firstNodeDataValue = std::to_string(firstNodeData[possition].value);

			if (firstNodeData[possition].string == keyStr || firstNodeDataValue == keyStr) return it;

			it = it->next;
		}

		return it;
	}

	List pushList(List& other)
	{
		node* it = other.first;
		//int otherSize = other.getSize();
		while (it)
		{
			std::vector<CellInfo> firstNodeData = it->data;

			this->insertLast(firstNodeData);
			if (it->next)
				it = it->next;
			else break;
		}
		return *this;
	}

	bool remove(std::vector<CellInfo> key)
	{
		if (!this->existFisrtNode()) return false;

		if (isFirstDataAndKeyEqual(first->data, key))
		{
			std::vector<CellInfo> x = first->data;
			removeFirst();
			return true;
		}
		node* prev = first;
		while (prev->next)
		{
			if (isFirstDataAndKeyEqual(prev->next->data, key))
			{
				node* n = prev->next;
				prev->next = n->next;
				std::vector<CellInfo> x = n->data;
				delete n;
				sizeList--;
				return true;
			}
			prev = prev->next;
		}
		return true;
	}

	bool areTwoVectorsEqual()
	{

	}

	bool isFirstDataAndKeyEqual(std::vector<CellInfo> first, std::vector<CellInfo> key)
	{
		bool flag = false;

		if (first.size() == key.size())
		{
			int count = 0;
			for (size_t i = 0; i < key.size(); i++)
			{
				if (first[i].isEqual(key[i])) count++;
			}

			if (count == first.size()) flag = true;
		}
		return flag;
	}

	node* firstEl()
	{
		return first;
	}

	bool remove(node* it)
	{
		if (!it) return true;
		if (it->next)
		{
			node* n = it->next;
			if (!n)
			{
				n = it;
				*it = *n;
				delete it;
				--sizeList;
				return true;
			}
			std::vector<CellInfo> x = it->data;
			*it = *n;
			delete n;
			--sizeList;
			if (!it->next)
			{
				last = it;
			}
			it = it->next;
		}
		else
		{

		}
		return true;
	}
};

bool writeToList(List& list, std::string& tableName)
{
	std::string plateNumber(TableRows + tableName);
	std::ofstream out(plateNumber, std::ios::binary);
	if (!out) return false;
	if (list.getSize() == 0) return false;
	size_t list_size = list.getSize();
	out.write(reinterpret_cast<char*>(&list_size), sizeof(list_size));

	node* current = list.firstEl();
	while (current)
	{
		size_t vec_size = current->data.size();
		out.write(reinterpret_cast<char*>(&vec_size), sizeof(vec_size));
		for (size_t i = 0; i < vec_size; i++)
		{
			CellInfo& el = current->data[i];
			writeStringToFile(out, el.string);
			out.write(reinterpret_cast<char*>(&el.value), sizeof(el.value));
			out.write(reinterpret_cast<char*>(&el.isString), sizeof(el.isString));
		}

		if (current->next)
			current = current->next;
		else break;
	}

	return true;
}

size_t readOnlySizeForList(std::string tableName)
{
	std::string plateNumber(TableRows + tableName);
	std::ifstream in(plateNumber, std::ios::binary);
	if (!in) return false;

	size_t list_size = 0;
	in.read(reinterpret_cast<char*>(&list_size), sizeof(list_size));

	return list_size;
}

bool readForList(List& list, std::string& tableName)
{
	std::string plateNumber(TableRows + tableName);
	std::ifstream in(plateNumber, std::ios::binary);
	if (!in) return false;

	size_t list_size = 0;
	in.read(reinterpret_cast<char*>(&list_size), sizeof(list_size));

	for (int i = 0; i < list_size; i++)
	{
		std::vector<CellInfo> x;
		size_t vec_size = 0;
		in.read(reinterpret_cast<char*>(&vec_size), sizeof(vec_size));
		for (int j = 0; j < vec_size; j++)
		{
			CellInfo el;
			readStringFromFile(in, el.string);
			in.read(reinterpret_cast<char*>(&el.value), sizeof(el.value));
			in.read(reinterpret_cast<char*>(&el.isString), sizeof(el.isString));
			x.push_back(el);
		}
		list.insertLast(x);
	}

	return true;
}

#pragma endregion

#pragma region TreeNodeAndTree
struct TreeNode
{
	std::string value;
	TreeNode* left, * right;

	bool operator<(const TreeNode& other)
	{
		return value < other.value; // lekxicographical compair
	}	bool operator==(const TreeNode& other)
	{
		return value == other.value; // leksicographical compair
	}
	TreeNode()
	{
		value = "NoName";
		left = right = nullptr;
	}
	TreeNode(int s)
	{
		value = "NoName";
		left = right = nullptr;
	}
	TreeNode(const std::string& value)
	{
		this->value = value;
		left = right = nullptr;
	}
};

class Tree
{
private:
	TreeNode* root;

	TreeNode* find(TreeNode* r, const std::string& key) const
	{
		if (!r)  return nullptr;

		if (r->value == key) return r;

		return r->value > key ? find(r->left, key) : find(r->right, key);
	}

	TreeNode* insert(TreeNode*& r, std::string& val)
	{
		if (!r)
		{
			r = new TreeNode(val);
			return r;
		}
		if (val < r->value) r->left = insert(r->left, val);
		else r->right = insert(r->right, val);
		if (val == r->value) return r;

	}

	TreeNode* findParent(TreeNode* r, const std::string& key)
	{
		if (!r) return nullptr;
		if (r->left->value == key || r->right->value == key)
		{
			return r;
		}
		findParent(r->left, key);
		findParent(r->right, key);
	}

	TreeNode* removeNode(TreeNode* root, std::string data) {
		if (root == nullptr) {
			return nullptr;
		}
		if (data < root->value) {  // data is in the left sub tree.
			root->left = removeNode(root->left, data);
		}
		else if (data > root->value) { // data is in the right sub tree.
			root->right = removeNode(root->right, data);
		}
		else {
			// case 1: no children
			if (root->left == nullptr && root->right == nullptr) {
				delete(root); // wipe out the memory, in C, use free function
				root = nullptr;
			}
			// case 2: one child (right)
			else if (root->left == nullptr) {
				TreeNode* temp = root; // save current node as a backup
				root = root->right;
				delete temp;
			}
			// case 3: one child (left)
			else if (root->right == nullptr) {
				TreeNode* temp = root; // save current node as a backup
				root = root->left;
				delete temp;
			}
			// case 4: two children
			else {
				TreeNode* temp = FindMin(root->right); // find minimal value of right sub tree
				root->value = temp->value; // duplicate the node
				root->right = removeNode(root->right, temp->value); // delete the duplicate node
			}
		}
		return root; // parent node can update reference
	}

	TreeNode* FindMin(TreeNode* root) {
		if (root == nullptr) {
			return nullptr; // or undefined.
		}
		if (root->left != NULL) {
			return FindMin(root->left); // left tree is smaller
		}
		return root;
	}

	bool remove(TreeNode*& r, const std::string& name)
	{
		if (r == nullptr) return false;
		TreeNode* toDelete = find(name);
		TreeNode* parent = findParent(r, name);

		int children = !toDelete->left + !toDelete->right;
		if (children == 0)
		{
			if (parent->left->value == name) parent->left = nullptr;
			else parent->right = nullptr;

			delete toDelete;
			return true;
		}

		else if (children == 1)
		{
			TreeNode* pos = parent->left->value == name ? parent->left : parent->right;
			TreeNode* par = findParent(r, name);

			toDelete->left ? pos = toDelete->left : pos = toDelete->right;
			delete toDelete;
			return true;
		}

		TreeNode* del;
		TreeNode* maxLeft = toDelete->left;
		while (maxLeft->right)
		{
			del = maxLeft;
			maxLeft = maxLeft->right;
		}

		maxLeft->right = nullptr;
		toDelete->value = maxLeft->value;
		delete maxLeft;

		return true;
	}
	TreeNode* copy(TreeNode* r, TreeNode* other)
	{
		if (!r) return nullptr;

		TreeNode* c = new TreeNode();

		c->left = copy(r->left, other);
		c->right = copy(r->right, other);

		return c;
	}

	void clear(TreeNode* r)
	{
		if (!r) return;

		clear(r->left);
		clear(r->right);

		delete r;
	}

	size_t getfullCount(TreeNode*& root)
	{
		if (root == NULL) {
			return 0;
		}
		else {
			return 1 + getfullCount(root->left) + getfullCount(root->right);
		}
	}

	void printLevel(TreeNode* r)
	{
		if (!r) return;

		std::queue<TreeNode*> q;

		q.push(r);

		TreeNode* p;

		while (!q.empty())
		{
			p = q.front();
			q.pop();

			std::cout << p->value << " ";

			if (p->left)
				q.push(p->left);
			if (p->right)
				q.push(p->right);
		}
	}

	void printLeafNodes(TreeNode* root)
	{
		if (!root)
			return;

		if (!root->left && !root->right)
		{
			std::cout << root->value << " ";
			return;
		}

		if (root->left)
			printLeafNodes(root->left);

		if (root->right)
			printLeafNodes(root->right);
	}

	TreeNode* insertNode(TreeNode*& t, std::string x)
	{
		if (t == NULL)
		{
			t = new TreeNode;
			t->value = x;
			t->left = t->right = NULL;
		}
		else if (x < t->value)
			t->left = insertNode(t->left, x);
		else if (x > t->value)
			t->right = insertNode(t->right, x);
		return t;
	}

public:
	Tree() : root(nullptr) {}
	Tree(const Tree& other)
	{
		copy(root, other.root);
	}
	Tree(TreeNode*& r)
	{
		root = r;
	}
	~Tree()
	{
		clear(root);
	}

	void printLevel()
	{
		return printLevel(root);
	}

	Tree& operator=(Tree& other)
	{
		copy(root, other.root);
		return *this;
	}

	TreeNode* getRoot()
	{
		return this->root;
	}

	TreeNode* find(const std::string& key) const
	{
		return find(root, key);
	}
	TreeNode* insert(std::string& addedNode)
	{
		return insert(root, addedNode);
		//return root;
	}
	TreeNode* insertNode(std::string& key)
	{
		return insertNode(root, key);
	}
	bool remove(const std::string& removedName)
	{
		return remove(root, removedName);
	}
	std::string serialize(TreeNode* root) {
		if (root == nullptr) {
			return "";
		}
		std::string s = root->value +
			"(" + serialize(root->left) + ")";
		if (root->right != nullptr) {
			s += "(" + serialize(root->right) + ")";
		}
		return s;
	}

	TreeNode* removeNode(std::string data)
	{
		return removeNode(root, data);
	}

	size_t getfullCount()
	{
		return getfullCount(root);
	}

	TreeNode* deserialize(std::string data) {
		std::string s = data;
		if (s.size() == 0) {
			return nullptr;
		}
		if (s[0] == ')') return nullptr;
		int j = 0;
		while (j < s.size() && s[j] != '(') j++;
		TreeNode* root = new TreeNode((s.substr(0, j)));
		int left = 0, i = j;
		// find separation between left and right definition
		while (i < s.size()) {
			if (s[i] == '(') {
				left++;
			}
			else if (s[i] == ')') {
				left--;
			}
			if (left == 0) {
				break;
			}
			i++;
		}
		if (j < s.size() - 1) {
			root->left = deserialize(s.substr(j + 1, i - 1 - j));
		}
		if (i + 1 < s.size() - 1) {
			root->right = deserialize(s.substr(i + 2, s.size() - i - 2));
		}
		return root;
	}

	void printLeafNodes()
	{
		std::cout << root->value << " ";
		printLeafNodes(root);
	}
};

#pragma endregion

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

void ToLowerCase(std::string& sen)
{
	for (size_t i = 0; i < sen.size(); i++)
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
	else std::cout << "Unable to open serialization tree file!";

	TreeNode* deserializedTree = helperTree.deserialize(serializatedTreeString);
	Tree tree(deserializedTree);

	do
	{
		std::string sen, command, originalSen;
		std::cout << "FMISql>";
		getline(std::cin, sen);
		originalSen = sen;
		ToLowerCase(sen);

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
			for (size_t i = 0; i < tableArguments.size(); i++)
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
