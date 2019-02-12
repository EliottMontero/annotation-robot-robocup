
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
    bool spectator=false;
    QAction *ouvrirAct;
    QMenu *m1;
    QString fichierchoisi;
    QSize taille;
    QPixmap pixmap;
    QSlider *slider;
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
    Window(QWidget *parent = 0);
   // ~Window();


public slots :
    void InitSpectatorInterface();
    void UpdateSpectatorInterface();

    //void resize();
};
#endif // WINDOW_H
