#ifndef TeamPANEL_H
#define TeamPANEL_H

#include <QScrollArea>

#include "robotPanel.h"


class TeamPanel : public QWidget
{
  Q_OBJECT

  public:
    TeamPanel();
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
    std::map<int, RobotPanel*> robotPanels;
    QVBoxLayout * layout_robot;
    QScrollArea * scrollAreaRobot;

};

#endif //TeamPanel_H
