#include <iostream>

namespace traitement
{

Robot::Robot(){
  penalty = 0;
  secs_till_unpenalised = 0;
}

Robot::~Robot(){
}

int Robot::getNumRobot() const{
  return numRobot;
}

int Robot::getPenalty() const{
  return penalty;
}

int Robot::getSecsTillUnpenalised() const{
  return secs_till_unpenalised;
}

int Robot::getYellowCardCount() const{
  return yellow_card_count;
}

int Robot::getRedCardCount() const{
  return red_card_count;
}

void Robot::update(){
  //TO DO
}


}
