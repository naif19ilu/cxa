#ifndef CXA_H
#define CXA_H

#ifdef __cplusplus
extern "C" {
#endif

/* prettiest way to define a flag since defining the struct
 * by hand is too verbose
 */
#define CXA_SET_STR(l,d,D,t,s)   {l, d, D, 0, (t | CXA_FLAG_ARG_TYPE_STR | CXA_FLAG_ARG_WASNT_SEEN | CXA_FLAG_ARG_GIVEN_NON), s}
#define CXA_SET_CHR(l,d,D,t,s)   {l, d, D, 0, (t | CXA_FLAG_ARG_TYPE_CHR | CXA_FLAG_ARG_WASNT_SEEN | CXA_FLAG_ARG_GIVEN_NON), s}
#define CXA_SET_SHT(l,d,D,t,s)   {l, d, D, 0, (t | CXA_FLAG_ARG_TYPE_SHT | CXA_FLAG_ARG_WASNT_SEEN | CXA_FLAG_ARG_GIVEN_NON), s}
#define CXA_SET_INT(l,d,D,t,s)   {l, d, D, 0, (t | CXA_FLAG_ARG_TYPE_INT | CXA_FLAG_ARG_WASNT_SEEN | CXA_FLAG_ARG_GIVEN_NON), s}
#define CXA_SET_LNG(l,d,D,t,s)   {l, d, D, 0, (t | CXA_FLAG_ARG_TYPE_LNG | CXA_FLAG_ARG_WASNT_SEEN | CXA_FLAG_ARG_GIVEN_NON), s}
#define CXA_SET_DBL(l,d,D,t,s)   {l, d, D, 0, (t | CXA_FLAG_ARG_TYPE_DBL | CXA_FLAG_ARG_WASNT_SEEN | CXA_FLAG_ARG_GIVEN_NON), s}

/* this flag must always be the last in your array to indicate
 * there's no more flags to be parsed
 */
#define CXA_SET_END             {NULL}

/* Specifies if the flag takes an argument
 * ex: --source=arg or -s arg
 */
#define CXA_FLAG_TAKER_NON      0b00000000
#define CXA_FLAG_TAKER_MAY      0b00000001
#define CXA_FLAG_TAKER_YES      0b00000010

/* Specifies the type expected of the argument
 * (only makes sense if CxaFlagHasArg is set to 1)
 */
#define CXA_FLAG_ARG_TYPE_STR   0b00000000
#define CXA_FLAG_ARG_TYPE_CHR   0b00000100
#define CXA_FLAG_ARG_TYPE_SHT   0b00001000
#define CXA_FLAG_ARG_TYPE_INT   0b00001100
#define CXA_FLAG_ARG_TYPE_LNG   0b00010000
#define CXA_FLAG_ARG_TYPE_DBL   0b00010100

/* When the library already has parsed all flags the programmer
 * will need to know whether the flag was given or not
 */
#define CXA_FLAG_WASNT_SEEN     0b00000000
#define CXA_FLAG_WAS_SEEN       0b00100000

#define CXA_FLAG_ARG_GIVEN_NON  0b00000000
#define CXA_FLAG_ARG_GIVEN_YES  0b01000000

/* The programmer which is using this library should only
 * use CXA_FLAG_SEEN_MASK in their caller function, others
 * are only meaningful here
 */
#define CXA_FLAG_TAKER_MASK     0b00000011
#define CXA_ARG_TYPE_MASK       0b00011100
#define CXA_FLAG_SEEN_MASK      0b00100000
#define CXA_FLAG_ARG_GIVEN_MASK 0b01000000

/* Factor in which the positional arguments grows
 */
#define CXA_POS_ARGS_GROWTH_FAC 32

typedef unsigned char CxaFlagHasArg;
typedef unsigned char CxaFlagArgType;

typedef unsigned char CxaFlagWasSeen;
typedef unsigned char CxaFlagArgGiven;

typedef unsigned char CxaFlagMeta;

struct CxaFlag
{
	char            *longname;
	char            *description;
	void            *destination;
	CxaFlagMeta     meta;
	char            shortname;
};

struct Cxa
{
	char          **positional;
	char          *stdinOpt;
	unsigned long len;
	unsigned long cap;
};

struct Cxa *cxa_execute (const unsigned char, char**, struct CxaFlag*, const char*);
void cxa_print_usage (const char*, const struct CxaFlag*);
void cxa_clean (struct Cxa*);

#ifdef __cplusplus
}
#endif

#endif
