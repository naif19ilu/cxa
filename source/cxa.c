/*
 * Cxa - Command line argument parser for C
 * Jul 6, 2025
 * cxa's source code (C version)
 */
#include "cxa.h"
#include <stdio.h>

void cxa_parse_arguments (struct CxaFlag *flags, const int argc, char **argv)
{
	printf("%d\n", flags[0].needs);
}

