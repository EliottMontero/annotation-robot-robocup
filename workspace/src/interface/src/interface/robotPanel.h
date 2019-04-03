#pragma once

#ifndef ROBOTPANEL_H
#define ROBOTPANEL_H

#include <QWidget>
#include <QObject>
#include <QLayout>
#include <QLabel>

class RobotPanel : public QWidget
{
  Q_OBJECT

public:
  RobotPanel();

  void updateNumber(int i);
  void updateAnnotation(bool pos, bool dir, bool trace, bool ball, bool target,
                        bool teamTrace, int robotTrace,
                        bool teamBall, int robotBall,
                        bool teamTarget, int robotTarget);

private:
  int idRobot;

  QLabel * label_number;
  QLabel * label_position;
  QLabel * label_direction;
  QLabel * label_trace;
  QLabel * label_ball;
  QLabel * label_target;

  };

  #endif //RobotPanel_H
