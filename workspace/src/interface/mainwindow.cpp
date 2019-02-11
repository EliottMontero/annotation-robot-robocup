#include "mainwindow.h"

#include <QtWidgets>

Window::Window(QWidget *parent)
    : QWidget(parent)
{
    if(init){
        QDesktopWidget dw;
        x=dw.width()*1;
        y=dw.height()*1;
    }

    else {
        QRect rec = QApplication::desktop()->screenGeometry();
        y = rec.height();
        x = rec.width();
    }
    QSize initSize(x,y);
    setGeometry(
            QStyle::alignedRect(
                Qt::LeftToRight,
                Qt::AlignCenter,
                initSize,
                qApp->desktop()->availableGeometry()
            )
        );



    mainbar = new QMenuBar(this);
    m1 = new QMenu("Mode de Vue",this);
    mainbar->addMenu(m1);

    ouvrirAct=new QAction(tr("Spectateur"),this);
    connect(ouvrirAct, SIGNAL(triggered()), this, SLOT(InitSpectatorInterface()));
    mainbar->addMenu(m1);
    m1->addAction(ouvrirAct);
    init=false;


    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(UpdateSpectatorInterface()));


    setWindowTitle(tr("CHOIX DU MODE"));
}

void Window::UpdateSpectatorInterface(){
    QRect rec = QApplication::desktop()->screenGeometry();
    y = this->size().height();
    x = this->size().width();
    QSize imageTaille(x/3,y/3);
    label1->setFixedSize(imageTaille);
    char out_string [30];
    sprintf(out_string, "%d", x);
    label2->setText(out_string);
}
void Window::update(){
    if(init){
        label3->setText("OOOOO");
        init=false;
    }
    else{
        init=true;
        label3->setText("ZZZZZ");
     }
}

void Window::InitSpectatorInterface(){
    QGridLayout *layout = new QGridLayout();
    setLayout(layout);
    spectator=true;

    label1=new QLabel(this);
    label1->setAlignment(Qt::AlignCenter);

    label2=new QLabel(this);
    label2->setAlignment(Qt::AlignCenter);
    label2->setText("label 2");

    label3=new QLabel(this);
    label3->setAlignment(Qt::AlignCenter);
    label3->setText("label 3");

    label4=new QLabel(this);
    label4->setAlignment(Qt::AlignCenter);
    label4->setText("label 4");

     pixmap.load("../interface/a.jpeg");
     QSize imageTaille(x/3,y/3);
     label1->setFixedSize(imageTaille);
     label1->setPixmap(pixmap);
     label1->setStyleSheet("QLabel { background-color : red; color : blue; }");

     label1->setMask(pixmap.mask());
     label1->show();

     layout->addWidget(mainbar,0,0);
     layout->addWidget(label1,3,0);
     layout->addWidget(label2,1,3);
     layout->addWidget(label3,2,3);
     layout->addWidget(label4,3,3);
     setWindowTitle(tr("SPECTATOR MODE"));
     QTimer *timer = new QTimer(this);
     connect(timer, SIGNAL(timeout()), this, SLOT(UpdateSpectatorInterface()));
     timer->start(100);
}


