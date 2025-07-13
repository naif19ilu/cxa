#ifndef CXA_H
#define CXA_H

#ifdef __cplusplus
extern "C" {
#endif

/* prettiest way to define a flag since defining the struct
 * by hand is too verbose
 */
#define CXA_SET_STR(l,d,D,h,s)   {l, d, D, 0, h, CXA_FLAG_ARG_TYPE_STR, 0, s}
#define CXA_SET_CHR(l,d,D,h,s)   {l, d, D, 0, h, CXA_FLAG_ARG_TYPE_CHR, 0, s}
#define CXA_SET_SHT(l,d,D,h,s)   {l, d, D, 0, h, CXA_FLAG_ARG_TYPE_SHT, 0, s}
#define CXA_SET_INT(l,d,D,h,s)   {l, d, D, 0, h, CXA_FLAG_ARG_TYPE_INT, 0, s}
#define CXA_SET_LNG(l,d,D,h,s)   {l, d, D, 0, h, CXA_FLAG_ARG_TYPE_LNG, 0, s}
#define CXA_SET_DBL(l,d,D,h,s)   {l, d, D, 0, h, CXA_FLAG_ARG_TYPE_DBL, 0, s}

/* this flag must always be the last in your array to indicate
 * there's no more flags to be parsed
 */
#define CXA_SET_END             {NULL}

/* Specifies if the flag takes an argument
 * ex: --source=arg or -s arg
 */
#define CXA_FLAG_TAKER_NON      0
#define CXA_FLAG_TAKER_MAY      1
#define CXA_FLAG_TAKER_YES      2

/* Specifies the type expected of the argument
 * (only makes sense if CxaFlagHasArg is set to 1)
 */
#define CXA_FLAG_ARG_TYPE_STR   0
#define CXA_FLAG_ARG_TYPE_CHR   1
#define CXA_FLAG_ARG_TYPE_SHT   2
#define CXA_FLAG_ARG_TYPE_INT   3
#define CXA_FLAG_ARG_TYPE_LNG   4
#define CXA_FLAG_ARG_TYPE_DBL   5

/* When the library already has parsed all flags the programmer
 * will need to know whether the flag was given or not
 */
#define CXA_FLAG_WASNT_SEEN     0
#define CXA_FLAG_WAS_SEEN       1

#define CXA_FLAG_ARG_GIVEN_NON  0
#define CXA_FLAG_ARG_GIVEN_YES  1

/* Factor in which the positional arguments grows
 */
#define CXA_POS_ARGS_GROWTH_FAC 32

typedef unsigned char CxaFlagHasArg;
typedef unsigned char CxaFlagArgType;

typedef unsigned char CxaFlagWasSeen;
typedef unsigned char CxaFlagArgGiven;

struct CxaFlag
{
	char            *longname;
	char            *description;
	void            *destination;
	CxaFlagWasSeen  seen;
	CxaFlagHasArg   has;
	CxaFlagArgType  argtype;
	CxaFlagArgGiven argiven;
	char            shortname;
};

void cxa_execute (const unsigned char, char**, struct CxaFlag*, const char*);
void cxa_print_usage (const char*, const struct CxaFlag*);

#ifdef __cplusplus
}
#endif

#endif
