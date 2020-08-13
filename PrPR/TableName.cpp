#include "TableName.h"

TableName::TableName() : name(), value(), size() {}
TableName::TableName(std::string name, std::string value)
{
	this->name = name;
	this->value = value;
	this->size = 0;
}
TableName::TableName(std::string name, std::string value, int s)
{
	this->name = name;
	this->value = value;
	size = 0;
}

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