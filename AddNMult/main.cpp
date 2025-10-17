#include <iostream>
#include "Lexer.h"
#include "Parser.h"
#include "CodeGen.h"

using namespace std;
using namespace addNMult;

int main() {
  // Read the entire content from std::cin into a single std::string
  // Thanks Google Gemini Flash 2.5
  string input (
    (istreambuf_iterator<char>(cin)),
    istreambuf_iterator<char>()
  );
  Lexer lexer(input);
  Parser p(lexer);
  try {
    auto prog = p.parseProgram();
    CodeGen cg("addNMult.cpp");
    auto* mainFn = cg.emit(*prog);
    if (!mainFn) {
      cerr << "codegen failed\n";
      return 1;
    }
    cg.module()->print(llvm::outs(), nullptr);
    return 0;
  } catch (const exception& e) {
    cerr << "error: " << e.what() << endl;
    return 1;
  }
}
