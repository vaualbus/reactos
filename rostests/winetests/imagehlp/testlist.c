/* Automatically generated file; DO NOT EDIT!! */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define STANDALONE
#include "wine/test.h"

extern void func_image(void);
extern void func_integrity(void);

const struct test winetest_testlist[] =
{
    { "image", func_image },
	{ "integrity", func_integrity },
    { 0, 0 }
};
