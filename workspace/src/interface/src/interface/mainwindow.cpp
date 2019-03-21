#include "mainwindow.h"



#define CV_IMG_WIDTH 480
#define CV_IMG_HEIGHT 640
#define SPD_INTERVAL 30
#define SPD_INTERVAL_FF 1

MainWindow::MainWindow()
{
  setWindowTitle(tr("SPECTATOR MODE"));

  cvImage = new cv::Mat(CV_IMG_WIDTH,CV_IMG_HEIGHT,CV_8UC4, Scalar(0,0,255));
  cvImage2 = new cv::Mat(CV_IMG_WIDTH,CV_IMG_HEIGHT,CV_8UC4, Scalar(0,255,0));
  cv::cvtColor(*cvImage,*cvImage, CV_BGR2RGB);
  cv::cvtColor(*cvImage2, *cvImage2, CV_BGR2RGB);

  zoneCentral = new QWidget;
  layout = new QGridLayout;

  labelVideo = new QLabel();
  labelVideo->setAlignment(Qt::AlignCenter);
  labelVideo->setStyleSheet("QLabel { background-color : red}");
  labelVideo->setScaledContents(true);
  labelVideo->setPixmap(QPixmap::fromImage(QImage(cvImage->data, cvImage->cols, cvImage->rows, cvImage->step, QImage::Format_RGB888)));

  label5=new QLabel(this);
  label5->setText("0");

  slider = new QSlider(Qt::Horizontal, this);

  boutonRobotChoice = new QPushButton("Choix Robot");
  boutonPause = new QPushButton("PAUSE");
  boutonFF = new QPushButton(">>");

  connect(boutonRobotChoice, SIGNAL(released()), this, SLOT (robotChoice()));
  connect(boutonPause, SIGNAL (released()), this, SLOT (togglePause()));
  connect(boutonFF, SIGNAL (released()), this, SLOT (toggleFF()));
  connect(slider, SIGNAL(valueChanged(int)), this, SLOT(sliderControl(int)));

  layout->addWidget(labelVideo,1,1,5,4);
  layout->addWidget(slider,7,1,1,4);
  layout->addWidget(label5,8,1,1,1);
  layout->addWidget(boutonPause,8,2,1,1);
  layout->addWidget(boutonFF,8,3,1,1);
  layout->addWidget(boutonRobotChoice,8,4,1,1);

  zoneCentral->setLayout(layout);
  setCentralWidget(zoneCentral);

  boolPause = false;
  boolFF = false;
  boolPosition = true;
  boolDirection = true;
  boolTrace = true;
  boolTarget = true;
  boolBall = true;


  //*** Partie communication avec traitement ***//

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

  // // While exit was not explicitly required, run
  now = 0;
  dt = 30 * 1000;//[microseconds]
  if (!manager.isLive()) {
    now = manager.getStart();
  }

  timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(changeImage()));
  timer->start(SPD_INTERVAL);
}


void MainWindow::sliderControl(int value){
  boolMove=true;
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
      }

      else {
        if(boolMove){
          now+=dt*200;
          boolMove=false;
        }
        else{
          now += dt;
        }
        char str[20];
        sprintf(str,"%d\n",now);
        label5->setText(str);
      }
      MessageManager::Status status = manager.getStatus(now);
      std::vector<cv::Scalar> team_colors = {cv::Scalar(255,0,255), cv::Scalar(255,255,0)};
      std::map<uint32_t,cv::Scalar> colors_by_team;
      std::map<uint32_t,cv::Scalar> score_by_team;

      //Team Msg
      for (int idx = 0; idx < status.gc_message.teams_size(); idx++) {
        const GCTeamMsg & team_msg = status.gc_message.teams(idx);
        uint32_t team_id = team_msg.team_number();

        if(teamBoards.find(team_id) == teamBoards.end()){
          createTeam(team_id);
        }

        if (team_msg.has_score()) {
          uint32_t team_score = team_msg.score();
          teamBoards[team_id]->updateScore(team_score);
        }
      }

      std::map<std::string, CalibratedImage> images_by_source;
      images_by_source = manager.getCalibratedImages(now);
      for (const auto & entry : images_by_source) {
        cv::Mat display_img = entry.second.getImg().clone();
        if (entry.second.isFullySpecified()) {
          const CameraMetaInformation & camera_information = entry.second.getCameraInformation();
          field.tagLines(camera_information, &display_img, cv::Scalar(0,0,0), 2);

          //Robot Msg
          for (const auto & robot_entry : status.robot_messages) {
            uint32_t team_id = robot_entry.first.team_id();
            if(teamBoards.find(team_id) == teamBoards.end()){
              createTeam(team_id);
            }

            uint32_t robot_id = robot_entry.first.robot_id();
            if(!teams[team_id].IsRobot(robot_id)){
              createRobot(robot_id, team_id);
            }

            if (robot_entry.second.has_perception()) {
              const Perception & perception = robot_entry.second.perception();
              for (int pos_idx = 0; pos_idx < perception.self_in_field_size(); pos_idx++) {

                /* position du robot */
                const WeightedPose & weighted_pose = perception.self_in_field(pos_idx);
                const PositionDistribution & position = weighted_pose.pose().position();
                Position pos;
                pos.setPosition(position.x(),position.y(), now);
                teams[team_id].setRobotPos(robot_id,pos);

                const AngleDistribution & dir = weighted_pose.pose().dir();
                Direction direction;
                direction.SetMean (dir.mean(),now);
                teams[team_id].setRobotDirRobot(robot_id,direction);

                const PositionDistribution & ball = perception.ball_in_self();
                Position pos_ball;
                pos_ball.setPosition(ball.x(), ball.y(), now);
                teams[team_id].setRobotPosBall(robot_id, pos_ball);

                //pour l'affichage de la position souhaitée mais pas encore fini.
           	   const Intention & intention = robot_entry.second.intention();
           	   const PositionDistribution & target_pos = intention.target_pose_in_field().position();

           	   Position pos_target;
           	   pos_target.setPosition(target_pos.x(),target_pos.y(), now);
           	   teams[team_id].setRobotPosTarget(robot_entry.first.robot_id(), pos_target);

               display_img = annotation->AddAnnotation(camera_information, teams[team_id].GetRobot(robot_id) , display_img, now);
              }
            }
          }
        }
        cv::cvtColor(display_img, display_img, CV_BGR2RGB);
        this->labelVideo->setPixmap(QPixmap::fromImage(QImage(display_img.data, display_img.cols, display_img.rows, display_img.step, QImage::Format_RGB888)));
      }
    }
    for (auto it : teamBoards){
      (it.second)->updateAnnotation(boolPosition, boolDirection, boolTrace, boolBall, boolTarget,
          annotation->getTeamTrace(), annotation->getRobotTrace(),
          annotation->getTeamBall(), annotation->getRobotBall(),
          annotation->getTeamTarget(), annotation->getRobotTarget());
    }
  }
}

void MainWindow::createTeam(int id){
  teamBoards[id] =  new TeamBoard();
  teamBoards[id]->setTeamNumber(id);

  if(teamBoards.size() == 1){
    teamBoards[id]->setGridLayout(layout,true);
  }
  if(teamBoards.size() == 2){
    teamBoards[id]->setGridLayout(layout,false);
  }

  Team t1;
  teams[id] = t1;

  teamBoards[id]->setSizeRobotArea();
}

void MainWindow::createRobot(int robotId, int teamId){
  teams[teamId].AddRobot(robotId);
  teams[teamId].setRobotTeam(robotId,teamId);
  teams[teamId].setRobotNum(robotId);

  teamBoards[teamId]->addRobot(robotId);
}

void MainWindow::togglePause(){
  if(boolPause){
    this->boutonPause->setText("PAUSE");
  }
  else{
    this->boutonPause->setText("PLAY");
  }
  boolPause = !boolPause;

}

void MainWindow::togglePosition(){
  annotation->togglePositionChoice();
  boolPosition = !boolPosition;
}

void MainWindow::toggleFF(){
  if(boolFF){
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
  for(auto it : teamBoards){
    (it.second)->setSizeRobotArea();
  }
}
