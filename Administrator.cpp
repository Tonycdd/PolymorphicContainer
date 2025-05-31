#define _CRT_SECURE_NO_WARNINGS
#include "Administrator.h"
#include <stdexcept>
#include <exception>

const unsigned int MAX = 50;

Administrator::Administrator() : User(), workExperience(0), comp(Comp::ONE)
{
}

Administrator::Administrator(const char* username, const char* password, unsigned int workExp, Comp c)
	:User(username, password), workExperience(workExp), comp(c)
{	
}

void Administrator::increaseWorkExperience()
{
	if (workExperience + 1 > MAX) {
		// toi shte se e pensioniral veche
		return;
	}
	workExperience++;
}

bool Administrator::operator==(const Administrator& other) const
{
	return ((unsigned int)comp == (unsigned int)other.comp);
}

bool Administrator::operator!=(const Administrator& other) const
{
	return !(*this == other);
}

bool Administrator::operator<(const Administrator& other) const
{
	return (*this == other && other.workExperience > workExperience);
}

bool Administrator::operator<=(const Administrator& other) const
{
	return (*this == other || other.workExperience > workExperience);
}

bool Administrator::operator>(const Administrator& other) const
{
	return (*this == other && workExperience > other.workExperience);
}

bool Administrator::operator>=(const Administrator& other) const
{
	return (*this == other || workExperience > other.workExperience);
}

Administrator& Administrator::operator++()
{
	switch (comp)
	{
	case ONE:
		comp = Comp::TWO;
		return *this;
	case TWO:
		comp = Comp::THREE;
		return *this;
	case THREE:
		comp = Comp::FOUR;
		return *this;
	case FOUR:
		comp = Comp::FIVE;
		return *this;
	case FIVE:
		std::cerr << "Five is the biggest comp!";
		return *this;
	default:
		std::cerr << "Invalid comp!";
		return *this;
	}
}

Administrator Administrator::operator++(int dummt)
{
	Administrator cpy(*this);
	++(*this);
	return cpy;
}

Administrator& Administrator::operator--()
{
	switch (comp)
	{
	case ONE:
		std::cerr << "One is the smallest comp!";
		return *this;
	case TWO:
		comp = Comp::ONE;
		return *this;
	case THREE:
		comp = Comp::TWO;
		return *this;
	case FOUR:
		comp = Comp::THREE;
		return *this;
	case FIVE:
		comp = Comp::FOUR;
		return *this;
	default:
		std::cerr << "Invalid comp!";
		return *this;
	}
}

Administrator Administrator::operator--(int dummy)
{
	Administrator cpy(*this);
	--(*this);
	return cpy;
}

void Administrator::serialize(std::ostream& os) const
{
	User::serialize(os);
	if (!os.good()) {
		throw std::ios_base::failure("Error with ostream!");
	}

	os << "Work experience: " << workExperience << "\n";
	if (!os.good()) {
		throw std::ios_base::failure("Error with ostream!");
	}
	
	os << "Comp: " << (unsigned int)comp << "\n";
	if (!os.good()) {
		throw std::ios_base::failure("Error with ostream!");
	}
}

void Administrator::deserialize(std::istream& is)
{
	Administrator temp;

	try {
		// Десериализираме базовия клас
		temp.User::deserialize(is);
		if (!is.good()) {
			throw std::ios_base::failure("Error reading User data!");
		}

		char workLabel1[20], workLabel2[20]; // "Work" "experience:"
		char compLabel[20];                   // "Comp:"
		int workExp;
		unsigned int compValue;

		// Четем "Work experience: X"
		is >> workLabel1 >> workLabel2 >> workExp;
		if (!is.good() || strcmp(workLabel1, "Work") != 0 ||
			strcmp(workLabel2, "experience:") != 0) {
			throw std::ios_base::failure("Invalid work experience format!");
		}

		// Четем "Comp: X"
		is >> compLabel >> compValue;
		if (!is.good() || strcmp(compLabel, "Comp:") != 0) {
			throw std::ios_base::failure("Invalid comp format!");
		}

		// Валидация (ако е необходимо)
		if (workExp < 0) {
			throw std::invalid_argument("Work experience cannot be negative!");
		}
		temp.workExperience = workExp;
		temp.comp = static_cast<Comp>(compValue);
	}
	
	catch(...)
	{
		// strong exception
		//*this ne e promenen
		// temp se unishtojava
		throw;
	}
	*this = std::move(temp);
}



std::ostream& operator<<(std::ostream& os, const Administrator& other)
{
	other.serialize(os);
	return os;
}

std::istream& operator>>(std::istream& is, Administrator& other)
{
	other.deserialize(is);
	return is;
}
