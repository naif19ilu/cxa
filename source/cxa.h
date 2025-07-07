/*
 * Cxa - Command line argument parser for C
 * Jul 6, 2025
 * cxa's defintions and source code if wanted
 */
#ifndef CXA_H
#define CXA_H

#ifdef __cplusplus
extern "C" {
#endif

#define CXA_FINAL_FLAG {NULL, NULL, NULL, 0, 0, 0}

enum CxaFlagSeen
{
	CXA_NOT_SEEN = 0,
	CXA_WAS_SEEN = 1,
};

enum CxaFlagArgument
{
	CXA_FLAG_ARGUMENT_NON = 0,
	CXA_FLAG_ARGUMENT_MAY = 1,
	CXA_FLAG_ARGUMENT_YES = 2
};

struct CxaFlag
{
	char *longname;
	char *argument;
	char *description;
	char shortname;
	enum CxaFlagSeen seen;
	enum CxaFlagArgument needs;
};

/* Indicates the number of flags the programmer
 * has defined (flags array length)
 */
static unsigned short cxa__totalNoFlags = 0;

/* Array for quick access to flags depending on their
 * shortname cxa__quickShortNames[id] = where the flag
 * is defined within flags array.
 */
static unsigned short cxa__quickShortNames[26 * 2 + 10] = {0};

void cxa_parse_arguments (struct CxaFlag*, const int, char**);

#ifdef __cplusplus
}
#endif
#endif

#ifdef CXA_HEADER_ONLY

#include <stdio.h>

static void get_number_of_flags (struct CxaFlag *flags)
{
	for (unsigned short i = 0; flags[i].longname != NULL; i++)
		cxa__totalNoFlags++;
}

void cxa_parse_arguments (struct CxaFlag *flags, const int argc, char **argv)
{
	get_number_of_flags(flags);
}

#endif
