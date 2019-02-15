#include <traitement/annotation.h>
#include <tclap/CmdLine.h>

using namespace traitement;

int main(int argc, char ** argv) {
  /* On lance un annotation manager pour lancer les annotations sans interface graphique*/

	Annotation manager;


	std::cout << "Score Début : " << manager.getScore() << std::endl;
	manager.launchAnnotation(argc, argv, true,manager.display_img );

  return 0;
}
