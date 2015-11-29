#include "logscreen.h"
#include "ui_logscreen.h"
#include <string>
#include <QString>
#include <QDebug>
#include "game_driver.h"
#include "game_modes.h"
#include "component.h"
#include "decorator.h"
#include "player_log.h"
#include "concrete_attack.h"
#include "concrete_fortify.h"
#include "concrete_reinforce.h"
using namespace std;
LogScreen::LogScreen(QWidget *parent, GameDriver *driver): QMainWindow(parent),ui(new Ui::LogScreen)
{
	this->driver = driver;
	ui->setupUi(this);
	//update();

}

LogScreen::~LogScreen()
{
	driver->detachObserver(this);
	delete ui;
}

void LogScreen::setPlayers(int nPlayers)
{
	players = nPlayers;
}

void LogScreen::setState(std::string stateSelected)
{
	states = stateSelected;
}

void LogScreen::setCurrentMode(Mode mode)
{
	currentMode = mode;
}

void LogScreen::update()
{
	ui->textEdit->insertPlainText("Hello Good day your logging has begun please select go to next phase to see logging effets\n");
	ui->textEdit->insertPlainText(QString("Players :%1").arg(players));
	qDebug("%d",players);
}

void LogScreen::observedUpdated()
{
	ui->textEdit->clear();
	string compareP = "Player "+ to_string(players);
	if(component ==nullptr){
		component = new PlayerLog(driver->getCurrentPlayerName());

		//component-
	}
	else{

	}
	qDebug("%s",compareP);

	if(driver->getCurrentPlayerName()== compareP || players==0){
		if(states=="All Phases"){

			if ( driver->getCurrentMode()==REINFORCEMENT){
				component = new ConcreteReinforce(component);
				ui->textEdit->insertPlainText(QString::fromStdString(component->getLog()));
			}
			else if (driver->getCurrentMode()==ATTACK){
				component = new ConcreteAttack(component);
				ui->textEdit->insertPlainText(QString::fromStdString(component->getLog()));
			}
			else if (driver->getCurrentMode()==FORTIFICATION){
				component = new ConcreteFortify (component);
				ui->textEdit->insertPlainText(QString::fromStdString(component->getLog()));

			}

		}
		else if(states=="Fortify" && driver->getCurrentMode()==FORTIFICATION){
			component = new ConcreteFortify(component);
			ui->textEdit->insertPlainText(QString::fromStdString(component->getLog()));
		}
		else if(states=="Attack" && driver->getCurrentMode()==ATTACK){
			component = new ConcreteAttack(component);
			ui->textEdit->insertPlainText(QString::fromStdString(component->getLog()));
		}
		else if(states=="Reinforce" && driver->getCurrentMode()==REINFORCEMENT){
			component = new ConcreteReinforce(component);
			ui->textEdit->insertPlainText(QString::fromStdString(component->getLog()));
		}
		//ui->textEdit->insertPlainText(QString::fromStdString(GameDriver::getInstance()->getCurrentPlayerName()));


	}
	//need to add states and clear screen
	//so that we have decorator for the three states.
	//playerLog.setName(), going to have to do garbage collection or end of round delete the player
	//and make a new one
	if(driver->getCurrentMode()==FORTIFICATION){
		component = nullptr;
		delete component;
	}



}
