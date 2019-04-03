#pragma once

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QSlider>
#include <QPushButton>

#include <hl_communication/message_manager.h>
#include <hl_monitoring/monitoring_manager.h>

#include <annotateImage/annotation.h>

#include "teamPanel.h"
#include "choiceDialog.h"

using namespace cv;
using namespace hl_communication;
using namespace hl_monitoring;
using namespace annotateImage;


class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow();
  void createTeam(int id);
  void createRobot(int robotId, int teamId);

public slots:
  void changeImage();
  void annotationChoice();
  void togglePause();
  void toggleFastForward();
  void togglePosition();
  void toggleDirection();
  void toggleTrace();
  void toggleBall();
  void toggleTarget();

private :
  MonitoringManager manager;
  Annotation *annotation;
  Field field;

  QLabel * labelVideo;
  Mat * gamePicture;

  QTimer * timer;
  uint64_t now;
  uint64_t initialTime;
  uint64_t endTime;
  uint64_t dt;

  std::map<int, Team> teams;
  std::map<int, TeamPanel*> teamPanels;

  QWidget * zoneCentral;
  QGridLayout * layout;

  QPushButton * buttonAnnotationChoice;
  QPushButton * buttonPause;
  QPushButton * buttonFastForward;

  QSlider * slider;
  QLabel * sliderValue;
  int oldSliderValue;

  bool boolPause;
  bool boolFastForward;
  bool boolPosition;
  bool boolDirection;
  bool boolTrace;
  bool boolBall;
  bool boolTarget;

protected:
  void resizeEvent(QResizeEvent *event) override;

};

#endif // MAINWINDOW_H
