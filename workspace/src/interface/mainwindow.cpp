#include "mainwindow.h"

#include <QtWidgets>

Window::Window(QWidget *parent) : QWidget(parent){
    QDesktopWidget dw;
    x=static_cast<int>(dw.width()*0.8);
    y=static_cast<int>(dw.height()*0.8);

    QSize initSize(x,y);
    setGeometry(QStyle::alignedRect(Qt::LeftToRight,Qt::AlignCenter,initSize,qApp->desktop()->availableGeometry()));
    this->setStyleSheet("background-color: grey;");


    bouton0 = new QPushButton("PAUSE", this);
    bouton1 = new QPushButton("Position", this);
    bouton2 = new QPushButton("Direction", this);

    connect(bouton0, SIGNAL (released()), this, SLOT (handleButton0()));
    connect(bouton1, SIGNAL (released()), this, SLOT (handleButton1()));
    connect(bouton2, SIGNAL (released()), this, SLOT (handleButton2()));

    bouton0->setVisible(false);
    bouton1->setVisible(false);
    bouton2->setVisible(false);

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


void Window::MatToImage(){
         QImage *Qim= new QImage(cvImage.data, cvImage.cols, cvImage.rows, cvImage.step, QImage::Format_ARGB32);
         QPixmap pix=QPixmap::fromImage(*Qim);
         label1->setPixmap(pix);
   }

void Window::UpdateSpectatorInterface(){
    y = this->size().height();
    x = this->size().width();

    int label_font_size=x/100;
    int button_font_size=x/120;

    int x_marge=10;
    int y_marge=20;
    float ratioL1=2.0f/3.0f;
    float ratioL2=2.f/12.0f;

    int ZERO=0;

    int posX_label1 = x/(2*x_marge);
    int posY_label1 = y-label1->height()-2*y/y_marge;
    int width_label1 = static_cast<int>(img_ratio*y*ratioL1);
    int height_label1 = static_cast<int>(y*ratioL1);

    int posX_label2 = static_cast<int>(x-(x*ratioL2)-(x/x_marge));
    int posY_label2 = (2*y/4)-(y/y_marge)-y/4;
    int width_label2 = static_cast<int>(x*ratioL2);
    int height_label2 = static_cast<int>(y*ratioL2);

    int posX_label3 = posX_label2;
    int posY_label3 = posY_label2 + y/4;
    int width_label3 = width_label2;
    int height_label3 = height_label2;

    int posX_label4 = posX_label2;
    int posY_label4 = posY_label3 + y/4;
    int width_label4 = width_label2;
    int height_label4 = height_label2;

    int posX_button0 = width_label1/2+x/(2*x_marge);
    int posY_button0 = static_cast<int>(y-(y/(2*y_marge))-y/y_marge);
    int width_button0 = label1->width()/10;
    int height_button0 = label1->width()/40;

    int posX_button1 = x/x_marge;
    int posY_button1 = y-label1->height()-4*y/y_marge;
    int width_button1 = width_button0;
    int height_button1 = height_button0;

    int posX_button2 = x/x_marge+3*bouton1->width();
    int posY_button2 = y-label1->height()-4*y/y_marge;
    int width_button2 = width_button1;
    int height_button2 =  height_button1;

    QSize sizeL1(width_label1, height_label1);
    label1->setFixedSize(sizeL1);
    label1->setGeometry(posX_label1, posY_label1, ZERO, ZERO);
    label2->setGeometry(posX_label2, posY_label2, width_label2, height_label2);
    label3->setGeometry(posX_label3, posY_label3, width_label3, height_label3);
    label4->setGeometry(posX_label4, posY_label4, width_label4, height_label4);

    QFont font = label2->font();
    font.setPointSize(label_font_size);
    font.setBold(true);
    label2->setFont(font);
    label3->setFont(font);
    label4->setFont(font);
    font = bouton0->font();
    font.setPointSize(button_font_size);
    font.setBold(false);
    bouton0->setFont(font);
    bouton1->setFont(font);
    bouton2->setFont(font);

    bouton0->setGeometry(posX_button0, posY_button0, width_button0, height_button0);
    bouton1->setGeometry(posX_button1, posY_button1, width_button1, height_button1);
    bouton2->setGeometry(posX_button2, posY_button2, width_button2, height_button2);

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
     label1->setPixmap(pixmap);
     label1->setStyleSheet("QLabel { background-color : red; color : blue; }");

     label1->setMask(pixmap.mask());
     label1->setScaledContents(true);

     setWindowTitle(tr("SPECTATOR MODE"));
     QTimer *timer = new QTimer(this);
     connect(timer, SIGNAL(timeout()), this, SLOT(UpdateSpectatorInterface()));
     timer->start(100);
}


