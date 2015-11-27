#include "game_driver.h"

GameDriver* GameDriver::getInstance()
{
	static GameDriver* instance = nullptr;
	if (instance == nullptr) {
		instance = new GameDriver();
	}
	return instance;
}

void GameDriver::setCurrentPlayerName(const std::string& name) {
	this->currentPlayerName = name;
	this->notifyObservers();
}

std::string GameDriver::getCurrentPlayerName() const {
	return this->currentPlayerName;
}

Mode GameDriver::getCurrentMode() const {
	return this->currentMode;
}

void GameDriver::setCurrentMode(const Mode& mode) {
	this->currentMode = mode;
	this->notifyObservers();
}

void GameDriver::attackPhase() {}

void GameDriver::reinforcePhase() {}

void GameDriver::fortificationPhase()
{
	// click on country to add to set
	// click again to remove
	// if array is full pop up the fortification dialog
	// display names of selected countries
	//fortificationDialog = fortificationDialog(c1);
}

void GameDriver::calculateReinforcementArmies(Player *p)
{
	// std::set<std::string> continents =p->getContinentsOwned();
	// std::set<std::string>::iterator itContinents = continents.begin();
	// int reinforcementArmies=0;
	// reinforcementArmies = p->getCountriesOwned().size()/3;
	// while(itContinents!= continents.end()){
	// 	reinforcementArmies += riskMap->getContinent(*itContinents)->getReinforcementBonus();
	// }
	// p->setReinforcements(reinforcementArmies);
}
