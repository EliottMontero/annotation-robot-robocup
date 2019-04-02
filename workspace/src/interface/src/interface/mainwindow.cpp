#include "mainwindow.h"



#define FRAME_DURATION 30000 //30fps in microseconds  

#define CV_IMG_WIDTH 480
#define CV_IMG_HEIGHT 640
#define SPD_INTERVAL 30
#define SPD_INTERVAL_FF 1

MainWindow::MainWindow()
{
  setWindowTitle(tr("SPECTATOR MODE"));

  gamePicture = new cv::Mat(CV_IMG_WIDTH,CV_IMG_HEIGHT,CV_8UC4, Scalar(0,0,255));
  cv::cvtColor(*gamePicture,*gamePicture, CV_BGR2RGB);
  

  zoneCentral = new QWidget;
  layout = new QGridLayout;

  labelVideo = new QLabel();
  labelVideo->setAlignment(Qt::AlignCenter);
  labelVideo->setStyleSheet("QLabel { background-color : red}");
  labelVideo->setScaledContents(true);
  labelVideo->setPixmap(QPixmap::fromImage(QImage(gamePicture->data, gamePicture->cols, gamePicture->rows, gamePicture->step, QImage::Format_RGB888)));


  sliderValue=new QLabel(this);
  sliderValue->setText("0");

  slider = new QSlider(Qt::Horizontal, this);

  buttonRobotChoice = new QPushButton("Choix Robot");
  buttonPause = new QPushButton("PAUSE");
  buttonFastForward = new QPushButton(">>");

  connect(buttonRobotChoice, SIGNAL(released()), this, SLOT (robotChoice()));
  connect(buttonPause, SIGNAL (released()), this, SLOT (togglePause()));
  connect(buttonFastForward, SIGNAL (released()), this, SLOT (toggleFastForward()));

  layout->addWidget(labelVideo,1,1,5,4);
  layout->addWidget(slider,7,1,1,4);
  layout->addWidget(sliderValue,8,1,1,1);
  layout->addWidget(buttonPause,8,2,1,1);
  layout->addWidget(buttonFastForward,8,3,1,1);
  layout->addWidget(buttonRobotChoice,8,4,1,1);

  zoneCentral->setLayout(layout);
  setCentralWidget(zoneCentral);

  boolPause = false;
  boolFastForward = false;
  boolPosition = true;
  boolDirection = true;
  boolTrace = true;
  boolTarget = true;
  boolBall = true;

  Json::Reader reader;
  Json::Value root;

  std::ifstream match_settings("match_settings.json");
  if (!match_settings.good())
  throw std::runtime_error("failde to open file match_settings.json");
  match_settings >> root;
  checkMember(root["match_setting"], "config");
  std::string conf = root["match_setting"]["config"].asString();
  std::cout << conf << std::endl;

  // manager = new MonitoringManager();
  manager.loadConfig(conf);
	    
	     
  annotation = new Annotation("annotation_settings.json");
  
  now = 0;
  if (!manager.isLive()) {
      std::set<std::string> image_provider =  manager.getImageProvidersNames();
    
    for (const auto & entry : image_provider)
      if (now < manager.getImageProvider(entry).getStart())
	  now =  manager.getImageProvider(entry).getStart();
  }
  
 
  oldSliderValue=now;

  timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(changeImage()));
  timer->start(SPD_INTERVAL);
}


void MainWindow::robotChoice(){
  ChoiceDialog * msgBox = new ChoiceDialog();
  msgBox->setInitGeneral(boolPosition, boolDirection, boolTrace, boolBall, boolTarget);
  msgBox->setTeamMap(teams);
  msgBox->setCurrentTrace(annotation->getTeamTrace(),annotation->getRobotTrace());
  msgBox->setCurrentBall(annotation->getTeamBall(),annotation->getRobotBall());
  msgBox->setCurrentTarget(annotation->getTeamTarget(),annotation->getRobotTarget());

  int r = msgBox->exec();

  if(r == 1){
    /*** GENERAL ***/
    if(boolPosition != msgBox->getGeneralPosition()) togglePosition();
    if(boolDirection != msgBox->getGeneralDirection()) toggleDirection();
    if(boolTrace != msgBox->getGeneralTrace()) toggleTrace();
    if(boolBall != msgBox->getGeneralBall()) toggleBall();
    if(boolTarget != msgBox->getGeneralTarget()) toggleTarget();

    /*** COMBOBOX ***/
    annotation->changeRobotTrace(msgBox->getNumberTeamTrace(),
                                 msgBox->getNumberRobotTrace());
    annotation->changeRobotBall(msgBox->getNumberTeamBall(),
                                msgBox->getNumberRobotBall());
    annotation->changeRobotTarget(msgBox->getNumberTeamTarget(),
				  msgBox->getNumberRobotTarget());

  }
}


/*
  Slot qui affiche l'image traitÃ©e sur labelVideo
*/
void MainWindow::changeImage(){

 
    if(!boolPause){
      manager.update();
      if (manager.isLive()) {
	now = getTimeStamp();
      }
     
      else {
	uint64_t total_duration = endTime - initialTime;
	if(slider->value()>oldSliderValue){
	  while(now - initialTime<(total_duration * slider->value())/100){
	    now += FRAME_DURATION;
	  }
	}
	else if(slider->value()<oldSliderValue){
	  while(now - initialTime>((total_duration * slider->value())/100)){
	    now -= FRAME_DURATION;
	  }
	}


	now += FRAME_DURATION;

	slider->setValue((now - initialTime)*100.0/total_duration);

	oldSliderValue=slider->value();

	char str[20];
	if(now >= endTime && endTime !=0){
	  now-=FRAME_DURATION;
	  sprintf(str,"end of video");
	}
	else
	    sprintf(str,"%d:%d\n",((now - initialTime)/1000000)/60, ((now - initialTime)/1000000)%60);
	sliderValue->setText(str);
	
      }
      MessageManager::Status status = manager.getStatus(now);
      std::map<uint32_t,cv::Scalar> score_by_team;
      //Team Msg
      for (int idx = 0; idx < status.gc_message.teams_size(); idx++) {
	const GCTeamMsg & team_msg = status.gc_message.teams(idx);
	uint32_t team_id = team_msg.team_number();

	if(teamPanels.find(team_id) == teamPanels.end()){
	  createTeam(team_id);
	}

	if (team_msg.has_score()) {
	  uint32_t team_score = team_msg.score();
	  teamPanels[team_id]->updateScore(team_score);
	}
      }
      std::map<std::string, CalibratedImage> images_by_source;
      images_by_source = manager.getCalibratedImages(now);
      for (const auto & entry : images_by_source) {
	std::string source_name = entry.first;
	cv::Mat display_img = entry.second.getImg().clone();
	if (entry.second.isFullySpecified()) {
	  const CameraMetaInformation & camera_information = entry.second.getCameraInformation();
	  if (annotation->annotation_choice["field"])
	    annotation->field.tagLines(camera_information, &display_img, cv::Scalar(0,0,0), 2);

          //Robot Msg
	  for (const auto & robot_entry : status.robot_messages) {
	    uint32_t team_id = robot_entry.first.team_id();
	    if(teamPanels.find(team_id) == teamPanels.end()){
	      createTeam(team_id);
	    }

	    uint32_t robot_id = robot_entry.first.robot_id();
	    if(!teams[team_id].IsRobot(robot_id)){
	      createRobot(robot_id, team_id);
	    }
	    teams[team_id].updateRobot(robot_entry.first.robot_id(), robot_entry.second);

	    display_img =annotation->AddAnnotation(camera_information, teams[team_id].getRobot(robot_entry.first.robot_id()) , display_img, now);
	  }

	}
	cv::cvtColor(display_img, display_img, CV_BGR2RGB);
	this->labelVideo->setPixmap(QPixmap::fromImage(QImage(display_img.data, display_img.cols, display_img.rows, display_img.step, QImage::Format_RGB888)));

	std::set<std::string> image_provider =  manager.getImageProvidersNames();
	for (const auto & entry : image_provider){
	  initialTime = manager.getImageProvider(entry).getStart();
	  endTime = manager.getImageProvider(entry).getEnd();
	}
      }
    }
    for (auto it : teamPanels){
      (it.second)->updateAnnotation(boolPosition, boolDirection, boolTrace, boolBall, boolTarget,
				    annotation->getTeamTrace(), annotation->getRobotTrace(),
				    annotation->getTeamBall(), annotation->getRobotBall(),
				    annotation->getTeamTarget(), annotation->getRobotTarget());
    }
  
}

void MainWindow::createTeam(int id){
  teamPanels[id] =  new TeamPanel();
  teamPanels[id]->setTeamNumber(id);

  if(teamPanels.size() == 1){
    teamPanels[id]->setGridLayout(layout,true);
  }
  if(teamPanels.size() == 2){
    teamPanels[id]->setGridLayout(layout,false);
  }

  Team t1;
  teams[id] = t1;

  teamPanels[id]->setSizeRobotArea();
}

void MainWindow::createRobot(int robotId, int teamId){
  teams[teamId].AddRobot(robotId);
  teams[teamId].setRobotTeam(robotId,teamId);
  teams[teamId].setRobotNum(robotId);

  teamPanels[teamId]->addRobot(robotId);
}

void MainWindow::togglePause(){
  if(boolPause){
    this->buttonPause->setText("PAUSE");
  }
  else{
    this->buttonPause->setText("PLAY");
  }
  boolPause = !boolPause;

}

void MainWindow::togglePosition(){
  annotation->togglePositionChoice();
  boolPosition = !boolPosition;
}

void MainWindow::toggleFastForward(){
  if(boolFastForward){
    this->buttonFastForward->setText(">>");
    timer->setInterval(SPD_INTERVAL);
  }
  else {
    this->buttonFastForward->setText(">");
    timer->setInterval(SPD_INTERVAL_FF);
  }
  boolFastForward = !boolFastForward;
}

void MainWindow::toggleDirection(){
  annotation->toggleDirectionChoice();
  boolDirection = !boolDirection;
}

void MainWindow::toggleTrace(){
  annotation->toggleTraceChoice();
  boolTrace = !boolTrace;
}

void MainWindow::toggleBall(){
  annotation->toggleBallChoice();
  boolBall = !boolBall;
}

void MainWindow::toggleTarget(){
  annotation->toggleTargetChoice();
  boolTarget = !boolTarget;
}

void MainWindow::resizeEvent(QResizeEvent *event){
  QMainWindow::resizeEvent(event);
  for(auto it : teamPanels){
    (it.second)->setSizeRobotArea();
  }
}
