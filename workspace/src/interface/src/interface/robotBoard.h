#ifndef ROBOTBOARD_H
#define ROBOTBOARD_H

#include <QWidget>
#include <QObject>
#include <QLayout>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>

#include <string>
#include <fstream>


class RobotBoard : public QWidget
{
  Q_OBJECT

  public:
    RobotBoard();
    int idRobot;

    QLabel * label_number;
    QLabel * label_position;
    QLabel * label_direction;
    QLabel * label_trace;
    QLabel * label_ball;

    void updateNumber(int i);
    void updateAnnotation(bool pos, bool dir,int idRobotTrace, int idRobotBall);

  signals:

  public slots:



};

#endif //RobotBoard_H
