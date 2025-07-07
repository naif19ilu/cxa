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
static unsigned short totalNoFlags = 0;

/* Array for quick access to flags depending on their
 * shortname quickShortNames[id] = where the flag
 * is defined within flags array.
 */
static short quickShortNames[26 * 2 + 10] = {0};

/* For better error messages */
static char *programsName;

void cxa_parse_arguments (char*, struct CxaFlag*, const int, char**);

#ifdef __cplusplus
}
#endif
#endif

#ifdef CXA_HEADER_ONLY

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

/* Error handling is harder to do in C, at least try to keep it
 * organized since there's no way to encapsulate the error functions...
 */

/* Actual parser starts from here, all from above is error handling
 * messages
 */
static short get_quick_shortname_key (const char id)
{
	if (isdigit(id)) { return id - '0'; }
	if (isupper(id)) { return id - 'A' + 10; }
	if (islower(id)) { return id - 'a' + 36; }
	return -1;
}

static void get_number_of_flags (struct CxaFlag *flags)
{
	for (unsigned short i = 0; flags[i].longname != NULL; i++)
		totalNoFlags++;
}

static void check_correct_shortname_usages (struct CxaFlag *flags)
{
	for (unsigned short i = 0; i < totalNoFlags; i++)
	{
		const short key = get_quick_shortname_key(flags[i].shortname);
		assert(key != -1 && "PROGRAMMER: PLEASE USE ONLY [a-zA-Z0-9] SHORTNAMES!");

		assert(quickShortNames[key] == 0 && "PROGRAMMER: THERE'S A DUPLICATED ID!");
		quickShortNames[key] = i + 1;

		const char *name = flags[i].longname;
		for (unsigned short j = 0; j < i; j++)
		{
			/* Null-byte character is guaranteed since they are hard coded strings */
			assert(strcmp(name, flags[j].longname) != 0 && "PROGRAMMER: THERE'S A DUPLICATED LONGNAME");
		}
	}
}

void cxa_parse_arguments (char *programsName, struct CxaFlag *flags, const int argc, char **argv)
{
	programsName = programsName;
	get_number_of_flags(flags);
	check_correct_shortname_usages(flags);
}

#endif
