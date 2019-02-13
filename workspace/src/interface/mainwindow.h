
#ifndef WINDOW_H
#define WINDOW_H
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
//#include "opencv2/core.hpp"


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
    //QGridLayout *layout;
    int sliderValue;
    QTimer *timer;
    float img_ratio;
    int x;
    int y;
    bool init=true;
    QAction *ouvrirAct;
    QMenu *m1;
    QString fichierchoisi;
    QSize taille;
    bool but0, but1, but2;
    QPushButton * bouton0;
    QPushButton * bouton1;
    QPushButton * bouton2;
    QPushButton * bouton3;
    QPushButton * bouton4;
    QPushButton * bouton5;
    bool play;
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
    QMenuBar *mainbar;
    QStandardItemModel *model;


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
