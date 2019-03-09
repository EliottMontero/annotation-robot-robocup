#ifndef TeamBOARD_H
#define TeamBOARD_H

#include <QWidget>
#include <QObject>
#include <QLayout>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>

#include <string>
#include <fstream>

#include "robotBoard.h"


class TeamBoard : public QWidget
{
  Q_OBJECT

  public:
    TeamBoard();

    QGroupBox * robotBox;
    std::map<int, RobotBoard*> robotBoards;

    QLabel * label_TeamNumber;
    QLabel * label_score;

    void setTeamNumber(int number);
    void updateScore(int score);
    void updateAnnotation(int idRobotTrace, int idRobotBall);
    void addRobot(int new_robot);



  signals:

  public slots:



};

#endif //TeamBoard_H
