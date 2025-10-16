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
<program>     -> <declaration> <return> ε
<return>      -> "return" <sum>
<declaration> -> <declaration> <let>

<let>       -> "let" <varname> "=" <sum>
<sum>       -> <prod> <sum'>
<sum'>      -> "+" <prod> <sum'> | ε
<prod>      -> <eval> <prod'>
<prod'>     -> "*" <eval> <prod'> | ε
<eval>      -> <number> | <varname> | "(" sum ")"
<varname>   -> <letter> | <varname> <letter> | <varname> <digit> | <varname> "_"
<number>    -> <digit> | <number> <digit>
<letter>    -> "A"…"Z" | "a"…"z"
<digit>     -> "0"…"9"
```
