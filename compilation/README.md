# Compilation

## The first compile

### Some flags
1. `g++ -o main main.cpp` 
here, `-o` specifies the output binary name.

2. Using `-Wall`
```bash
➜  $ g++ -o a main.cpp -Wall          
main.cpp:5:9: warning: unused variable 'unused_var' [-Wunused-variable]
    int unused_var = 1;
        ^
main.cpp:6:9: warning: unused variable 'uninitialized_var' [-Wunused-variable]
    int uninitialized_var;
        ^
2 warnings generated.
```

3. Why -Wall, why not -Wsomething-specific?

`g++ -o a main.cpp -Wuninitialized`
Only when *using* the uninitialized_var (uncommenting out L7)
```bash
main.cpp:7:44: warning: variable 'uninitialized_var' is uninitialized when used here [-Wuninitialized]
    std::cout << "uninitialized_var = " << uninitialized_var << '\n';
                                           ^~~~~~~~~~~~~~~~~
main.cpp:6:26: note: initialize the variable 'uninitialized_var' to silence this warning
    int uninitialized_var;
                         ^
                          = 0
1 warning generated.
```

4. Preprocessor macros / Preprocessor-defines

-Dflags can be used in the following way:
```bash
➜  compilation git:(main) ✗ g++ -o a main.cpp 
➜  compilation git:(main) ✗ ./a               
uninitialized_var = 0
Hello World
number of elements = 100
➜  compilation git:(main) ✗ g++ -o a main.cpp -DARR_SIZE=10  
➜  compilation git:(main) ✗ ./a                            
uninitialized_var = 1
Hello World
number of elements = 10
```

### Adding another file

```bash
➜  compilation git:(main) ✗ bear -- g++ -o main main.cpp -std=c++20
Undefined symbols for architecture arm64:
  "square(int)", referenced from:
      _main in main-70b355.o
ld: symbol(s) not found for architecture arm64
clang: error: linker command failed with exit code 1 (use -v to see invocation)
```

**Undefined symbols** is a linker error. `main.cpp` forms one single TU where each symbol is resolved (square exists in `foo.h`). However, the *definition* is looked up at link-time, which searches for TU foo.o which was never compiled.

```bash
➜  compilation git:(main) ✗ bear -- g++ -o main main.cpp foo.cpp -std=c++20
➜  compilation git:(main) ✗ ./main
uninitialized_var = 0
Hello World
number of elements = 100
square(3) = 9
➜  compilation git:(main) ✗ 
```

**Side note:** `bear` generates compile_commands.json for clangd to provide intellisense.

## Digging deeper

### -E

` g++ -E main.cpp | tail -40 ` should show you the preprocessor output when trying to compile main.cpp as a TU. This includes foo.h literally copy-pasted on top. We truncate the output to skip the larger header files `iostream.h`.

### Consequence

Note that since the preprocessor literally dumps the `.h` text replacing the `#include` lines, any definition in a preceding include precedes one coming from a latter `#include` in the preprocessed output. 

If `main.cpp` had the following include orders,
```c++
#include "double_the_square.h"
#include "foo.h"
```
we would have
```bash
$ g++ -o main main.cpp foo.cpp -std=c++20
In file included from main.cpp:3:
./double_the_square.h:5:14: error: use of undeclared identifier 'square'
    return 2*square(x); // note that we did not include square.h here
             ^
1 error generated.
```

However, switching the order with foo.h declared first we get:
```bash
➜  compilation git:(main) ✗ g++ -o main main.cpp foo.cpp -std=c++20
➜  compilation git:(main) ✗ ./main
uninitialized_var = 0
Hello World
number of elements = 100
square(3) = 9
doubling the square of (4) = 32
➜  compilation git:(main) ✗ 
```

Although this is a working example, it is worth noting that each interface or header file should be self-sufficient to provide us with the usage of that API. And creating a header-library like our `double_the_square.h` is a terrible practice for the real world.

## Static and Dynamic libraries

Background: [learncpp](https://www.learncpp.com/cpp-tutorial/a1-static-and-dynamic-libraries/)

### Creating a static library

Let's try to use foo.h to create a static lib.

```bash
$ g++ -std=c++20 -c foo.cpp -o foo.o
$ ar rcs libfoo.a foo.o
```
Here, `ar` stands for archive, linux's archive utility tool.
each letter means:

r = replace or insert files into the archive
c = create the archive if it doesn’t exist
s = write an index (symbol table) for faster linking

### using static lib

```bash
g++ -std=c++20 main.cpp -L. -lfoo -o main
```

`-Lpath` : Location of library foo is path
`lfoo` : link against library libfoo

You can also directly write
```bash
g++ main.cpp ./libfoo.a -o main
```
with the exact path of `libfoo.a`. 

#### caveat
Note that linker process L to R and 
> The linker only pulls in code from a static library if it has already seen an unresolved symbol that needs it.
So, `g++ ./libfoo.a main.cpp -o main` would give linker error since during the pass, libfoo.a does not contribute to anything since no symbol required at the L->R parse at that point.

eg if **`libA.a` depends on `libB.b`**
`g++ main.o -lA -lB   # is correct ✔`

### create shared lib
1. create `foo.o` object file again
2. This time, g++ can directly create a dynamic lib.
```bash
g++ -std=c++20 -shared foo.o -o libfoo.so
```

### Usage of shared lib

```bash
g++ -std=c++20 -o main main.cpp -L. -lfoo
```
this uses libfoo.so.
On Linux systems, you also need to set `$RD_LIBRARY_PATH` to include your libfoo.so location so linker can resolve the `square()` symbol at runtime.



















