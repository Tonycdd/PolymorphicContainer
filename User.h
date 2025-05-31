#pragma once
#include <iostream>

/*
Реализирайте клас Потребител, който има следните характеристики:

email - символен низ с произволна дължина, който съдържа точно един символ @ и играе роля на потребителско име;
Парола - непразен символен низ с произволна дължина.
Реализирайте методи за коректен жизнен цикъл, както и достъп до данните. Не се очаква промяна на потребителското име.

Предефинирайте оператори за вход и изход в поток и сравнение за равенство. Два потребителя са равни, ако имат еднакви имейли.
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

