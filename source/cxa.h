#ifndef CXA_H
#define CXA_H

#ifdef __cplusplus
extern "C" {
#endif

#define CXA_FINAL_FLAG   {NULL, NULL, NULL, 0, 0, 0}

enum CxaFlagArg
{
	CXA_FLAG_NEEDS_ARG_NON = 0,
	CXA_FLAG_NEEDS_ARG_MAY = 1,
	CXA_FLAG_NEEDS_ARG_YES = 2,
};

enum CxaFlagSeen
{
	CXA_FLAG_NOT_SEEN = 0,
	CXA_FLAG_WAS_SEEN = 1
};

struct CxaFlag
{
	char             *longName;
	char             *desc;
	char             *argument;
	char             shortName;
	enum CxaFlagArg  needs;
	enum CxaFlagSeen seen;
};

void cxa_parse (struct CxaFlag*, const unsigned int, char**);

#ifdef __cplusplus
}
#endif

#endif

/*
 *  ________________
 * < implementation >
 *  ----------------
 *   \
 *    \
 *       /\_)o<
 *      |      \
 *      | O . O|
 *       \_____/
 */
#ifdef CXA_HEADER_ONLY

#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

/* Quick way to acess flags depending of their shortname
 */
static short quickShortNamesAcess[26 * 2 + 10];

static void init_shortnames_keys (void);
static void check_names_are_ok (struct CxaFlag*);

static short get_quick_access_key (const char);

void cxa_parse (struct CxaFlag *flags, const unsigned int argc, char **argv)
{
	init_shortnames_keys();
	check_names_are_ok(flags);
}

static void init_shortnames_keys (void)
{
	const unsigned short lim = 26 * 2 + 10;
	for (unsigned short i = 0; i < lim; i++)
		quickShortNamesAcess[i] = -1;
}

static void check_names_are_ok (struct CxaFlag *flags)
{
	for (unsigned short i = 0; flags[i].longName != NULL; i++)
	{
		const char shrt1 = flags[i].shortName;
		const short key = get_quick_access_key(shrt1);

		assert(key != -1 && "PROGRAMMER: ONLY ALPHANUMERIC SHORTNAMES ARE ALLOWED");
		assert(quickShortNamesAcess[key] == -1 && "PROGRAMMER: THERE'S A DUPLICATED SHORTNAME");

		quickShortNamesAcess[key] = i;

		const char *long1 = flags[i].longName;
		for (unsigned short j = 0; j < i; j++)
			assert(strcmp(long1, flags[j].longName) && "PROGRAMMER: THERE'S A DUPLICATED LONGNAME");
	}
}

static short get_quick_access_key (const char a)
{
	if (isdigit(a)) { return a - '0'; }
	if (islower(a)) { return a - 'a' + 10; }
	if (isupper(a)) { return a - 'A' + 36; }

	return -1;
}

#endif

