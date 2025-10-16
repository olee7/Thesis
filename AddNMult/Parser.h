#pragma once
#include <memory>
#include <string>
#include <stdexcept>
#include <ostream>
#include <vector>
#include <cstdint>    
#include "Lexer.h"

namespace addNMult {

    struct Expression { 
        virtual ~Expression() = default; 
    };

    struct NumberExpression : Expression { 
        std::uint64_t value; 
        explicit NumberExpression(std::uint64_t v): value(v) {} 
    };

    struct VarExpression : Expression { 
        std::string name; 
        explicit VarExpression(std::string n): name(std::move(n)) {} 
    };

    enum class Op { Add, Mul };

    struct BinaryExpression : Expression {
        // if we have 2 + 3
        Op op; // this will hold +
        std::unique_ptr<Expression> lhs, rhs; // and lhs will hold 2 and rhs will hold 3.
        BinaryExpression(Op o, std::unique_ptr<Expression> a, std::unique_ptr<Expression> b)
            : op(o), lhs(std::move(a)), rhs(std::move(b)) {}
    };

    struct VarDecl {
        std::string name;
        std::unique_ptr<Expression> value;
    };

    struct Program {
        std::vector<VarDecl> decls; // Vector of <sum> in: <let> -> "let" <varname> "=" <sum>
        std::unique_ptr<Expression> ret;
    };

    class Parser {
        public:
            explicit Parser(Lexer& lx);
            std::unique_ptr<Program> parseProgram();
            std::unique_ptr<VarDecl> parseLet();

        private:
            Lexer& lex;
            Token token;

            void next();
            bool is(TokenKind k) const;
            void expect(TokenKind k, const char* what);

            std::unique_ptr<Expression> parseRHS();
            std::unique_ptr<Expression> parseSumNums();
            std::unique_ptr<Expression> parseProdNums();
            std::unique_ptr<Expression> parseEval();
    };

}
