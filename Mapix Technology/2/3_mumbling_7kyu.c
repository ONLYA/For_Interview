/*
 * https://www.codewars.com/kata/5667e8f4e3f572a8f2000039/c
 * This time no story, no theory. The examples below show you how to write function accum:
 *
 * Examples:
 * accum("abcd") -> "A-Bb-Ccc-Dddd"
 * accum("RqaEzty") -> "R-Qq-Aaa-Eeee-Zzzzz-Tttttt-Yyyyyyy"
 * accum("cwAt") -> "C-Ww-Aaa-Tttt"
 * The parameter of accum is a string which includes only letters from a..z and A..Z.
 *
 * */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

char *accum(const char *source)
{
	char *result, temp[2];
	int i, j;
	int length = strlen(source);

	temp[1] = '\0';
	printf("%d\n", length);
	int total_length = (1+length)*length/2 + length;

	result = calloc(total_length, sizeof(char));
	result[0] = toupper(source[0]);

	for (i = 1; source[i]; i++)
	{
		strcat(result, "-");
		for (j = 0; j < i + 1; j++)
		{
			temp[0] = j ? tolower(source[i]) : toupper(source[i]);
			strcat(result, temp);
		}
	}

	return result;
}

int main()
{
	char *input, *output;

	scanf("%s", input);

	output = accum(input);
	printf("%s\n", output);

	free(output); output = NULL;

	return 0;
}
