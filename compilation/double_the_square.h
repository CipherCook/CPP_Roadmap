// We don't _define_ functions .h because including .h files can then violate
// ODR. Here is an improper usage of a definition inside a header file.
int double_the_square(int x)
{
    return 2*square(x); // note that we did not include square.h here
}

/* 
Another problem with definitions in headers is, here, the definition
requires including `foo.h` for the usage of square function.  
This introduces transitive includes, and if we can avoid them, we should avoid them.
However, for the purposes of our terrible example, we are also not going to include "foo.h".
*/