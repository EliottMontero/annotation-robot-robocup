#include "robotPanel.h"

RobotPanel::RobotPanel()
{
  QLayout * layout = new QVBoxLayout;

  label_number = new QLabel;

  label_position = new QLabel;
  label_position->setText("   POS : OFF");

  label_direction = new QLabel;
  label_direction->setText("   DIR : OFF");

  label_trace = new QLabel;
  label_trace->setText("   TRACE : OFF");

  label_ball = new QLabel;
  label_ball->setText("   BALL : OFF");

  label_target = new QLabel;
  label_target->setText("   TARGET : OFF");


  layout->addWidget(label_number);
  layout->addWidget(label_position);
  layout->addWidget(label_direction);
  layout->addWidget(label_trace);
  layout->addWidget(label_ball);
  layout->addWidget(label_target);

  this->setLayout(layout);

}

void RobotPanel::updateNumber(int i){
  label_number->setText("Robot n°" + QString::number(i));
  idRobot = i;
}

void RobotPanel::updateAnnotation(bool pos, bool dir, bool trace,bool ball, bool target,
                      bool teamTrace, int robotTrace,
                      bool teamBall, int robotBall,
                      bool teamTarget, int robotTarget){

  if(pos){
    label_position->setText("   POS : ON");
  }else {
    label_position->setText("   POS :OFF");
  }

  if(dir){
    label_direction->setText("   DIR : ON");
  }else{
    label_direction->setText("   DIR : OFF");
  }

  if(trace && teamTrace && (idRobot == robotTrace)){
    label_trace->setText("   TRACE : ON");
  }else {
    label_trace->setText("   TRACE : OFF");
  }

  if(ball && teamBall && (idRobot == robotBall)){
    label_ball->setText("   BALL : ON");
  }else {
    label_ball->setText("   BALL : OFF");
  }

  if(target && teamTarget && (idRobot == robotTarget)){
    label_target->setText("   TARGET : ON");
  }else {
    label_target->setText("   TARGET : OFF");
  }
}
