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

#include "teamBoard.h"
#include "choiceDialog.h"

#include <hl_communication/message_manager.h>
#include <hl_monitoring/monitoring_manager.h>
#include <hl_communication/utils.h>
#include <hl_monitoring/utils.h>
#include <hl_monitoring/field.h>

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
    QSlider * slider;
    QLabel * labelVideo;
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

    uint64_t now;
    uint64_t dt;
    Field field;

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

private :
    MonitoringManager manager;
    Annotation *annotation;

    std::map<int, Team> teams;
    std::map<int, TeamBoard*> teamBoards;




protected:
  void resizeEvent(QResizeEvent *event) override;

};

#endif // MAINWINDOW_H
