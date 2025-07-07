/*
 * Cxa - Command line argument parser for C
 * Jul 6, 2025
 * Tester file
 */
#define CXA_HEADER_ONLY
#include "cxa.h"
#include <stdio.h>

int main (int argc, char **argv)
{
	struct CxaFlag flags[] =
	{
		{"document", 'd', CXA_FLAG_NEEDS_ARG_YES},
		{"usage",    'u', CXA_FLAG_NEEDS_ARG_MAY},
		{"verbose",  'v', CXA_FLAG_NEEDS_ARG_NON},
		CXA_FINAL_FLAG
	};

	cxa_init("tester", argc, argv, flags);
	while (cxa_next() != 0)
	{
	}

	return 0;
}
