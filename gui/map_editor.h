#ifndef MAPEDITOR_H
#define MAPEDITOR_H

#include <QMainWindow>
#include <QMutexLocker>

#include "risk_map.h"
#include "observer.h"
#include "country_name_dialog.h"
#include "enum_tooltype.h"
#include "map_scene.h"

namespace Ui {
	class MapEditor;
}

class MapEditor : public QMainWindow, public Observer {
	Q_OBJECT
public:
	MapEditor(QWidget *parent = 0);
	~MapEditor();
	ToolMode getSelectedTool();

private slots:
	void on_filenameLineEdit_textChanged(QString text);
	void on_browsePushButton_clicked();
	void on_loadPushButton_clicked();
	void on_newPushButton_clicked();

	void on_saveMapPushButton_clicked();
	void on_addCountryPushButton_clicked();
	void on_removeCountryPushButton_clicked();
	void on_moveCountryPushButton_clicked();
	void on_addNeighbourPushButton_clicked();
	void on_removeNeighbourPushButton_clicked();

private:
	bool validateFilename(const QString& text);
	void observedUpdated();
	void connectNeighboursVisit(std::map<const std::string, bool>& visited, const Country* country);
	Ui::MapEditor* ui;
	MapScene* scene;
	RiskMap* observedMap;
	ToolMode tool;
	mutable QMutex mutex;
};
#endif // MAPEDITOR_H
