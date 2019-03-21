#ifndef CHOICEDIALOG_H
#define CHOICEDIALOG_H

#include <QWidget>
#include <QObject>
#include <QDialog>
#include <QFormLayout>
#include <QCheckBox>
#include <QGroupBox>
#include <QButtonGroup>
#include <QComboBox>
#include <QDialogButtonBox>

#include <traitement/team.h>
#include <string>

using namespace traitement;

class ChoiceDialog : public QDialog
{
  Q_OBJECT

  public :
    ChoiceDialog();

    std::map<int,Team> teamMap;

    QCheckBox * positionCheck;
    QCheckBox * directionCheck;
    QCheckBox * traceCheck;
    QCheckBox * ballCheck;

    QComboBox * traceTeamComboBox;
    QComboBox * traceRobotComboBox;

    QComboBox * ballTeamComboBox;
    QComboBox * ballRobotComboBox;

    void setInitGeneral(bool position, bool direction, bool trace, bool ball);
    void setCurrentTrace(int team, int robot);
    void setCurrentBall(int team, int robot);
    void setTeamMap(std::map<int,Team> & teams);
    bool getGeneralPosition();
    bool getGeneralDirection();
    bool getGeneralTrace();
    bool getGeneralBall();
    int getNumberRobotBall();
    int getNumberRobotTrace();
    int getNumberTeamBall();
    int getNumberTeamTrace();

    signals:

    public slots :
    void chargeRobotTrace(int i);
    void chargeRobotBall(int i);

};

#endif //CHOICEDIALOG_H
