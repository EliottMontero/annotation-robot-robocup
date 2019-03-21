#include "teamBoard.h"

TeamBoard::TeamBoard()
{
    layout_robot = new QVBoxLayout;
    QVBoxLayout * layout_head = new QVBoxLayout;

    label_TeamNumber = new QLabel();
    label_TeamNumber->setText("Team X");
    label_TeamNumber->setFont(QFont("Arial",24,3,false));
    label_TeamNumber->setAlignment(Qt::AlignCenter);

    label_score = new QLabel();
    label_score->setText("?");
    label_score->setFont(QFont("Arial",18,3,false));
    label_score->setAlignment(Qt::AlignCenter);

    line = new QFrame;
    line->setGeometry(QRect(320,150,170,5));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    scrollAreaRobot = new QScrollArea;
    scrollAreaRobot->setFixedSize(10,10);

    robotBox = new QGroupBox();
    robotBox->setTitle("Robots");

    QWidget * robotFrame = new QWidget;
    robotFrame->setLayout(layout_robot);

    layout_head->addWidget(label_TeamNumber);
    layout_head->addWidget(label_score);

    head_Widget = new QWidget;
    head_Widget->setLayout(layout_head);
}

void TeamBoard::setGridLayout(QGridLayout * layout_main, bool isLeft){
  layout = layout_main;
  if(isLeft){
    layout->addWidget(head_Widget,1,0,1,1);
    layout->addWidget(line,2,0,1,1);
    layout->addWidget(scrollAreaRobot,3,0,3,1);
  }else{
    layout->addWidget(head_Widget,1,5,1,1);
    layout->addWidget(line,2,5,1,1);
    layout->addWidget(scrollAreaRobot,3,5,3,1);
  }
}

void TeamBoard::setSizeRobotArea(){
  QRect cell = layout->cellRect(1,3);
  QRect cellLabel1 = layout->cellRect(1,2);
  if(cell.isValid()){
    int w = cell.width();
    int h = cellLabel1.height();
    scrollAreaRobot->setFixedSize(w,h*3);
  }
}

void TeamBoard::setTeamNumber(int number){
  label_TeamNumber->setText("Team " + QString::number(number));
  idTeam = number;
}


void TeamBoard::updateScore(int score){
  label_score->setText(QString::number(score));
}

void TeamBoard::updateAnnotation(bool pos, bool dir, bool trace, bool ball, bool target,
                                 int teamTrace, int robotTrace,
                                 int teamBall, int robotBall,
                                 int teamTarget, int robotTarget){
  bool boolTeamTrace = (teamTrace == idTeam);
  bool boolTeamBall = (teamBall == idTeam);
  bool boolTeamTarget = (teamTarget == idTeam);

  for(auto it : robotBoards){
    (it.second)->updateAnnotation(pos, dir, trace, ball, target,
                                  boolTeamTrace, robotTrace,
                                  boolTeamBall, robotBall,
                                  boolTeamTarget, robotTarget);
  }
}


void TeamBoard::addRobot(int new_robot){
    robotBoards[new_robot] = new RobotBoard;
    layout_robot->addWidget(robotBoards[new_robot]);
    robotBoards[new_robot]->updateNumber(new_robot);


    QWidget * robotFrame = new QWidget;
    robotFrame->setLayout(layout_robot);
    scrollAreaRobot->setWidget(robotFrame);
}
