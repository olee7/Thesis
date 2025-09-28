//ChatGPT wrote this.
#pragma once
#include <iosfwd>

namespace addNMult {
    struct Expression;
    struct VarDecl;

    void print_expression(const Expression* e, std::ostream& out, int indent = 0);
    void print_vardecl(const VarDecl* d, std::ostream& out);
}
