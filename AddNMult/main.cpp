#include <iostream>
#include "Lexer.h"
#include "Parser.h"
#include "CodeGen.h"

using namespace std;
using namespace addNMult;

int main() {
  std::string input = "let x = 2 * 2 + (4)";
  Lexer lexer(input);
  Parser p(lexer);
  try {
    auto decl = p.parseLet();
    CodeGen cg("addNMult.cpp");
    auto* mainFn = cg.emit(*decl);
    if (!mainFn) {
      std::cerr << "codegen failed\n";
      return 1;
    }
    cg.module()->print(llvm::outs(), nullptr);
    return 0;
  } catch (const std::exception& e) {
    std::cerr << "error: " << e.what() << std::endl;
    return 1;
  }
}
