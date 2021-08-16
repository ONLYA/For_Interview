/*
 * https://www.codewars.com/kata/57cebe1dc6fdc20c57000ac9/c
 * Simple, given a string of words, return the length of the shortest word(s).
 * 
 * String will never be empty and you do not need to account for different data types.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

ssize_t find_short(const char *s)
{
    ssize_t output = 100;
    char *word, *str = malloc(strlen(s) + 1);

    strcpy(str, s);

    while ((word = strtok_r(str, " ", &str))) output = output < strlen(word) ? output : strlen(word);
    
    return output > 0 ? output : -1;
}

int main()
{
    char input[] = "apple six what are you doing hi haha";
    // scanf("%s", input);
    printf("The length of the shortest word is %ld.\n", find_short("apple six what are you doing hi haha"));
    return (0);
}