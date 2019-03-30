#ifndef CHOICEDIALOGCOMBOBOX_H
#define CHOICEDIALOGCOMBOBOX_H

#include <QWidget>
#include <QObject>
#include <QDialog>
#include <QFormLayout>
#include <QGroupBox>
#include <QButtonGroup>
#include <QComboBox>
#include <QDialogButtonBox>

#include <traitement/team.h>
#include <string>

using namespace traitement;

class ChoiceDialogComboBox : public QWidget
{
  Q_OBJECT

  public :
    ChoiceDialogComboBox();

    std::map<int,Team> teamMap;

    QGroupBox * groupBox;

    QComboBox * teamComboBox;
    QComboBox * robotComboBox;

    void setTitle(QString title);

    void setCurrent(int team, int robot);
    void setTeamMap(std::map<int,Team> & teams);

    int getNumberRobot();
    int getNumberTeam();

    signals:

    public slots :
    void chargeRobot(int i);

};

#endif //CHOICEDIALOGCOMBOBOX_H
