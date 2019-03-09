#include "teamBoard.h"

TeamBoard::TeamBoard()
{
    //QVBoxLayout * layout = new QVBoxLayout;
    QVBoxLayout * layout_robot = new QVBoxLayout;
    QVBoxLayout * layout_head = new QVBoxLayout;


    label_TeamNumber = new QLabel();
    label_TeamNumber->setText("Team X");
    label_TeamNumber->setFont(QFont("Arial",24,3,false));

    label_score = new QLabel();
    label_score->setText("?");
    label_score->setFont(QFont("Arial",18,3,false));
    label_score->setAlignment(Qt::AlignCenter);

    line = new QFrame;
    line->setGeometry(QRect(320,150,170,5));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    robotBox = new QGroupBox();
    robotBox->setTitle("Robots");
    robotBox->setLayout(layout_robot);

    layout_head->addWidget(label_TeamNumber);
    layout_head->addWidget(label_score);

    head_Widget = new QWidget;
    head_Widget->setLayout(layout_head);

}

void TeamBoard::setGridLayout(QGridLayout * layout, bool isLeft){
  if(isLeft){
    layout->addWidget(head_Widget,1,0,1,1);
    layout->addWidget(line,2,0,1,1);
    layout->addWidget(robotBox,3,0,1,1);
  }else{
    layout->addWidget(head_Widget,1,5,1,1);
    layout->addWidget(line,2,5,1,1);
    layout->addWidget(robotBox,3,5,1,1);
  }

  //this->setLayout(layout);
}

void TeamBoard::setTeamNumber(int number){
  label_TeamNumber->setText("Team " + QString::number(number));
}


void TeamBoard::updateScore(int score){
  label_score->setText(QString::number(score));
}

void TeamBoard::updateAnnotation(bool pos, bool dir, int idRobotTrace, int idRobotBall){
  for(auto it : robotBoards){
    (it.second)->updateAnnotation(pos, dir, idRobotTrace, idRobotBall);
  }
}


void TeamBoard::addRobot(int new_robot){
    robotBoards[new_robot] = new RobotBoard;
    QLayout * layout = robotBox->layout();
    layout->addWidget(robotBoards[new_robot]);
    robotBoards[new_robot]->updateNumber(new_robot);
}
