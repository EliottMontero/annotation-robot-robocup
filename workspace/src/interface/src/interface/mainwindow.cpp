#include "mainwindow.h"

//#include <traitement/annotation.h>

#include <QtWidgets>


#define LABEL_FONT_RATIO 100
#define BUTTON_FONT_RATIO 120
#define X_MARGE_SIZE 10
#define Y_MARGE_SIZE 20
#define WINDOW_REFRESH_RATE 10
#define CV_IMG_WIDTH 480
#define CV_IMG_HEIGHT 640


Window::Window(QWidget *parent) : QWidget(parent){
	QDesktopWidget dw;
	//taille de la fenetre à l'ouverture, ici 80% de l'écran
	x=static_cast<int>(dw.width()*0.8);
	y=static_cast<int>(dw.height()*0.8);

	QSize initSize(x,y);
	setGeometry(QStyle::alignedRect(Qt::LeftToRight,Qt::AlignCenter,initSize,qApp->desktop()->availableGeometry()));
	this->setStyleSheet("background-color: grey;");

	bouton0 = new QPushButton("PAUSE", this);
	bouton1 = new QPushButton("Position", this);
	bouton2 = new QPushButton("Direction", this);

	QObject::connect(bouton0, SIGNAL (released()), this, SLOT (handleButton0()));
	QObject::connect(bouton1, SIGNAL (released()), this, SLOT (handleButton1()));
	QObject::connect(bouton2, SIGNAL (released()), this, SLOT (handleButton2()));

	bouton0->setVisible(false);
	bouton1->setVisible(false);
	bouton2->setVisible(false);

	InitSpectatorInterface();
	setWindowTitle(tr("CHOIX DU MODE"));
}

Window::~Window(){

}
//gere l'activation du bouton 0
void Window::handleButton0(){
	if (but0){
		but0=false;
		bouton0->setText("PLAY");
	}
	else{
		but0=true;
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

/* fonction d'actualisation de l'interface, qui sert pour l'instant presque exclusivement en cas
de redimmensionnement de la fenetre. Se raffraichit toutes les WINDOW_REFRESH_RATE milisecondes.
*/

void Window::UpdateSpectatorInterface(){
	y = this->size().height();
	x = this->size().width();
	//taille de la police des boutons et labels
	int label_font_size=min(x,y)/LABEL_FONT_RATIO;
	int button_font_size=min(x,y)/BUTTON_FONT_RATIO;

	int y_marge=Y_MARGE_SIZE;
	int x_marge=X_MARGE_SIZE;
	float ratioL1=2.0f/3.0f;
	float ratioL2=2.f/12.0f;
	//initialisation des tailles et positions des labels et boutons
	label1->setPixmap(QPixmap::fromImage(QImage(cvImage->data, cvImage->cols, cvImage->rows, cvImage->step, QImage::Format_RGB888)));
	int width_label1 = static_cast<int>(img_ratio*y*ratioL1);
	int height_label1 = static_cast<int>(y*ratioL1);
	int posX_label1 = x/(2*x_marge);
	int posY_label1 = y-height_label1-2*y/y_marge;

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
	int width_button0 = x/20;
	int height_button0 = y/30;

	//meme taille que le bouton0 pour le moment
	int posX_button1 = x/x_marge;
	int posY_button1 = y/y_marge;
	int width_button1 = width_button0;
	int height_button1 = height_button0;
	//a droite de bouton1
	int posX_button2 = posX_button1+3*width_button1;
	int posY_button2 = posY_button1;
	int width_button2 = width_button1;
	int height_button2 = height_button1;

	// on assigne aux widgets leurs tailles et positions.

	QSize sizeL1(width_label1, height_label1);
	label1->setFixedSize(sizeL1);
	label1->setGeometry(posX_label1, posY_label1, 0, 0);//les 0 sont la largeur & hauteur du label. Ne fonctionnent pas pour un label contennant une pixmap.
	label2->setGeometry(posX_label2, posY_label2, width_label2, height_label2);
	label3->setGeometry(posX_label3, posY_label3, width_label3, height_label3);
	label4->setGeometry(posX_label4, posY_label4, width_label4, height_label4);

	//les labels ont la meme taille de police qui dépend de la taille de la fenetre.
	QFont font = label2->font();
	font.setPointSize(label_font_size);
	font.setBold(true);
	label2->setFont(font);
	label3->setFont(font);
	label4->setFont(font);
	font = bouton0->font();
	//de meme pour les boutons, avec une taille différente
	font.setPointSize(button_font_size);
	font.setBold(false);
	bouton0->setFont(font);
	bouton1->setFont(font);
	bouton2->setFont(font);

	bouton0->setGeometry(posX_button0, posY_button0, width_button0, height_button0);
	bouton1->setGeometry(posX_button1, posY_button1, width_button1, height_button1);
	bouton2->setGeometry(posX_button2, posY_button2, width_button2, height_button2);

	//pour etre bien certain de les afficher
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

	cvImage = new cv::Mat(CV_IMG_WIDTH,CV_IMG_HEIGHT,CV_8UC4, Scalar(0,0,255));

	/*
	Ne fonctionne pas encore, partie qui récuperera l'image annotée du module traitement.

	char** argV=(char**)malloc(9*sizeof(char*));
	for(int i=0; i<9; i++){
		argV[i]=(char*)malloc(100*sizeof(char));
	}
	sprintf(argV[0],"~/emmc2/workspace/devel/lib/traitement/test_traitement");
	sprintf(argV[1],"-c");
	sprintf(argV[2],"replay.json");
	sprintf(argV[3],"-f");
	sprintf(argV[4],"eirlab.json");
	sprintf(argV[5],"-a");
	sprintf(argV[6],"0");
	sprintf(argV[7],"-t");
	sprintf(argV[8],"1");
	manager->launchAnnotation(9, argV, false, *cvImage);
	*/

	//label1 sera le label contenant l'image de la video
	label1=new QLabel(this);
	label1->setAlignment(Qt::AlignCenter);

	//label2 = SCORE
	label2=new QLabel(this);
	label2->setAlignment(Qt::AlignCenter);
	label2->setText("SCORE : \n\n Score");
	label2->setStyleSheet("margin-left: 10px; border-radius: 25px; background: #9F9072; color: #4A0C46;");
	label2->setFrameStyle(QFrame::Panel | QFrame::Raised);

	//label3 = Equipe en possession & action en cours
	label3=new QLabel(this);
	label3->setAlignment(Qt::AlignCenter);
	label3->setText("ACTION EN COURS : \n \n Action");
	label3->setStyleSheet("margin-left: 10px; border-radius: 25px; background: #9F9072; color: #4A0C46;");
	label3->setFrameStyle(QFrame::Panel | QFrame::Raised);

	//label4 = Robots en touche
	label4=new QLabel(this);
	label4->setAlignment(Qt::AlignCenter);
	label4->setText("EN TOUCHE : \n\n En touche");
	label4->setStyleSheet("margin-left: 10px; border-radius: 25px; background: #9F9072; color: #4A0C46;");
	label4->setFrameStyle(QFrame::Panel | QFrame::Raised);

	//détermine le le ratio de l'image à afficher dans l'interface pour ne pas qu'elle soit crop, ou déformée.
	img_ratio=CV_IMG_HEIGHT/CV_IMG_WIDTH;

	label1->setStyleSheet("QLabel { background-color : red}");

	label1->setScaledContents(true);

	setWindowTitle(tr("SPECTATOR MODE"));

	//rafraichit l'interface toutes les WINDOW_REFRESH_RATE millisecondes
	QTimer *timer = new QTimer(this);
	QObject::connect(timer, SIGNAL(timeout()), this, SLOT(UpdateSpectatorInterface()));
	timer->start(WINDOW_REFRESH_RATE);
}
