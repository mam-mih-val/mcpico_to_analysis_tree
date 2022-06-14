//
// Created by mikhail on 6/14/22.
//

#include "converter.h"

int main(){
  auto converter = Converter( "/home/mikhail/mcpico.list", "out.root", "atree" );
  converter.Run();
  return 0;
}