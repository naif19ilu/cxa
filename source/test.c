/*
 * Cxa - C executable arguments parser
 * Jul 7, 2025
 * Tester file
 */
#include "cxa.h"
#include <stdio.h>

struct Program
{
	int nopages;
	int nooptmz;
};

int main (int argc, char **argv)
{
	struct Program p =
	{
		.nopages = 0,
		.nooptmz = 0
	};

	struct CxaFlag flags[] =
	{
		CXA_SET_INT("pages", "no pages", &p.nopages, CXA_FLAG_TAKER_YES, 'p'),
		CXA_SET_INT("optmz", "no optmz", &p.nooptmz, CXA_FLAG_TAKER_YES, 'o'),
		CXA_SET_END
	};


	struct Cxa *cxa = cxa_execute((unsigned char) argc, argv, flags, "test");
	printf("%d : %d\n", p.nopages, p.nooptmz);

	cxa_clean(cxa);
	return 0;
}
