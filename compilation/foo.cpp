#include <stdexcept>
#include <format>


int square(int input)
{
    const int sq = input * input;
    if ( input != 0 && (sq / input) != input )
    {
        throw std::overflow_error( std::format("input {} too big", input) );
    }
    return sq;
}