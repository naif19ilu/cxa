/*
 * Cxa - C executable arguments parser
 * Jul 7, 2025
 * Tester file
 */
#include "cxa.h"
#include <stdio.h>

int main (int argc, char **argv)
{
	static struct CxaFlag flags[] =
	{
		CXA_SET_INT("pages", "no pages", NULL, CXA_FLAG_TAKER_YES, 'p'),
		CXA_SET_INT("optmz", "no optmz", NULL, CXA_FLAG_TAKER_YES, 'o'),
		CXA_SET_END
	};

	cxa_execute((unsigned char) argc, argv, flags, "test");
	return 0;
}
