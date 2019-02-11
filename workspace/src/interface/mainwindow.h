
#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>
#include <string>
#include <QWidget>
#include <QMenu>
#include <QMenuBar>
#include <QImageReader>
#include <QLabel>
#include <QPixmap>
#include <QSize>
#include <QDesktopWidget>
#include <QMainWindow>
#include "opencv2/core.hpp"
#include <QGridLayout>


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
    QTimer *timer;
    int x;
    int y;
    bool init=true;
    bool spectator=false;
    QAction *ouvrirAct;
    QMenu *m1;
    QString fichierchoisi;
    QSize taille;
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
    Window(QWidget *parent = 0);
   // ~Window();


public slots :
    void InitSpectatorInterface();
    void UpdateSpectatorInterface();

};
#endif // WINDOW_H
