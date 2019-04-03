#pragma once

#ifndef CHOICEDIALOG_H
#define CHOICEDIALOG_H

#include <QDialog>
#include <QCheckBox>
#include <QDialogButtonBox>

#include "choiceComboBox.h"

class ChoiceDialog : public QDialog
{
  Q_OBJECT

public :
  ChoiceDialog();

  void setTeamMap(std::map<int,Team> & teams);
  void setInitGeneral(bool position, bool direction, bool trace,
                      bool ball, bool target);
  void setCurrentTrace(int team, int robot);
  void setCurrentBall(int team, int robot);
  void setCurrentTarget(int team, int robot);

  bool getGeneralPosition();
  bool getGeneralDirection();
  bool getGeneralTrace();
  bool getGeneralBall();
  bool getGeneralTarget();

  int getNumberRobotBall();
  int getNumberRobotTrace();
  int getNumberRobotTarget();
  int getNumberTeamTrace();
  int getNumberTeamBall();
  int getNumberTeamTarget();

private :
  std::map<int,Team> teamMap;

  QCheckBox * positionCheck;
  QCheckBox * directionCheck;
  QCheckBox * traceCheck;
  QCheckBox * ballCheck;
  QCheckBox * targetCheck;

  ChoiceComboBox * traceBox;
  ChoiceComboBox * ballBox;
  ChoiceComboBox * targetBox;

  };

  #endif //CHOICEDIALOG_H
