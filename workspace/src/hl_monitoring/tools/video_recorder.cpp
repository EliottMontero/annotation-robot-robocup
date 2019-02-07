/**
 * Acquire a video from a specific input stream and save it in a movie along
 * with some meta_information
 */
#include <hl_monitoring/opencv_image_provider.h>

#include <opencv2/highgui.hpp>
#include <tclap/CmdLine.h>

using namespace hl_monitoring;

int main(int argc, char ** argv) {
  TCLAP::CmdLine cmd("Record a video based on OpenCV input", ' ', "0.9");

  TCLAP::ValueArg<std::string> video_arg("i", "input", "The path to the input", true,
                                         "/dev/video0", "string");
  TCLAP::ValueArg<std::string> output_arg("o", "output", "The path to the output video", true,
                                          "output.avi", "string");
  cmd.add(video_arg);
  cmd.add(output_arg);

  try {
    cmd.parse(argc, argv);
    
  } catch (const TCLAP::ArgException & e) {
    std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
  }

  OpenCVImageProvider provider(video_arg.getValue(), output_arg.getValue());

  bool exit = false;

  while(!exit) {
    cv::Mat img = provider.getNextImg();
    cv::imshow("Display", img);
    char key = cv::waitKey(10);
    switch(key) {
      case 'q':
        exit = true;
        break;
    }
  }

}
