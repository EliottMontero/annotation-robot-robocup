#include "traitement/annotation.h"


#include <string>
#include <sstream>
#include <iostream>
#include <json/value.h>
#include <jsoncpp/json/json.h>
#include <fstream>

#define SIZE_CIRCLE_POS 10
#define SIZE_ARROW 50
#define SIZE_CIRCLE_TRACE 5



using namespace hl_communication;
using namespace hl_monitoring;


namespace traitement{
  Annotation::Annotation(std::string file){
    Json::Reader reader;
    Json::Value root;

    std::ifstream annotation_settings(file);
    if (!annotation_settings.good())
      throw std::runtime_error("failed to open file" + file);
    annotation_settings >> root;

    checkMember(root, "position");
    checkMember(root["position"], "write");
    checkMember(root["position"], "number");
    checkMember(root, "direction");
    checkMember(root["direction"], "write");
    checkMember(root, "trace");
    checkMember(root["trace"], "write");
    checkMember(root["trace"], "optimized");
    checkMember(root, "ball");
    checkMember(root["ball"], "write");
    checkMember(root, "target");
    checkMember(root["target"], "write");

    
    annotation_choice["position"]=root["position"]["write"].asBool();
    annotation_choice["number"]=root["position"]["number"].asBool();
    annotation_choice["direction"]=root["direction"]["write"].asBool();
    annotation_choice["trace"]=root["trace"]["write"].asBool();
    annotation_choice["optimized"]=root["trace"]["optimized"].asBool();
    annotation_choice["ball"]=root["ball"]["write"].asBool();
    annotation_choice["target"]=root["target"]["write"].asBool();

    checkMember(root["position"], "circle_size");
    checkMember(root["trace"], "circle_size");
    checkMember(root["direction"], "arrow_size");
    checkMember(root["trace"], "robot_num");
    checkMember(root["trace"], "old_pos_number");
    checkMember(root["ball"], "ball_size");
    checkMember(root["ball"], "robot_num");
    checkMember(root["target"], "target_size");
    checkMember(root["target"], "robot_num");


    sizecircle = root["position"]["circle_size"].asUInt();
    sizecircletrace = root["trace"]["circle_size"].asUInt();
    sizearrow = root["direction"]["arrow_size"].asUInt();
    robottrace = root["trace"]["robot_num"].asUInt();
    nbtrace = root["trace"]["old_pos_number"].asUInt();
    ballsize = root["ball"]["ball_size"].asUInt();
    robotball = root["ball"]["robot_num"].asUInt();
    targetsize = root["target"]["target_size"].asUInt();
    robottarget = root["target"]["robot_num"].asUInt();

    checkMember(root["color_team_1"], "num");
    checkMember(root["color_team_1"], "r");
    checkMember(root["color_team_1"], "b");
    checkMember(root["color_team_1"], "g");

    checkMember(root["color_team_2"], "num");
    checkMember(root["color_team_2"], "r");
    checkMember(root["color_team_2"], "b");
    checkMember(root["color_team_2"], "g");


    cv::Scalar color1 = {root["color_team_1"]["r"].asUInt(), root["color_team_1"]["g"].asUInt(), root["color_team_1"]["b"].asUInt()};

    cv::Scalar color2 = {root["color_team_2"]["r"].asUInt(), root["color_team_2"]["g"].asUInt(), root["color_team_2"]["b"].asUInt()};

    color[root["color_team_1"]["num"].asUInt()]=color1;
    color[root["color_team_2"]["num"].asUInt()]=color2;
  }



  Annotation::~Annotation(){
  }


  cv::Mat  Annotation::annotePosition(CameraMetaInformation camera_information, RobotInformation rb ,cv::Mat display,  uint64_t now){
    cv::Mat overlay;
    display.copyTo(overlay);
    Position pos;
    pos = rb.getPosRobot();
    cv::Point3f pos_in_field(pos.x, pos.y, 0.0);
    cv::Point2f pos_in_img = fieldToImg(pos_in_field, camera_information);
    if (color.find(rb.getTeam())!=color.end())
      cv::circle(overlay,pos_in_img, sizecircle, color[rb.getTeam()],cv::FILLED);
    
    else
      cv::circle(overlay,pos_in_img, sizecircle, cv::Scalar(0,0,0),cv::FILLED);
    //calcul compliqué à cause du time stamp
    float sec = nbtrace*1000000.0;
    float opacity = (sec-(now-pos.time_stamp))/sec;
    cv::addWeighted(overlay,opacity, display, 1-opacity, 0,display);
    if (annotation_choice["number"]){
      cv::putText(display, std::to_string(rb.numRobotInformation), cv::Point2f(pos_in_img.x-sizecircle*3/4,pos_in_img.y+sizecircle*3/4), cv::FONT_HERSHEY_TRIPLEX ,0.7,cv::Scalar(0,0,0),2);
    }

    //cv::circle(display,pos_in_img, sizecircle, color[rb.getTeam()],cv::FILLED);
    return display;
  }

  cv::Mat  Annotation::annoteTarget(CameraMetaInformation camera_information, RobotInformation rb ,cv::Mat display,  uint64_t now){
    cv::Mat overlay;
    display.copyTo(overlay);
    Position pos;
    pos = rb.getPosTarget();
    cv::Point3f pos_in_field(pos.x, pos.y, 0.0);
    cv::Point2f pos_in_img = fieldToImg(pos_in_field, camera_information);
    Position robot;
    robot = rb.getPosRobot();
    cv::Point3f pos_in_fieldr(robot.x, robot.y, 0.0);
    cv::Point2f pos_in_imgr = fieldToImg(pos_in_fieldr, camera_information);
    if (color.find(rb.getTeam())!=color.end()){
      cv::Scalar s =  color[rb.getTeam()];
      cv::drawMarker (overlay,pos_in_img, cv::Scalar(s[0]/2,s[1]/2,s[2]/2), cv::MARKER_TILTED_CROSS, 10, 2, 8);
      cv::LineIterator it(overlay, pos_in_img, pos_in_imgr, 8);            // get a line iterator
      for(int i = 0; i < it.count; i++,it++)
	if ( i%10<5 ) {
	  (*it)[0] = s[0]/2;
	  (*it)[1] = s[1]/2;
	  (*it)[2] = s[2]/2;
	}   
      
    }
    
    else
      cv::circle(overlay,pos_in_img, targetsize, cv::Scalar(0,0,0),cv::FILLED);
    //calcul compliqué à cause du time stamp
    float sec = nbtrace*1000000.0;
    float opacity = (sec-(now-pos.time_stamp))/sec;
    cv::addWeighted(overlay,opacity, display, 1-opacity, 0,display);
   

    return display;
  }

  cv::Mat  Annotation::annoteDirection( CameraMetaInformation camera_information, RobotInformation rb,cv::Mat display, uint64_t now){
    Position pos;
    pos = rb.getPosRobot();
    Direction dir;
    dir = rb.getDirRobot();
    // calcul pos
    cv::Point3f pos_in_field(pos.x, pos.y, 0.0);
    cv::Point2f pos_in_img = fieldToImg(pos_in_field, camera_information);
    //calcul dir
    cv::Point3f pos_in_fielddir(pos.x+cos(dir.mean), pos.y+sin(dir.mean), 0.0);
    cv::Point2f pos_in_imgdir = fieldToImg(pos_in_fielddir, camera_information);
    /* reduction taille des flèches à une longueur de 50 pour que la taille des flèches soit homogène*/
    float hypo = sqrt((pos_in_imgdir.x - pos_in_img.x)*(pos_in_imgdir.x - pos_in_img.x) +(pos_in_imgdir.y- pos_in_img.y)*(pos_in_imgdir.y- pos_in_img.y));
    cv::Point2f fleche;
    fleche.x =  pos_in_img.x + (sizearrow*(pos_in_imgdir.x - pos_in_img.x)/hypo);
    fleche.y= pos_in_img.y + (sizearrow*(pos_in_imgdir.y- pos_in_img.y)/hypo);
    /*Affichage couleur pour les angles de degrès bizarre*/
    if (dir.mean > 2*CV_PI)
      cv :: arrowedLine(display, pos_in_img, fleche, cv::Scalar(0,0,0), 2, 0, 0.1);
    else
      if (color.find(rb.getTeam())!=color.end())
	cv :: arrowedLine(display, pos_in_img, fleche, color[rb.getTeam()], 2, 0, 0.1);
      else
	cv :: arrowedLine(display, pos_in_img, fleche, cv::Scalar(0,0,0), 2, 0, 0.1);

    return display;
  }


  cv::Mat  Annotation::annoteTrace(CameraMetaInformation camera_information, RobotInformation rb,cv::Mat display,  uint64_t now){
    int qsize = rb.sizeOfQueue();
    Position old_pos;
    for (int i = 0; i<qsize; i++){
      if (annotation_choice["optimized"]){
      
	cv::Mat overlay;
	display.copyTo(overlay);
	Position p;
	p = rb.getTraceRobot();
	
	float sec = nbtrace*1000000.0;
	float opacity = (sec-(now-p.time_stamp))/sec;
	if(i == 0 || (i!=0 && (abs(old_pos.x-p.x)>sizecircletrace*1.5/100 || abs(old_pos.y-p.y)>sizecircletrace*1.5/100))){
	    old_pos=p;
	    cv::Point3f pos_in_field(p.x, p.y, 0.0);
	    cv::Point2f pos_in_img = fieldToImg(pos_in_field, camera_information);
	    if (color.find(rb.getTeam())!=color.end()){
	      cv::Scalar s =  color[rb.getTeam()];
	      cv::circle(overlay,pos_in_img, sizecircletrace, cv::Scalar(s[0]/2,s[1]/2,s[2]/2),cv::FILLED);
	    }
	    else{
	      cv::circle(overlay,pos_in_img, sizecircletrace, cv::Scalar(0,0,0),cv::FILLED);
	    }
	    
	    cv::addWeighted(overlay,opacity, display,1- opacity, 0,display);
	}
      
      }
      else{
	
	Position p;
	p = rb.getTraceRobot();
              
	cv::Point3f pos_in_field(p.x, p.y, 0.0);
	cv::Point2f pos_in_img = fieldToImg(pos_in_field, camera_information);
	if (color.find(rb.getTeam())!=color.end()){
	  cv::Scalar s =  color[rb.getTeam()];
	  cv::circle(display,pos_in_img, sizecircletrace, cv::Scalar(s[0]/2,s[1]/2,s[2]/2),cv::FILLED);
	}
	else 
	  cv::circle(display,pos_in_img, sizecircletrace, cv::Scalar(0,0,0),cv::FILLED);
      }
    }
    return display;
    
  }
  
  cv::Mat  Annotation::annoteBall(CameraMetaInformation camera_information, RobotInformation rb,cv::Mat display,  uint64_t now){
    cv::Mat overlay;
    display.copyTo(overlay);
    Position ball;
    ball = rb.getPosBall();
    Position robot;
    robot = rb.getPosRobot();
    Direction dir;
    dir = rb.getDirRobot();
    cv::Point2f position;
    position.x = robot.x + ball.x*cos(dir.mean)-ball.y*sin(dir.mean);
    position.y = robot.y + ball.x*sin(dir.mean)+ball.y*cos(dir.mean);
    cv::Point3f pos_in_field(position.x, position.y, 0.0);
    cv::Point2f pos_in_img = fieldToImg(pos_in_field, camera_information);
    cv::circle(overlay,pos_in_img, ballsize, cv::Scalar(125,125,125),cv::FILLED);
    float sec = nbtrace*1000000.0;
    float opacity = (sec-(now-ball.time_stamp))/sec;
    cv::addWeighted(overlay,opacity, display, 1-opacity, 0,display);
    return display;
  }


  cv::Mat Annotation::AddAnnotation( CameraMetaInformation camera_information, RobotInformation rb ,cv::Mat display,  uint64_t now){
    //delete old pose
    Position p = rb.oldPos.front();
    while (((nbtrace*1000000.0-(now-p.time_stamp))/nbtrace*1000000.0) <0){
      rb.removePos();
      p = rb.oldPos.front();
    }
    
    if (annotation_choice["direction"])
      display = annoteDirection( camera_information, rb, display, now);
    if (annotation_choice["position"])
      display = annotePosition(camera_information, rb , display, now);
    if (annotation_choice["trace"] && rb.getNumRobotInformation() == robottrace){
      display = annoteTrace(camera_information, rb, display, now);
    }
    if (annotation_choice["ball"] && rb.getNumRobotInformation() == robotball){
      display = annoteBall( camera_information, rb, display, now);
    }
    if (annotation_choice["target"] && rb.getNumRobotInformation() == robottarget){
      display = annoteTarget( camera_information, rb, display, now);
    }
    return display;
  }

  void Annotation::togglePositionChoice(){
    annotation_choice["position"] = !annotation_choice["position"];
  }

  void Annotation::toggleDirectionChoice(){
    annotation_choice["direction"] = !annotation_choice["direction"];
  }

  void Annotation::toggleTraceChoice(){
    annotation_choice["trace"] = !annotation_choice["trace"];
  }

  void Annotation::toggleBallChoice(){
    annotation_choice["ball"] = !annotation_choice["ball"];
  }

  void Annotation::changeRobotTrace(int robot){
    robottrace = robot;
  }

  int Annotation::getRobotTrace(){
    return robottrace;
  }

  void Annotation::changeRobotBall(int robot){
    robotball = robot;
  }

  int Annotation::getRobotBall(){
    return robotball;
  }
}

