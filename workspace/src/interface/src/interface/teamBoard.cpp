#include "teamBoard.h"


TeamBoard::TeamBoard()
{
    QVBoxLayout * layout = new QVBoxLayout;
    QVBoxLayout * layout_robot = new QVBoxLayout;

    label_TeamNumber = new QLabel();
    label_TeamNumber->setText("Team X");
    label_TeamNumber->setFont(QFont("Arial",24,3,false));

    label_score = new QLabel();
    label_score->setText("NA");
    label_score->setFont(QFont("Arial",18,3,false));
    label_score->setAlignment(Qt::AlignCenter);

    robotBox = new QGroupBox();
    robotBox->setTitle("Robots");
    robotBox->setLayout(layout_robot);

    layout->addWidget(label_TeamNumber);
    layout->addWidget(label_score);
    layout->addWidget(robotBox);

    this->setLayout(layout);
}


void TeamBoard::setTeamNumber(int number){
  label_TeamNumber->setText("Team " + QString::number(number));
}


void TeamBoard::updateScore(int score){
  label_score->setText(QString::number(score));
}


void TeamBoard::updateRobot(int new_robot){
  QLabel * label = new QLabel;
  label->setText("Robot n°" + QString::number(new_robot));
  QLayout * layout = robotBox->layout();
  layout->addWidget(label);

}
