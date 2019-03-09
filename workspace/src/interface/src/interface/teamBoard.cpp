#include "teamBoard.h"


TeamBoard::TeamBoard()
{
    QVBoxLayout * layout = new QVBoxLayout;
    QVBoxLayout * layout_robot = new QVBoxLayout;

    label_TeamNumber = new QLabel();
    label_TeamNumber->setText("Team X");
    label_TeamNumber->setFont(QFont("Arial",24,3,false));

    label_score = new QLabel();
    label_score->setText("?");
    label_score->setFont(QFont("Arial",18,3,false));
    label_score->setAlignment(Qt::AlignCenter);

    QFrame *line = new QFrame;
    line->setGeometry(QRect(320,150,170,5));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    robotBox = new QGroupBox();
    robotBox->setTitle("Robots");
    robotBox->setLayout(layout_robot);

    layout->addWidget(label_TeamNumber);
    layout->addWidget(label_score);
    layout->addWidget(line);
    layout->addWidget(robotBox);

    this->setLayout(layout);
}


void TeamBoard::setTeamNumber(int number){
  label_TeamNumber->setText("Team " + QString::number(number));
}


void TeamBoard::updateScore(int score){
  label_score->setText(QString::number(score));
}

void TeamBoard::updateAnnotation(int idRobotTrace, int idRobotBall){
  for(auto it : robotBoards){
    (it.second)->updateAnnotation(idRobotTrace, idRobotBall);
  }
}


void TeamBoard::addRobot(int new_robot){
    robotBoards[new_robot] = new RobotBoard;
    QLayout * layout = robotBox->layout();
    layout->addWidget(robotBoards[new_robot]);
    robotBoards[new_robot]->updateNumber(new_robot);
}
