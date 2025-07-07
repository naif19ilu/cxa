/*
 * cxa - C executable arguments parser
 * Jul 6, 2025
 * Definitions and implementation
 *
 * If the programmer only wants to use the header file they'll
 * have to define CXA_ONLY_HEADER before including cxa.h in their
 * main file, like this:
 *
 * #define CXA_ONLY_HEADER
 * #include "cxa.h"
 *
 * and do not include cxa.c in the compilation process in order
 * to avoid redefinitions
 */
#ifndef CXA_H
#define CXA_H

#define CXA_FINAL_FLAG	{NULL, NULL, '\0', 0}

enum CxaFlagArg
{
	CXA_FLAG_ARG_YES,
	CXA_FLAG_ARG_NON,
	CXA_FLAG_ARG_MAY
};

struct CxaFlag
{
	char *longname;
	char *argument;
	char shortname;
	enum CxaFlagArg hasArg;
};

void cxa_parse_arguments (struct CxaFlag*, const int, char**);

#endif

#ifdef CXA_ONLY_HEADER

#include <stdio.h>

void cxa_parse_arguments (struct CxaFlag *flags, const int argc, char **argv)
{
	printf("hello world from header\n");
}

#endif
