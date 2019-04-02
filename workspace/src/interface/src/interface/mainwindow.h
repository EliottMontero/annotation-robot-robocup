#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QSlider>

#include "opencv2/core.hpp"
#include <opencv/cv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>

#include <traitement/annotation.h>
#include <traitement/position.h>
#include <traitement/direction.h>
#include <traitement/team.h>

#include "teamPanel.h"
#include "choiceDialog.h"

#include <hl_communication/message_manager.h>
#include <hl_monitoring/monitoring_manager.h>
#include <hl_communication/utils.h>
#include <hl_monitoring/utils.h>
#include <hl_monitoring/field.h>

#include <tclap/CmdLine.h>
#include <sstream>


using namespace cv;
using namespace hl_communication;
using namespace hl_monitoring;
using namespace traitement;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    void createTeam(int id);
    void createRobot(int robotId, int teamId);

    public slots:
    void sliderControl(int i);
    void robotChoice();
    void changeImage();
    void togglePause();
    void toggleFastForward();
    void togglePosition();
    void toggleDirection();
    void toggleTrace();
    void toggleBall();
    void toggleTarget();

private :
    MonitoringManager manager;
    Annotation *annotation;
    Field field;

    QLabel * labelVideo;
    cv::Mat * gamePicture;

    QTimer * timer;
    uint64_t now;
    uint64_t initialTime;
    uint64_t endTime;
    uint64_t dt;

    std::map<int, Team> teams;
    std::map<int, TeamPanel*> teamPanels;

    QPushButton * buttonRobotChoice;
    QPushButton * buttonPause;
    QPushButton * buttonFastForward;

    QSlider * slider;
    QLabel * sliderValue;
    int oldSliderValue;
    bool boolMove=false;

    bool boolPause;
    bool boolFastForward;
    bool boolPosition;
    bool boolDirection;
    bool boolTrace;
    bool boolBall;
    bool boolTarget;

    QWidget * zoneCentral;
    QGridLayout * layout;

protected:
  void resizeEvent(QResizeEvent *event) override;

};

#endif // MAINWINDOW_H
