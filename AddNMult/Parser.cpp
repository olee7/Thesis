#include "Parser.h"

namespace addNMult {

    Parser::Parser(Lexer& lexer) : lex(lexer) { next(); }

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
        auto valueExpr = parseRHS();
        if (!is(TokenKind::Eof)) throw std::runtime_error("unexpected tokens after RHS");
        return std::make_unique<VarDecl>(VarDecl{std::move(name), std::move(valueExpr)});
    }

    std::unique_ptr<Expression> Parser::parseRHS() {
        if (is(TokenKind::Varname)) {
            std::string v = token.stringToken;
            next();
            return std::make_unique<VarExpression>(std::move(v));
        }
        return parseSumNums();
    }

    std::unique_ptr<Expression> Parser::parseSumNums() {
        auto e = parseProdNums();
        while (is(TokenKind::Plus)) {
            next();
            auto r = parseProdNums();
            e = std::make_unique<BinaryExpression>(Op::Add, std::move(e), std::move(r));
        }
        return e;
    }

    std::unique_ptr<Expression> Parser::parseProdNums() {
        auto e = parseNumberOperand();
        while (is(TokenKind::Star)) {
            next();
            auto r = parseNumberOperand();
            e = std::make_unique<BinaryExpression>(Op::Mul, std::move(e), std::move(r));
        }
        return e;
    }

    std::unique_ptr<Expression> Parser::parseNumberOperand() {
        if (!is(TokenKind::Number)) throw std::runtime_error("expected number");
        auto v = token.numberValue;
        next();
        return std::make_unique<NumberExpression>(v);
    }

}
