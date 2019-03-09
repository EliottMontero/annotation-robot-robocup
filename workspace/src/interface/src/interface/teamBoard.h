#ifndef TeamBOARD_H
#define TeamBOARD_H

#include <QWidget>
#include <QObject>
#include <QLayout>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include <QScrollArea>

#include <string>
#include <fstream>

#include "robotBoard.h"


class TeamBoard : public QWidget
{
  Q_OBJECT

  public:
    TeamBoard();
    void setGridLayout(QGridLayout * layout_main, bool isLeft);
    void setSizeRobotArea();

    QGroupBox * robotBox;
    std::map<int, RobotBoard*> robotBoards;

    QScrollArea * scrollAreaRobot;
    QVBoxLayout * layout_robot;

    QGridLayout * layout;


    QLabel * label_TeamNumber;
    QLabel * label_score;
    QWidget * head_Widget;
    QFrame * line;

    void setTeamNumber(int number);
    void updateScore(int score);
    void updateAnnotation(bool pos, bool dir, int idRobotTrace, int idRobotBall);
    void addRobot(int new_robot);



  signals:

  public slots:



};

#endif //TeamBoard_H
