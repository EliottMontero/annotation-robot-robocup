#include "mainwindow.h"

#include <QtWidgets>

Window::Window(QWidget *parent)
    : QWidget(parent)
{
    QDesktopWidget dw;
    x=dw.width()*1;
    y=dw.height()*1;
    x=400;
    y=300;
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

    bouton = new QPushButton("My Button", this);
    connect(bouton, SIGNAL (released()), this, SLOT (handleButton()));
    bouton->setVisible(false);

    ouvrirAct=new QAction(tr("Spectateur"),this);
    connect(ouvrirAct, SIGNAL(triggered()), this, SLOT(InitSpectatorInterface()));
    mainbar->addMenu(m1);
    m1->addAction(ouvrirAct);
    init=false;


    InitSpectatorInterface();
    setWindowTitle(tr("CHOIX DU MODE"));

}

void Window::handleButton(){
    if (play){
        //player->pause();
        play=false;
        bouton->setText("PLAY");
    }
    else{
        //player->play();
        play=true;
        bouton->setText("PAUSE");

    }
}

void Window::UpdateSpectatorInterface(){


    y = this->size().height();
    x = this->size().width();

    //ici la taille des widgets par rapport Ã  la fenÃ¨tre.
    int x_marge=10;
    int y_marge=20;
    float ratioL1=2.0f/3.0f;
    float ratioL2=2.f/12.0f;

    //vw->setGeometry(static_cast<int>(x/(2*x_marge)),static_cast<int>(y-(2*y/y_marge)-(y*ratioL1)),static_cast<int>(x*ratioL1),static_cast<int>(y*ratioL1));

    QSize sizeL1(img_ratio*y*ratioL1,y*ratioL1);
    label1->setFixedSize(sizeL1);
    label1->setGeometry((int)x/(2*x_marge),y-label1->height()-2*y/y_marge,0,0);
    //label1->setGeometry(10,10,0,0);


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

    //slider->setGeometry(x/(2*x_marge), y-(y/y_marge)-slider->height(),vw->width(), slider->height());

    //label5->setGeometry(x/(2*x_marge)-label5->width(),y-(4*y/(3*y_marge))-slider->height(),30,30);
    //char str[3];
    //snprintf(str, 3, "%d", slider->value());
    //label5->setText(str);

    //label6->setGeometry(vw->width()+x/(2*x_marge)+10,y-(y/y_marge)-slider->height(),slider->height(), slider->height());

    bouton->setGeometry(label1->width()/2+x/(2*x_marge), static_cast<int>(y-(y/(2*y_marge))-bouton->height()*ratioL1),label1->width()/10, label1->width()/40);
    font = bouton->font();
    font.setPointSize(x/120);
    font.setBold(false);
    bouton->setFont(font);

    label1->setVisible(true);
    label2->setVisible(true);
    label3->setVisible(true);
    label4->setVisible(true);
    //label5->setVisible(true);
    //label6->setVisible(true);
    //slider->setVisible(true);
    bouton->setVisible(true);
    //vw->setVisible(false);
}

//on initialise l'interface dans le style que lulu a proposÃ©
void Window::InitSpectatorInterface(){
    //player = new QMediaPlayer;
    //vw = new QVideoWidget(this);
    //player->setVideoOutput(vw);
    //img_ratio = (float)vw->sizeHint().width()/(float)vw->sizeHint().height();
    //player->setMedia(QUrl::fromLocalFile("/home/flo/emmc2/2vs1/dynamic_video.avi"));
    //vw->setGeometry(100,100,300,400);
    //vw->show();
    bouton->setText("PAUSE");
    //player->play();
    //play=true;




    //label1 sera le label contenant l'image de la vidÃ©o
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

    //valeur dÃ©but slider
    //label5=new QLabel(this);
    //label5->setAlignment(Qt::AlignCenter);
    //label5->setText("-0-");
    //label5->setStyleSheet("margin-left: 10px; border-radius: 25px; background: #9F9072; color: #4A0C46;");
    //label5->setFrameStyle(QFrame::Panel | QFrame::Raised);

    //valeur fin slider
    //label6=new QLabel(this);
    //label6->setAlignment(Qt::AlignCenter);
    //label6->setText("-100-");

     pixmap.load("../interface/a.jpeg");
     img_ratio=(float)pixmap.width()/(float)pixmap.height();
     QSize imageTaille(x/3,y/3);
     label1->setFixedSize(imageTaille);
     label1->setPixmap(pixmap);
     label1->setStyleSheet("QLabel { background-color : red; color : blue; }");

     label1->setMask(pixmap.mask());
     label1->setScaledContents(true);

     //slider=new QSlider(Qt::Horizontal,this);
     //slider->setTickInterval(1);
     //slider->setMaximum(100);
     //slider->setMinimum(0);



     setWindowTitle(tr("SPECTATOR MODE"));
     QTimer *timer = new QTimer(this);
     connect(timer, SIGNAL(timeout()), this, SLOT(UpdateSpectatorInterface()));
     timer->start(10);
}


