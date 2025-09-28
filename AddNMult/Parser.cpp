#include "Parser.h"

namespace addNMult {

    Parser::Parser(Lexer& lx) : lex(lx) { next(); }

    void Parser::next() { token = lex.next(); }

    bool Parser::is(TokenKind k) const { return token.kind == k; }

    void Parser::expect(TokenKind k, const char* what) {
        if (!is(k)) throw std::runtime_error(std::string("expected ") + what);
        next();
    }

    std::unique_ptr<VarDecl> Parser::parseLet() {
        expect(TokenKind::Let, "'let'");
        if (!is(TokenKind::Varname)) throw std::runtime_error("expected identifier");
        std::string name = token.stringToken;
        next();
        expect(TokenKind::Eq, "'='");
        auto val = parseSum();
        return std::make_unique<VarDecl>(VarDecl{std::move(name), std::move(val)});
    }

    std::unique_ptr<Expression> Parser::parseSum() {
        auto expression = parseProd();
        while (is(TokenKind::Plus)) {
            next();
            auto rhs = parseProd();
            expression = std::make_unique<BinaryExpression>(
                Op::Add, 
                std::move(expression), 
                std::move(rhs));
        }
        return expression;
    }

    std::unique_ptr<Expression> Parser::parseProd() {
        auto expression = parseOperand();
        while (is(TokenKind::Star)) {
            next();
            auto rhs = parseOperand();
            expression = std::make_unique<BinaryExpression>(
                Op::Mul, 
                std::move(expression), 
                std::move(rhs));
        }
        return expression;
    }

    std::unique_ptr<Expression> Parser::parseOperand() {
        if (is(TokenKind::Number)) {
            auto numericValue = token.numberValue;
            next();
            return std::make_unique<NumberExpression>(numericValue);
        }
        if (is(TokenKind::Varname)) {
            auto stringValue = token.stringToken;
            next();
            return std::make_unique<VarExpression>(std::move(stringValue));
        }
        throw std::runtime_error("expected number or variable");
    }

}
