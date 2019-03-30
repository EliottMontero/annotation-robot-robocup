#include "choiceDialogComboBox.h"

ChoiceDialogComboBox::ChoiceDialogComboBox()
{

  groupBox = new QGroupBox;

  QHBoxLayout * layoutBox = new QHBoxLayout;

  teamComboBox = new QComboBox;
  teamComboBox->setToolTip("Number Team");
  robotComboBox = new QComboBox;
  robotComboBox->setToolTip("Number Robot");

  layoutBox->addWidget(teamComboBox);
  layoutBox->addWidget(robotComboBox);
  groupBox->setLayout(layoutBox);


  /*** CONNECT ***/
  connect(teamComboBox, SIGNAL(currentIndexChanged(int)),this, SLOT(chargeRobot(int)));

  QHBoxLayout * layout = new QHBoxLayout;
  layout->addWidget(groupBox);
  this->setLayout(layout);
}

void
ChoiceDialogComboBox::setTitle(QString title) {
  groupBox->setTitle(title);
}

void
ChoiceDialogComboBox::setTeamMap(std::map<int,Team> & teams) {
  teamMap = teams;
  for(auto team : teamMap){
    teamComboBox->addItem(QString::number(team.first));
  }
}


void
ChoiceDialogComboBox::setCurrent(int team, int robot) {
  int numberTeam = teamComboBox->findText(QString::number(team));
  if(numberTeam != -1){
    teamComboBox->setCurrentIndex(numberTeam);
  }
  int numberRobot = robotComboBox->findText(QString::number(robot));
  if(numberRobot != -1){
    robotComboBox->setCurrentIndex(numberRobot);
  }
}

void
ChoiceDialogComboBox::chargeRobot(int i) {
  int numberTeam = (teamComboBox->itemText(i)).toInt();
  std::map<int, RobotInformation> robots = (teamMap[numberTeam]).getRobotMap();
  robotComboBox->clear();
  for(auto robot : robots){
      robotComboBox->addItem(QString::number(robot.first));
  }
}

int
ChoiceDialogComboBox::getNumberTeam() {
  return (teamComboBox->currentText()).toInt();
}

int
ChoiceDialogComboBox::getNumberRobot() {
  return (robotComboBox->currentText()).toInt();
}
