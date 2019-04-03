#pragma once

#ifndef TEAMPANEL_H
#define TEAMPANEL_H

#include <QScrollArea>
#include <QGroupBox>

#include "robotPanel.h"

class TeamPanel : public QWidget
{
  Q_OBJECT

public:
  TeamPanel();

  void setGridLayout(QGridLayout * layout_main, bool isLeft);
  void setSizeRobotArea();
  void setTeamNumber(int number);

  void addRobot(int new_robot);
  void updateScore(int score);
  void updateAnnotation(bool pos, bool dir, bool trace, bool ball, bool target,
                        int teamTrace, int robotTrace,
                        int teamBall, int robotBall,
                        int teamTarget, int robotTarget);

private :
  int idTeam;

  QGridLayout * layout;
  QWidget * head_Widget;
  QLabel * label_TeamNumber;
  QLabel * label_score;
  QFrame * line;

  std::map<int, RobotPanel*> robotPanels;
  QScrollArea * scrollAreaRobot;
  QVBoxLayout * layout_robot;
  QGroupBox * robotBox;

};

#endif //TEAMPANEL_H
