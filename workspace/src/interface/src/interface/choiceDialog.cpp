#include "choiceDialog.h"

ChoiceDialog::ChoiceDialog()
{
  QGridLayout * layout = new QGridLayout;

  //***GENERAL BOX***
  QGroupBox * generalBox = new QGroupBox;
  generalBox->setTitle("Général");
  QVBoxLayout * layoutGeneralBox = new QVBoxLayout;

  positionCheck = new QCheckBox((QString)"Position");
  directionCheck = new QCheckBox((QString)"Direction");
  traceCheck = new QCheckBox((QString)"Trace");
  ballCheck = new QCheckBox((QString)"Ball");

  layoutGeneralBox->addWidget(positionCheck);
  layoutGeneralBox->addWidget(directionCheck);
  layoutGeneralBox->addWidget(traceCheck);
  layoutGeneralBox->addWidget(ballCheck);

  generalBox->setLayout(layoutGeneralBox);


  QFrame * line = new QFrame;
  line->setGeometry(QRect(320,150,170,5));
  line->setFrameShape(QFrame::VLine);
  line->setFrameShadow(QFrame::Sunken);

  //***TRACE COMBOBOX***
  QGroupBox *traceBox = new QGroupBox;
  traceBox->setTitle("Trace");
  QHBoxLayout * layoutTraceBox = new QHBoxLayout;

  traceTeamComboBox = new QComboBox;

  traceRobotComboBox = new QComboBox;
  traceRobotComboBox->addItem((QString)"Robot 1");
  traceRobotComboBox->addItem((QString)"Robot 2");

  layoutTraceBox->addWidget(traceTeamComboBox);
  layoutTraceBox->addWidget(traceRobotComboBox);
  traceBox->setLayout(layoutTraceBox);

  //***BALL COMBOBOX***
  QGroupBox *ballBox = new QGroupBox;
  ballBox->setTitle("Ball");
  QHBoxLayout * layoutBallBox = new QHBoxLayout;

  ballTeamComboBox = new QComboBox;

  ballRobotComboBox = new QComboBox;
  ballRobotComboBox->addItem((QString)"Robot 1");
  ballRobotComboBox->addItem((QString)"Robot 2");

  layoutBallBox->addWidget(ballTeamComboBox);
  layoutBallBox->addWidget(ballRobotComboBox);
  ballBox->setLayout(layoutBallBox);




  QDialogButtonBox * buttonDialog =
    new QDialogButtonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);


  layout->addWidget(generalBox,0,0,3,1);
  layout->addWidget(line,0,1,3,1);
  layout->addWidget(traceBox,0,3,1,1);
  layout->addWidget(ballBox,1,3,1,1);
  layout->addWidget(buttonDialog,2,3,1,1);

  this->setLayout(layout);


  /*** CONNECT ***/
  connect(buttonDialog, SIGNAL(accepted()),this, SLOT(accept()));
  connect(buttonDialog, SIGNAL(rejected()),this, SLOT(reject()));

  connect(traceTeamComboBox, SIGNAL(currentIndexChanged(int)),this, SLOT(chargeRobotTrace(int)));
  connect(ballTeamComboBox, SIGNAL(currentIndexChanged(int)),this, SLOT(chargeRobotBall(int)));

}

void
ChoiceDialog::setTeamMap(std::map<int,Team> & teams) {
  teamMap = teams;
  for(auto team : teamMap){
    traceTeamComboBox->addItem(QString::number(team.first));
    ballTeamComboBox->addItem(QString::number(team.first));
  }
}

void
ChoiceDialog::setInitGeneral(bool position, bool direction, bool trace, bool ball){
  positionCheck->setChecked(position);
  directionCheck->setChecked(direction);
  traceCheck->setChecked(trace);
  ballCheck->setChecked(ball);
}

bool
ChoiceDialog::getGeneralPosition(){
  return positionCheck->isChecked();
}

bool
ChoiceDialog::getGeneralDirection(){
  return directionCheck->isChecked();
}

bool
ChoiceDialog::getGeneralTrace(){
  return traceCheck->isChecked();
}

bool
ChoiceDialog::getGeneralBall(){
  return ballCheck->isChecked();
}

void
ChoiceDialog::chargeRobotTrace(int i) {
  int numberTeam = (traceTeamComboBox->itemText(i)).toInt();
  std::map<int, RobotInformation> robots = (teamMap[numberTeam]).getRobotMap();
  traceRobotComboBox->clear();
  for(auto robot : robots){
      traceRobotComboBox->addItem(QString::number(robot.first));
  }
}

void
ChoiceDialog::chargeRobotBall(int i) {
  int numberTeam = (ballTeamComboBox->itemText(i)).toInt();
  std::map<int, RobotInformation> robots = (teamMap[numberTeam]).getRobotMap();
  ballRobotComboBox->clear();
  for(auto robot : robots){
      ballRobotComboBox->addItem(QString::number(robot.first));
  }
}

int
ChoiceDialog::getNumberRobotTrace() {

  return (traceRobotComboBox->currentText()).toInt();
}

int
ChoiceDialog::getNumberTeamTrace() {

  return (traceTeamComboBox->currentText()).toInt();
}

int
ChoiceDialog::getNumberRobotBall() {
  return (ballRobotComboBox->currentText()).toInt();
}

int
ChoiceDialog::getNumberTeamBall() {
  return (ballTeamComboBox->currentText()).toInt();
}
