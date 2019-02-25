#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QObject>
#include <QLayout>
#include <QLabel>
#include <QPushButton>
#include <QTimer>


#include "opencv2/core.hpp"
#include <opencv/cv.hpp>

#include <traitement/annotation.h>


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

        cv::Mat * cvImage;
        cv::Mat * cvImage2;
        QPushButton * bouton0;
        QPushButton * bouton1;
        QPushButton * bouton2;
        QPushButton * bouton3;
        QPushButton * bouton4;
        QLabel * label1;

        bool boolPause;
        bool boolPosition;
        bool boolDirection;
        bool boolTrace;
        bool boolBall;

        MonitoringManager manager;
        uint64_t now;
        uint64_t dt;
        Field field;
        Annotation *annotation;
        std::map<int, Team> teams;

    signals:

    public slots:
        void changeImage();
        void togglePause();
        void togglePosition();
        void toggleDirection();
        void toggleTrace();
        void toggleBall();
};

#endif // MAINWINDOW_H
