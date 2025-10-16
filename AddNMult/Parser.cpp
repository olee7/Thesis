#include "Parser.h"

namespace addNMult {

    Parser::Parser(Lexer& lexer) : lex(lexer) { next(); }

    void Parser::next() { token = lex.next(); }

    bool Parser::is(TokenKind k) const { return token.kind == k; }

    void Parser::expect(TokenKind k, const char* what) {
        if (!is(k)) throw std::runtime_error(std::string("expected ") + what);
        next();
    }

    std::unique_ptr<Program> Parser::parseProgram() {
        auto program = std::make_unique<Program>();

        while (is(TokenKind::Let)) {
            auto decl = parseLet();
            program->decls.push_back(VarDecl{std::move(decl->name), std::move(decl->value)});
        }

        expect(TokenKind::Return, "'return'");
        program->ret = parseRHS();

        if (!is(TokenKind::Eof)) {
            throw std::runtime_error("unintended token");
        }
        return program;
    }

    std::unique_ptr<VarDecl> Parser::parseLet() {
        expect(TokenKind::Let, "'let'");
        if (!is(TokenKind::Varname)) throw std::runtime_error("expected identifier");
        std::string name = token.stringToken;
        next();
        expect(TokenKind::Eq, "'='");
        auto valueExpr = parseRHS();
        return std::make_unique<VarDecl>(VarDecl{std::move(name), std::move(valueExpr)});
    }

    std::unique_ptr<Expression> Parser::parseRHS() {
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
        auto e = parseEval();
        while (is(TokenKind::Star)) {
            next();
            auto r = parseEval();
            e = std::make_unique<BinaryExpression>(Op::Mul, std::move(e), std::move(r));
        }
        return e;
    }

    std::unique_ptr<Expression> Parser::parseEval() {
        switch (token.kind) {
            case TokenKind::Number: {
                auto tokenVal = token.numberValue;
                next();
                return std::make_unique<NumberExpression>(tokenVal);
            }
            case TokenKind::Varname: {
                std::string tokenVal = token.stringToken;
                next();
                return std::make_unique<VarExpression>(std::move(tokenVal));
            }
            case TokenKind::OpenParen: {
                next();
                auto tokenVal = parseSumNums();
                expect(TokenKind::CloseParen, "')'");
                return tokenVal;
            }
            default:
                throw std::runtime_error("expected a number, variable, or parenthensis.");
        }   
    }
}
