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

#define CXA_FINAL_FLAG {NULL, 0, 0}

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

struct
{
	unsigned short *seenIds;
	char **args;
} CxaAns;

void cxa_parse (const char*, struct CxaFlag*, const unsigned int, char**);
void cxa_clean (void);

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

/* Array for quick access to flags depending on their
 * shortname quickShortNames[id] = where the flag
 * is defined within flags array.
 */
static short quickShortNames[26 * 2 + 10] = {0};

static unsigned short get_number_of_flags (struct CxaFlag*);
static void make_sure_names_are_unique (struct CxaFlag*, const unsigned short);

static short get_quick_shortname_key (const char);

void cxa_parse (const char *name, struct CxaFlag *flags, const unsigned int argc, char **argv)
{
	const unsigned short noFlags = get_number_of_flags(flags);

	CxaAns.seenIds = (unsigned short*) calloc(noFlags, sizeof(unsigned short));
	CxaAns.args    = (char**) calloc(noFlags, sizeof(char*));

	assert(CxaAns.seenIds && "PROGRAMMER: cannot allocate memory");
	assert(CxaAns.args    && "PROGRAMMER: cannot allocate memory");	

	make_sure_names_are_unique(flags, noFlags);
}

void cxa_clean (void)
{
	free(CxaAns.seenIds);
	free(CxaAns.args);
}

static unsigned short get_number_of_flags (struct CxaFlag *flags)
{
	unsigned short noFlags = 0;
	for (unsigned short i = 0; flags[i].longname != NULL; i++)
		noFlags++;
	return noFlags;
}

static void make_sure_names_are_unique (struct CxaFlag *flags, const unsigned short noFlags)
{
	for (unsigned short i = 0; i < noFlags; i++)
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

static short get_quick_shortname_key (const char id)
{
	if (isdigit(id)) { return id - '0'; }
	if (isupper(id)) { return id - 'A' + 10; }
	if (islower(id)) { return id - 'a' + 36; }
	return -1;
}

#endif
