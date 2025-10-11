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

llvm::Function* CodeGen::emit(const VarDecl& decl) {
    auto* fnTy = llvm::FunctionType::get(i64Ty(ctx), false);
    auto* fn = llvm::Function::Create(fnTy, llvm::Function::ExternalLinkage, "addNMult", mod.get());
    auto* entry = llvm::BasicBlock::Create(ctx, "entry", fn);
    builder->SetInsertPoint(entry);

    auto* slot = builder->CreateAlloca(i64Ty(ctx), nullptr, decl.name);
    named[decl.name] = slot;

    // Begin depth first, left to right, tree traversal.
    llvm::Value* val = codegen(decl.value.get());
    if (!val) { fn->eraseFromParent(); return nullptr; }

    builder->CreateStore(val, slot);
    builder->CreateRet(val);

    if (llvm::verifyFunction(*fn, &llvm::errs())) {
        fn->eraseFromParent();
        return nullptr;
    }
    return fn;
}
