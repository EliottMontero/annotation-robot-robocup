#ifndef TeamBOARD_H
#define TeamBOARD_H

#include <QScrollArea>

#include "robotBoard.h"


class TeamBoard : public QWidget
{
  Q_OBJECT

  public:
    TeamBoard();
    void setGridLayout(QGridLayout * layout_main, bool isLeft);
    void setTeamNumber(int number);
    void addRobot(int new_robot);
    void setSizeRobotArea();
    void updateScore(int score);
    void updateAnnotation(bool pos, bool dir, bool trace, bool ball, bool target,
                          int teamTrace, int robotTrace,
                          int teamBall, int robotBall,
                          int teamTarget, int robotTarget);

  private :
    int idTeam;

    QLabel * label_TeamNumber;
    QLabel * label_score;
    QGridLayout * layout;
    QWidget * head_Widget;
    QFrame * line;

    QGroupBox * robotBox;
    std::map<int, RobotBoard*> robotBoards;
    QVBoxLayout * layout_robot;
    QScrollArea * scrollAreaRobot;

};

#endif //TeamBoard_H
