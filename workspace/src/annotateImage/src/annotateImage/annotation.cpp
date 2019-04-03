#include "annotateImage/annotation.h"

namespace annotateImage{

Annotation::Annotation(std::string file){
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
  checkMember(root, "transparency");


  annotation_choice["position"]=root["position"]["write"].asBool();
  annotation_choice["number"]=root["position"]["number"].asBool();
  annotation_choice["direction"]=root["direction"]["write"].asBool();
  annotation_choice["trace"]=root["trace"]["write"].asBool();
  annotation_choice["ball"]=root["ball"]["write"].asBool();
  annotation_choice["target"]=root["target"]["write"].asBool();
  annotation_choice["field"]=root["field"]["write"].asBool();
  annotation_choice["score"]=root["score"]["write"].asBool();
  annotation_choice["transparency"]=root["transparency"].asBool();


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

  score_pos = cv::Point2f(root["score"]["x"].asUInt(),
                          root["score"]["y"].asUInt());

  checkMember(root["color_team_1"], "num");
  checkMember(root["color_team_1"], "r");
  checkMember(root["color_team_1"], "b");
  checkMember(root["color_team_1"], "g");

  checkMember(root["color_team_2"], "num");
  checkMember(root["color_team_2"], "r");
  checkMember(root["color_team_2"], "b");
  checkMember(root["color_team_2"], "g");

  cv::Scalar color0 = {0,0,0};

  cv::Scalar color1 = {root["color_team_1"]["b"].asUInt(),
                       root["color_team_1"]["g"].asUInt(),
                       root["color_team_1"]["r"].asUInt()};

  cv::Scalar color2 = {root["color_team_2"]["b"].asUInt(),
                       root["color_team_2"]["g"].asUInt(),
                       root["color_team_2"]["r"].asUInt()};

  //0 for unkonwn teams because we initialize team_id = 0
  color[0]=color0;
  color[root["color_team_1"]["num"].asUInt()]=color1;
  color[root["color_team_2"]["num"].asUInt()]=color2;
}

Annotation::~Annotation(){
}

bool Annotation::isMessageValid(uint64_t time_stamp,
                                uint64_t now, int delay){
  //seconds to microseconds
  return ((((delay*s_to_us-(now-time_stamp))/delay*s_to_us)>0 ));
}

bool Annotation::isPosValid(cv::Point2f pos, int img_x, int img_y){
  return (pos.x <= img_x && pos.y <= img_y);
}

cv::Mat Annotation::annoteScore(std::map<int, Team>teams, cv::Mat display){
  std::stringstream scoreString;
  for (auto it=teams.begin();it != teams.end(); ++it){
    if (it == teams.begin())
      scoreString << teams[it->first].getScore();
    else
      scoreString << "-" << teams[it->first].getScore();
  }

  std::string score = scoreString.str();
  cv::putText(display, score, score_pos, cv::FONT_HERSHEY_SIMPLEX,
              1.5, color[0], 2);
  return display;
}

cv::Mat  Annotation::annotePosition(CameraMetaInformation camera_information,
                                    Position pos,
                                    cv::Mat display,
                                    uint64_t now){

  cv::Point3f pos_in_field(pos.x, pos.y, 0.0);
  cv::Point2f pos_in_img = fieldToImg(pos_in_field,camera_information);

  if (isPosValid(pos_in_img, display.cols, display.rows)){

    auto colorAnnotation = color[0];
    if (color.find(team_id)!=color.end())
    colorAnnotation = color[team_id];

    if (annotation_choice["transparency"]){
      float delay = delay_annotation*s_to_us;
      float opacity = (delay-(now-pos.time_stamp))/delay;
      if (opacity > 0){
        cv::Mat overlay;
        display.copyTo(overlay);
        cv::circle(overlay, pos_in_img, sizecircle,
                   colorAnnotation, cv::FILLED);
        cv::addWeighted(overlay, opacity, display,
                        1-opacity, 0, display);
      }
    }
    else{
      cv::circle(display,pos_in_img, sizecircle,
                 colorAnnotation, cv::FILLED);
    }

    if (annotation_choice["number"]){
      auto colorText = cv::Scalar(0,0,0);
      if (team_id == 0 || color.find(team_id)==color.end())
      colorText = cv::Scalar(255,255,255);

      cv::putText(display, std::to_string(id_robot),
      cv::Point2f(pos_in_img.x-sizenumber,pos_in_img.y+sizenumber),
      cv::FONT_HERSHEY_SIMPLEX, sizenumber/10, colorText, 2);
    }
  }

  return display;
}


cv::Mat Annotation::annoteTarget(CameraMetaInformation camera_information,
                                 RobotMsg robot , Position pos_target,
                                 cv::Mat display, uint64_t now){

  cv::Point3f pos_in_field(pos_target.x, pos_target.y, 0.0);
  cv::Point2f pos_in_img = fieldToImg(pos_in_field, camera_information);

  if (isPosValid(pos_in_img, display.cols, display.rows)){

    cv::Scalar s = color[0];
    if (color.find(team_id)!=color.end())
    s = 0.5 * color[team_id]; //s*0.5 gives us the color but darker

    float opacity = 1;
    cv::Mat overlay;
    if (annotation_choice["transparency"]){
      float delay = delay_annotation*s_to_us;
      opacity = (delay-(now-pos_target.time_stamp))/delay;

      if (opacity > 0){
        display.copyTo(overlay);
        cv::drawMarker(overlay,pos_in_img, s, cv::MARKER_TILTED_CROSS,
                       targetsize, 2, 8);
        }
      }
      else {
      cv::drawMarker (display,pos_in_img, s, cv::MARKER_TILTED_CROSS,
        targetsize, 2, 8);
      }

      if (robot.has_perception() && opacity > 0) {
        const Perception & perception = robot.perception();
        const WeightedPose & weighted_pose = perception.self_in_field(0);

        if (weighted_pose.pose().has_position()){
          const PositionDistribution & position =
                                          weighted_pose.pose().position();

          cv::Point3f pos_in_fieldr(position.x(),position.y(), 0.0);
          cv::Point2f pos_in_imgr =
                                fieldToImg(pos_in_fieldr, camera_information);

          cv::LineIterator it = cv::LineIterator(display, pos_in_img,
                                                 pos_in_imgr, 8);

          if(annotation_choice["transparency"])
            it = cv::LineIterator(overlay, pos_in_img, pos_in_imgr, 8);

          for(int i = 0; i < it.count; i++,it++)
            if (i%(dashsize*2)>=dashsize){
              (*it)[0] = s[0];
              (*it)[1] = s[1];
              (*it)[2] = s[2];
            }
          }

          if(annotation_choice["transparency"])
            cv::addWeighted(overlay,opacity, display, 1-opacity, 0,display);
        }
    }
    return display;
}

cv::Mat Annotation::annoteDirection(CameraMetaInformation camera_information,
                                    Direction dir, Position pos,
                                    cv::Mat display, uint64_t now){

  cv::Point3f pos_in_field(pos.x, pos.y, 0.0);
  cv::Point2f pos_in_img = fieldToImg(pos_in_field, camera_information);
  if (isPosValid(pos_in_img, display.cols, display.rows)){

    cv::Point3f pos_in_fielddir(pos.x+cos(dir.mean), pos.y+sin(dir.mean), 0.0);
    cv::Point2f pos_in_imgdir = fieldToImg(pos_in_fielddir, camera_information);
    /*reducing size of arrows for homogeneity*/
    float hypo = sqrt(
                   (pos_in_imgdir.x - pos_in_img.x)
                  *(pos_in_imgdir.x - pos_in_img.x)
                 + (pos_in_imgdir.y- pos_in_img.y)
                  *(pos_in_imgdir.y- pos_in_img.y)
                );

    cv::Point2f fleche;
    fleche.x = pos_in_img.x + (sizearrow*(pos_in_imgdir.x - pos_in_img.x)/hypo);
    fleche.y = pos_in_img.y + (sizearrow*(pos_in_imgdir.y- pos_in_img.y)/hypo);

    cv::Scalar colorAnnotation = color[0];
    if (color.find(team_id)!=color.end())
    colorAnnotation = color[team_id];
    /* If the direction is > 2*PI, we draw the arrow in black*/
    if (dir.mean > 2*CV_PI)
    colorAnnotation = color[0];

    if (annotation_choice["transparency"]){
      float delay = delay_annotation*s_to_us;
      float opacity = (delay-(now-dir.time_stamp))/delay;
      if (opacity > 0){
        cv::Mat overlay;
        display.copyTo(overlay);
        cv::arrowedLine(overlay, pos_in_img, fleche, colorAnnotation,
                        2, 0, 0.1);
        cv::addWeighted(overlay, opacity, display,
                        1-opacity, 0, display);
      }
    }
    else {
      cv::arrowedLine(display, pos_in_img, fleche, colorAnnotation, 2, 0, 0.1);
    }
  }

  return display;
}


cv::Mat Annotation::annoteTrace(CameraMetaInformation camera_information,
                                std::map<uint64_t, Position> trace,
                                cv::Mat display, uint64_t now){

  cv::Point2f old_pos;
  std::map<uint64_t, Position>::iterator it;
  uint64_t limit_time = now-(delay_old_pos*s_to_us);

  for (it = trace.begin(); it != trace.end(); ++it){
    if(it->first >= limit_time && it->first<= now){
      Position p;
      p = trace[it->first];

      cv::Scalar s = color[0];
      if (color.find(team_id)!=color.end())
      s = 0.5 * color[team_id];

      cv::Point3f pos_in_field(p.x, p.y, 0.0);
      cv::Point2f pos_in_img = fieldToImg(pos_in_field, camera_information);
      if (isPosValid(pos_in_img, display.cols, display.rows)){

        if (annotation_choice["transparency"]){
          float delay = delay_old_pos*s_to_us;
          float opacity = (delay-(now-p.time_stamp))/delay;

          cv::Mat overlay;
          display.copyTo(overlay);

          // we do not draw the position if it is covered by the old one

          if(it == trace.begin()
                        || (abs(old_pos.x-pos_in_img.x) > sizecircletrace*3/4
                        || abs(old_pos.y-pos_in_img.y)>sizecircletrace*3/4)){
            old_pos=pos_in_img;
            cv::circle(overlay, pos_in_img, sizecircletrace, s, cv::FILLED);
            cv::addWeighted(overlay, opacity, display, 1-opacity, 0, display);
          }
        }
        else{
          cv::circle(display, pos_in_img, sizecircletrace, s, cv::FILLED);
        }
      }
    }
  }

  return display;
}


cv::Mat  Annotation::annoteBall(CameraMetaInformation camera_information,
                                Position pos_ball, Position pos, Direction dir,
                                cv::Mat display,  uint64_t now){

  cv::Point2f position;
  position.x = pos.x +  pos_ball.x*cos(dir.mean)- pos_ball.y*sin(dir.mean);
  position.y = pos.y +  pos_ball.x*sin(dir.mean)+ pos_ball.y*cos(dir.mean);

  cv::Point3f pos_in_field(position.x, position.y, 0.0);
  cv::Point2f pos_in_img = fieldToImg(pos_in_field, camera_information);

  if (isPosValid(pos_in_img, display.cols, display.rows)){

    cv::Scalar colorBall = cv::Scalar(125,125,125);

    if (annotation_choice["transparency"]){
      float delay = delay_annotation*s_to_us;
      float opacity = (delay-(now- pos_ball.time_stamp))/delay;
      if (opacity > 0){
        cv::Mat overlay;
        display.copyTo(overlay);
        cv::circle(overlay, pos_in_img, ballsize, colorBall, cv::FILLED);
        cv::addWeighted(overlay, opacity, display, 1-opacity, 0, display);
      }
    }
    else{
      cv::circle(display, pos_in_img, ballsize, colorBall, cv::FILLED);
    }
  }

  return display;
}

cv::Mat Annotation::addAnnotation(CameraMetaInformation camera_information,
                                  RobotInformation robot,
                                  cv::Mat display, uint64_t now){

  RobotMsg rb = robot.getMessageRobot();
  team_id = robot.team;
  id_robot = robot.robot;

  if (annotation_choice["trace"] && id_robot == robottrace
        &&  team_id == teamtrace && !robot.getRobotTrace().empty()){
    display = annoteTrace(camera_information, robot.getRobotTrace(),
                          display, now);
  }

  if (isMessageValid(rb.time_stamp(), now, delay_annotation)){
    if (rb.has_perception()) {
      const Perception & perception = rb.perception();
      const WeightedPose & weighted_pose = perception.self_in_field(0);

      if (weighted_pose.pose().has_position()){
        const PositionDistribution & position = weighted_pose.pose().position();
        Position pos;
        pos.setPosition(position.x(),position.y(), rb.time_stamp());

        if (weighted_pose.pose().has_dir()){
          const AngleDistribution & dir = weighted_pose.pose().dir();
          Direction direction;
          direction.setMean(dir.mean(),rb.time_stamp());
          if(annotation_choice["direction"])
          display = annoteDirection(camera_information, direction, pos,
                                    display, now);

          if (annotation_choice["ball"] &&  team_id == teamball
                                        && id_robot == robotball){
            const PositionDistribution & ball = perception.ball_in_self();
            Position pos_ball;
            pos_ball.setPosition(ball.x(), ball.y(), rb.time_stamp());
            display = annoteBall(camera_information, pos_ball, pos, direction,
                                 display, now);
          }
        }
        /*Position is the last add on the image*/
        if(annotation_choice["position"])
          display = annotePosition(camera_information, pos, display, now);
      }
    }

    if (rb.has_intention()) {
      const Intention & intention = rb.intention();

      if (annotation_choice["target"] &&  team_id == teamtarget
           && id_robot == robottarget && intention.has_target_pose_in_field()){
        const PositionDistribution & target_pos =
                                    intention.target_pose_in_field().position();
        Position pos_target;
        pos_target.setPosition(target_pos.x(),target_pos.y(), rb.time_stamp());
        display = annoteTarget(camera_information,rb,pos_target,display,now);
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
