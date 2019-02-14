#include <traitement/annotation.h>
#include <tclap/CmdLine.h>

using namespace traitement;

int main(int argc, char ** argv) {
	Annotation manager;/*

	TCLAP::CmdLine cmd("Acquire and display one or multiple streams along with meta-information",
                     ' ', "0.9");

  TCLAP::ValueArg<std::string> config_arg("c", "config", "The path to the json configuration file",
                                          true, "config.json", "string");
  TCLAP::ValueArg<std::string> field_arg("f", "field", "The path to the json description of the file",
                                          true, "field.json", "string");
  TCLAP::SwitchArg verbose_arg("v", "verbose", "If enabled display all messages received",
                               cmd, false);
  cmd.add(config_arg);

  cmd.add(field_arg);

  try {
    cmd.parse(argc, argv);

  } catch (const TCLAP::ArgException & e) {
    std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
  }
  Annotation manager;

  std::string conf = config_arg.getValue();
  std::string field = field_arg.getValue();*/
  manager.launchAnnotation(argc, argv, true);

  return 0;
}
