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
  targetCheck  = new QCheckBox((QString)"Target");

  layoutGeneralBox->addWidget(positionCheck);
  layoutGeneralBox->addWidget(directionCheck);
  layoutGeneralBox->addWidget(traceCheck);
  layoutGeneralBox->addWidget(ballCheck);
  layoutGeneralBox->addWidget(targetCheck);

  generalBox->setLayout(layoutGeneralBox);


  QFrame * line = new QFrame;
  line->setGeometry(QRect(320,150,170,5));
  line->setFrameShape(QFrame::VLine);
  line->setFrameShadow(QFrame::Sunken);

  /***COMBOBOX***/
  traceBox = new ChoiceDialogComboBox();
  traceBox->setTitle(QString("Trace"));
  ballBox = new ChoiceDialogComboBox();
  ballBox->setTitle(QString("Ball"));
  targetBox = new ChoiceDialogComboBox();
  targetBox->setTitle(QString("Target"));


  QDialogButtonBox * buttonDialog =
    new QDialogButtonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);


  layout->addWidget(generalBox,0,0,3,1);
  layout->addWidget(line,0,1,3,1);
  layout->addWidget(traceBox,0,3,1,1);
  layout->addWidget(ballBox,1,3,1,1);
  layout->addWidget(targetBox,2,3,1,1);
  layout->addWidget(buttonDialog,3,3,1,1);

  this->setLayout(layout);


  /*** CONNECT ***/
  connect(buttonDialog, SIGNAL(accepted()),this, SLOT(accept()));
  connect(buttonDialog, SIGNAL(rejected()),this, SLOT(reject()));
}

void
ChoiceDialog::setTeamMap(std::map<int,Team> & teams) {
  traceBox->setTeamMap(teams);
  ballBox->setTeamMap(teams);
  targetBox->setTeamMap(teams);
}

void
ChoiceDialog::setInitGeneral(bool position, bool direction, bool trace, bool ball, bool target){
  positionCheck->setChecked(position);
  directionCheck->setChecked(direction);
  traceCheck->setChecked(trace);
  ballCheck->setChecked(ball);
  targetCheck->setChecked(target);
}

void
ChoiceDialog::setCurrentTrace(int team, int robot) {
  traceBox->setCurrent(team,robot);
}

void
ChoiceDialog::setCurrentBall(int team, int robot) {
  ballBox->setCurrent(team,robot);
}

void
ChoiceDialog::setCurrentTarget(int team, int robot) {
  targetBox->setCurrent(team,robot);
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

bool
ChoiceDialog::getGeneralTarget(){
  return targetCheck->isChecked();
}

int
ChoiceDialog::getNumberRobotTrace() {

  return traceBox->getNumberRobot();
}

int
ChoiceDialog::getNumberTeamTrace() {

  return traceBox->getNumberTeam();
}

int
ChoiceDialog::getNumberRobotBall() {
  return ballBox->getNumberRobot();
}

int
ChoiceDialog::getNumberTeamBall() {
  return ballBox->getNumberTeam();
}

int
ChoiceDialog::getNumberRobotTarget() {
  return targetBox->getNumberRobot();
}

int
ChoiceDialog::getNumberTeamTarget() {
  return targetBox->getNumberTeam();
}
