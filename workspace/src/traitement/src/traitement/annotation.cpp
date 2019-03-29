#include "traitement/annotation.h"


#include <string>
#include <sstream>
#include <iostream>
#include <json/value.h>
#include <jsoncpp/json/json.h>
#include <fstream>


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
    checkMember(root, "ball");
    checkMember(root["ball"], "write");
    checkMember(root, "target");
    checkMember(root["target"], "write");
    checkMember(root["field"], "write");
    checkMember(root["score"], "write");
    checkMember(root, "optimized");


    annotation_choice["position"]=root["position"]["write"].asBool();
    annotation_choice["number"]=root["position"]["number"].asBool();
    annotation_choice["direction"]=root["direction"]["write"].asBool();
    annotation_choice["trace"]=root["trace"]["write"].asBool();
    annotation_choice["ball"]=root["ball"]["write"].asBool();
    annotation_choice["target"]=root["target"]["write"].asBool();
    annotation_choice["field"]=root["field"]["write"].asBool();
    annotation_choice["score"]=root["score"]["write"].asBool();
    annotation_choice["optimized"]=root["optimized"].asBool();
    
    
    checkMember(root, "delay_annotation");
    delay_annotation=root["delay_annotation"].asInt();


    checkMember(root["position"], "circle_size");
    checkMember(root["trace"], "circle_size");
    checkMember(root["direction"], "arrow_size");
    checkMember(root["trace"], "robot_num");
    checkMember(root["trace"], "team_num");
    checkMember(root["trace"], "delay_old_pos");
    checkMember(root["ball"], "ball_size");
    checkMember(root["ball"], "robot_num");
    checkMember(root["ball"], "team_num");
    checkMember(root["target"], "target_size");
    checkMember(root["target"], "dash_size");
    checkMember(root["target"], "robot_num");
    checkMember(root["target"], "team_num");
    checkMember(root["field"], "name");
    checkMember(root["score"], "x");
    checkMember(root["score"], "y");

    sizecircle = root["position"]["circle_size"].asUInt();
    sizenumber = sizecircle*0.75; //define to see better the numbers
    sizecircletrace = root["trace"]["circle_size"].asUInt();
    sizearrow = root["direction"]["arrow_size"].asUInt();
    robottrace = root["trace"]["robot_num"].asUInt();
    teamtrace = root["trace"]["team_num"].asUInt();
    delay_old_pos = root["trace"]["delay_old_pos"].asUInt();
    ballsize = root["ball"]["ball_size"].asUInt();
    robotball = root["ball"]["robot_num"].asUInt();
    teamball = root["ball"]["team_num"].asUInt();
    targetsize = root["target"]["target_size"].asUInt();
    dashsize = root["target"]["dash_size"].asUInt();
    robottarget = root["target"]["robot_num"].asUInt();
    teamtarget = root["target"]["team_num"].asUInt();
    f = root["field"]["name"].asString();
    field.loadFile(f);
    score_pos = cv::Point2f(root["score"]["x"].asUInt(),root["score"]["y"].asUInt());
    
    

    checkMember(root["color_team_1"], "num");
    checkMember(root["color_team_1"], "r");
    checkMember(root["color_team_1"], "b");
    checkMember(root["color_team_1"], "g");

    checkMember(root["color_team_2"], "num");
    checkMember(root["color_team_2"], "r");
    checkMember(root["color_team_2"], "b");
    checkMember(root["color_team_2"], "g");

    cv::Scalar color0 = {0,0,0};

    cv::Scalar color1 = {root["color_team_1"]["r"].asUInt(), root["color_team_1"]["g"].asUInt(), root["color_team_1"]["b"].asUInt()};

    cv::Scalar color2 = {root["color_team_2"]["r"].asUInt(), root["color_team_2"]["g"].asUInt(), root["color_team_2"]["b"].asUInt()};

    //0 for unkonwn teams because we initialize team_id = 0
    color[0]=color0;
    color[root["color_team_1"]["num"].asUInt()]=color1;
    color[root["color_team_2"]["num"].asUInt()]=color2;
  }

  Annotation::~Annotation(){
  }

  bool Annotation::IsMessageValid(uint64_t time_stamp, uint64_t now, int delay){
    return ((((delay*s_to_us-(now-time_stamp))/delay*s_to_us)>0 )); //seconds to microseconds
  }

  cv::Mat Annotation::annoteScore(std::map<int, Team>teams, cv::Mat display)
  {
    std::stringstream scoreString;
    for (std::map<int, Team>::iterator it=teams.begin();it != teams.end(); ++it)
      {	  
	if (it == teams.begin())
	  scoreString << teams[it->first].getScore();
	else
	  scoreString << "-" << teams[it->first].getScore();
      }
    std::string score = scoreString.str();
    
    cv::putText(display, score, score_pos, cv::FONT_HERSHEY_SIMPLEX, 1.5, color[0], 2);
    return display;
  }
  


  cv::Mat  Annotation::annotePosition(CameraMetaInformation camera_information,Position pos ,cv::Mat display,  uint64_t now){
    
    cv::Point3f pos_in_field(pos.x, pos.y, 0.0);
    cv::Point2f pos_in_img = fieldToImg(pos_in_field, camera_information);
    
    if (annotation_choice["optimized"]){
      float delay = delay_annotation*s_to_us;
      float opacity = (delay-(now-pos.time_stamp))/delay;
      if (opacity>0){
	cv::Mat overlay;
	display.copyTo(overlay);
	if (color.find(team_id)!=color.end())
	  cv::circle(overlay,pos_in_img, sizecircle, color[team_id],cv::FILLED);
	else
	  cv::circle(overlay,pos_in_img, sizecircle, color[0],cv::FILLED);
	cv::addWeighted(overlay,opacity, display, 1-opacity, 0,display);	
      }
    }
    
    else{
      if (color.find(team_id)!=color.end())
	cv::circle(display,pos_in_img, sizecircle, color[team_id],cv::FILLED);
      else
	cv::circle(display,pos_in_img, sizecircle, color[0],cv::FILLED);
    }
    
    if (annotation_choice["number"]){
      if (team_id == 0 || color.find(team_id)==color.end())
	cv::putText(display, std::to_string(id_robot), cv::Point2f(pos_in_img.x-sizenumber,pos_in_img.y+sizenumber), cv::FONT_HERSHEY_SIMPLEX ,sizenumber/10,cv::Scalar(255,255,255),2);
      else
	cv::putText(display, std::to_string(id_robot), cv::Point2f(pos_in_img.x-sizenumber,pos_in_img.y+sizenumber), cv::FONT_HERSHEY_SIMPLEX ,sizenumber/10,cv::Scalar(0,0,0),2);
    }
    
    return display;
  }

  cv::Mat  Annotation::annoteTarget(CameraMetaInformation camera_information,RobotMsg robot ,Position pos_target,cv::Mat display,  uint64_t now){
    
    cv::Point3f pos_in_field(pos_target.x, pos_target.y, 0.0);
    cv::Point2f pos_in_img = fieldToImg(pos_in_field, camera_information);

    
    if (annotation_choice["optimized"]){
      float delay = delay_annotation*s_to_us;
      float opacity = (delay-(now-pos_target.time_stamp))/delay;
      if (opacity>0){
	
	cv::Mat overlay;
	display.copyTo(overlay);
	cv::Scalar s = color[0];
	if (color.find(team_id)!=color.end())
	  s = color[team_id];
	
	// s/2 gives us the color but darker
	cv::drawMarker (overlay,pos_in_img, cv::Scalar(s[0]/2,s[1]/2,s[2]/2), cv::MARKER_TILTED_CROSS, targetsize, 2, 8);
	
	if (robot.has_perception()) {
	  
	  const Perception & perception = robot.perception();
	  const WeightedPose & weighted_pose = perception.self_in_field(0);
	  
	  if (weighted_pose.pose().has_position())
	    {
	      const PositionDistribution & position = weighted_pose.pose().position();
	      
	      cv::Point3f pos_in_fieldr(position.x(),position.y(), 0.0);
	      cv::Point2f pos_in_imgr = fieldToImg(pos_in_fieldr, camera_information);
	      
	      cv::LineIterator it(overlay, pos_in_img, pos_in_imgr, 8);            
	      for(int i = 0; i < it.count; i++,it++)
		if ( i%dashsize*2<dashsize ) {
		  (*it)[0] = s[0]/2;
		  (*it)[1] = s[1]/2;
		  (*it)[2] = s[2]/2;
		}
	    }
	}
	cv::addWeighted(overlay,opacity, display, 1-opacity, 0,display);
      }
  
    }
    
    
    else{

      cv::Scalar s = color[0];
      if (color.find(team_id)!=color.end())
	s = color[team_id];
      cv::drawMarker (display,pos_in_img, cv::Scalar(s[0]/2,s[1]/2,s[2]/2), cv::MARKER_TILTED_CROSS, targetsize, 2, 8);
    
      if (robot.has_perception()) {
      
	const Perception & perception = robot.perception();
	const WeightedPose & weighted_pose = perception.self_in_field(0);
      
	if (weighted_pose.pose().has_position())
	  {
	    const PositionDistribution & position = weighted_pose.pose().position();
	  
	    cv::Point3f pos_in_fieldr(position.x(),position.y(), 0.0);
	    cv::Point2f pos_in_imgr = fieldToImg(pos_in_fieldr, camera_information);
	  
	    cv::LineIterator it(display, pos_in_img, pos_in_imgr, 8);            
	    for(int i = 0; i < it.count; i++,it++)
	      if ( i%dashsize*2<dashsize ) {
		(*it)[0] = s[0]/2;
		(*it)[1] = s[1]/2;
		(*it)[2] = s[2]/2;
	      }
	  }
      }
    }
  
    return display;
  }

  cv::Mat  Annotation::annoteDirection( CameraMetaInformation camera_information,  Direction dir, Position pos,cv::Mat display, uint64_t now){

    cv::Point3f pos_in_field(pos.x, pos.y, 0.0);
    cv::Point2f pos_in_img = fieldToImg(pos_in_field, camera_information);

    cv::Point3f pos_in_fielddir(pos.x+cos(dir.mean), pos.y+sin(dir.mean), 0.0);
    cv::Point2f pos_in_imgdir = fieldToImg(pos_in_fielddir, camera_information);
    /*reducing size of arrows for homogeneity*/
    float hypo = sqrt((pos_in_imgdir.x - pos_in_img.x)*(pos_in_imgdir.x - pos_in_img.x) +(pos_in_imgdir.y- pos_in_img.y)*(pos_in_imgdir.y- pos_in_img.y));
    cv::Point2f fleche;
    fleche.x =  pos_in_img.x + (sizearrow*(pos_in_imgdir.x - pos_in_img.x)/hypo);
    fleche.y= pos_in_img.y + (sizearrow*(pos_in_imgdir.y- pos_in_img.y)/hypo);
    
    
    if (annotation_choice["optimized"]){
      float delay = delay_annotation*s_to_us;
      float opacity = (delay-(now-dir.time_stamp))/delay;
      if (opacity>0){
	cv::Mat overlay;
	display.copyTo(overlay);
	/* If the direction is > 2*PI, we draw the arrow in black*/
	if (dir.mean > 2*CV_PI)
	  cv :: arrowedLine(overlay, pos_in_img, fleche, cv::Scalar(0,0,0), 2, 0, 0.1);
	else
	  if (color.find(team_id)!=color.end())
	    cv :: arrowedLine(overlay, pos_in_img, fleche, color[team_id], 2, 0, 0.1);
	  else
	    cv :: arrowedLine(overlay, pos_in_img, fleche, color[0], 2, 0, 0.1);
       
	cv::addWeighted(overlay,opacity, display, 1-opacity, 0,display);
      }
    }
    else {
      if (dir.mean > 2*CV_PI)
	cv :: arrowedLine(display, pos_in_img, fleche, cv::Scalar(0,0,0), 2, 0, 0.1);
      else
	if (color.find(team_id)!=color.end())
	  cv :: arrowedLine(display, pos_in_img, fleche, color[team_id], 2, 0, 0.1);
	else
	  cv :: arrowedLine(display, pos_in_img, fleche, color[0], 2, 0, 0.1);
    }
    return display;

  }


  
  cv::Mat  Annotation::annoteTrace(CameraMetaInformation camera_information,  RobotInformation robot,cv::Mat display,  uint64_t now){

    Position old_pos;
    
    for (std::map<uint64_t, Position>::iterator it=robot.robot_trace.begin(); it!=robot.robot_trace.end(); ++it)
      {
	Position p;
	p = robot.getTraceRobot(it->first);
	
	if (annotation_choice["optimized"]){
	
	  cv::Mat overlay;
	  display.copyTo(overlay);
	 
	  float delay = delay_old_pos*s_to_us;
	  float opacity = (delay-(now-p.time_stamp))/delay;
	  // we do not draw the position if more than the half is covered by the old one
	  if(it==robot.robot_trace.begin()|| (it!=robot.robot_trace.begin() && (abs(old_pos.x-p.x)>sizecircletrace*1.5/100 || abs(old_pos.y-p.y)>sizecircletrace*1.5/100))){
	    old_pos=p;
	    cv::Point3f pos_in_field(p.x, p.y, 0.0);
	    cv::Point2f pos_in_img = fieldToImg(pos_in_field, camera_information);
	    
	    cv::Scalar s = color[0];
	    if (color.find(team_id)!=color.end())
	      s = color[team_id];
	    cv::circle(overlay,pos_in_img, sizecircletrace, cv::Scalar(s[0]/2,s[1]/2,s[2]/2),cv::FILLED);
	  	    
	    cv::addWeighted(overlay,opacity, display,1-opacity, 0,display);
	  }
	  
	}
	else{
	  
	  Position p;
	  p = robot.getTraceRobot(it->first);
	  
	  cv::Point3f pos_in_field(p.x, p.y, 0.0);
	  cv::Point2f pos_in_img = fieldToImg(pos_in_field, camera_information);
	  cv::Scalar s = color[0];
	  if (color.find(team_id)!=color.end())
	    s = color[team_id];
	  cv::circle(display,pos_in_img, sizecircletrace, cv::Scalar(s[0]/2,s[1]/2,s[2]/2),cv::FILLED);
	 
	}
      }
    

  
    return display;
  
  }

  cv::Mat  Annotation::annoteBall(CameraMetaInformation camera_information, Position pos_ball, Position pos, Direction dir,cv::Mat display,  uint64_t now){
    cv::Point2f position;
    position.x = pos.x +  pos_ball.x*cos(dir.mean)- pos_ball.y*sin(dir.mean);
    position.y = pos.y +  pos_ball.x*sin(dir.mean)+ pos_ball.y*cos(dir.mean);
    cv::Point3f pos_in_field(position.x, position.y, 0.0);
    cv::Point2f pos_in_img = fieldToImg(pos_in_field, camera_information);

    if (annotation_choice["optimized"]){
      float delay = delay_annotation*s_to_us;
      float opacity = (delay-(now- pos_ball.time_stamp))/delay;
      if (opacity>0)
	{
	  cv::Mat overlay;
	  display.copyTo(overlay);
	  cv::circle(overlay,pos_in_img, ballsize, cv::Scalar(125,125,125),cv::FILLED);
	  
	  cv::addWeighted(overlay,opacity, display, 1-opacity, 0,display);
	}
    }

    else{
      cv::circle(display,pos_in_img, ballsize, cv::Scalar(125,125,125),cv::FILLED);
    }
  
    return display;
  }


  cv::Mat Annotation::AddAnnotation( CameraMetaInformation camera_information, RobotInformation robot ,cv::Mat display,  uint64_t now){
    
    if (!robot.robot_trace.empty()){
      
      uint64_t limit_time = now-(delay_old_pos*s_to_us);
      //erase too old position
      for (std::map<uint64_t, Position>::iterator it=robot.robot_trace.begin();(it->first<=limit_time && it->first != 0); ++it)
	{	  
	  robot.removeOnePos(it->first);
	}
     
      //erase too recent position (if we go back in the video)
      auto it=robot.robot_trace.upper_bound(now);
      if (it!=robot.robot_trace.end())
	{ 
	  robot.removeFiewPos(now);
	}
      
      
    }
    	
    RobotMsg rb = robot.getMessageRobot();
    
    team_id = robot.team;
    id_robot = robot.robot;

    
    if (annotation_choice["trace"] && id_robot == robottrace &&  team_id == teamtrace && !robot.getRobotTrace().empty()){
      display = annoteTrace(camera_information, robot, display, now);
    }
  
    if (IsMessageValid(rb.time_stamp(), now, delay_annotation))
      {
	if (rb.has_perception()) {
	  const Perception & perception = rb.perception();
	  const WeightedPose & weighted_pose = perception.self_in_field(0);	  
	  if (annotation_choice["position"]&& weighted_pose.pose().has_position())
	    {
	      const PositionDistribution & position = weighted_pose.pose().position();
	      Position pos;
	      pos.setPosition(position.x(),position.y(), rb.time_stamp());
	      
	      if (annotation_choice["direction"]&& weighted_pose.pose().has_dir())
		{
		  const AngleDistribution & dir = weighted_pose.pose().dir();
		  Direction direction;
		  direction.SetMean(dir.mean(),rb.time_stamp());		  
		  display = annoteDirection( camera_information,direction, pos, display, now);
		      
		  if (annotation_choice["ball"] &&  team_id == teamball && id_robot == robotball){
		    const PositionDistribution & ball = perception.ball_in_self();
		    Position pos_ball;
		    pos_ball.setPosition(ball.x(), ball.y(), rb.time_stamp());
		    display = annoteBall( camera_information, pos_ball, pos, direction , display, now);
		  }
		  
		}
	      /*Position is the last add on the image*/
	      display = annotePosition(camera_information, pos, display, now);
	    }
	  
	}

		

    
	if (rb.has_intention()) {
	  const Intention & intention = rb.intention();
	  if (annotation_choice["target"] &&  team_id == teamtarget && id_robot == robottarget && intention.has_target_pose_in_field()){
	    const PositionDistribution & target_pos = intention.target_pose_in_field().position();
	    Position pos_target;
	    pos_target.setPosition(target_pos.x(),target_pos.y(), rb.time_stamp());
	    display = annoteTarget(camera_information,  rb, pos_target, display, now);
	  }
	}
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

  void Annotation::toggleTargetChoice(){
    annotation_choice["target"] = !annotation_choice["target"];
  }

  void Annotation::changeRobotTrace(int team , int robot){
    teamtrace = team;
    robottrace = robot;
  }

  int Annotation::getRobotTrace(){
    return robottrace;
  }

  int Annotation::getTeamTrace(){
    return teamtrace;
  }

  void Annotation::changeRobotBall(int team, int robot){
    teamball = team;
    robotball = robot;
  }

  int Annotation::getRobotBall(){
    return robotball;
  }

  int Annotation::getTeamBall(){
    return teamball;
  }

  void Annotation::changeRobotTarget(int team, int robot){
    teamtarget = team;
    robottarget = robot;
  }

  int Annotation::getRobotTarget(){
    return robottarget;
  }

  int Annotation::getTeamTarget(){
    return teamtarget;
  }
}
