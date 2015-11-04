#include "country.h"
#include "player.h"
#include <algorithm>

Player::Player(std::string name)
{
	this->name = name;
}


void Player::addCountry(const std::string& countryName)
{
	namesOfCountriesOwned.insert(countryName);
	// map.getCountry(countryName)->setPlayer(name);
}


void Player::removeCountry(const std::string& countryName)
{
	std::set<std::string>::iterator countryOwnedIterator;
	int index = 0;
	for(countryOwnedIterator = namesOfCountriesOwned.begin(); countryOwnedIterator != namesOfCountriesOwned.end();countryOwnedIterator++){
		index++;
		if(*countryOwnedIterator == countryName){
			namesOfCountriesOwned.erase(countryOwnedIterator);
			break;
		}
	}
	// map.getCountry(countryName)->setPlayer("");
}

bool Player::hasCountry(const std::string& countryName) const
{
	 std::set<std::string>::iterator countryOwnedIterator;
	 for(countryOwnedIterator = namesOfCountriesOwned.begin(); countryOwnedIterator != namesOfCountriesOwned.end();countryOwnedIterator++){
		if(*countryOwnedIterator == countryName){
			return true;
		}
	}
	return false;
}

bool Player::hasContinent(const std::string& continentName) const
{
	//std::set<std::string> countryList = map.getCountriesInContinent(continentName);
	std::set<std::string>::iterator countryListIterator;

	std::set<std::string>::iterator countriesOwnedIterator;
	int matchFound=0;

	// for (countryListIterator = countryList.begin(); countryListIterator != countryList.end(); countryListIterator++) {
	// 	for (countriesOwnedIterator = namesOfCountriesOwned.begin(); countriesOwnedIterator != namesOfCountriesOwned.end(); countriesOwnedIterator++) {
	// 		if (countryListIterator == countriesOwnedIterator ) {
	// 			matchFound++;
	// 		}
	// 	}
	// }

	// if (matchFound == countryList.size()) {
	// 	return true;
	// }

	return false;
}

std::set<std::string> Player::getContinentsOwned() const
{
	return this->namesOfContinentsOwned;
}

std::string Player::getName() const
{
	return name;
}

void Player::addReinforcements(const int& amount)
{
	reinforcements += amount;
}

void Player::removeReinforcements(const int& amount)
{
	if(reinforcements - amount >= 0 ){
		reinforcements -= amount;
	}
}

void Player::setReinforcements(const int& amount)
{
	reinforcements = amount;
}

int Player::getReinforcements() const
{
	return this->reinforcements;
}


void Player::registerObserver(PlayerObserver *observer)
{
	observerList.insert(observer);
}

void Player::unregisterObserver(PlayerObserver *observer)
{
   //observerList.erase(std::remove(observerList.begin(), observerList.end(), observer), observerList.end());

}

void Player::notifyObserver()
{
	getTotalArmy();
	for(std::set<PlayerObserver*>::iterator observerListIterator = observerList.begin(); observerListIterator != observerList.end(); observerListIterator++){
		(*observerListIterator)->update(namesOfCountriesOwned,namesOfContinentsOwned,reinforcements,totalArmy,battleWon);
	}
}

void Player::setTotalArmy(const int &totalArmy)
{
	this->totalArmy = totalArmy;
}

int Player::getTotalArmy() const
{
	return totalArmy;
}

std::set<std::string> Player::getCountryOwned(){
	std::set<std::string> copy(namesOfCountriesOwned);
	return copy;
}

void Player::increaseBattleWon(){
	battleWon++;
}

void Player::decreaseBattleWon()
{
	battleWon--;
}

void Player::setBattlesWon(const int &battleWon)
{
	this->battleWon = battleWon;
}

int Player::getBattlesWon() const
{
	return battleWon;
}



