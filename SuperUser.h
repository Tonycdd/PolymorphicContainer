#pragma once
#include "Administrator.h"
/*
Реализирайте клас СуперПотребител, който е администратор, но може да създава потребители, чрез 
предефиниран функционален оператор с подходящи параметри. Трябва да съхранява потребителските имена на 
всички създадени от него потребители. Добавете оператор за индексиране, който получава като аргумент 
потребителско име, и проверява дали потребителят с 
това име е бил създаден от съответния СуперПотребител или не. Предефинирайте оператори за вход и изход в поток.
*/

class SuperUser : Administrator
{
public:
	SuperUser(const char* username, const char* password,
		unsigned int workExp, Comp c,size_t capacity = 8);
	
	// ne e ok da gi imame, tui kato iskame samo po edin jiv obekt v tozi moment ot tozi klas
	SuperUser(const SuperUser& other);
	SuperUser& operator=(const SuperUser& other);
	SuperUser(SuperUser&& other)noexcept;
	SuperUser& operator=(SuperUser&& other)noexcept;
	virtual ~SuperUser()noexcept;

	inline size_t getSize()const { return size; };
	inline const char* const* getListOfNames()const { return listOfNames; };
	inline bool isEmpty()const { return size == 0; };
	
	User* operator()(const char* username, const char* password); // creates new users
	
	const char* operator[](const char* name) const;
	char*& operator[](const char* name);

	virtual void serialize(std::ostream& os)const override;
	virtual void deserialize(std::istream& is)override;
	virtual User* clone()const override { return new SuperUser(*this); };

	friend std::ostream& operator<<(std::ostream& os,const SuperUser& other);
	friend std::istream& operator>>(std::istream& is, SuperUser& other);

protected:
	SuperUser();
	void resize(size_t newCapacity);

private:
	char** listOfNames;
	size_t size;
	size_t capacity;

	static unsigned int count;
};

