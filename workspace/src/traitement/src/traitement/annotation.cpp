#include "traitement/annotation.h"


#include <hl_communication/utils.h>
#include <hl_monitoring/monitoring_manager.h>
#include <hl_monitoring/utils.h>
#include <hl_monitoring/field.h>
#include <traitement/position.h>
#include <traitement/direction.h>


#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <tclap/CmdLine.h>
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
    checkMember(root, "direction");
    checkMember(root["direction"], "write");
    checkMember(root, "trace");
    checkMember(root["trace"], "write");
    checkMember(root, "ball");
    checkMember(root["ball"], "write");

    annotation_choice["position"]=root["position"]["write"].asBool();
    annotation_choice["direction"]=root["direction"]["write"].asBool();
    annotation_choice["trace"]=root["trace"]["write"].asBool();
    annotation_choice["ball"]=root["ball"]["write"].asBool();

    checkMember(root["position"], "circle_size");
    checkMember(root["trace"], "circle_size");
    checkMember(root["direction"], "arrow_size");
    checkMember(root["trace"], "robot_num");
    checkMember(root["ball"], "ball_size");
    checkMember(root["ball"], "robot_num");


    sizecircle = root["position"]["circle_size"].asUInt();
    sizecircletrace = root["trace"]["circle_size"].asUInt();
    sizearrow = root["direction"]["arrow_size"].asUInt();
    robottrace = root["trace"]["robot_num"].asUInt();
    ballsize = root["ball"]["ball_size"].asUInt();
    robotball = root["ball"]["robot_num"].asUInt();

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


  cv::Mat  Annotation::annotePosition(CameraMetaInformation camera_information, RobotInformation rb ,cv::Mat display){
    Position pos;
    pos = rb.getPosRobot();
    cv :: Point3f pos_in_field(pos.x, pos.y, 0.0);
    cv :: Point2f pos_in_img = fieldToImg(pos_in_field, camera_information);
    // cv::Point2f pos_in_img(pos.x, pos.y);
    if (color.find(rb.getTeam())!=color.end())
      cv::circle(display,pos_in_img, sizecircle,  color[rb.getTeam()],cv::FILLED);
    else
      cv::circle(display,pos_in_img, sizecircle, cv::Scalar(0,0,0),cv::FILLED);

    return display;
  }
  cv::Mat  Annotation::annoteDirection( CameraMetaInformation camera_information, RobotInformation rb,cv::Mat display){
    Position pos;
    pos = rb.getPosRobot();
    Direction dir;
    dir = rb.getDirRobot();
    // calcul pos
    cv :: Point3f pos_in_field(pos.x, pos.y, 0.0);
    cv :: Point2f pos_in_img = fieldToImg(pos_in_field, camera_information);
    //calcul dir
    cv :: Point3f pos_in_fielddir(pos.x+cos(dir.mean), pos.y+sin(dir.mean), 0.0);
    cv :: Point2f pos_in_imgdir = fieldToImg(pos_in_fielddir, camera_information);
    /* reduction taille des flèches à une longueur de 50 pour que la taille des flèches soit homogène*/
    float hypo = sqrt((pos_in_imgdir.x - pos_in_img.x)*(pos_in_imgdir.x - pos_in_img.x) +(pos_in_imgdir.y- pos_in_img.y)*(pos_in_imgdir.y- pos_in_img.y));
    cv :: Point2f fleche;
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


  cv::Mat  Annotation::annoteTrace(CameraMetaInformation camera_information, RobotInformation rb,cv::Mat display){
    int qsize = rb.sizeOfQueue();
    for (int i = 0; i<qsize; i++){
      Position p;
      p = rb.getTraceRobot();
      //      display = annotePosition(p, camera_information, rb, display);
      cv :: Point3f pos_in_field(p.x, p.y, 0.0);
      cv :: Point2f pos_in_img = fieldToImg(pos_in_field, camera_information);
      cv::circle(display,pos_in_img, sizecircletrace, cv::Scalar(0,0,0),cv::FILLED);
    }
    return display;

  }

  cv::Mat  Annotation::annoteBall(CameraMetaInformation camera_information, RobotInformation rb,cv::Mat display){
    Position ball;
    ball = rb.getPosBall();
    Position robot;
    robot = rb.getPosRobot();
    Direction dir;
    dir = rb.getDirRobot();
    cv::Point2f position;
    position.x = robot.x + ball.x*cos(dir.mean)-ball.y*sin(dir.mean);
    position.y = robot.y + ball.x*sin(dir.mean)+ball.y*cos(dir.mean);
    cv :: Point3f pos_in_field(position.x, position.y, 0.0);
    cv :: Point2f pos_in_img = fieldToImg(pos_in_field, camera_information);
    cv::circle(display,pos_in_img, ballsize, cv::Scalar(125,125,125),cv::FILLED);
    return display;
  }


  cv::Mat Annotation::AddAnnotation( CameraMetaInformation camera_information, RobotInformation rb ,cv::Mat display){
    if (annotation_choice["position"])
      display = annotePosition(camera_information, rb , display);
    if (annotation_choice["direction"])
      display = annoteDirection( camera_information, rb, display);
    if (annotation_choice["trace"] && rb.getNumRobotInformation() == robottrace){
      display = annoteTrace(camera_information, rb, display);
    }
    if (annotation_choice["ball"] && rb.getNumRobotInformation() == robotball){
      display = annoteBall( camera_information, rb, display);
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


}

