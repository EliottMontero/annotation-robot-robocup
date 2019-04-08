### ANNOTATIONS-robot

/!\------/!\

If you had a version of this project older than 15/02/2019, you will lose the
2vs1/ folder by getting any newer version, you will then need to download it
if you want to test the project once again, see the INSTALLATION part.

/!\------/!\

The goal of this project is to display robots logs on a video from a football
match of robots,, which is the equivalent of "Augmented Logs", and helps
understand the logs' content through what happens in the video, and the robots'
behaviour.
There are two important packages in this project : annotateImage and interface, where
annotateImage is in charge of treating images to display the informations, and
interface is in charge of the UI and using the annotateImage package to display the
modified video.

# INSTALLATION

In order to install the project, there are several steps :

- You'll first need several packages : pip, libprotobuf-dev, protobuf-compiler,
libjsoncpp-dev, libtclap-dev, qtmultimedia5-dev, libopencv, gcc, cmake, git :

      sudo apt install pip libprotobuf-dev protobuf-compiler libjsoncpp-dev\
      libtclap-dev qtmultimedia5-dev libopencv gcc cmake git

- Install catkin_tools through pip :

      pip install -U catkin_tools

- Import the project's submodules :

      git submodule init
      git submodule update

Now you should have every dependencies needed to make this project work.
You may need to download QT 5.9.5 from the official website :
https://www.qt.op/download to build the interface.

You can add the 2vs1/ folder (which contains videos and logs of a match 2 robots
vs 1 robot) if you want to try the project in the repository next to the
workspace/ folder by downloading the following google drive :

https://drive.google.com/drive/folders/1euuFUpFEmJQSxWQ8DGrLt1pGioKHVwfI?usp=sharing

At the root of the project you should now have the following folders : 2vs1/,
rapports/, memoire/, and workspace/.


# BUILDING THE PROJECT

To build the project you'll need to be in the workspace/ folder.
For the first build you'll first need to input the following line :

      catkin init

Which will initialize the catkin settings on your device.
Then, every time you want to build the project, go to the workspace/ folder
and input the following line :

      catkin build

If you want a package's executables files, you'll need to go to
workspace/build/<package_name> and change the value of BUILD_TRAITEMENT_TOOLS:BOOL
to ON in the CMakeCache.txt file.

# LAUNCH annotateImage

In order to launch the annotateImage, you'll need a folder containing the video
files and logs, see the INSTALLATION part to download the given files (2vs1/ or
2019_03_21/ for instance) containing examples of the needed files to test the project.

To lauch the annotateImage main executable, go to a given folder with video
and input the following line :

      ../workspace/devel/lib/annotateImage/main_annotateImage

All settings can be changed in annotation_settings.json or match_settings.json.

If you change the value of the BUILD_ANNOTATEIMAGE_TOOLS:BOOL to ON in
workspace/build/annotateImage/CMakeCache.txt and built with catkin in workspace,
you now have the init and tests executables :

init_match : gives us the robots and team playing in the match.
test_log : reads the video with fake interruptions of logs for each robot.
test_time : returns a result.csv which gives us execution times for some functions.
test_video : launches five main_annotateImage and a test_time to test performance
of the computer

# LAUNCH INTERFACE

After you changed the value of the BUILD_ANNOTATEIMAGE_TOOLS:BOOL to ON in
workspace/build/interface/CMakeCache.txt and built with catkin in workspace,
go to the 2vs1/ folder and input the following line in the terminal :

      ../workspace/devel/lib/interface/main

You don't need any arguments because they are given by the json file
annotation_settings.json
