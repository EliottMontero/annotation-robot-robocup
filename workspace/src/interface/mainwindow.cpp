#include "mainwindow.h"

#include <QtWidgets>

Window::Window(QWidget *parent) : QWidget(parent){
    QDesktopWidget dw;
    x=dw.width()*1;
    y=dw.height()*1;
    x=1600;
    y=1000;
    QSize initSize(x,y);
    setGeometry(QStyle::alignedRect(Qt::LeftToRight,Qt::AlignCenter,initSize,qApp->desktop()->availableGeometry()));
    this->setStyleSheet("background-color: grey;");

    mainbar = new QMenuBar(this);
    m1 = new QMenu("Mode de Vue",this);
    mainbar->addMenu(m1);

    bouton0 = new QPushButton("PAUSE", this);
    bouton1 = new QPushButton("Position", this);
    bouton2 = new QPushButton("Direction", this);

    connect(bouton0, SIGNAL (released()), this, SLOT (handleButton0()));
    connect(bouton1, SIGNAL (released()), this, SLOT (handleButton1()));
    connect(bouton2, SIGNAL (released()), this, SLOT (handleButton2()));

    bouton0->setVisible(false);
    bouton1->setVisible(false);
    bouton2->setVisible(false);

    ouvrirAct=new QAction(tr("Spectateur"),this);
    connect(ouvrirAct, SIGNAL(triggered()), this, SLOT(InitSpectatorInterface()));
    mainbar->addMenu(m1);
    m1->addAction(ouvrirAct);
    init=false;
    InitSpectatorInterface();
    setWindowTitle(tr("CHOIX DU MODE"));
}

void Window::handleButton0(){
    if (play){
        play=false;
        bouton0->setText("PLAY");
    }
    else{
        play=true;
        bouton0->setText("PAUSE");
    }
}

void Window::handleButton1(){
    if (but1) {
        bouton1-> setStyleSheet("margin-left: 10px; border-radius: 25px; background: #DC524A; color: #000000;");
        but1=false;
   }
    else
    {
        bouton1->setStyleSheet("margin-left: 10px; border-radius: 25px; background: #2FD265; color: #000000;");
        but1 = true;
    }
}


void Window::handleButton2(){
    if (but2) {
        bouton2->setStyleSheet("margin-left: 10px; border-radius: 25px; background: #DC524A; color: #000000;");
        but2=false;
   }
    else
    {
        bouton2->setStyleSheet("margin-left: 10px; border-radius: 25px; background: #2FD265; color: #000000;");
        but2 = true;
    }
}

void Window::UpdateSpectatorInterface(){

    y = this->size().height();
    x = this->size().width();

    //ici la taille des widgets par rapport a  la fenetre.
    int x_marge=10;
    int y_marge=20;
    float ratioL1=2.0f/3.0f;
    float ratioL2=2.f/12.0f;

    QSize sizeL1(static_cast<int>(img_ratio*y*ratioL1),static_cast<int>(y*ratioL1));
    label1->setFixedSize(sizeL1);
    label1->setGeometry(x/(2*x_marge),y-label1->height()-2*y/y_marge,0,0);


    QSize sizeL2(static_cast<int>(x*ratioL2),static_cast<int>(y*ratioL2));
    label2->setFixedSize(sizeL2);
    label2->setGeometry(static_cast<int>(x-(x*ratioL2)-(x/x_marge)),(2*y/4)-(y/y_marge)-y/4,static_cast<int>(x*ratioL2),static_cast<int>(y*ratioL2));
    QFont font = label2->font();
    font.setPointSize(x/100);
    font.setBold(true);
    label2->setFont(font);

    label3->setFixedSize(sizeL2);
    label3->setGeometry(static_cast<int>(x-(x*ratioL2)-(x/x_marge)), (3*y/4)-(y/y_marge)-y/4,0,0);
    font = label4->font();
    font.setPointSize(x/100);
    font.setBold(true);
    label3->setFont(font);

    label4->setFixedSize(sizeL2);
    label4->setGeometry(static_cast<int>(x-(x*ratioL2)-(x/x_marge)), y-(y/y_marge)-y/4,0,0);
    font = label4->font();
    font.setPointSize(x/100);
    font.setBold(true);
    label4->setFont(font);

    bouton0->setGeometry(label1->width()/2+x/(2*x_marge), static_cast<int>(y-(y/(2*y_marge))-bouton0->height()*ratioL1),label1->width()/10, label1->width()/40);
    font = bouton0->font();
    font.setPointSize(x/120);
    font.setBold(false);
    bouton0->setFont(font);
    bouton1->setFont(font);
    bouton2->setFont(font);

    bouton1->setGeometry(x/x_marge, y-label1->height()-4*y/y_marge,bouton0->width(), bouton0->height());
    bouton2->setGeometry(x/x_marge+3*bouton1->width(), y-label1->height()-4*y/y_marge,bouton0->width(), bouton0->height());


    label1->setVisible(true);
    label2->setVisible(true);
    label3->setVisible(true);
    label4->setVisible(true);
    bouton0->setVisible(true);
    bouton1->setVisible(true);
    bouton2->setVisible(true);
}



//on initialise l'interface dans le style que lulu a propose
void Window::InitSpectatorInterface(){

    //label1 sera le label contenant l'image de la video
    label1=new QLabel(this);
    label1->setAlignment(Qt::AlignCenter);

    //label2 = SCORE
    label2=new QLabel(this);
    label2->setAlignment(Qt::AlignCenter);
    label2->setText("SCORE : \n\n 12 millions");
    label2->setStyleSheet("margin-left: 10px; border-radius: 25px; background: #9F9072; color: #4A0C46;");
    label2->setFrameStyle(QFrame::Panel | QFrame::Raised);

    //label3 = Equipe en possession & action en cours
    label3=new QLabel(this);
    label3->setAlignment(Qt::AlignCenter);
    label3->setText("ACTION EN COURS : \n \n pas nous.");
    label3->setStyleSheet("margin-left: 10px; border-radius: 25px; background: #9F9072; color: #4A0C46;");
    label3->setFrameStyle(QFrame::Panel | QFrame::Raised);

    //label4 = Robots en touche
    label4=new QLabel(this);
    label4->setAlignment(Qt::AlignCenter);
    label4->setText("EN TOUCHE : \n\n on s'touche");
    label4->setStyleSheet("margin-left: 10px; border-radius: 25px; background: #9F9072; color: #4A0C46;");
    label4->setFrameStyle(QFrame::Panel | QFrame::Raised);

     pixmap.load("../interface/a.jpeg");
     img_ratio=(float)pixmap.width()/(float)pixmap.height();
     QSize imageTaille(x/3,y/3);
     label1->setFixedSize(imageTaille);
     label1->setPixmap(pixmap);
     label1->setStyleSheet("QLabel { background-color : red; color : blue; }");

     label1->setMask(pixmap.mask());
     label1->setScaledContents(true);

     setWindowTitle(tr("SPECTATOR MODE"));
     QTimer *timer = new QTimer(this);
     connect(timer, SIGNAL(timeout()), this, SLOT(UpdateSpectatorInterface()));
     timer->start(10);
}


