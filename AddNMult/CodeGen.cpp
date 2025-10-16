#include "CodeGen.h"
#include <llvm/IR/Verifier.h>

using namespace llvm;
using namespace addNMult;

CodeGen::CodeGen(std::string moduleName) {
    mod = std::make_unique<Module>(std::move(moduleName), ctx);
    mod->setSourceFileName("addNMult.cpp");
    builder = std::make_unique<IRBuilder<>>(ctx);
}

static llvm::Type* i64Ty(llvm::LLVMContext& ctx) {
    return llvm::Type::getInt64Ty(ctx);
}

Value* CodeGen::codegen(const Expression* e) {
    if (auto n = dynamic_cast<const NumberExpression*>(e)) return codegenNumber(n);
    if (auto v = dynamic_cast<const VarExpression*>(e))   return codegenVar(v);
    if (auto b = dynamic_cast<const BinaryExpression*>(e)) return codegenBinary(b);
    return nullptr;
}

Value* CodeGen::codegenNumber(const NumberExpression* e) {
    return ConstantInt::get(i64Ty(ctx), e->value, false);
}

Value* CodeGen::codegenVar(const VarExpression* e) {
    auto it = named.find(e->name);
    if (it == named.end()) return nullptr;
    return builder->CreateLoad(i64Ty(ctx), it->second, e->name);
}

Value* CodeGen::codegenBinary(const BinaryExpression* e) {
    Value* L = codegen(e->lhs.get());
    Value* R = codegen(e->rhs.get());
    if (!L || !R) return nullptr;
    switch (e->op) {
        case Op::Add: return builder->CreateAdd(L, R, "addval");
        case Op::Mul: return builder->CreateMul(L, R, "mulval");
    }
    return nullptr;
}

llvm::Function* CodeGen::emit(const Program& program) {
    auto* functionType = llvm::FunctionType::get(i64Ty(ctx), false);
    auto* function = llvm::Function::Create(functionType, llvm::Function::ExternalLinkage, "addNMult", mod.get());
    auto* entryBlock = llvm::BasicBlock::Create(ctx, "entry", function);
    builder->SetInsertPoint(entryBlock);

    // Because now we can have multiple let declarations.
    // Declaration is one <let>.
    for (const auto& declaration : program.decls) {
        auto* variableSlot = builder->CreateAlloca(i64Ty(ctx), nullptr, declaration.name);
        named[declaration.name] = variableSlot;
        llvm::Value* initializerValue = codegen(declaration.value.get());
        if (!initializerValue) { 
            function->eraseFromParent(); 
            return nullptr; 
        }
        builder->CreateStore(initializerValue, variableSlot);
    }

    llvm::Value* returnValue = codegen(program.ret.get());
    if (!returnValue) { function->eraseFromParent(); return nullptr; }
    builder->CreateRet(returnValue);

    if (llvm::verifyFunction(*function, &llvm::errs())) {
        function->eraseFromParent();
        return nullptr;
    }
    return function;
}