#include "mainwindow.h"



#define CV_IMG_WIDTH 480
#define CV_IMG_HEIGHT 640
#define SPD_INTERVAL 30
#define SPD_INTERVAL_FF 1

MainWindow::MainWindow()
{
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
    label2=new QLabel();
    label2->setAlignment(Qt::AlignCenter);
    label2->setText("SCORE : \n\n Score");
    label2->setStyleSheet("margin-left: 10px; border-radius: 25px; background: #9F9072; color: #4A0C46;");
    label2->setFrameStyle(QFrame::Panel | QFrame::Raised);

    //label3 = Equipe en possession & action en cours
    label3=new QLabel();
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


    label1->setStyleSheet("QLabel { background-color : red}");
    label1->setScaledContents(true);
    label1->setPixmap(QPixmap::fromImage(QImage(cvImage->data, cvImage->cols, cvImage->rows, cvImage->step, QImage::Format_RGB888)));


    setWindowTitle(tr("SPECTATOR MODE"));



    layout->addWidget(label1,1,1,5,5);
    layout->addWidget(label2,2,6,1,1);
    layout->addWidget(label3,3,6,1,1);
    layout->addWidget(label4,4,6,1,1);

    boutonRobotChoice = new QPushButton("Choix Robot");
    boutonPause = new QPushButton("PAUSE");
    boutonFF = new QPushButton(">>");
    boutonPosition = new QPushButton("Position : ON");
    boutonDirection = new QPushButton("Direction : ON");
    boutonTrace = new QPushButton("Trace : ON");
    boutonChangeTrace = new QPushButton("Trace : Change Robot");
    boutonChangeBall = new QPushButton("Ball : Change Robot");
    boutonBall = new QPushButton("Ball : ON");

    boolPause = false;
    boolFF = false;
    boolPosition = true;
    boolDirection = true;
    boolTrace = true;
    addRobot = false;
    boolBall = true;

    connect(boutonRobotChoice, SIGNAL(released()), this, SLOT (robotChoice()));
    connect(boutonPause, SIGNAL (released()), this, SLOT (togglePause()));
    connect(boutonFF, SIGNAL (released()), this, SLOT (toggleFF()));
    connect(boutonPosition, SIGNAL (released()), this, SLOT (togglePosition()));
    connect(boutonDirection, SIGNAL (released()), this, SLOT (toggleDirection()));
    connect(boutonTrace, SIGNAL (released()), this, SLOT (toggleTrace()));
    connect(boutonChangeTrace, SIGNAL (released()), this, SLOT (changeTrace()));
    connect(boutonBall, SIGNAL (released()), this, SLOT (toggleBall()));
    connect(boutonChangeBall, SIGNAL (released()), this, SLOT (changeBall()));
    
    layout->addWidget(boutonPause,7,3,1,1);
    layout->addWidget(boutonRobotChoice,7,4,1,1);
    layout->addWidget(boutonFF,7,5,1,1);
    layout->addWidget(boutonPosition,0,1,1,1);
    layout->addWidget(boutonDirection,0,2,1,1);
    layout->addWidget(boutonTrace,0,3,1,1);
    layout->addWidget(boutonBall,0,4,1,1);
    layout->addWidget(boutonChangeTrace,0,5,1,1);
    layout->addWidget(boutonChangeBall,0,6,1,1);

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
    robot_trace.push(annotation->getRobotTrace());
    robot_ball.push(annotation->getRobotBall());    
  // While exit was not explicitly required, run
    now = 0;
    dt = 30 * 1000;//[microseconds]
    if (!manager.isLive()) {
      now = manager.getStart();
    }

    

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(changeImage()));
    timer->start(SPD_INTERVAL);


} 


void MainWindow::robotChoice(){
	QMessageBox msgBox;
	msgBox.setInformativeText("Do you want to save your changes?");
msgBox.setText(tr("Confirm?"));
QAbstractButton* rob1 = msgBox.addButton(tr("Robot 1"), QMessageBox::YesRole);
msgBox.addButton(tr("Nope"), QMessageBox::NoRole);


	int ret = msgBox.exec();


	if (msgBox.clickedButton()==rob1) {
      printf("oulala\n");
	}
	else{
		printf("nonononon\n");
	}

}  





/*
  Slot qui affiche l'image traitée sur label1
*/
void MainWindow::changeImage(){

  if(!manager.isGood()){
    while(true){
      printf("FINI WESH");
    }
  }
  else{
    if(!boolPause){
      manager.update();
      if (manager.isLive()) {
        now = getTimeStamp();
      } else {
        now += dt;
      }
      MessageManager::Status status = manager.getStatus(now);
      std::vector<cv::Scalar> team_colors = {cv::Scalar(255,0,255), cv::Scalar(255,255,0)};
      std::map<uint32_t,cv::Scalar> colors_by_team;
      std::map<uint32_t,cv::Scalar> score_by_team;;

      QString text = "";

      for (int idx = 0; idx < status.gc_message.teams_size(); idx++) {
	const GCTeamMsg & team_msg = status.gc_message.teams(idx);
	if (team_msg.has_score()) {
	  uint32_t team_number = team_msg.team_number();
	  uint32_t team_score = team_msg.score();
	  text = text + "team n°" +  QString::number(team_number) +
	    " : " + QString::number(team_score) + "\n";
	}
      }

      this->label2->setText("SCORE : \n\n" + text);

      
      QString robots_in_game = "";
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
		  addRobot=true;
		}
		/* position du robot */
		const WeightedPose & weighted_pose = perception.self_in_field(pos_idx);
		const PositionDistribution & position = weighted_pose.pose().position();
		Position pos;
		pos.setPosition(position.x(),position.y(), now);
		teams[team_id].setRobotPos(robot_entry.first.robot_id(),pos);
		
		const AngleDistribution & dir = weighted_pose.pose().dir();
		Direction direction;
		direction.SetMean (dir.mean());
		teams[team_id].setRobotDirRobot(robot_entry.first.robot_id(),direction);
		
		const PositionDistribution & ball = perception.ball_in_self();
		Position pos_ball;
		pos_ball.setPosition(ball.x(), ball.y(), now);
		teams[team_id].setRobotPosBall(robot_entry.first.robot_id(), pos_ball);
		
		display_img =annotation->AddAnnotation(camera_information, teams[team_id].GetRobot(robot_entry.first.robot_id()) , display_img, now);
		
		robots_in_game = robots_in_game + "robot "
		  +  QString::number(robot_entry.first.robot_id())
		  + " team " +  QString::number(team_id) + "\n";
		
		
		if (addRobot){
		  int i = robot_entry.first.robot_id();
		  if (i != robot_trace.front())
		    robot_trace.push(robot_entry.first.robot_id());

		  if (i!= robot_ball.front())
		    robot_ball.push(robot_entry.first.robot_id());
		  addRobot =false; 
		}
		
	      }	       
	    }
	  }
	}
       cv::cvtColor(display_img, display_img, CV_BGR2RGB);
       this->label1->setPixmap(QPixmap::fromImage(QImage(display_img.data, display_img.cols, display_img.rows, display_img.step, QImage::Format_RGB888)));
       this->label4->setText(" ROBOT IN GAME : \n\n" + robots_in_game);
       this->label3->setText(" ROBOT TRACE : \n " + QString::number(robot_trace.front())+ " \n ROBOT BALL : \n" + QString::number(robot_ball.front()));
     }
   }
 }
}

void MainWindow::togglePause(){
  if(boolPause){ //boolPause -> on est en pause, donc on veut restart
    this->boutonPause->setText("PAUSE");
  }
  else{
    this->boutonPause->setText("START");
  }
  boolPause = !boolPause;

}

void MainWindow::togglePosition(){
  if(boolPosition){ //boolPosition -> on affiche, donc on ne veut plus afficher
    this->boutonPosition->setText("Position : OFF");
  }
  else{
    this->boutonPosition->setText("Position : ON");
  }
  annotation->togglePositionChoice();
  boolPosition = !boolPosition;

}

void MainWindow::toggleFF(){
  if(boolFF){//boolFF ->On est en FF, on veut passer en normal
    this->boutonFF->setText(">>");
    timer->setInterval(SPD_INTERVAL);
  }
  else {
    this->boutonFF->setText(">");
    timer->setInterval(SPD_INTERVAL_FF);
  }
  boolFF = !boolFF;
}

void MainWindow::toggleDirection(){
  if(boolDirection){ //boolDirection -> on affiche, donc on ne veut plus afficher
    this->boutonDirection->setText("Direction : OFF");
  }
  else{
    this->boutonDirection->setText("Direction : ON");
  }
  annotation->toggleDirectionChoice();
  boolDirection = !boolDirection;

}

void MainWindow::toggleTrace(){
  if(boolTrace){ //boolDirection -> on affiche, donc on ne veut plus afficher
    this->boutonTrace->setText("Trace : OFF");
  }
  else{
    this->boutonTrace->setText("Trace : ON");
  }
  annotation->toggleTraceChoice();
  boolTrace = !boolTrace;

}

void MainWindow::toggleBall(){
  if(boolBall){ //boolDirection -> on affiche, donc on ne veut plus afficher
    this->boutonBall->setText("Ball : OFF");
  }
  else{
    this->boutonBall->setText("Ball : ON");
  }
  annotation->toggleBallChoice();
  boolBall = !boolBall;

}

void MainWindow::changeTrace(){
  int old_robot = robot_trace.front();
  robot_trace.pop();
  robot_trace.push(old_robot);
  annotation->changeRobotTrace(robot_trace.front());
}


void MainWindow::changeBall(){
  int old_robot = robot_ball.front();
  robot_ball.pop();
  robot_ball.push(old_robot);
  annotation->changeRobotBall(robot_ball.front());
}
