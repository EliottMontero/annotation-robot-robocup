#include "robotBoard.h"

RobotBoard::RobotBoard()
{
  QLayout * layout = new QVBoxLayout;
  label_number = new QLabel;

  label_position = new QLabel;
  label_position->setText("POS : OFF");

  label_direction = new QLabel;
  label_direction->setText("DIR : OFF");

  label_trace = new QLabel;
  label_trace->setText("TRACE : OFF");

  label_ball = new QLabel;
  label_ball->setText("Ball : OFF");


  layout->addWidget(label_number);
  layout->addWidget(label_position);
  layout->addWidget(label_direction);
  layout->addWidget(label_trace);
  layout->addWidget(label_ball);

  this->setLayout(layout);

}

void RobotBoard::updateNumber(int i){
  label_number->setText("Robot n°" + QString::number(i));
  idRobot = i;
}

void RobotBoard::updateAnnotation(int idRobotTrace, int idRobotBall){
  if(idRobot == idRobotTrace){
    label_trace->setText("TRACE : ON");
  }else {
    label_trace->setText("TRACE : OFF");
  }

  if(idRobot == idRobotBall){
    label_ball->setText("BALL : ON");
  }else {
    label_ball->setText("BALL : OFF");
  }
}
