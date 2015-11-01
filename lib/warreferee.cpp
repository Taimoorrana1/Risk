#include "warreferee.h"
#include "dices.h"
#include <iostream>

WarReferee::WarReferee()
{
    dices = new Dices(MAX_DICES);
}

void WarReferee::setPlayers(Player& attacker1, Player& defender1){
//    this->defender = &defender1;
//    this->attacker = &attacker1;
}

/**
 * Calculates the loses each country occurs after the dice roll- if dices are specified
 */
void WarReferee::calculateLosses(Country& attackerCountry, int attackerDices, Country& defenderCountry, int defenderDices)
{
//    this->attackerDices = attackerDices;
//    this->defenderDices = defenderDices;
//    calculateLossesHelper(attackerCountry, attackerDices, defenderCountry, defenderDices);
}


/**
 * Calculates the loses each country occurs after the dice roll - if dices are not specified
 */
void WarReferee::calculateLosses(Country& attackerCountry, Country& defenderCountry){
    // Determine amount of dices for the attacker depending on the army size
    int attackerSoldiers = attackerCountry.getArmies();
    int defenderSoldiers = defenderCountry.getArmies();

    if (attackerSoldiers > 3){
        attackerDices = 3;
    }else if(attackerSoldiers == 3){
        attackerDices = 2;
    }else if(attackerSoldiers == 2){
        attackerDices = 1;
    }else{
        std::cout << "error in calculateLosses Method\n";
    }

    if(defenderSoldiers >= 2){
        defenderDices = 2;
    }else if(defenderSoldiers == 1){
        defenderDices = 1;
    }else{
        std::cout << "error in calculateLosses Method\n";
    }
    
    calculateLossesHelper(attackerCountry, attackerDices, defenderCountry, defenderDices);
}



/**
 * Calculates the loses each country occurs after the dice roll - HELPER
 */
void WarReferee::calculateLossesHelper(Country& attackerCountry, int attackerDices, Country& defenderCountry, int defenderDices){
//     dices rolled by both players
    std::vector<int> attackerResults = dices->roll(attackerDices);
    std::vector<int> defenderResults = dices->roll(defenderDices);
    
    int attackerLosses = 0;
    int defenderLosses = 0;
    
//    //Compare roll results
//    //Attacker's highest dice is compared to Defender's highest dice
    for(int x = 0; x < defenderDices; x++){
        if(attackerResults[x] > defenderResults[x]){
            defenderLosses++;
        }else{
            attackerLosses++;
        }
    }
    

    
//    // Remove soldiers lost from the battle
    if (attackerCountry.getArmies() > attackerLosses) {
        attackerCountry.setArmies(attackerCountry.getArmies()-attackerLosses);
    }else{
        std::cout << "Error: Could not remove soldiers from attackerCountry\n";
    }
    if (defenderCountry.getArmies() >= defenderLosses) {
        defenderCountry.setArmies(defenderCountry.getArmies()-defenderLosses);
    }else{
        std::cout << "Error: Could not remove soldiers from defenderCountry\n";
    }
}





void WarReferee::allInMode(Country& attackerCountry, Country& defenderCountry){
    
    while (attackerCountry.getArmies() > 1 && attackerCountry.getPlayer() != defenderCountry.getPlayer()) {
        calculateLosses(attackerCountry,defenderCountry);
        if (defenderCountry.getArmies() <= 0) {
            attacker->addCountry(defenderCountry.getNameCountry());
            defenderCountry.setPlayer(attacker->getName());
            //defenderPlayer->removeCountry(defender);
        }
    }
    std::cout << attackerCountry.getArmies() << std::endl;
    
    if (attackerCountry.getArmies() <= 1 ) {
        std::cout << "Attacker cannot attack anymore\n";
    }else{
        std::cout << "Attacker won and conquered\n";
        attacker->transferSoldiers(attackerCountry, defenderCountry, attackerDices);
    }
}






void WarReferee::startWar(Player& attacker,Country& attackerCountry, Player& defender, Country& defenderCountry)
{
    setPlayers(attacker, defender);
    allInMode(attackerCountry,defenderCountry);
}

WarReferee::~WarReferee(){
    delete dices;
}

WarReferee& WarReferee::getInstance()
{
    static WarReferee* instance = nullptr;
    if(instance == nullptr){
        instance = new WarReferee();
    }
    return *instance;
}

