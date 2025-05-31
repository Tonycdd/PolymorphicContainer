#pragma once
#include "User.h"

/*
Реализирайте клас Администратор, който е потребител с допълнителни свойства като години стаж 
(цяло неотрицателно число) и ниво на компетентност (цяло число между 1 и 5).

Реализирайте необходимите методи за коректен жизнен цикъл, както и оператори за сравнение (наредба).
По-голям е администраторът с по-високо ниво на компетентност. В случай на равна компетентност, по-голям е този с повече години трудов стаж.

Реализирайте методи за работа с тези характеристики, както и оператори ++ и -- , които увеличават / 
намаляват компетентността на администратор с единица.

Предефинирайте оператори за вход и изход в поток.

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

