#include <ldCLtemplate.h>
#include "tree.h"

Tree tree;

int main() {
  LDCL ldcl("ercha.cl","main");
  ldcl.setArg(&tree,sizeof tree,CL_MEM_READ_WRITE,LDCL_NEED_READ);
  ldcl.run();
}
