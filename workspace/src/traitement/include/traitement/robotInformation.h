#pragma once

#include <traitement/position.h>
#include <queue>          // std::queue


namespace traitement
{

  //Class similaire à celle présente dans hl_communication
  class RobotInformation {

  public:
    int numRobotInformation;
    std::queue<Position> oldPos;
    RobotInformation();
    ~RobotInformation();

    

    int getNumRobotInformation() const;
    int getPenalty() const;
    int getSecsTillUnpenalised() const;
    int getYellowCardCount() const;
    int getRedCardCount() const;
    int sizeOfQueue();
    Position getPosition();
    void update(Position pos);

  private:
    
    int penalty;
    int secs_till_unpenalised;
    int yellow_card_count;
    int red_card_count;



  };

}
