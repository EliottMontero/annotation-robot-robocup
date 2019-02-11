
#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QMenu>
#include <QMenuBar>
#include <QImageReader>
#include <QLabel>
#include <QPixmap>
#include <QSize>


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
    QAction *ouvrirAct;
    QMenu *m1;
    QString fichierchoisi;
    QSize taille;
    QPixmap pixmap;
    QLabel * label1;
    QLabel * label2;
    QImageReader *image;
    QMenuBar *mainbar;
    QStandardItemModel *model;

public:
    Window(QWidget *parent = 0);
   // ~Window();


public slots :
    void afficher();

};
#endif // WINDOW_H
