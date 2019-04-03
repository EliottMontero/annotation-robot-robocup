#include <stdlib.h>
#include <stdio.h>

int main(){

  for (int i=0; i<5; i++)
    system("x-terminal-emulator -e \"../workspace/devel/lib/annotateImage/main_annotateImage\"");

  system("../workspace/devel/lib/annotateImage/test_time");

  return 0;
}
