/*
 * cxa - C executable arguments parser
 * Jul 6, 2025
 * C implementation
 *
 * If the programmer only wants to use the header file they'll
 * have to define CXA_ONLY_HEADER before including cxa.h in their
 * main file, like this:
 *
 * #define CXA_ONLY_HEADER
 * #include "cxa.h"
 *
 * and do not include cxa.c in the compilation process in order
 * to avoid redefinitions
 */
#ifndef CXA_ONLY_HEADER
#include "cxa.h"

void cxa_parse_arguments (struct CxaFlag *flags, const int argc, char **argv)
{
	printf("hello world from C file\n");
}

#endif
