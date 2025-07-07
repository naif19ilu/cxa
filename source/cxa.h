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

void cxa_parse (const char*, struct CxaFlag*, const unsigned int, char**);

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

/* Points to the last flag seen as long as it needs
 * an argyment (even if it's MAY)
 */
static struct CxaFlag *flagNeedsArg = NULL;

/* Name of the executable program which is using
 * this library
 */
static char *callerName = NULL;

static void error_invalid_shortname_by_user (const char *opt, const char shortname)
{
	const char *src =
	"cxa::%s::fatal: invalid shortname provided\n"
	" the '%c' is not a valid flag shortname; found in '-%s' option\n";
	fprintf(stderr, src, callerName, shortname, opt);
	exit(1);
}

static void error_unknown_shortname (const char *opt, const char shortname)
{
	const char *src =
	"cxa::%s::fatal: unknown shortname provided\n"
	" the '%c' is an unknown flag shortname; found in '-%s' option\n";
	fprintf(stderr, src, callerName, shortname, opt);
	exit(1);
}

static void error_multiple_argument_taker_in_short_family (const char *opt, const char a, const char b)
{
	const char *src =
	"cxa::%s::fatal: more than one flag takes argument in family\n"
	" both '%c' and '%c' flags take or may take an argument; found in '-%s' option\n"
	" please put them apart: ... -%c <argument> -%c <argument> ...\n";
	fprintf(stderr, src, callerName, a, b, opt, a, b);
	exit(1);
}

static void init_shortnames_keys (void);
static void check_names_are_ok (struct CxaFlag*);

static short get_quick_access_key (const char);
static void handle_shortnames (const char*, const size_t, struct CxaFlag*);

void cxa_parse (const char *caller, struct CxaFlag *flags, const unsigned int argc, char **argv)
{
	init_shortnames_keys();
	check_names_are_ok(flags);

	callerName = (char*) caller;

	/* This is flase until '--' option is found, anything
	 * after that will trated as a positional argument
	 */
	unsigned char endOfFlags = 0;

	for (unsigned int i = 1; i < argc; i++)
	{
		const char *option  = argv[i];
		const size_t length = strlen(option);

		if (endOfFlags)
		{
		}
		else if (length == 2 && *option == '-' && option[1] == '-')
		{
		}
		else if (length  > 2 && *option == '-' && option[1] == '-')
		{
		}
		else if (length == 1 && *option == '-')
		{
		}
		else if (length > 1 && *option == '-')
		{
			handle_shortnames(option, length, flags);
		}
		else
		{
		}
	}
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

static void handle_shortnames (const char *opt, const size_t len, struct CxaFlag *flags)
{
	for (size_t i = 1; i < len; i++)
	{
		const char thishort = opt[i];
		const short key = get_quick_access_key(thishort);

		if (key == -1)
		{
			error_invalid_shortname_by_user(opt + 1, thishort);
		}
		if (quickShortNamesAcess[key] == -1)
		{
			error_unknown_shortname(opt + 1, thishort);
		}

		struct CxaFlag *flag = &flags[quickShortNamesAcess[key]];
		if (flagNeedsArg && flag->needs != CXA_FLAG_NEEDS_ARG_NON)
		{
			error_multiple_argument_taker_in_short_family(opt + 1, flagNeedsArg->shortName, thishort);
		}
		if (flag->needs != CXA_FLAG_NEEDS_ARG_NON)
		{
			flagNeedsArg = flag;
		}
		flag->seen = CXA_FLAG_WAS_SEEN;
	}
}

#endif

