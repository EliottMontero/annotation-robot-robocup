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



using namespace cv;


class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        MainWindow();

        cv::Mat * cvImage;
        cv::Mat * cvImage2;

        QLabel * label1;

        int tour;


    signals:

    public slots:
        void changeImage();
};

#endif // MAINWINDOW_H
