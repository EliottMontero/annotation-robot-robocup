#pragma once

#include <hl_communication/utils.h>
#include <hl_communication/message_manager.h>
#include <hl_monitoring/monitoring_manager.h>
#include <hl_monitoring/utils.h>
#include <hl_monitoring/field.h>
#include <traitement/robotInformation.h>
#include <traitement/position.h>
#include <traitement/direction.h>
#include <hl_communication/wrapper.pb.h>

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <google/protobuf/message.h>


using namespace hl_monitoring;
using namespace hl_communication;


namespace traitement{
  class Annotation{
  public:
    std::map<std::string, bool>annotation_choice;
    int delay_annotation;
    uint32_t team_id = 0;
    uint32_t id_robot = 0;
    float s_to_us = 1000000.0;
  
   
    

    int sizecircle;
    float sizenumber = 0.0;
    int sizecircletrace;
    int sizearrow;
    int robottrace;
    int teamtrace;
    int delay_old_pos;
    int ballsize;
    int robotball;
    int teamball;
    int targetsize;
    int dashsize;
    int robottarget;
    int teamtarget;
    std::map<int ,cv::Scalar> color;


    Annotation(std::string);
    ~Annotation();

    bool IsMessageValid(uint64_t time_stamp, uint64_t now, int delay);

    cv::Mat annotePosition(CameraMetaInformation camera_information,Position pos,cv::Mat display,  uint64_t now);
    cv::Mat annoteTarget(CameraMetaInformation camera_information, RobotMsg robot, Position pos_target ,cv::Mat display,  uint64_t now);
    cv::Mat annoteDirection(CameraMetaInformation camera_information, Direction dir, Position pos,cv::Mat display,  uint64_t now);
    cv::Mat annoteTrace(CameraMetaInformation camera_information, RobotInformation robot,cv::Mat display,  uint64_t now);
    cv::Mat annoteBall(CameraMetaInformation camera_information, Position pos_ball, Position pos, Direction dir,cv::Mat display,  uint64_t now);
    cv::Mat AddAnnotation(CameraMetaInformation camera_information,  RobotInformation robot,cv::Mat display,  uint64_t now);

    /*this is for the interface*/
    void togglePositionChoice();
    void toggleDirectionChoice();
    void toggleTraceChoice();
    void toggleBallChoice();
    void toggleTargetChoice();

    void changeRobotTrace(int team, int robot);
    int getRobotTrace();
    int getTeamTrace();
    void changeRobotBall(int team, int robot);
    int getRobotBall();
    int getTeamBall();
    void changeRobotTarget(int team, int robot);
    int getRobotTarget();
    int getTeamTarget();


  };
}
