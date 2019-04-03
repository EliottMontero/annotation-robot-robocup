#pragma once

#ifndef CHOICECOMBOBOX_H
#define CHOICECOMBOBOX_H

#include <QWidget>
#include <QObject>
#include <QFormLayout>
#include <QGroupBox>
#include <QComboBox>

#include <annotateImage/team.h>
using namespace annotateImage;

class ChoiceComboBox : public QWidget
{
  Q_OBJECT

public :
  ChoiceComboBox();

  void setTitle(QString title);
  void setTeamMap(std::map<int,Team> & teams);
  void setCurrent(int team, int robot);

  int getNumberTeam();
  int getNumberRobot();

public slots :
  void chargeRobot(int i);

private :
  std::map<int,Team> teamMap;

  QGroupBox * groupBox;
  QComboBox * teamComboBox;
  QComboBox * robotComboBox;

};

#endif //CHOICECOMBOBOX_H
