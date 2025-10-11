cmake -S . -B build -G Ninja -DLLVM_DIR="$LLVM_DIR" -DCMAKE_BUILD_TYPE=Release

cmake --build build -j

<let>      -> "let" <varname> "=" ( <varname> | <sum> )
<sum>      -> <prod> <sum'>
<sum'>     -> "+" <prod> <sum'> | ε
<prod>     -> <number> <prod'>
<prod'>    -> "*" <number> <prod'> | ε
<varname>  -> <letter> | <varname> <letter> | <varname> <digit> | <varname> "_"
<number>   -> <digit> | <number> <digit>
<letter>   -> "A"…"Z" | "a"…"z"
<digit>    -> "0"…"9"
