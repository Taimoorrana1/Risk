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
    map.getCountryObj(countryName)->setPlayer(name);
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
    map.getCountryObj(countryName)->setPlayer("");
}

bool Player::hasCountry(const std::string& countryName)
{
     std::set<std::string>::iterator countryOwnedIterator;
     for(countryOwnedIterator = namesOfCountriesOwned.begin(); countryOwnedIterator != namesOfCountriesOwned.end();countryOwnedIterator++){
        if(*countryOwnedIterator == countryName){
            return true;
        }
    }
    return false;
}

bool Player::hasContinent(const std::string& continentName)
{
    std::set<std::string> countryList = map.listCountriesInsideContinent(continentName);
    std::set<std::string>::iterator countryListIterator;
    
    std::set<std::string>::iterator countriesOwnedIterator;
    int matchFound=0;
    
    for (countryListIterator = countryList.begin(); countryListIterator != countryList.end(); countryListIterator++) {
        for (countriesOwnedIterator = namesOfCountriesOwned.begin(); countriesOwnedIterator != namesOfCountriesOwned.end(); countriesOwnedIterator++) {
            if (countryListIterator == countriesOwnedIterator ) {
                matchFound++;
            }
        }
    }
    
    if (matchFound == countryList.size()) {
        return true;
    }
    
    return false;
}

std::string Player::getName(){
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

void Player::transferSoldiers(std::string& countryFromName, std::string& countryToName,const int& soldiers){
    Country* countryfrom = map.getCountryObj(countryFromName);
    Country* countryTo = map.getCountryObj(countryToName);
    
    if (countryfrom->getPlayer() == this->name && countryTo->getPlayer() == this->name) {
        countryfrom->removeArmies(soldiers);
        countryTo->addArmies(soldiers);
    }
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
    for(std::set<PlayerObserver*>::iterator observerListIterator = observerList.begin(); observerListIterator != observerList.end(); observerListIterator++){
        (*observerListIterator)->update(namesOfCountriesOwned,namesOfContinentsOwned,reinforcements,totalArmy,battleWon);
    }
}

std::set<std::string> Player::getCountryOwned(){
    std::set<std::string> copy(namesOfCountriesOwned);
    return copy;
}

void Player::increaseBattleWon(){
    battleWon++;
}



