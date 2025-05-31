#pragma once
#include <iostream>

/*
������������ ���� ����������, ����� ��� �������� ��������������:

email - �������� ��� � ���������� �������, ����� ������� ����� ���� ������ @ � ����� ���� �� ������������� ���;
������ - �������� �������� ��� � ���������� �������.
������������ ������ �� �������� ������ �����, ����� � ������ �� �������. �� �� ������ ������� �� ��������������� ���.

�������������� ��������� �� ���� � ����� � ����� � ��������� �� ���������. ��� ����������� �� �����, ��� ���� ������� ������.
*/

class User
{
public:

	User(const char* username,
		const char* passowrd);
	User(const User& other);
	User& operator=(const User& other);
	User(User&& other) noexcept;
	User& operator=(User&& other)noexcept;
	virtual ~User() noexcept;

	inline const char* getUsername() const { return username; };
	void getPassword(std::ostream& os = std::cout)const; 

	bool changePassword(const char* newPassword);
	// username does not change

	static bool validatePassoword(const char* password);
	static bool validateUsername(const char* name);

	bool operator==(const User& other)const;
	bool operator!=(const User& other)const;

	virtual void serialize(std::ostream& os)const;
	virtual void deserialize(std::istream& is);
	virtual User* clone()const { return new User(*this); };

	friend std::ostream& operator<<(std::ostream& os, const User& other);
	friend std::istream& operator>>(std::istream& is, User& other);
protected: 
	User();
private:
	char* username;
	char* password;

};

