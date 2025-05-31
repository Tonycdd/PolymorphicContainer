#pragma once
#include "User.h"
#include "Administrator.h"
#include "SuperUser.h"

/*
Реализирайте клас Система, която се характеризира с определено име (произволен символен низ). Системата притежава произволен
брой регистрирани в нея потребители и произволен брой администратори. Тя може да има или да няма СуперПотребител. Ако такъв има, той е единствен.

Могат да се създават нови системи, копия на съществуващи, но информацията от вече съществуваща система не може да бъде копирана
във друга вече съществуваща система.

Реализирайте методи за коректен жизнен цикъл на системата, както и за достъп до данните. Реализирайте също следните оператори:

·       Оператор за събиране, който да добавя потребител в системата. Това е възможно, ако в системата няма потребител със същото име.

·       Функция, с която се добавя администратор. Той не трябва да е бил нито потребител, нито администратор в тази система.

·       Оператор за изваждане, който премахва потребител или администратор от системата. Премахването е възможно, 
ако поисканият потребител е в системата, но не е последният администратор.

·       Оператор ^ , който по подадено число – минимално ниво на компетентност, връща указател към най-малкия от 
администраторите, който е с поне такова ниво на компетентност, ако има такъв, или nullptr, ако няма.

·       Оператори за вход и изход от поток.
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

