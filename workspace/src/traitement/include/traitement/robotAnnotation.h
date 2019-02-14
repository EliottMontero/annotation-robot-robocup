#pragma once

#include

namespace traitement
{

  //Class similaire à celle présente dans hl_communication
  class RobotInformation {

  public:
    RobotInformation()
    ~RobotInformation();

    int getNumRobot() const;
    int getPenalty() const;
    int getSecsTillUnpenalised() const;
    int getYellowCardCount() const;
    int getRedCardCount() const;

    void update();

  private:
    int numRobot;
    int penalty;
    int secs_till_unpenalised;
    int yellow_card_count;
    int red_card_count;



  };

}
