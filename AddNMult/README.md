cmake -S . -B build -G Ninja -DLLVM_DIR="$LLVM_DIR" -DCMAKE_BUILD_TYPE=Release

cmake --build build -j

./build/addnmult > addNMult.ll

clang-18 -c addNMult.ll -o addNMult.o

clang++-18 addNMultCaller.cpp addNMult.o -o addNMult

./addNMult

Assuming you have a snippet of code like the below:
```
#include <cstdint>
#include <iostream>

extern "C" std::int64_t addNMult();

int main() {
    std::cout << addNMult() << '\n';
}
```
the program should give you the output "3".

```
<let>      -> "let" <varname> "=" ( <varname> | <sum> )
<sum>      -> <prod> <sum'>
<sum'>     -> "+" <prod> <sum'> | ε
<prod>     -> <number> <prod'>
<prod'>    -> "*" <number> <prod'> | ε
<varname>  -> <letter> | <varname> <letter> | <varname> <digit> | <varname> "_"
<number>   -> <digit> | <number> <digit>
<letter>   -> "A"…"Z" | "a"…"z"
<digit>    -> "0"…"9"
```
