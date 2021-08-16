/*
 * https://www.codewars.com/kata/563b662a59afc2b5120000c6/train/c
 * In a small town the population is p0 = 1000 at the beginning of a year. The population regularly increases by 2 percent per year and moreover 50 new inhabitants per year come to live in the town. How many years does the town need to see its population greater or equal to p = 1200 inhabitants?
 * 
 * At the end of the first year there will be: 
 * 1000 + 1000 * 0.02 + 50 => 1070 inhabitants
 *
 * At the end of the 2nd year there will be: 
 * 1070 + 1070 * 0.02 + 50 => 1141 inhabitants (** number of inhabitants is an integer **)
 *
 * At the end of the 3rd year there will be:
 * 1141 + 1141 * 0.02 + 50 => 1213
 *
 * It will need 3 entire years.
 *
 * More generally given parameters:
 *
 * p0, percent, aug (inhabitants coming or leaving each year), p (population to surpass)
 *
 * the function nb_year should return n number of entire years needed to get a population greater or equal to p.
 *
 * aug is an integer, percent a positive or null floating number, p0 and p are positive integers (> 0)
 *
 * Examples:
 * nb_year(1500, 5, 100, 5000) -> 15
 * nb_year(1500000, 2.5, 10000, 2000000) -> 10
 * Note:
 * Don't forget to convert the percent parameter as a percentage in the body of your function: if the parameter percent is 2 you have to convert it to 0.02.
 * */

#include <stdio.h>
#include <stdlib.h>
//#include <math.h>

int nbYear(int p0, double percent, int aug, int p);

int main()
{
	int p0;
	double percent;
	int aug;
	int p;

	printf("Enter p0, percent, aug and p (seperated by space): ");
	scanf("%d %lf %d %d", &p0, &percent, &aug, &p);

	printf("It needs %d years.\n", nbYear(p0, percent, aug, p));
	return 0;
}

/* // Another solution with pure maths, but this is slower than the next solution
int nbYear(int p0, double percent, int aug, int p)
{
	double temp = aug / (percent / 100);
	double a = (p + temp) / (p0 + temp);
	return ceil(log2(a) / log2(1 + percent / 100));
}
*/

int nbYear(int p0, double percent, int aug, int p)
{
	int result = 0;
	int _p = p0;

	while (_p < p)
	{
		_p = _p * (1 + percent/100) + aug;
		result++;
	}
	return result;
}
