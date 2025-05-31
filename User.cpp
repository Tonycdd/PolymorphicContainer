#define _CRT_SECURE_NO_WARNINGS
#include "User.h"
#include <stdexcept>
#include <exception>

User::User() : username(nullptr), password(nullptr)
{
}

User::User(const char* username, const char* passowrd)
	:User()
{
	if (!User::validateUsername(username) || !User::validatePassoword(password)) {
		throw std::invalid_argument("Invalid user or pass!");
	}

	// inache sa valindi
	this->username = new char[strlen(username) + 1];
	
	this->password = new(std::nothrow) char[strlen(password) + 1];
	if (!password) {
		delete[]this->username;
		throw std::bad_alloc();
	}
	strcpy(this->username, username);
	strcpy(this->password, passowrd);


}

User::User(const User& other)
{
	username = new char[strlen(other.username) + 1];
	password = new(std::nothrow) char[strlen(other.password) + 1];
	if (!password) {
		delete[]username;
		throw std::bad_alloc();
	}
	strcpy(username, other.username);
	strcpy(password, other.password);
}

User& User::operator=(const User& other)
{
	if (this != &other) {
		User temp(other);
		*this = std::move(temp);
	}
	return *this;
}

User::User(User&& other) noexcept
	:username(username), password(nullptr)
{
	std::swap(username, other.username);
	std::swap(password, other.password);
}

User& User::operator=(User&& other) noexcept
{
	if (this != &other) {
		std::swap(username, other.username);
		std::swap(password, other.password);
	}
	return *this;
}

User::~User() noexcept
{
	delete[]username;
	delete[]password;
}

void User::getPassword(std::ostream& os) const
{
	for (size_t i = 0; password[i] != '\0' ; i++)
	{
		os << "*";
	}
	os << '\n';
}

bool User::changePassword(const char* newPassword)
{
	if (!validatePassoword(newPassword)) {
		return false;
	}
	char* tempPass = new(std::nothrow) char[strlen(newPassword) + 1]; 
	if (!tempPass) {
		return false;
	}
	strcpy(tempPass, newPassword);
	delete[]password;
	password = tempPass;
	return true;
}

bool User::validatePassoword(const char* password)
{
	if (!password) {
		return false;
	}
	else if (password[0] == '\0') {
		return false;
	}
	return true;
}

bool User::validateUsername(const char* name)
{
	if (!name) {
		return false;
	}
	else if (name[0] == '\0') {
		return false;
	}
	size_t counter = 0;
	for (size_t i = 0; name[i] != '\0'; i++)
	{
		if (name[i] == '@') {
			counter++;
		}
	}
	if (counter != 1) {
		return false;
	}
	return true;
}

bool User::operator==(const User& other) const
{
	return (strcmp(username, other.username) == 0);
}

bool User::operator!=(const User& other) const
{
	return !(*this == other);
}

void User::serialize(std::ostream& os) const
{
	if (!os.good()) {
		throw std::ios_base::failure("Error with ostream!");
	}

	// inache zapisvame neshtata na usera
	os << "Username: " << username << "\n";
	if (!os.good()) {
		throw std::ios_base::failure("Error with ostream!");
	}
	os << "Password: ";
	if (!os.good()) {
		throw std::ios_base::failure("Error with ostream!");
	}
	getPassword(os);
	if (!os.good()) {
		throw std::ios_base::failure("Error with ostream!");
	}
}

void User::deserialize(std::istream& is)
{
	char usernameLabel[20];
	char passwordLabel[20];
	char actualUsername[512];
	char actualPassword[512];

	// Четем "Username:" и после самия username
	is >> usernameLabel >> actualUsername;
	if (!is.good()) {
		throw std::ios_base::failure("Error reading username!");
	}

	// Проверяваме дали лейбъла е правилен
	if (strcmp(usernameLabel, "Username:") != 0) {
		throw std::ios_base::failure("Invalid username format!");
	}

	// Четем "Password:" и после самата password
	is >> passwordLabel >> actualPassword;
	if (!is.good()) {
		throw std::ios_base::failure("Error reading password!");
	}

	// Проверяваме дали лейбъла е правилен
	if (strcmp(passwordLabel, "Password:") != 0) {
		throw std::ios_base::failure("Invalid password format!");
	}

	// Strong exception safety
	User temp(actualUsername, actualPassword);
	*this = std::move(temp);
}


std::ostream& operator<<(std::ostream& os, const User& other)
{
	other.serialize(os);
	return os;
}

std::istream& operator>>(std::istream& is, User& other)
{
	other.deserialize(is);
	return is;
}
