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
		{ .longname = "document", .description = "file to be compiled",   'd', .needs = CXA_FLAG_ARGUMENT_YES },
		{ .longname = "verbose",  .description = "display operations",    'v', .needs = CXA_FLAG_ARGUMENT_NON },
		{ .longname = "usage",    .description = "print program's usage", 'u', .needs = CXA_FLAG_ARGUMENT_MAY },
		CXA_FINAL_FLAG
	};

	cxa_parse_arguments(flags, argc, argv);
	return 0;
}
