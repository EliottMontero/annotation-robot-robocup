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

#endif //RobotBoard_H
