#include "List.h"


CellInfo::CellInfo()
{
	isString = true;
	string = "No name";
	value = NULL;
}
CellInfo::CellInfo(std::string str)
{
	isString = true;
	string = str;
	value = NULL;
}
CellInfo::CellInfo(int val)
{
	isString = false;
	value = val;
	string = "";
}

CellInfo& CellInfo::operator=(CellInfo& other)
{
	isString = other.isString;
	string = other.string;
	value = other.value;

	return *this;
}

bool CellInfo::isEqual(CellInfo& other)
{
	if ((this->isString == other.isString) && (this->string == other.string) && (this->value == other.value))return true;
	return false;
}

bool CellInfo::isGreather(CellInfo& other)
{
	if ((this->string > other.string) && (this->value < other.value)) return true;
	return false;
}

bool CellInfo::isSmaller(CellInfo& other)
{
	if ((this->string < other.string) && (this->value > other.value)) return true;
	return false;
}

bool CellInfo::operator==(CellInfo& other)
{
	if ((this->isString == other.isString) && (this->string == other.string) && (this->value == other.value))return true;
	return false;
}

node::node()
{
	std::vector<CellInfo> dataNode;
	this->data = dataNode;
	this->next = nullptr;
}

node::node(std::vector<CellInfo> x)
{
	data = x;
}

node& node::operator=(node& other)
{
	this->next = other.next;
	this->data = other.data;
	return*this;
}

bool node::operator==(node& other)
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

bool node::operator!=(node& other)
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

bool node::operator>(node& other)
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

bool node::operator<(node& other)
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

List::List() { first = last = nullptr; }

List::List(const List& ll)
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

List::~List()
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

bool List::isEmpty() const
{
	if (sizeList == 0) return true;
	return false;
}

List List::getAllTableArgs(std::string& originalSen, std::vector<TableName>& deserializedVector)
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

void List::insertFront(std::vector<CellInfo> x)
{
	node* nodeToInsert = new node(x);
	//nodeToInsert->data = x;
	nodeToInsert->next = first;
	first = nodeToInsert;
	sizeList++;
}

List& List::operator=(const List& ll)
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

List List::orderBy(int pos)
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

void List::insertLast(std::vector<CellInfo> x)
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

List List::distinctRows(List& other)
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

bool List::existFisrtNode()
{
	if (this->first) return true;
	else return false;
}

bool List::existNode(node* other)
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

List List::equalStatment(std::string value, int tablePossition)
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

List List::notEqualStatment(std::string value, int tablePossition)
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

List List::greatherStatment(std::string value, int tablePossition)
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

List List::smallerStatment(std::string value, int tablePossition)
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

List List::removeAll(List& other)
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

void List::printNodes(std::vector<int> possitions, std::vector<std::string>& params)
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


List& List::concatList(List& other)
{
	List thisList = *this;
	node* otherNext = other.first;

	this->pushList(other);

	return *this;
}

List& List::concatListNotEqual(List& other)
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

List& List::concatListSmallerThen(List& other)
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

List& List::concatListGreatherThen(List& other)
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

bool List::removeFirst()
{
	if (isEmpty()) return false;
	node* firstNode = first;
	first = first->next;
	firstNode->next = nullptr;
	delete firstNode;
	sizeList--;
	return true;
}

int List::getSize() const
{
	return sizeList;
}

node* List::find(std::vector<CellInfo> key)
{
	node* it = first;
	while (it && !isFirstDataAndKeyEqual(it->data, key)) it = it->next;

	return it;
}

node* List::find(std::string firstEl)
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

std::vector<node*> List::findAllOccurances(std::string keyStr, int possition)
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

node* List::findAnyOccurance(std::string keyStr, int possition)
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

List List::pushList(List& other)
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

bool List::remove(std::vector<CellInfo> key)
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

bool List::isFirstDataAndKeyEqual(std::vector<CellInfo> first, std::vector<CellInfo> key)
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

node* List::firstEl()
{
	return first;
}

bool List::remove(node* it)
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