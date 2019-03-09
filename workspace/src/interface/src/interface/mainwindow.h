#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QObject>
#include <QLayout>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QSlider>
#include <QMessageBox>


#include "opencv2/core.hpp"
#include <opencv/cv.hpp>

#include <traitement/annotation.h>

#include "teamBoard.h"


#include <hl_communication/utils.h>

#include <hl_communication/message_manager.h>
#include <hl_monitoring/monitoring_manager.h>
#include <hl_monitoring/utils.h>
#include <hl_monitoring/field.h>
#include <traitement/position.h>
#include <traitement/direction.h>
#include <traitement/team.h>



#include <opencv2/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include <tclap/CmdLine.h>
#include <string>
#include <fstream>

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

    QTimer * timer;
    cv::Mat * cvImage;
    cv::Mat * cvImage2;
    QPushButton * boutonRobotChoice;
    QPushButton * boutonPause;
    QPushButton * boutonFF;
    QPushButton * boutonPosition;
    QPushButton * boutonDirection;
    QPushButton * boutonTrace;
    QPushButton * boutonChangeTrace;
    QPushButton * boutonBall;
    QPushButton * boutonChangeBall;
    QSlider * slider;
    QLabel * label1;
    QLabel * label5;

    bool boolPause;
    bool boolFF;
    bool boolPosition;
    bool boolDirection;
    bool boolTrace;
    bool addRobot;
    std::queue<int> robot_trace;
    std::queue<int> robot_ball;
    bool boolBall;
    bool boolMove=false;

    MonitoringManager manager;
    uint64_t now;
    uint64_t dt;
    Field field;
    Annotation *annotation;
    std::map<int, Team> teams;
    std::map<int, TeamBoard*> teamBoards;

    QWidget * zoneCentral;
    QGridLayout * layout;


    signals:

    public slots:
    void sliderControl(int i);
    void robotChoice();
    void changeImage();
    void togglePause();
    void toggleFF();
    void togglePosition();
    void toggleDirection();
    void toggleTrace();
    void toggleBall();
    void changeTrace();
    void changeBall();
};

#endif // MAINWINDOW_H
