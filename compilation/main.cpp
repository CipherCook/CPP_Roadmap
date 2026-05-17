#include <iostream>
#include <ostream>
#include "foo.h"
// #include "double_the_square.h"


#ifndef ARR_SIZE
#define ARR_SIZE 100 
#endif

void use_foo()
{
    std::cout << "Using foo.h. square(3) = " << square(3) << std::endl;
}

int main()
{
    int unused_var = 1;
    int uninitialized_var;
    std::cout << "uninitialized_var = " << uninitialized_var << '\n';
    
    std::cout << "Hello World" << std::endl;

    int arr[ARR_SIZE] = {0};

    std::cout << "number of elements = " << sizeof(arr) / sizeof(int) << std::endl;   
    
    use_foo();

    // std::cout << "doubling the square of (4) = " << double_the_square(4) <<std::endl;
    return 0;
}