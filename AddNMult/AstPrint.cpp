// ChatGPT wrote this.
#include "Parser.h"
#include "AstPrint.h"
#include <ostream>

using namespace addNMult;

static void indent(std::ostream& out, int n) {
    for (int i = 0; i < n; ++i) out.put(' ');
}

void addNMult::print_expression(const Expression* e, std::ostream& out, int level) {
    if (auto n = dynamic_cast<const NumberExpression*>(e)) {
        indent(out, level);
        out << "Number(" << n->value << ")\n";
        return;
    }
    if (auto v = dynamic_cast<const VarExpression*>(e)) {
        indent(out, level);
        out << "Var(" << v->name << ")\n";
        return;
    }
    if (auto b = dynamic_cast<const BinaryExpression*>(e)) {
        indent(out, level);
        out << "Binary(" << (b->op == Op::Add ? "+" : "*") << ")\n";
        print_expression(b->lhs.get(), out, level + 2);
        print_expression(b->rhs.get(), out, level + 2);
        return;
    }
    indent(out, level);
    out << "UnknownExpr\n";
}

void addNMult::print_vardecl(const VarDecl* d, std::ostream& out) {
    out << "Let " << d->name << " =\n";
    print_expression(d->value.get(), out, 2);
}
