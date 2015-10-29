#ifndef COUNTRY_H
#define COUNTRY_H

#include <string>
#include <vector>
#include "continent.h"
#include "player.h"

class Continent; //forward declaration
class Player; //forward declaration

class Country {
private:
	std::string name;
	Continent* continent;
	Player* owner;
	int x, y, soldiers;
	std::vector<Country> neighbours = std::vector<Country>();

public:
    Country();
	Country(std::string name, Continent& continent, int x, int y);
	Country(const Country& country);
	~Country();
	std::string getName() const;
	void setName(std::string name);
	Continent getContinent() const;
	void setContinent(Continent& continent);
	Player* getOwner() const;
	void setOwner(Player* player);
	int getPositionX() const;
	void setPositionX(int x);
	int getPositionY() const;
	void setPositionY(int y);
	int getSoldiers() const;
	void setSoldiers(int soldiers);
    void removeSoldiers(int soldiers);
    void addSoldiers(int soldiers);
	std::vector<Country> getNeighbours() const;
	void addNeighbour(Country country);

};
#endif // COUNTRY_H