#pragma once
#include "User.h"
#include "Administrator.h"
#include "SuperUser.h"

/*
������������ ���� �������, ����� �� ������������� � ���������� ��� (���������� �������� ���). ��������� ��������� ����������
���� ������������ � ��� ����������� � ���������� ���� ��������������. �� ���� �� ��� ��� �� ���� ���������������. ��� ����� ���, ��� � ���������.

����� �� �� �������� ���� �������, ����� �� ������������, �� ������������ �� ���� ������������ ������� �� ���� �� ���� ��������
��� ����� ���� ������������ �������.

������������ ������ �� �������� ������ ����� �� ���������, ����� � �� ������ �� �������. ������������ ���� �������� ���������:

�       �������� �� ��������, ����� �� ������ ���������� � ���������. ���� � ��������, ��� � ��������� ���� ���������� ��� ������ ���.

�       �������, � ����� �� ������ �������������. ��� �� ������ �� � ��� ���� ����������, ���� ������������� � ���� �������.

�       �������� �� ���������, ����� �������� ���������� ��� ������������� �� ���������. ������������ � ��������, 
��� ���������� ���������� � � ���������, �� �� � ���������� �������������.

�       �������� ^ , ����� �� �������� ����� � ��������� ���� �� �������������, ����� �������� ��� ���-������ �� 
����������������, ����� � � ���� ������ ���� �� �������������, ��� ��� �����, ��� nullptr, ��� ����.

�       ��������� �� ���� � ����� �� �����.
*/


class Container
{
public:
	Container(const char* name, size_t capacity = 8);
	Container(const Container& other);
	Container& operator=(const Container& other) = delete;
	Container(Container&& other)noexcept;
	Container& operator=(Container&& other)noexcept = delete;
	~Container()noexcept;

	inline const char* getName()const { return name; };
	inline const User* const* getUsers()const { return users; };
	inline size_t getSize()const { return size; };
	inline size_t getCapacity()const { return capacity; };
	inline bool getState()const { return hasSuperUser; };

	Container& operator+=(const User& other);
	Container operator+(const User& other)const ;

	Container& operator-=(const User& other);
	Container operator-(const User& other)const;
	
	User* operator^(Comp c) const;

private:
	char* name;
	User** users;
	size_t size;
	size_t capacity;

	void resize(size_t newCapacity);
	bool canAdd(const User& other)const;
	inline void changeState() { hasSuperUser = !hasSuperUser; }; // dk if it works
	bool hasSuperUser; // samo edin e!
};

