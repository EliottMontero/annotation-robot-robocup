#pragma once

#include <traitement/position.h>
#include <traitement/direction.h>
#include <queue>       

namespace traitement
{
  /* Dans cette classe, nous stockons les informations des robots que l'on obtient dans les messages*/
  class RobotInformation {

  public:
    int numRobotInformation;

    RobotInformation();
    ~RobotInformation();
    void setTeam(int team_id);
    void setnumRobotInformation(int robot_id);
    void setPosRobot(Position pos);
    void setPosBall(Position pos);
    void setPosTarget(Position pos);
    void setDirRobot(Direction dir);
    Position getPosBall();
    Position getPosRobot();
    Position getPosTarget();
    Direction getDirRobot();
    int getTeam() const;
    int getNumRobotInformation() const;
    int getPenalty() const;
    int getSecsTillUnpenalised() const;
    int getYellowCardCount() const;
    int getRedCardCount() const;

    /*pour afficher les x anciennes positions du robot afin d'avoir la trace du robot*/
    std::queue<Position> oldPos;
    int sizeOfQueue();
    /*retourne la position en haut de la queue, la place en bas de la queue
     lorsqu'on lance cette fonction on la lance sizeOfQueue fois,
    le fait de lire le haut et de le mettre ensuite au bout de la queue
    nous permet de lire chaque position une fois puis de revenir comme avant la lecture*/
    Position getTraceRobot();
    /*ajoute une position dans la queue*/
    void stockPos(Position pos);
    void removePos();

  private:
    int team;
    int penalty;
    int secs_till_unpenalised;
    int yellow_card_count;
    int red_card_count;
    Position ball_pos;
    Position robot_pos;
    Position target_pos;
    Direction robot_dir;



  };

}
