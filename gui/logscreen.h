#ifndef LOGSCREEN_H
#define LOGSCREEN_H
#include <QMainWindow>
#include <string>
#include "observer.h"
#include "game_modes.h"
#include "component.h"
#include "game_driver.h"
/**
 * The namespace used for linking with .ui file widget.
 */
namespace Ui{
	class LogScreen;
}
/**
 * @brief The LogScreen class the display of the LogScreen Observer
 *
 */
class LogScreen : public QMainWindow, public Observer

{
	Q_OBJECT

public:
	/**
	 * @brief LogScreen
	 * @param parent - the parent widget
	 */
	explicit LogScreen(QWidget *parent=0, GameDriver* driver=nullptr);

	~LogScreen();
	/**
	 * @brief setPlayers sets the number of players
	 * @param nPlayers
	 */
	void setPlayers(int nPlayers);
	/**
	 * @brief setState
	 * @param stateSelected
	 */
	void setState(std::string stateSelected);
	/**
	 * @brief getPlayers
	 * @return
	 */
	int getPlayers();
	/**
	 * @brief setCurrentMode
	 * @param mode
	 */

	void setCurrentMode (Mode mode);
	/**
	 * @brief getCurrentMode
	 * @return
	 */
	Mode getCurrentMode();
	/**
	 * @brief getState
	 * @return
	 */
	std::string getState();
	/**
	 * @brief update not the observer update but used as an update for initiaition of the class.
	 */
	void update();
	/**
	 * @brief observedUpdated
	 * called each time the observer class changes to update the view
	 * it checks if we are in the correct state and have the correct player and then it will then update the view accordingly
	 */
	virtual void observedUpdated();
private:
	Ui::LogScreen *ui=nullptr;
	int players=-1;
	std::string states;
	Mode currentMode;
	Component *component = nullptr;
	GameDriver* driver =nullptr;

};

#endif // LOGSCREEN_H