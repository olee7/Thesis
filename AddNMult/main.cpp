#include <iostream>
#include "Lexer.h"
#include "Parser.h"
#include "CodeGen.h"

using namespace std;
using namespace addNMult;

int main() {
  std::string input =
      "let x = 2 * 2 + (4)\n" // 8
      "let y = x * 2\n" // 16
      "return x + y"; // 24
  Lexer lexer(input);
  Parser p(lexer);
  try {
    auto prog = p.parseProgram();
    CodeGen cg("addNMult.cpp");
    auto* mainFunction = cg.emit(*prog);
    if (!mainFunction) {
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