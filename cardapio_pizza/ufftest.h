#ifndef UFF_TEST_H
#define UFF_TEST_H
#define MAX_FAILS 100
#define MSG_SIZE 255
#define EPSILON 0.0001

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define TOKENPASTE(x, y) x ## y
#define TOKENPASTE2(x, y) TOKENPASTE(x, y)


#define ABS(x) ((x) < 0 ? -(x) : (x))
int compare_double(double a, double b) {
	return (ABS(a - b) < EPSILON);
}

#define BEGIN_TESTS() \
	int main(void) { \
		char *message = NULL; \
		clock_t begin, end; \
		double time_spent = 0.0; \
		int tests = 0; \
		int test_failed = 0; \
		int fails = 0; \
		int fail_it = 0; \
		int skip = 1; \
		int executing_test = 0; \
		char last[MSG_SIZE]; \
		char extra[MSG_SIZE]; \
		char messages[MAX_FAILS][MSG_SIZE]; \
		begin = clock();

#define CHECK_TEST() \
	if (executing_test) { \
		if (test_failed) { \
			printf("F"); \
		} else { \
			printf("."); \
		} \
		AFTER_TEST \
	}

#define END_TESTS() \
		CHECK_TEST() \
		end = clock(); \
		time_spent = (double)(end - begin) / CLOCKS_PER_SEC; \
		printf("\n"); \
		if (fails) { \
			printf("======================================================================\n"); \
			for (fail_it = 0; fail_it < fails; fail_it++) { \
				printf("FAIL: %s\n", messages[fail_it * 2]); \
				printf("%s\n", messages[fail_it * 2 + 1]); \
			} \
		} \
		printf("----------------------------------------------------------------------\n"); \
		printf("Ran %d tests in %f\n\n", tests, time_spent); \
		if (fails) { \
			printf("FAILED (%d failures)\n", fails); \
		} else { \
			printf("OK\n"); \
		} \
		return fails; \
	}

#define TEST(str) do { \
		sprintf(extra, " "); \
		sprintf(last, "%s", str); \
		CHECK_TEST(); \
		executing_test = 1; \
		test_failed = 0; \
		tests++; \
		skip = 0; \
		BEFORE_TEST; \
	} while (0)
	
#define _TEST(str) skip = 1;

#define UFFASSERT(condition) \
	if (!(condition)) { \
		if (!test_failed) { \
			test_failed = 1; \
			if (fails < MAX_FAILS/2) { \
				sprintf(messages[fails * 2], "LINE %d - %s", __LINE__, last); \
				sprintf(messages[fails * 2 + 1], "   %s", extra); \
				fails++; \
			} \
		} \
	}


#define ASSERT_EQUAL(a, b) \
	sprintf(extra, "%s != %s", #a, #b); \
 	UFFASSERT(a == b)

#define ASSERT_NOT_EQUAL(a, b) \
	sprintf(extra, "%s == %s", #a, #b); \
 	UFFASSERT(a != b)

#define ASSERT_TRUE(c) \
	sprintf(extra, "%s is FALSE", #c); \
 	UFFASSERT(c)

#define ASSERT_FALSE(c) \
	sprintf(extra, "%s is TRUE", #c); \
 	UFFASSERT(!c)

#define ASSERT_EQUAL_CMP(d, a, b, cmp) \
	sprintf(extra, "%s != %s", #a, #b); \
 	UFFASSERT(cmp(d, a, b))

#define ASSERT_NOT_EQUAL_CMP(d, a, b, cmp) \
	sprintf(extra, "%s == %s", #a, #b); \
 	UFFASSERT(!cmp(d, a, b))

#define ASSERT_FLOAT_EQUAL(a, b) \
 	ASSERT_EQUAL_CMP(a, b, compare_double)

#define ASSERT_FLOAT_NOT_EQUAL(a, b) \
 	ASSERT_NOT_EQUAL_CMP(a, b, compare_double)

#ifndef BEFORE_TEST
#define BEFORE_TEST
#endif


#ifndef AFTER_TEST
#define AFTER_TEST
#endif

#endif