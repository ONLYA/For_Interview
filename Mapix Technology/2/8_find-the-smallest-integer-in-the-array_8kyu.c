/*
 * https://www.codewars.com/kata/55a2d7ebe362935a210000b2
 * Given an array of integers your solution should find the smallest integer.
 *
 * For example:
 *
 * Given [34, 15, 88, 2] your solution will return 2
 * Given [34, -345, -1, 100] your solution will return -345
 * You can assume, for the purpose of this kata, that the supplied array will not be empty.
 * 
*/

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

int find_smallest_int(int *vec, size_t len)
{
    int i, j;
    int result = __INT_MAX__;
    int temp;
    for (i = 0, j = len - 1; j >= i; i++, j--)
    {
        if (vec[i] < vec[j]) temp = vec[i];
        else temp = vec[j];

        if (temp < result) result = temp;
    }
    return result;
}

int main()
{
    int result = find_smallest_int((int[]){5,7,4,78,4,6,1,3,8}, 9);
    printf("The smallest num is %d\n", result);
    return 0;
}