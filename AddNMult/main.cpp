#include "AstPrint.h"  // ChatGPT wrote this

#include <iostream>
#include "Lexer.h"
#include "Parser.h"

using namespace addNMult;

int main() {
  std::string input = "let x = 1 + y * 3";
  Lexer lexer(input);
  Parser parser(lexer);
  try {
    auto varDecl = parser.parseLet();
    print_vardecl(varDecl.get(), std::cout);
    std::cout << "ok" << std::endl;
    return 0;
  } catch (const std::exception& e) {
    std::cerr << "error: " << e.what() << std::endl;
    return 1;
  }
}
