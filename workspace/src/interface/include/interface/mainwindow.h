#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QObject>
#include <QLayout>
#include <QLabel>
#include <QPushButton>


#include "opencv2/core.hpp"
#include <opencv/cv.hpp>



using namespace cv;


class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        MainWindow();

        cv::Mat * cvImage;

    signals:

    public slots:
};

#endif // MAINWINDOW_H
