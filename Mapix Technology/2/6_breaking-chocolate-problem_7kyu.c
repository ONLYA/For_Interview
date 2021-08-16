/*
 * https://www.codewars.com/kata/534ea96ebb17181947000ada/train/c
 * Your task is to split the chocolate bar of given dimension n x m into small squares. Each square is of size 1x1 and unbreakable. Implement a function that will return minimum number of breaks needed.
 *
 * For example if you are given a chocolate bar of size 2 x 1 you can split it to single squares in just one break, but for size 3 x 1 you must do two breaks.
 *
 * If input data is invalid you should return 0 (as in no breaks are needed if we do not have any chocolate to split). Input will always be a non-negative integer.
 * 
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

uint32_t breaking_chocolate(uint32_t n, uint32_t m)
{
    if ((n <= 1 && m <= 1) || m == 0 || n == 0) return 0;
    return m * n - 1;
}

int main()
{
    return 0;
}