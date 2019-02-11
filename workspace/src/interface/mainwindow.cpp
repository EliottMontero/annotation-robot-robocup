#include "mainwindow.h"

#include <QtWidgets>

Window::Window(QWidget *parent)
    : QWidget(parent)
{
    QDesktopWidget dw;
    x=dw.width()*1;
    y=dw.height()*1;
    QSize initSize(x,y);
    setGeometry(
            QStyle::alignedRect(
                Qt::LeftToRight,
                Qt::AlignCenter,
                initSize,
                qApp->desktop()->availableGeometry()
            )
        );
    //this->setFixedSize(initSize);
    this->setStyleSheet("background-color: grey;");
    mainbar = new QMenuBar(this);
    m1 = new QMenu("Mode de Vue",this);
    mainbar->addMenu(m1);

    ouvrirAct=new QAction(tr("Spectateur"),this);
    connect(ouvrirAct, SIGNAL(triggered()), this, SLOT(InitSpectatorInterface()));
    mainbar->addMenu(m1);
    m1->addAction(ouvrirAct);
    init=false;
    this->setStyleSheet("background-color: black;");

    setWindowTitle(tr("CHOIX DU MODE"));
}

void Window::UpdateSpectatorInterface(){
    y = this->size().height();
    x = this->size().width();

    //ici la taille des widgets par rapport à la fenètre.
    int x_marge=10;
    int y_marge=20;
    float ratioL1=2.0/3.0;
    float ratioL2=1.7/12.0;

    QSize sizeL1(x*ratioL1,y*ratioL1);
    label1->setFixedSize(sizeL1);
    label1->setGeometry((int)x/(2*x_marge),y-(y/y_marge)-(y*ratioL1),x*ratioL1,y*ratioL1);

    QSize sizeL2(x*ratioL2,y*ratioL2);
    label2->setFixedSize(sizeL2);
    label2->setGeometry(x-(x*ratioL2)-(x/x_marge), (2*y/4)-(y/y_marge)-y/4,x*ratioL2,y*ratioL2);
    QFont font = label2->font();
    font.setPointSize(x/100);
    font.setBold(true);
    label2->setFont(font);

    label3->setFixedSize(sizeL2);
    label3->setGeometry(x-(x*ratioL2)-(x/x_marge), (3*y/4)-(y/y_marge)-y/4,0,0);
    font = label4->font();
    font.setPointSize(x/100);
    font.setBold(true);
    label3->setFont(font);

    label4->setFixedSize(sizeL2);
    label4->setGeometry(x-(x*ratioL2)-(x/x_marge), y-(y/y_marge)-y/4,0,0);
    font = label4->font();
    font.setPointSize(x/100);
    font.setBold(true);
    label4->setFont(font);
}

//on initialise l'interface dans le style que lulu a proposé
void Window::InitSpectatorInterface(){
      QGridLayout *layout = new   QGridLayout();
    setLayout(layout);
    spectator=true;

    //label1 sera le label contenant l'image de la vidéo
    label1=new QLabel(this);
    //label1->setAlignment(Qt::AlignCenter);

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
     //QSize imageTaille(x/3,y/3);
     //label1->setFixedSize(imageTaille);
     label1->setPixmap(pixmap);
     label1->setStyleSheet("QLabel { background-color : red; color : blue; }");

     //label1->setMask(pixmap.mask());
     label1->setScaledContents(true);

     //layout->addWidget(mainbar,0,0);
     layout->addWidget(label1,2,0);
     layout->addWidget(label2,0,2);
     layout->addWidget(label3,0,1);
     layout->addWidget(label4,0,0);
     setWindowTitle(tr("SPECTATOR MODE"));
     QTimer *timer = new QTimer(this);
     connect(timer, SIGNAL(timeout()), this, SLOT(UpdateSpectatorInterface()));
     timer->start(100);
}


