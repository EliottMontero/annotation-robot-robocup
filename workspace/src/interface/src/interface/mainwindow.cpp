#include "mainwindow.h"

#define CV_IMG_WIDTH 480
#define CV_IMG_HEIGHT 640

MainWindow::MainWindow()
{

    cvImage = new cv::Mat(CV_IMG_WIDTH,CV_IMG_HEIGHT,CV_8UC4, Scalar(0,0,255));

    QWidget * zoneCentral = new QWidget;
    QGridLayout * layout = new QGridLayout;


    //label1 sera le label contenant l'image de la video
    QLabel * label1 = new QLabel();
    label1->setAlignment(Qt::AlignCenter);

    //label2 = SCORE
    QLabel * label2=new QLabel();
    label2->setAlignment(Qt::AlignCenter);
    label2->setText("SCORE : \n\n Score");
    label2->setStyleSheet("margin-left: 10px; border-radius: 25px; background: #9F9072; color: #4A0C46;");
    label2->setFrameStyle(QFrame::Panel | QFrame::Raised);

    //label3 = Equipe en possession & action en cours
    QLabel * label3=new QLabel(this);
    label3->setAlignment(Qt::AlignCenter);
    label3->setText("ACTION EN COURS : \n \n Action");
    label3->setStyleSheet("margin-left: 10px; border-radius: 25px; background: #9F9072; color: #4A0C46;");
    label3->setFrameStyle(QFrame::Panel | QFrame::Raised);

    //label4 = Robots en touche
    QLabel * label4=new QLabel(this);
    label4->setAlignment(Qt::AlignCenter);
    label4->setText("EN TOUCHE : \n\n En touche");
    label4->setStyleSheet("margin-left: 10px; border-radius: 25px; background: #9F9072; color: #4A0C46;");
    label4->setFrameStyle(QFrame::Panel | QFrame::Raised);


    label1->setStyleSheet("QLabel { background-color : red}");
    label1->setScaledContents(true);
    label1->setPixmap(QPixmap::fromImage(QImage(cvImage->data, cvImage->cols, cvImage->rows, cvImage->step, QImage::Format_RGB888)));


    setWindowTitle(tr("SPECTATOR MODE"));



    layout->addWidget(label1,1,1,5,5);
    layout->addWidget(label2,2,6,1,1);
    layout->addWidget(label3,3,6,1,1);
    layout->addWidget(label4,4,6,1,1);


    QPushButton * bouton0 = new QPushButton("PAUSE");
    QPushButton * bouton1 = new QPushButton("Position");
    QPushButton * bouton2 = new QPushButton("Direction");

    layout->addWidget(bouton0,7,3,1,1);
    layout->addWidget(bouton1,0,2,1,1);
    layout->addWidget(bouton2,0,4,1,1);



    zoneCentral->setLayout(layout);
    setCentralWidget(zoneCentral);
}
