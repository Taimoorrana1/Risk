#include <iostream>

#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QDebug>
#include <QPushButton>
#include "risk.h"
#include "player.h"
#include "country.h"
#include "dices.h"
#include "continent.h"
#include "warreferee.h"
#include "score.h"
#include "button.h"
int main(int argc, char *argv[])
{

	QApplication application(argc, argv);

    QGraphicsScene* scene = new QGraphicsScene();
    QGraphicsView* view = new QGraphicsView(scene);

    Player taimoor("taimoor");
    Player rana("rana");
    Continent america("America", 10);
    Country canada("Canada", america,3,3);
    Country us("Us",america,3,4);
    taimoor.addCountry(canada);
    rana.addCountry(us);
    WarReferee warreferee;
    canada.addSoldiers(20);
    us.addSoldiers(20);

    Score* taimoorScore = new Score(0,canada.getSoldiers());
    scene->addItem(taimoorScore);
    Score* ranaScore =  new Score(0,us.getSoldiers());
    ranaScore->setPos(ranaScore->x(),ranaScore->y() +20);
    scene->addItem(ranaScore);


    Button* button = new Button("Roll Dices");
    button->move(0,40);

    QObject::connect(button,SIGNAL(clicked()),button,SLOT(pressed(taimoor,canada,rana,us)));
    scene->addWidget(button);
    //warreferee.startWar(taimoor,canada,rana,us);
//    std::cout << canada.getSoldiers() << "\n";
//    std::cout << us.getSoldiers() << "\n";

    view->show();

    return application.exec();
}
