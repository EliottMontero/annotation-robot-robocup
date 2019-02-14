
#ifndef WINDOW_H
#define WINDOW_H

#include <traitement/annotation.h>
#include "stdlib.h"
#include "stdio.h"
#include <iostream>
#include <QApplication>
#include <QDesktopWidget>
#include <QGuiApplication>
#include <QGridLayout>
#include <QImageReader>
#include <QLabel>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QPixmap>
#include <QSize>
#include <QSlider>
#include <QStyle>
#include <QWidget>
#include <QWindow>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QDebug>
#include <string>
#include "opencv2/core.hpp"
#include <opencv/cv.hpp>



using namespace cv;
using namespace traitement;

class QDataWidgetMapper;
class QLabel;
class QLineEdit;
class QPushButton;
class QSpinBox;
class QStandardItemModel;
class QTextEdit;

class Window : public QWidget
{
    Q_OBJECT
protected :
    QTimer *timer;
    float img_ratio;
    int x;
    int y;
    bool but0, but1, but2;
    QPushButton * bouton0;
    QPushButton * bouton1;
    QPushButton * bouton2;
    QPushButton * bouton3;
    QPushButton * bouton4;
    QPushButton * bouton5;
    cv::Mat * cvImage;


    bool play;
    bool nextFrame;
    QPixmap pixmap;
    QLabel * label0;
    QLabel * label1;
    QLabel * label2;
    QLabel * label3;
    QLabel * label4;
    QLabel * label5;
    QLabel * label6;
    QLabel * label7;
    QLabel * label8;
    QLabel * label9;
    QImageReader *image;
    //Annotation annotation;
        Annotation * manager;
public:
    Window(QWidget *parent = nullptr);
    //~Window();


public slots :
    void InitSpectatorInterface();
    void UpdateSpectatorInterface();
    void handleButton0();
    void handleButton1();
    void handleButton2();
    //void resize();
};
#endif // WINDOW_H
