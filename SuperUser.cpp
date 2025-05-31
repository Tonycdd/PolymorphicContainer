#define _CRT_SECURE_NO_WARNINGS
#include "SuperUser.h"
#include <stdexcept>
#include <exception>

unsigned int SuperUser::count = 0;

SuperUser::SuperUser(const char* username, const char* password, unsigned int workExp, Comp c,size_t capacity)
	:Administrator(username,password,workExp,c),size(0),capacity(capacity)
{
	if (count < 1) {
		listOfNames = new char* [capacity] {};
		++count;
	}
	else {
		throw std::runtime_error("Cannot make more than one!");
	}
}

SuperUser::SuperUser(const SuperUser& other) : size(0),capacity(other.capacity)
{
	listOfNames = new char* [capacity];
	for (size_t i = 0; i < capacity; i++)
	{
		listOfNames[i] = nullptr;
	}

	// opitvame se da si napravim nashi kopiq sega
	try
	{
		for (size_t i = 0; i < other.size; i++)
		{
			listOfNames[i] = new char[strlen(other.listOfNames[i]) + 1];
			strcpy(listOfNames[i], other.listOfNames[i]);
			size++;
		}
	}
	catch (...)
	{
		for (size_t i = 0; i < size; i++)
		{
			delete[] listOfNames[i];
		}
		delete[] listOfNames;
		throw;
	}
}

SuperUser& SuperUser::operator=(const SuperUser& other)
{
	if (this != &other) {
		SuperUser cpy(other); // can throw
		*this = std::move(cpy);
	}
	return *this;
}

SuperUser::SuperUser(SuperUser&& other) noexcept
	:listOfNames(nullptr), size(other.size), capacity(other.capacity)
{
	std::swap(listOfNames, other.listOfNames);
}

SuperUser& SuperUser::operator=(SuperUser&& other) noexcept
{
	if (this != &other) {
		std::swap(listOfNames, other.listOfNames);
		std::swap(size, other.size);
		std::swap(capacity, other.capacity);
	}
	return *this;
}

SuperUser::~SuperUser() noexcept
{
	for (size_t i = 0; i < size; i++)
	{
		delete[] listOfNames[i];
	}
	delete[]listOfNames;
}

SuperUser::SuperUser()
	:Administrator(),size(0),capacity(4)
{
	listOfNames = new char*[capacity];
	for (size_t i = 0; i < capacity; i++)
	{
		listOfNames[i] = nullptr;
	}
}

User* SuperUser::operator()(const char* username, const char* password)
{
	//suzdava useri i dobavq texniqt username i password
	//tursim purvo dali veche e bilo suzdadeno
	
	try
	{
		char* name = this->operator[](username);
		// moje da xvurli no e ok
		// inache e namereno imeto 
		return nullptr; // veche e bil suzdaden!!!!

	}
	catch (const std::runtime_error& e)
	{
		// znachi ne e namereno imeto
		// trqbva da se opitame da suzdaem nov user
		User* newUser = nullptr;
		newUser = new User(username, password); // moje i da ne uspee
		// ako ne uspee e ok	
		//inache e usqql
		// trqbva da dobavim imeto my
		try
		{
			if (size + 1 >= capacity) {
				resize(capacity * 2);
			}
			//ako tuk tova new xvurli newUser trqbva da go iztriem
			listOfNames[size] = new char[strlen(username) + 1];
			strcpy(listOfNames[size], username);
			size++;
			return newUser;
		}
		catch (...)
		{
			delete newUser;
			throw;
		}
	}
}

const char* SuperUser::operator[](const char* name) const
{
	if (name == nullptr) {
		throw std::invalid_argument("Invalid name- cannot be nullptr!");
	}

	for (size_t i = 0; i < size; i++)
	{
		if (strcmp(listOfNames[i], name) == 0) {
			return listOfNames[i];
		}
	}
	throw std::runtime_error("Not found!");
}

char*& SuperUser::operator[](const char* name)
{
	if (name == 0)
	{
		throw std::invalid_argument("Invalid name- cannot be nullptr!");
	}

	for (size_t i = 0; i < size; i++)
	{
		if (strcmp(listOfNames[i], name) == 0) {
			return listOfNames[i];
		}
	}
	throw std::runtime_error("Not found!");
}

void SuperUser::serialize(std::ostream& os) const
{
	Administrator::serialize(os);
	if (!os.good()) {
		throw std::ios_base::failure("Erorr with ostream!");
	}
	os << "Size: " << size << "\n";
	if (!os.good()) {
		throw std::ios_base::failure("Erorr with ostream!");
	}
	for (size_t i = 0; i < size; i++)
	{
		os << listOfNames[i] << "\n";
		if (!os.good()) {
			throw std::ios_base::failure("Erorr with ostream!");
		}
	}
}

void SuperUser::deserialize(std::istream& is)
{
	// tuk imame samo edin jiv SuperUser- ne mojem da imame poveche ot nego.. toest ne stava strong exception s ideqta za temp i std::move
	char** oldNames = listOfNames;
	size_t oldSize = size;
	size_t oldCap = capacity;

	listOfNames = nullptr;
	size = 0;
	capacity = 0;

	try
	{
		Administrator::deserialize(is);
		char buff[20]; // size: 
		size_t newSize;
		is >> buff >> newSize;
		if (!is.good()) {
			throw std::ios_base::failure("Error with istream!");
		}

		is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		if (newSize > 0) {
			listOfNames = new char* [newSize] {};
			capacity = newSize;
			
			char buffer[1024];
			
			for (size_t i = 0; i < newSize; i++)
			{
				is.getline(buffer, 1024);
				if (!is.good()) {
					throw std::ios_base::failure("Error with istream!");
				}

				listOfNames[i] = new char[strlen(buffer) + 1];
				strcpy(listOfNames[i], buffer);
				size++;
			}
		}

		for (size_t i = 0; i < oldSize; i++)
		{
			delete[]oldNames[i];
		}delete[]oldNames;

	}
	catch (...)
	{
		for (size_t i = 0; i < size; i++)
		{
			delete[] listOfNames[i];
		}
		delete[]listOfNames;

		listOfNames = oldNames;
		size = oldSize;
		capacity = oldCap;
		throw;
	}
}

void SuperUser::resize(size_t newCapacity)
{
	size_t cap = newCapacity > 0? newCapacity : 1;
	if(cap <= capacity)
	{
		return;
	}
	char** temp = new char* [cap](); // ako xvurli e ok
	for (size_t i = 0; i < size; i++)
	{
		temp[i] = listOfNames[i]; // size < capacity sus sigurnost , ne pravim izlishni kopiq samo prenasochvame 
	}
	delete[]this->listOfNames;
	listOfNames = temp;
	capacity = cap;
}

std::ostream& operator<<(std::ostream& os, const SuperUser& other)
{
	other.serialize(os);
	return os;
}

std::istream& operator>>(std::istream& is, SuperUser& other)
{
	other.deserialize(is);
	return is;
}
