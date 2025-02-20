#pragma once

#if defined(_DEBUG)
	#define DONE(step_name) std::cout <<  #step_name " Done!" << std::endl
	#ifdef _MSC_VER
		#include <crtdbg.h>
	#define LANG_ASSERT(condition, ...) _ASSERT_EXPR(condition, L#__VA_ARGS__)
	#else
		#include <cassert>
		#include <stdio.h>
		#include <stdlib.h>

		#define LANG_ASSERT(condition) \
			if(!condition) \
			{ \
				assert(condition); \
				printf(__VA_ARGS__);
			} else { }	
	#endif // _MSC_VER
#else
	#define LANG_ASSERT(condition, ...)
	#define DONE(step_name)
#endif