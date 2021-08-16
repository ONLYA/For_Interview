/*
 * https://www.codewars.com/kata/523f5d21c841566fde000009/c
 * Your goal in this kata is to implement a difference function, which subtracts one list from another and returns the result.
 *
 * It should remove all values from list a, which are present in list b keeping their order.
 * 
 * array_diff({1, 2}, 2, {1}, 1, *z) == {2} (z == 1)
 * If a value is present in b, all of its occurrences must be removed from the other:
 * 
 * array_diff({1, 2, 2, 2, 3}, 5, {2}, 1, *z) == {1, 3} (z == 2)
 * NOTE: In C, assign return array length to pointer *z
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//  return a dynamically allocated array
//  assign the length of this array to *z
//  memory will be freed by the test suite

int *array_diff(const int *arr1, size_t n1, const int *arr2, size_t n2, size_t *z) {

    //  <----  hajime!
    int *result_arr = malloc(n1 + n2 + 1);
    int size = -1;

    int i, j;
    bool flag = false;

    for (i = 0; i < n1; i++)
    {
        for (j = 0; j < n2; j++) flag |= (arr1[i] == arr2[j]);
        if (!flag)
        {
            size += 1;
            *(result_arr + size) = arr1[i];
        }
        flag = false;
    }
    *z = size + 1;

    return result_arr;

}

int main()
{
    const int arr1[5] = {1, 2, 3, 4, 5};
    const int arr2[3] = {1, 3, 4};
    long *z;
    int *result = array_diff(arr1, 5, arr2, 3, z);

    printf("%d %d %ld\n", result[0], result[1], *z);
    return 0;
}