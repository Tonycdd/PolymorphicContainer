#include "Container.h"

Container::Container(const char* name, size_t capacity)
	:name(nullptr), size(0), capacity(capacity), hasSuperUser(false)
{
	if (name == nullptr) {
		throw std::invalid_argument("Error");
	}
	else if (name[0] == '\0') {
		throw std::invalid_argument("Name cannot be empty!");
	}
	this->name = new char[strlen(name) + 1]; // ako xvurli ne e problem
	strcpy(this->name, name);

	try
	{
		users = new User * [capacity];
		// tova e ok, davame validno sustoqnie
		for (size_t i = 0; i < capacity; i++)
		{
			users[i] = nullptr;
		}

	}
	catch (...)
	{
		delete[]this->name;
		throw;
	}
}

Container::Container(const Container& other)
	:name(nullptr),users(nullptr), size(0),capacity(other.capacity),hasSuperUser(false)
{
	// ako xvurli ne e problem
	name = new char[strlen(other.name) + 1];
	strcpy(name, other.name);
	
	try
	{
		users = new User * [capacity];
		for (size_t i = 0; i < capacity; i++)
		{
			users[i] = nullptr;
		}

		for (size_t i = 0; i < other.size; i++)
		{
			//tuk trqbva da vidim dali na nqkoe mqsto userut,koito shte dobavim ne e sluchaino SuperUser
			// seshtam se samo za nachina s dynamicCast
			
			// tova shte stane vednuj samo zashtoto other e validen i ima superUser samo vednuj
			if (hasSuperUser == false) {
				const SuperUser* a = dynamic_cast<const SuperUser*>(other.users[i]);
				if (a != nullptr) {
					changeState();
				}
			}

			users[i] = other.users[i]->clone();
			size++;
		}
	}
	catch (...)
	{	
		// triem uspehsno zadelenite
		for (size_t i = 0; i < size; i++)
		{
			delete users[i];
		}
		//triem si dvata masiva
		delete[] users;
		delete[] name;
	}
}

Container::Container(Container&& other) noexcept
	:name(nullptr), users(nullptr), size(0), capacity(0), hasSuperUser(false)
{
	std::swap(name, other.name);
	std::swap(users, other.users);
	std::swap(size, other.size);
	std::swap(capacity, other.capacity);
	std::swap(hasSuperUser, other.hasSuperUser);
}

Container::~Container() noexcept
{
	for (size_t i = 0; i < size; i++)
	{
		delete users[i];
	}
	delete[]users;
}

Container& Container::operator+=(const User& other)
{
	// trqbva da vidim dali mojem da go dobavim
	if (!canAdd(other)) {
		// or throw exception

		std::cerr << "Cannot add this user!" << "\n";
		return *this;
	}

	// inache mojem da go dobavim
	if (size + 1 >= capacity) {
		resize(capacity * 2); // capacity nikoga ne e 0 , taka che tova e ok operaciq osven ako nqma mqsto
	}
	try
	{
		users[size] = other.clone(); // this can throw, nothing we can do here
		size++;
		return *this;
	}
	catch (...)
	{
		//nqmame leak, no prosto ne iskame tozi operator da xvurlq i zatova poglushtame greshkata
		// tova ne e mnogo dobra praktika, no v sluchaq raboti
		std::cerr << "Something went wrong with clone function!" << "\n";
		return *this;
	}
}

Container Container::operator+(const User& other)const
{
	// za tozi ne sum mnogo siguren dali e taka??

	// pravim kopie na nashiq
	Container cpy(*this);
	cpy += other; // preizpozlvame operatora +=
	return cpy; // rvo, no ne razchitame, shte se kopira nanovo potencialno -> tova e bavna operaciq, mojem da vrushtame const &, 
	// no ne e dobra praktika
}

Container& Container::operator-=(const User& other)
{
	// dali mojem da go maxnem??
	const SuperUser* a = dynamic_cast<const SuperUser*>(&other);
	if (a != nullptr) {
		std::cerr << "Cannot remove superUser!";
		return *this;
	}

	// znachi ne e super user, togava proverqvame dali izosbhto e v sistemata
	size_t countOfAdmins = 0;
	int indexOfUserToRemove = -1;
	bool isInSystem = false;

	for (size_t i = 0; i < size; i++)
	{
		const Administrator* admin = dynamic_cast<const Administrator*>(users[i]);
		if (admin != nullptr) {
			countOfAdmins++;
		}

		if (!isInSystem && strcmp(users[i]->getUsername(), other.getUsername()) == 0) {
			// togava e v sistemata ni
			indexOfUserToRemove = i;
			isInSystem = true;
		}
	}

	if (indexOfUserToRemove < 0) {
		std::cerr << "Not found!" << "\n";
		return *this;
	}

	// ako e administrator sluchaino da ne e posledniqt
	if ((dynamic_cast<Administrator*>(users[indexOfUserToRemove]) && countOfAdmins <= 1)){
		std::cerr << "Cannot remove last Admin!" << '\n';
			return *this;
	}
	std::swap(users[indexOfUserToRemove], users[size - 1]);
	delete users[size - 1];
	users[size - 1] = nullptr;
	size--;

	return *this;
}

Container Container::operator-(const User& other) const
{
	// tuk pak ne znam dali iskame tochno tova, zatova mojem da si ostavim samo -=
	Container cpy(*this);
	cpy -= other;
	return cpy;
}

User* Container::operator^(Comp c) const
{
	for (size_t i = 0; i < size; i++)
	{
		Administrator* admin = dynamic_cast<Administrator*>(users[i]);
		if (admin != nullptr) {
			// znachi e admin
			if ((unsigned int)admin->getCompetity() >= (unsigned int) c) {
				// togava toi e purviqt i go vrushtame
				return admin;
			}
		}
	}
	return nullptr;
}

void Container::resize(size_t newCapacity)
{
	if (newCapacity <= capacity) {
		return;
	}
	User** tmp = new User * [newCapacity];
	
	capacity = newCapacity; // pravim go otdoly,zashtoto new moje da xvurli

	for (size_t i = 0; i < capacity; i++)
	{
		tmp[i] = nullptr;
	}

	// sega mestim ukazatelite
	for (size_t i = 0; i < size; i++)
	{
		tmp[i] = users[i];
	}
	delete[] users; // triem samo masiva ot ukazatelite, bez samite ukazateli
	users = tmp;
}

bool Container::canAdd(const User& other) const
{
	for (size_t i = 0; i < size; i++)
	{
		if (strcmp(users[i]->getUsername(), other.getUsername()) == 0) {
			return false;
		}
	}

	// inache trqbva da vidim dali e superUser
	const SuperUser* a = dynamic_cast<const SuperUser*>(&other);
	if (hasSuperUser && a != nullptr) { return false; };
	
	return true;
}
