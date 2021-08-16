/*
 * https://www.codewars.com/kata/56efc695740d30f963000557
 * Define String.prototype.toAlternatingCase (or a similar function/method such as to_alternating_case/toAlternatingCase/ToAlternatingCase in your selected language; see the initial solution for details) such that each lowercase letter becomes uppercase and each uppercase letter becomes lowercase. For example:
 * to_alternating_case("hello world"); // => "HELLO WORLD"
 * to_alternating_case("HELLO WORLD"); // => "hello world"
 * to_alternating_case("hello WORLD"); // => "HELLO world"
 * to_alternating_case("HeLLo WoRLD"); // => "hEllO wOrld"
 * to_alternating_case("12345"); // => "12345" (Non-alphabetical characters are unaffected)
 * to_alternating_case("1a2b3c4d5e"); // => "1A2B3C4D5E"
 * to_alternating_case("String.prototype.toAlternatingCase"); // => "sTRING.PROTOTYPE.TOaLTERNATINGcASE"
 * As usual, your function/method should be pure, i.e. it should not mutate the original string.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *to_alternating_case(const char *s)
{
    int len = strlen(s);
    int i;
    char *output = malloc(len + 1);

    for (i = 0; i < len; i++)
    {
        output[i] = isalpha(s[i]) && toupper(s[i]) == s[i] ? tolower(s[i]) : toupper(s[i]);
    }
    output[len] = '\0';

    return output;
}

int main()
{
    printf("%s\n", to_alternating_case("heLlo 2WOr5lD!"));
    return 0;
}