/*
 * cxa - C executable arguments parser
 * Jul 6, 2025
 * Tester file
 */
#define CXA_ONLY_HEADER
#include "cxa.h"

int main (int narg, char **args)
{
	static struct CxaFlag flags[] =
	{
		{"document", NULL, 'd', CXA_FLAG_ARG_YES},
		CXA_FINAL_FLAG
	};

	cxa_parse_arguments(flags, narg, args);
	return 0;
}
