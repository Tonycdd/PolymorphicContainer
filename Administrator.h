#pragma once
#include "User.h"

/*
������������ ���� �������������, ����� � ���������� � ������������ �������� ���� ������ ���� 
(���� ������������� �����) � ���� �� ������������� (���� ����� ����� 1 � 5).

������������ ������������ ������ �� �������� ������ �����, ����� � ��������� �� ��������� (�������).
��-����� � ��������������� � ��-������ ���� �� �������������. � ������ �� ����� �������������, ��-����� � ���� � ������ ������ ������ ����.

������������ ������ �� ������ � ���� ��������������, ����� � ��������� ++ � -- , ����� ���������� / 
��������� ��������������� �� ������������� � �������.

�������������� ��������� �� ���� � ����� � �����.

*/

enum Comp {
	ONE = 1,
	TWO = 2,
	THREE = 3,
	FOUR = 4,
	FIVE = 5
};

class Administrator : public User
{
public:
	Administrator(const char* username, const char* password, unsigned int workExp, Comp c);
	Administrator(const Administrator& other) = default;
	Administrator& operator=(const Administrator& other) = default;
	Administrator(Administrator&& other)noexcept = default;
	Administrator& operator=(Administrator&& other)noexcept = default;
	virtual ~Administrator()noexcept = default;

	inline unsigned int getWorkExperience()const { return workExperience; };
	inline Comp getCompetity()const { return comp; };

	void increaseWorkExperience(); // ++
	
	bool operator==(const Administrator& other)const;
	bool operator!=(const Administrator& other)const;
	bool operator<(const Administrator& other)const;
	bool operator<=(const Administrator& other)const;
	bool operator>(const Administrator& other)const;
	bool operator>=(const Administrator& other)const;
	
	Administrator& operator++();
	Administrator operator++(int dummt);

	Administrator& operator--();
	Administrator operator--(int dummy);


	virtual void serialize(std::ostream& os)const override;
	virtual void deserialize(std::istream& is)override;
	virtual User* clone()const override { return new Administrator(*this); };

	friend std::ostream& operator<<(std::ostream& os, const Administrator& other);
	friend std::istream& operator>>(std::istream& is, Administrator& other);

protected:
	Administrator();

private:
	unsigned int workExperience;
	Comp comp;
};

