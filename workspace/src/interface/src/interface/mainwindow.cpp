#include "mainwindow.h"



#define CV_IMG_WIDTH 480
#define CV_IMG_HEIGHT 640

MainWindow::MainWindow()
{
    tour = 0;
    cvImage = new cv::Mat(CV_IMG_WIDTH,CV_IMG_HEIGHT,CV_8UC4, Scalar(0,0,255));
    cvImage2 = new cv::Mat(CV_IMG_WIDTH,CV_IMG_HEIGHT,CV_8UC4, Scalar(0,255,0));
    cv::cvtColor(*cvImage,*cvImage, CV_BGR2RGB);
    cv::cvtColor(*cvImage2, *cvImage2, CV_BGR2RGB);


    QWidget * zoneCentral = new QWidget;
    QGridLayout * layout = new QGridLayout;


    //label1 sera le label contenant l'image de la video
    label1 = new QLabel();
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

    //Partie communication avec traitement


    Json::Reader reader;
    Json::Value root;


    std::ifstream match_settings("match_settings.json");
    if (!match_settings.good())
      throw std::runtime_error("failde to open file match_settings.json");
    match_settings >> root;

    checkMember(root["match_setting"], "config");
    checkMember(root["match_setting"], "field");
    std::string conf = root["match_setting"]["config"].asString();
    std::cout << conf << std::endl;



    manager.loadConfig(conf);

    std::string f = root["match_setting"]["field"].asString();
    field.loadFile(f);

    std::cout << f << std::endl;

    annotation = new Annotation("annotation_settings.json");
  // While exit was not explicitly required, run
    now = 0;
    dt = 30 * 1000;//[microseconds]
    if (!manager.isLive()) {
      now = manager.getStart();
      //time_start = manager.getStart();
    }

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(changeImage()));
    timer->start(10);


}

void MainWindow::changeImage(){

  if(!manager.isGood()){
    while(true){
      printf("FINI WESH");
    }
  }
  else{
    manager.update();
    if (manager.isLive()) {
      now = getTimeStamp();
    } else {
      now += dt;
      }
    MessageManager::Status status = manager.getStatus(now);
    std::vector<cv::Scalar> team_colors = {cv::Scalar(255,0,255), cv::Scalar(255,255,0)};
    std::map<uint32_t,cv::Scalar> colors_by_team;

   std::map<std::string, CalibratedImage> images_by_source =
      manager.getCalibratedImages(now);
   for (const auto & entry : images_by_source) {
     cv::Mat display_img = entry.second.getImg().clone();
     if (entry.second.isFullySpecified()) {
       const CameraMetaInformation & camera_information = entry.second.getCameraInformation();
       field.tagLines(camera_information, &display_img, cv::Scalar(0,0,0), 2);
       // Basic drawing of robot estimated position
       for (const auto & robot_entry : status.robot_messages) {
	 uint32_t team_id = robot_entry.first.team_id();
         if (robot_entry.second.has_perception()) {
	   const Perception & perception = robot_entry.second.perception();
	   for (int pos_idx = 0; pos_idx < perception.self_in_field_size(); pos_idx++) {

	     /* init des robots présent sur le jeu*/
	     if (teams.find(team_id)==teams.end()){
	       Team t1;
	       teams[team_id]=t1;
	     }
	     if (!teams[team_id].IsRobot(robot_entry.first.robot_id())){
	       teams[team_id].AddRobot(robot_entry.first.robot_id());
	       teams[team_id].setRobotTeam(robot_entry.first.robot_id(),team_id);
	       teams[team_id].setRobotNum(robot_entry.first.robot_id());

	     }


	     /* position du robot */
	     const WeightedPose & weighted_pose = perception.self_in_field(pos_idx);
	     const PositionDistribution & position = weighted_pose.pose().position();
	     Position pos;
	     pos.setPosition(position.x(),position.y());
	     teams[team_id].RobotUpdate(robot_entry.first.robot_id(),pos);

	     const AngleDistribution & dir = weighted_pose.pose().dir();
	     Direction direction;
	     direction.SetMean (dir.mean())  ;

	     const PositionDistribution & ball = perception.ball_in_self();
	     Position pos_ball;
	     pos_ball.setPosition(ball.x(), ball.y());
	     teams[team_id].setRobotPosBall(robot_entry.first.robot_id(), pos_ball);

	     display_img =annotation->AddAnnotation(pos,direction, camera_information, teams[team_id].GetRobot(robot_entry.first.robot_id()) , display_img);
	   }

	 }
       }
     }

     cv::cvtColor(display_img, display_img, CV_BGR2RGB);
     this->label1->setPixmap(QPixmap::fromImage(QImage(display_img.data, display_img.cols, display_img.rows, display_img.step, QImage::Format_RGB888)));


   }


  }


  /*
  tour++;
  if(tour%2==1){
    this->label1->setPixmap(QPixmap::fromImage(QImage(cvImage2->data, cvImage2->cols, cvImage2->rows, cvImage2->step, QImage::Format_RGB888)));
  }
  else{
    this->label1->setPixmap(QPixmap::fromImage(QImage(cvImage->data, cvImage->cols, cvImage->rows, cvImage->step, QImage::Format_RGB888)));
  }
  */


}
