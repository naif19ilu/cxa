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

struct
{
	char          **args;
	unsigned long len;
	unsigned long cap;
	unsigned long factor;
} CxaPositional;

void cxa_parse (const char*, struct CxaFlag*, const unsigned int, char**, const unsigned long);
void cxa_debug (struct CxaFlag*);
void cxa_document (struct CxaFlag*, const char*);

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

#define MAX(a, b)   ((a) > (b) ? (a) : (b))

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

static void error_missing_argument (void)
{
	const char *src =
	"cxa::%s::fatal: missing argument\n"
	" the '--%s' (%c) flag needs an argument but none was given\n";
	fprintf(stderr, src, callerName, flagNeedsArg->longName, flagNeedsArg->shortName);
	exit(1);
}

static void error_unknown_longname (const char *opt, const unsigned short len)
{
	const char *src =
	"cxa::%s::fatal: unknown longname flag\n"
	" the '--%.*s' is not defined by the program\n";
	fprintf(stderr, src, callerName, len, opt);
	exit(1);
}

static void init_shortnames_keys (void);
static void check_names_are_ok (struct CxaFlag*);

static short get_quick_access_key (const char);
static void handle_shortnames (const char*, const size_t, struct CxaFlag*);

static void handle_freewords (const char*);
static void add_positional_arg (const char*);

static void make_sure_flag_has_its_arg(void);
static void hanlde_longnames (const char*, const size_t, struct CxaFlag*);

void cxa_parse (const char *caller, struct CxaFlag *flags, const unsigned int argc, char **argv, const unsigned long posArgsFactor)
{
	init_shortnames_keys();
	check_names_are_ok(flags);

	CxaPositional.cap    = posArgsFactor;
	CxaPositional.factor = posArgsFactor;
	CxaPositional.len    = 0;
	CxaPositional.args   = (char**) calloc(CxaPositional.cap, sizeof(char*));
	assert(CxaPositional.args && "SYSTEM: cannot allocate memory");

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
			add_positional_arg(option);
			continue;
		}
		else if (length == 2 && *option == '-' && option[1] == '-')
		{
			endOfFlags = 1;
		}
		else if (length  > 2 && *option == '-' && option[1] == '-')
		{
			make_sure_flag_has_its_arg();
			hanlde_longnames(option, length, flags);
		}
		else if (length == 1 && *option == '-')
		{
		}
		else if (length > 1 && *option == '-')
		{
			make_sure_flag_has_its_arg();
			handle_shortnames(option, length, flags);
		}
		else
		{
			handle_freewords(option);
		}
	}
	make_sure_flag_has_its_arg();
}

/* This function is intended to be use only to inspect the flags and then
 * exit the program. The output is not the best looking so you better pipe
 * this function output to column command for better looking: ... | column -t -s '-'
 */
void cxa_debug (struct CxaFlag *flags)
{
	printf("Flags\n");
	for (unsigned int i = 0; flags[i].longName; i++)
	{
		struct CxaFlag *f = &flags[i];
		printf("  %s - %c - %s - %s\n", f->longName, f->shortName, f->desc, (f->argument) ? f->argument : "NO_ARG");
	}

	printf("Positional\n");
	for (unsigned long i = 0; i < CxaPositional.len; i++)
	{
		printf("  %ld. - %s\n", i, CxaPositional.args[i]);
	}
	exit(0);
}

void cxa_document (struct CxaFlag *flags, const char *pdesc)
{
	printf("\n\x1b[1mUsage\x1b[0m: %s - %s\n\n", callerName, pdesc);
	printf("flags:\n");

	unsigned short largestName = 0;
	unsigned short largestDesc = 0;

	for (unsigned int i = 0; flags[i].longName; i++)
	{
		largestName = MAX(largestName, strlen(flags[i].longName));
		largestDesc = MAX(largestDesc, strlen(flags[i].desc    ));
	}

	largestName += 2;
	largestDesc += 2;

	static const char *const enum2string[] =
	{
		"no-argument",
		"optional-argument",
		"needs-argument"
	};

	for (unsigned int i = 0; flags[i].longName; i++)
	{
		printf("  \x1b[2m-\x1b[0m%c or \x1b[2m--\x1b[0m%-*s%-*s(%s)\n",
		flags[i].shortName, largestName, flags[i].longName,
		largestDesc, flags[i].desc, enum2string[flags[i].needs]);
	}

	putchar(10);
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

static void handle_freewords (const char *opt)
{
	if (flagNeedsArg == NULL) { add_positional_arg(opt); return; }

	flagNeedsArg->argument = (char*) opt;
	flagNeedsArg = NULL;
}

static void add_positional_arg (const char *arg)
{
	if (CxaPositional.len == CxaPositional.cap)
	{
		CxaPositional.cap += CxaPositional.factor;
		CxaPositional.args = (char**) reallocarray(CxaPositional.args, CxaPositional.cap, sizeof(char**));
		assert(CxaPositional.args && "SYSTEM: cannot reallocate memory");
	}
	CxaPositional.args[CxaPositional.len++] = (char*) arg;
}

static void make_sure_flag_has_its_arg (void)
{
	if (flagNeedsArg != NULL && flagNeedsArg->needs == CXA_FLAG_NEEDS_ARG_YES && flagNeedsArg->argument == NULL)
	{
		error_missing_argument();
	}
}

static void hanlde_longnames (const char *opt, const size_t len, struct CxaFlag *flags)
{
	const char *nodashes = opt + 2;
	char *possibleArg    = NULL;

	unsigned short flagNameLen = 0;

	for (size_t i = 0; nodashes[i]; i++)
	{
		if (nodashes[i] == '=') { possibleArg = nodashes + i + 1; break; }
		flagNameLen++;
	}

	for (unsigned short i = 0; flags[i].longName; i++)
	{
		struct CxaFlag *flag = &flags[i];
		if (flag->seen == CXA_FLAG_WAS_SEEN) { return; }

		const unsigned short thisLen = strlen(flag->longName);

		if (!strncmp(nodashes, flag->longName, (flagNameLen <  thisLen) ? flagNameLen : thisLen))
		{
			flagNeedsArg = flag;
			flag->seen = CXA_FLAG_WAS_SEEN;

			if (possibleArg) { flag->argument = possibleArg; }
			return;
		}
	}
	error_unknown_longname(nodashes, flagNameLen);
}

#endif
