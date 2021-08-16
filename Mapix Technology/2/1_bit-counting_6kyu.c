/*
https://www.codewars.com/kata/526571aae218b8ee490006f4/solutions/c
Write a function that takes an integer as input, and returns the number of bits that are equal to one in the binary representation of that number. You can guarantee that input is non-negative.

Example: The binary representation of 1234 is 10011010010, so the function should return 5 in this case
*/

#include <stddef.h>

size_t countBits(unsigned value);

size_t countBits(unsigned value)
{
  int count = 0;
  while (value)
  {
    count += (value & 1);
    value >>= 1;
  }
  return count;
}
