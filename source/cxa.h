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

#define CXA_FINAL_FLAG          {NULL, 0, 0}

enum CxaFlagNeeds
{
	CXA_FLAG_NEEDS_ARG_NON = 0,
	CXA_FLAG_NEEDS_ARG_MAY = 1,
	CXA_FLAG_NEEDS_ARG_YES = 2,
};

struct CxaFlag
{
	char *longname;
	char shortname;
	enum CxaFlagNeeds needs;
};

void cxa_init (const char*, const unsigned int, char**, struct CxaFlag*);
unsigned char cxa_next (void);

#ifdef __cplusplus
}
#endif
#endif

#ifdef CXA_HEADER_ONLY

#include <ctype.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

/* Defines the name of the program which is using
 * the library for better error reporting
 */
static char *callerName = NULL;

/* This is basically argc and argv but made global
 * for this file to avoid passing them as argument
 * for each call
 */
static unsigned int noArgs = 0;
static char **argsGiven;

/* Pointer to the original array of flags defined
 * by the programmer
 */
static struct CxaFlag *definedFlags;

/* Number of flags the programmer defined, it is not
 * passed as argument since it can be tedious
 */
static unsigned short noFlags = 0;

static short quickShortnamesAccess[26 * 2 + 10];

static void get_total_flags (void);
static void check_for_duplicates (void);

static short get_shortname_as_key (const char);

void cxa_init (const char *name, const unsigned int argc, char **argv, struct CxaFlag *flags)
{
	callerName   = (char*) name;
	noArgs       = argc;
	argsGiven    = argv;
	definedFlags = flags;

	get_total_flags();
	check_for_duplicates();
}

unsigned char cxa_next (void)
{
	return 0;
}

static void get_total_flags (void)
{
	for (unsigned short i = 0; definedFlags[i].longname != NULL; i++)
		noFlags++;
}

static void check_for_duplicates (void)
{
	for (unsigned short i = 0; i < noFlags; i++)
	{
		const char shortname = definedFlags[i].shortname;
		const short key = get_shortname_as_key(shortname);

		assert(key != -1 && "PROGRAMMER: THERE'S AN INVALID SHORTNAME");
		assert(quickShortnamesAccess[key] == 0 && "PROGRAMMER: THERE'S A REPEATED SHORTNAME");

		quickShortnamesAccess[key] = i + 1;

		const char *longname = definedFlags[i].longname;
		for (unsigned short j = 0; j < i; j++)
			assert(strcmp(longname, definedFlags[j].longname) != 0 && "PROGRAMMER: THERE'S A DUPLICATED LONGNAME");
	}
}

static short get_shortname_as_key (const char shortname)
{
	if (isdigit(shortname)) { return shortname - '0'; }
	if (isupper(shortname)) { return shortname - 'A' + 10; }
	if (islower(shortname)) { return shortname - 'a' + 36; }
	return -1;
}

#endif

