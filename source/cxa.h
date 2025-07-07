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
	struct CxaFlag **flags;
	char **itsArgs;
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

/* For better error reports :) */
static char *programsName = NULL;

/* Pointer to the last flag seen as long as the flag
 * takes an argument even if its mandatory or likely (YES or MAY)
 */
static struct CxaFlag *lastSeen = NULL;

/* These two work to index both 'flags' and 'itsArgs'
 * field of the 'CxaAns' structure
 */
static unsigned short noFlagsSeen = 0;
static unsigned short noArgsSaved = 0;

static void error_invalid_shortname (const char *argument, const char shortname)
{
	const char *err =
	"cxa::%s::fatal: inavlid shortname provided\n"
	"  the '%c' shortname is not valid; found in '-%s' option\n";
	fprintf(stderr, err, programsName, shortname, argument);
	exit(EXIT_FAILURE);
}

static void error_unknown_shortname (const char *argument, const char shortname)
{
	const char *err =
	"cxa::%s::fatal: unknown shortname provided\n"
	"  the '%c' shortname is unknown; found in '-%s' option\n";
	fprintf(stderr, err, programsName, shortname, argument);
	exit(EXIT_FAILURE);
}

static void error_multiple_argshortnames (const char *argument, const char f, const char s)
{
	const char *err =
	"cxa::%s::fatal: more than one flag takes argument\n"
	" both '%c' and '%c' flags take an argument; found in '-%s' option\n"
	" solution: ... -%c <arg> -%c <arg>\n";
	fprintf(stderr, err, programsName, f, s, argument, f, s);
	exit(EXIT_FAILURE);
}

static unsigned short get_number_of_flags (struct CxaFlag*);
static void make_sure_names_are_unique (struct CxaFlag*, const unsigned short);

static short get_quick_shortname_key (const char);
static void handle_shortnames (struct CxaFlag*, const char*, const size_t);

void cxa_parse (const char *name, struct CxaFlag *flags, const unsigned int argc, char **argv)
{
	const unsigned short noFlags = get_number_of_flags(flags);
	programsName = (char*) name;

	CxaAns.flags   = (struct CxaFlag**) calloc(noFlags, sizeof(struct CxaFlag*));
	CxaAns.itsArgs = (char**) calloc(noFlags, sizeof(char*));

	assert(CxaAns.flags   && "PROGRAMMER: cannot allocate memory");
	assert(CxaAns.itsArgs && "PROGRAMMER: cannot allocate memory");	

	make_sure_names_are_unique(flags, noFlags);

	for (unsigned int i = 1; i < argc; i++)
	{
		const char *arg  = argv[i];
		const size_t len = strlen(arg);
		
		if (*arg == '-' && len > 1)
		{
			handle_shortnames(flags, arg, len);
		}
	}
}

void cxa_clean (void)
{
	free(CxaAns.flags);
	free(CxaAns.itsArgs);
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

static void handle_shortnames (struct CxaFlag *flags, const char *arg, const size_t len)
{
	for (size_t i = 1; i < len; i++)
	{
		const char shortname = arg[i];
		const short key = get_quick_shortname_key(shortname);

		if (key == -1) { error_invalid_shortname(arg + 1, shortname); }
		if (quickShortNames[key] == 0) { error_unknown_shortname(arg + 1, shortname); }

		struct CxaFlag *flag = &flags[quickShortNames[key] - 1];

		if (lastSeen != NULL && flag->needs != CXA_FLAG_NEEDS_ARG_NON) { error_multiple_argshortnames(arg + 1, lastSeen->shortname, shortname); }
		if (flag->needs != CXA_FLAG_NEEDS_ARG_NON) { lastSeen = flag; }
		CxaAns.flags[noFlagsSeen++] = flag;
	}
}

#endif
