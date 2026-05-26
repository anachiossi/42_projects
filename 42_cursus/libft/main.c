/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <anade-mo <marvin@42.fr>>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 14:27:39 by anade-mo          #+#    #+#             */
/*   Updated: 2026/05/20 14:28:37 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <bsd/string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct s_int_test
{
	const char	*lable;      /* label printed next to the result     */
	int			input;      /* value passed to both yours and libc  */
	int			expected;   /* pre-computed using original functions isdigit(input) etc.    */
}	t_int_test;

typedef struct s_strlen_test
{
	const char	*lable;
	const char	*input;
	size_t		expected;   /* pre-computed: strlen(input)          */
}	t_strlen_test;

/* int f(const char *) — atoi */
typedef struct s_atoi_test
{
	const char	*lable;
	const char	*input;
	int			expected;
}	t_atoi_test;

/* int f(const char *, const char *, size_t) — strncmp */
typedef struct s_strncmp_test
{
	const char	*lable;
	const char	*s1;
	const char	*s2;
	size_t		n;
	int			expected;
}	t_strncmp_test;

/* char *f(const char *, int) — strchr, strrchr */
/* expected_off: byte offset from str start where result lands; -1 means NULL */
typedef struct s_strfind_test
{
	const char	*lable;
	const char	*str;
	int			c;
	int			expected_off;
}	t_strfind_test;

/* void *f(const void *, int, size_t) — memchr */
typedef struct s_memfind_test
{
	const char	*lable;
	const char	*buf; // raw bytes, not necessarily a null-terminated string
	int			c;
	size_t		n;
	int			expected_off;
}	t_memfind_test;

/* char *f(const char *, const char *, size_t) — strnstr */
typedef struct s_strnstr_test
{
	const char	*lable;
	const char	*haystack;
	const char	*needle;
	size_t		len;
	int			expected_off; // byte offset from haystack start where result lands; -1 means NULL
}	t_strnstr_test;

/* char *f(int) — ft_itoa */
typedef struct s_itoa_test
{
	const char	*lable;
	int			n;
	const char	*expected;
}	t_itoa_test;

/* char *f(const char *) — ft_strdup */
typedef struct s_strdup_test
{
	const char	*lable;
	const char	*input;
	const char	*expected;
}	t_strdup_test;

/* char *f(const char *, unsigned int, size_t) — ft_substr */
typedef struct s_substr_test
{
	const char		*lable;
	const char		*s;
	unsigned int	start;
	size_t			len;
	const char		*expected;
}	t_substr_test;

/* char *f(const char *, const char *) — ft_strjoin */
typedef struct s_strjoin_test
{
	const char	*lable;
	const char	*s1;
	const char	*s2;
	const char	*expected;
}	t_strjoin_test;

/* char *f(const char *, const char *) — ft_strtrim */
typedef struct s_strtrim_test
{
	const char	*lable;
	const char	*s1;
	const char	*set;
	const char	*expected;
}	t_strtrim_test;

/* char **f(const char *, char) — ft_split */
/* expected is a NULL-terminated word list embedded in the struct */
typedef struct s_split_test
{
	const char	*lable;
	const char	*s;
	char		c;
	int			expected_count;
	const char	*expected[12]; // enough for the test cases, including the final NULL
}	t_split_test;

/* size_t f(char *, const char *, size_t) — strlcpy, strlcat */
/* dst is mutable: copy dst_init to a fresh buffer before each call */
typedef struct s_strl_test
{
	const char	*lable;
	const char	*dst_init;
	const char	*src;
	size_t		size;
	size_t		expected_ret; // pre-computed: strlcpy(dst_init, src, size) or strlcat(dst_init, src, size)
	const char	*expected_dst; // pre-computed: dst_init after strlcpy(dst_init, src, size) or strlcat(dst_init, src, size)
}	t_strl_test;

/* void *f(void *, int, size_t) — ft_memset */
typedef struct s_memset_test
{
	const char	*lable;
	const char	*init;
	int			c;
	size_t		n;
}	t_memset_test;

/* void f(void *, size_t) — ft_bzero */
typedef struct s_bzero_test
{
	const char	*lable;
	const char	*init;
	size_t		n;
}	t_bzero_test;

/* void *f(void *, const void *, size_t) — ft_memcpy, ft_memmove */
typedef struct s_memcopy_test
{
	const char	*lable;
	const char	*dst_init;
	const char	*src_init;
	size_t		n;
	int			overlap;   /* 1 = call with dst+5,src instead of dst,src */
}	t_memcopy_test;

static void	test_isalpha(void)
{
	t_int_test	tests[] = {
		{"'A' uppercase",  'A',   isalpha('A')},
		{"'z' lowercase",  'z',   isalpha('z')},
		{"'d' lowercase",  'd',   isalpha('d')},
		{"'{' just above", '{',   isalpha('{')},
		{"' ' space",      ' ',   isalpha(' ')},
	};
	int			n = sizeof(tests) / sizeof(tests[0]);
	int			i;
	int			got;

	printf("\t🔵 Testing ft_isalpha 🔵\n\tint ft_isalpha(int c)\n\n");
	i = 0;
	while (i < n)
	{
		got = ft_isalpha(tests[i].input);
		printf("Test %02d - %s\n", i + 1, tests[i].lable);
		printf("  yours:    [%d]\n  expected: [%d]  %s\n\n",
			got, tests[i].expected,
			(!!got == !!tests[i].expected) ? "✅ PASS" : "❌ FAIL");
		i++;
	}
}

static void	test_isdigit(void)
{
	t_int_test	tests[] = {
		{"'0' lower bound", '0', isdigit('0')},
		{"'6' middle",      '6', isdigit('6')},
		{"'9' upper bound", '9', isdigit('9')},
		{"'/' just below",  '/', isdigit('/')},
		{"':' just above",  ':', isdigit(':')},
	};
	int			n = sizeof(tests) / sizeof(tests[0]);
	int			i;
	int			got;

	printf("\t🔵 Testing ft_isdigit 🔵\n\tint ft_isdigit(int c)\n\n");
	i = 0;
	while (i < n)
	{
		got = ft_isdigit(tests[i].input);
		printf("Test %02d - %s\n", i + 1, tests[i].lable);
		printf("  yours:    [%c]\n  expected: [%c]  %s\n\n",
			got, tests[i].expected,
			(!!got == !!tests[i].expected) ? "✅ PASS" : "❌ FAIL");
		i++;
	}
}

static void	test_isalnum(void)
{
	t_int_test	tests[] = {
		{"'A' letter",   'A', isalnum('A')},
		{"'9' digit",    '9', isalnum('9')},
		{"'?' neither",  '?', isalnum('?')},
	};
	int			n = sizeof(tests) / sizeof(tests[0]);
	int			i;
	int			got;

	printf("\t🔵 Testing ft_isalnum 🔵\n\tint ft_isalnum(int c)\n\n");
	i = 0;
	while (i < n)
	{
		got = ft_isalnum(tests[i].input);
		printf("Test %02d - %s\n", i + 1, tests[i].lable);
		printf("  yours:    [%c]\n  expected: [%c]  %s\n\n",
			got, tests[i].expected,
			(!!got == !!tests[i].expected) ? "✅ PASS" : "❌ FAIL");
		i++;
	}
}

static void	test_isascii(void)
{
	t_int_test	tests[] = {
		{"'\\t' tab (ctrl char)",  '\t',   isascii('\t')},
		{"'\\0' NUL",              '\0',   isascii('\0')},
		{"ESC (033)",              '\033', isascii('\033')},
		{"-1 (out of range)",      -1,     isascii(-1)},
		{"0 (NUL via int)",        0,      isascii(0)},
	};
	int			n = sizeof(tests) / sizeof(tests[0]);
	int			i;
	int			got;

	printf("\t🔵 Testing ft_isascii 🔵\n\tint ft_isascii(int c)\n\n");
	i = 0;
	while (i < n)
	{
		got = ft_isascii(tests[i].input);
		printf("Test %02d - %s\n", i + 1, tests[i].lable);
		printf("  yours:    [%c]\n  expected: [%c]  %s\n\n",
			got, tests[i].expected,
			(!!got == !!tests[i].expected) ? "✅ PASS" : "❌ FAIL");
		i++;
	}
}

static void	test_isprint(void)
{
	t_int_test	tests[] = {
		{"'A' letter",        'A',  isprint('A')},
		{"' ' space",         ' ',  isprint(' ')},
		{"'~' upper bound",   '~',  isprint('~')},
		{"'\\t' tab",         '\t', isprint('\t')},
		{"DEL (127)",         127,  isprint(127)},
	};
	int			n = sizeof(tests) / sizeof(tests[0]);
	int			i;
	int			got;

	printf("\t🔵 Testing ft_isprint 🔵\n\tint ft_isprint(int c)\n\n");
	i = 0;
	while (i < n)
	{
		got = ft_isprint(tests[i].input);
		printf("Test %02d - %s\n", i + 1, tests[i].lable);
		printf("  yours:    [%c]\n  expected: [%c]  %s\n\n",
			got, tests[i].expected,
			(!!got == !!tests[i].expected) ? "✅ PASS" : "❌ FAIL");
		i++;
	}
}

static void	test_strlen(void)
{
	t_strlen_test	tests[] = {
		{"\"Roma\"",
			"Roma",
			strlen("Roma")},
		{"\"42\"",
			"42",
			strlen("42")},
		{"long sentence",
			"To be or not to be, that is the question. ",
			strlen("To be or not to be, that is the question. ")},
	};
	int				n = sizeof(tests) / sizeof(tests[0]);
	int				i;
	size_t			got;

	printf("\t🔵 Testing ft_strlen 🔵\n\tsize_t ft_strlen(const char *s)\n\n");
	i = 0;
	while (i < n)
	{
		got = ft_strlen(tests[i].input);
		printf("Test %02d - %s\n", i + 1, tests[i].lable);
		printf("  yours:    [%zu]\n  expected: [%zu]  %s\n\n",
			got, tests[i].expected,
			(got == tests[i].expected) ? "✅ PASS" : "❌ FAIL");
		i++;
	}
}

static void	test_memset(void)
{
	t_memset_test	tests[] = {
		{"space fill n=2",  "unbelievable", ' ',   2},
		{"zero  fill n=2",  "unbelievable", 0,     2},
		{"0xFF  fill n=2",  "unbelievable", 255,   2},
		{"dot   fill n=0",  "unbelievable", '.',   0},
	};
	int				n = sizeof(tests) / sizeof(tests[0]);
	int				i;
	char			yours[50];
	char			ref[50];

	printf("\t🔵 Testing ft_memset 🔵\n\tvoid *ft_memset(void *s, int c, size_t n)\n\n");
	i = 0;
	while (i < n)
	{
		memset(yours, 0, 50);
		memset(ref, 0, 50);
		memcpy(yours, tests[i].init, strlen(tests[i].init) + 1);
		memcpy(ref, tests[i].init, strlen(tests[i].init) + 1);
		ft_memset(yours, tests[i].c, tests[i].n);
		memset(ref, tests[i].c, tests[i].n);
		printf("Test %02d - %s\n", i + 1, tests[i].lable);
		printf("  init: \"%s\", c: %d, n: %zu\n",
			tests[i].init, tests[i].c, tests[i].n);
		printf("  yours:    [%s]\n  expected: [%s]  %s\n\n",
			yours, ref,
			memcmp(yours, ref, 50) == 0 ? "✅ PASS" : "❌ FAIL");
		i++;
	}
}

static void	test_bzero(void)
{
	t_bzero_test	tests[] = {
		{"zero n=2 bytes", "unbelievable", 2},
	};
	int				n = sizeof(tests) / sizeof(tests[0]);
	int				i;
	int				j;
	char			yours[50];
	char			ref[50];

	printf("\t🔵 Testing ft_bzero 🔵\n\tvoid ft_bzero(void *s, size_t n)\n\n");
	i = 0;
	while (i < n)
	{
		memset(yours, 0, 50);
		memset(ref, 0, 50);
		memcpy(yours, tests[i].init, strlen(tests[i].init) + 1);
		memcpy(ref, tests[i].init, strlen(tests[i].init) + 1);
		ft_bzero(yours, tests[i].n);
		bzero(ref, tests[i].n);
		printf("Test %02d - %s\n", i + 1, tests[i].lable);
		printf("  init: \"%s\", n: %zu\n", tests[i].init, tests[i].n);
		printf("  bytes yours:    ");
		j = 0;
		while (j < 12)
			printf("[%d]", ((unsigned char *)yours)[j++]);
		printf("\n  bytes expected: ");
		j = 0;
		while (j < 12)
			printf("[%d]", ((unsigned char *)ref)[j++]);
		printf("  %s\n\n",
			memcmp(yours, ref, 50) == 0 ? "✅ PASS" : "❌ FAIL");
		i++;
	}
}

static void	test_memcpy(void)
{
	t_memcopy_test	tests[] = {
		{"n=4  partial",  " sunlight", "moonflower", 4,  0},
		{"n=10 full src", " sunlight", "moonflower", 10, 0},
		{"n=0  noop",     " sunlight", "moonflower", 0,  0},
	};
	int				n = sizeof(tests) / sizeof(tests[0]);
	int				i;
	char			yours_dst[50];
	char			yours_src[50];
	char			ref_dst[50];
	char			ref_src[50];

	printf("\t🔵 Testing ft_memcpy 🔵\n\tvoid *ft_memcpy(void *dest, const void *src, size_t n)\n\n");
	i = 0;
	while (i < n)
	{
		memset(yours_dst, 0, 50); memset(yours_src, 0, 50);
		memset(ref_dst, 0, 50);   memset(ref_src, 0, 50);
		memcpy(yours_dst, tests[i].dst_init, strlen(tests[i].dst_init) + 1);
		memcpy(yours_src, tests[i].src_init, strlen(tests[i].src_init) + 1);
		memcpy(ref_dst, tests[i].dst_init, strlen(tests[i].dst_init) + 1);
		memcpy(ref_src, tests[i].src_init, strlen(tests[i].src_init) + 1);
		ft_memcpy(yours_dst, yours_src, tests[i].n);
		memcpy(ref_dst, ref_src, tests[i].n);
		printf("Test %02d - %s\n", i + 1, tests[i].lable);
		printf("  src: \"%s\", dst: \"%s\", n: %zu\n",
			tests[i].src_init, tests[i].dst_init, tests[i].n);
		printf("  yours:    [%s]\n  expected: [%s]  %s\n\n",
			yours_dst, ref_dst,
			memcmp(yours_dst, ref_dst, 50) == 0 ? "✅ PASS" : "❌ FAIL");
		i++;
	}
}

static void	test_memmove(void)
{
	/*
	** overlap=0: ft_memmove(dst, src+5, n)  — src is ahead of dst
	** overlap=1: ft_memmove(dst+5, src, n)  — dst is ahead of src (overlapping)
	*/
	t_memcopy_test	tests[] = {
		{"non-overlapping dst,src+5", "ping-pong", "ping-pong", 4, 0},
		{"overlapping     dst+5,src", "ping-pong", "ping-pong", 4, 1},
	};
	int				n = sizeof(tests) / sizeof(tests[0]);
	int				i;
	char			yours[50];
	char			ref[50];

	printf("\t🔵 Testing ft_memmove 🔵\n\tvoid *ft_memmove(void *dest, const void *src, size_t n)\n\n");
	i = 0;
	while (i < n)
	{
		memset(yours, 0, 50);
		memset(ref, 0, 50);
		memcpy(yours, tests[i].src_init, strlen(tests[i].src_init) + 1);
		memcpy(ref, tests[i].src_init, strlen(tests[i].src_init) + 1);
		if (tests[i].overlap)
		{
			ft_memmove(yours + 5, yours, tests[i].n);
			memmove(ref + 5, ref, tests[i].n);
		}
		else
		{
			ft_memmove(yours, yours + 5, tests[i].n);
			memmove(ref, ref + 5, tests[i].n);
		}
		printf("Test %02d - %s\n", i + 1, tests[i].lable);
		printf("  init: \"%s\", n: %zu\n", tests[i].src_init, tests[i].n);
		printf("  yours:    [%s]\n  expected: [%s]  %s\n\n",
			yours, ref,
			memcmp(yours, ref, 50) == 0 ? "✅ PASS" : "❌ FAIL");
		i++;
	}
}

static void	test_strlcpy(void)
{
	t_strl_test	tests[] = {
		{"size < src+dst-1", "Don`t", " Panic", 6,  6, " Pani"},
		{"size > src+dst-1", "Don`t", " Panic", 20, 6, " Panic"},
	};
	int			n = sizeof(tests) / sizeof(tests[0]);
	int			i;
	char		yours[50];
	size_t		ret;

	printf("\t🔵 Testing ft_strlcpy 🔵\n\tsize_t ft_strlcpy(char *dst, const char *src, size_t size)\n\n");
	i = 0;
	while (i < n)
	{
		memset(yours, 0, 50);
		memcpy(yours, tests[i].dst_init, strlen(tests[i].dst_init) + 1);
		ret = ft_strlcpy(yours, tests[i].src, tests[i].size);
		printf("Test %02d - %s\n", i + 1, tests[i].lable);
		printf("  src: \"%s\", dst_init: \"%s\", size: %zu\n",
			tests[i].src, tests[i].dst_init, tests[i].size);
		printf("  yours:    dst=[%s] ret=%zu\n", yours, ret);
		printf("  expected: dst=[%s] ret=%zu  %s\n\n",
			tests[i].expected_dst, tests[i].expected_ret,
			(ret == tests[i].expected_ret
				&& strcmp(yours, tests[i].expected_dst) == 0)
			? "✅ PASS" : "❌ FAIL");
		i++;
	}
}

static void	test_strlcat(void)
{
	t_strl_test	tests[] = {
		{"size=6  no room to append", "Don`t", " Panic", 6,  11, "Don`t"},
		{"size=20 full append",       "Don`t", " Panic", 20, 11, "Don`t Panic"},
	};
	int			n = sizeof(tests) / sizeof(tests[0]);
	int			i;
	char		yours[50];
	size_t		ret;

	printf("\t🔵 Testing ft_strlcat 🔵\n\tsize_t ft_strlcat(char *dst, const char *src, size_t size)\n\n");
	i = 0;
	while (i < n)
	{
		memset(yours, 0, 50);
		memcpy(yours, tests[i].dst_init, strlen(tests[i].dst_init) + 1);
		ret = ft_strlcat(yours, tests[i].src, tests[i].size);
		printf("Test %02d - %s\n", i + 1, tests[i].lable);
		printf("  src: \"%s\", dst_init: \"%s\", size: %zu\n",
			tests[i].src, tests[i].dst_init, tests[i].size);
		printf("  yours:    dst=[%s] ret=%zu\n", yours, ret);
		printf("  expected: dst=[%s] ret=%zu  %s\n\n",
			tests[i].expected_dst, tests[i].expected_ret,
			(ret == tests[i].expected_ret
				&& strcmp(yours, tests[i].expected_dst) == 0)
			? "✅ PASS" : "❌ FAIL");
		i++;
	}
}

static void	test_toupper(void)
{
	t_int_test	tests[] = {
		{"'a' lowercase letter",  'a', toupper('a')},
		{"'Z' already uppercase", 'Z', toupper('Z')},
	};
	int			n = sizeof(tests) / sizeof(tests[0]);
	int			i;
	int			got;

	printf("\t🔵 Testing ft_toupper 🔵\n\tint ft_toupper(int c)\n\n");
	i = 0;
	while (i < n)
	{
		got = ft_toupper(tests[i].input);
		printf("Test %02d - %s\n", i + 1, tests[i].lable);
		printf("  yours:    [%c] (%d)\n  expected: [%c] (%d)  %s\n\n",
			got, got, tests[i].expected, tests[i].expected,
			(got == tests[i].expected) ? "✅ PASS" : "❌ FAIL");
		i++;
	}
}

static void	test_tolower(void)
{
	t_int_test	tests[] = {
		{"'A' uppercase",       'A', tolower('A')},
		{"'z' already lower",   'z', tolower('z')},
	};
	int			n = sizeof(tests) / sizeof(tests[0]);
	int			i;
	int			got;

	printf("\t🔵 Testing ft_tolower 🔵\n\tint ft_tolower(int c)\n\n");
	i = 0;
	while (i < n)
	{
		got = ft_tolower(tests[i].input);
		printf("Test %02d - %s\n", i + 1, tests[i].lable);
		printf("  yours:    [%c] (%d)\n  expected: [%c] (%d)  %s\n\n",
			got, got, tests[i].expected, tests[i].expected,
			(got == tests[i].expected) ? "✅ PASS" : "❌ FAIL");
		i++;
	}
}

static void	test_strchr(void)
{
	/* "augur_core": a=0 u=1 g=2 u=3 r=4 _=5 c=6 o=7 r=8 e=9 \0=10 */
	const char		*str = "augur_core";
	const char		*empty = "";
	t_strfind_test	tests[] = {
		{"positive 'o'",      str,   'o',  7},
		{"negative 'x'",      str,   'x',  -1},
		{"null char '\\0'",   str,   '\0', 10},
		{"empty str 'a'",     empty, 'a',  -1},
	};
	int				n = sizeof(tests) / sizeof(tests[0]);
	int				i;
	char			*got;
	int				got_off;

	printf("\t🔵 Testing ft_strchr 🔵\n\tchar *ft_strchr(const char *s, int c)\n\n");
	i = 0;
	while (i < n)
	{
		got = ft_strchr(tests[i].str, tests[i].c);
		got_off = got ? (int)(got - tests[i].str) : -1;
		printf("Test %02d - %s\n", i + 1, tests[i].lable);
		printf("  yours:    [%s] offset=%d\n", got ? got : "(null)", got_off);
		printf("  expected: [%s] offset=%d  %s\n\n",
			tests[i].expected_off >= 0
				? tests[i].str + tests[i].expected_off : "(null)",
			tests[i].expected_off,
			got_off == tests[i].expected_off ? "✅ PASS" : "❌ FAIL");
		i++;
	}
}

static void	test_strrchr(void)
{
	const char		*str = "augur_core";
	const char		*empty = "";
	t_strfind_test	tests[] = {
		{"positive 'o'",      str,   'o',  7},
		{"negative 'x'",      str,   'x',  -1},
		{"null char '\\0'",   str,   '\0', 10},
		{"empty str 'a'",     empty, 'a',  -1},
	};
	int				n = sizeof(tests) / sizeof(tests[0]);
	int				i;
	char			*got;
	int				got_off;

	printf("\t🔵 Testing ft_strrchr 🔵\n\tchar *ft_strrchr(const char *s, int c)\n\n");
	i = 0;
	while (i < n)
	{
		got = ft_strrchr(tests[i].str, tests[i].c);
		got_off = got ? (int)(got - tests[i].str) : -1;
		printf("Test %02d - %s\n", i + 1, tests[i].lable);
		printf("  yours:    [%s] offset=%d\n", got ? got : "(null)", got_off);
		printf("  expected: [%s] offset=%d  %s\n\n",
			tests[i].expected_off >= 0
				? tests[i].str + tests[i].expected_off : "(null)",
			tests[i].expected_off,
			got_off == tests[i].expected_off ? "✅ PASS" : "❌ FAIL");
		i++;
	}
}

static void	test_strncmp(void)
{
	t_strncmp_test	tests[] = {
		{"equal n=5",           "augur_core", "augur_core",  5,  0},
		{"equal n>strlen",      "augur_core", "augur_core",  20, 0},
		{"equal n=0",           "augur_core", "augur_core",  0,  0},
		{"diff  n=strlen(s1)",  "augur_core", "augur_coreX", 11, strncmp("augur_core", "augur_coreX", 11)},
		{"diff  n<strlen(s1)",  "augur_core", "augur_coreX", 5,  0},
	};
	int				n = sizeof(tests) / sizeof(tests[0]);
	int				i;
	int				got;

	printf("\t🔵 Testing ft_strncmp 🔵\n\tint ft_strncmp(const char *s1, const char *s2, size_t n)\n\n");
	i = 0;
	while (i < n)
	{
		got = ft_strncmp(tests[i].s1, tests[i].s2, tests[i].n);
		printf("Test %02d - %s\n", i + 1, tests[i].lable);
		printf("  s1: \"%s\"  s2: \"%s\"  n: %zu\n",
			tests[i].s1, tests[i].s2, tests[i].n);
		printf("  yours:    [%d]\n  expected: [%d]  %s\n\n",
			got, tests[i].expected,
			(got == 0) == (tests[i].expected == 0)
			&& (got < 0) == (tests[i].expected < 0)
			? "✅ PASS" : "❌ FAIL");
		i++;
	}
}

static void	test_memchr(void)
{
	/* "augur_core" 10 bytes (0-9), null at 10 — not included in n=10 scan */
	const char		*str = "augur_core";
	const char		*empty = "";
	t_memfind_test	tests[] = {
		{"positive 'o'  n=10",     str,   'o',  10, 7},
		{"negative 'x'  n=10",     str,   'x',  10, -1},
		{"null '\\0'    n=10",     str,   '\0', 10, -1},
		{"empty buf 'a' n=1",      empty, 'a',  1,  -1},
	};
	int				n = sizeof(tests) / sizeof(tests[0]);
	int				i;
	void			*got;
	int				got_off;

	printf("\t🔵 Testing ft_memchr 🔵\n\tvoid *ft_memchr(const void *s, int c, size_t n)\n\n");
	i = 0;
	while (i < n)
	{
		got = ft_memchr(tests[i].buf, tests[i].c, tests[i].n);
		got_off = got ? (int)((char *)got - tests[i].buf) : -1;
		printf("Test %02d - %s\n", i + 1, tests[i].lable);
		printf("  yours:    [%s] offset=%d\n",
			got ? (char *)got : "(null)", got_off);
		printf("  expected: [%s] offset=%d  %s\n\n",
			tests[i].expected_off >= 0
				? tests[i].buf + tests[i].expected_off : "(null)",
			tests[i].expected_off,
			got_off == tests[i].expected_off ? "✅ PASS" : "❌ FAIL");
		i++;
	}
}

static void	test_strnstr(void)
{
	/* haystack: "Looking for a needle in a haystack"
	** "needle" starts at offset 14, last char at 19 — fits in n=20 */
	const char		*hay = "Looking for a needle in a haystack";
	t_strnstr_test	tests[] = {
		{"needle found   n=20", hay, "needle", 20, 14},
		{"pencil missing n=5",  hay, "pencil", 5,  -1},
		{"empty needle   n=0",  hay, "",       0,  0},
	};
	int				n = sizeof(tests) / sizeof(tests[0]);
	int				i;
	char			*got;
	int				got_off;

	printf("\t🔵 Testing ft_strnstr 🔵\n\tchar *ft_strnstr(const char *haystack, const char *needle, size_t len)\n\n");
	i = 0;
	while (i < n)
	{
		got = ft_strnstr(tests[i].haystack, tests[i].needle, tests[i].len);
		got_off = got ? (int)(got - tests[i].haystack) : -1;
		printf("Test %02d - %s\n", i + 1, tests[i].lable);
		printf("  needle: \"%s\"  len: %zu\n", tests[i].needle, tests[i].len);
		printf("  yours:    [%s] offset=%d\n", got ? got : "(null)", got_off);
		printf("  expected: [%s] offset=%d  %s\n\n",
			tests[i].expected_off >= 0
				? tests[i].haystack + tests[i].expected_off : "(null)",
			tests[i].expected_off,
			got_off == tests[i].expected_off ? "✅ PASS" : "❌ FAIL");
		i++;
	}
}

static void	test_atoi(void)
{
	t_atoi_test	tests[] = {
		{"positive",           "42",             atoi("42")},
		{"negative",           "-42",            atoi("-42")},
		{"plus sign",          "+42",            atoi("+42")},
		{"double sign",        "-+42",           atoi("-+42")},
		{"double space",       "  42",           atoi("  42")},
		{"double number",      "  42  00  ",     atoi("  42  00  ")},
		{"words head",         "words 42",       atoi("words 42")},
		{"words tail",         "42 words",       atoi("42 words")},
		{"other whitespace",   "  \t\n\r\f\v  42", atoi("  \t\n\r\f\v  42")},
		{"INT_MAX",            "2147483647",     atoi("2147483647")},
		{"INT_MIN",            "-2147483648",    atoi("-2147483648")},
		{"overflow max",       "2147483649",     atoi("2147483649")},
		{"overflow min",       "-2147483649",    atoi("-2147483649")},
		{"zero",               "0",              atoi("0")},
		{"negative zero",      "-0",             atoi("-0")},
		{"only minus",         "-",              atoi("-")},
		{"only spaces",        "    ",           atoi("    ")},
		{"empty",              "",               atoi("")},
	};
	int			n = sizeof(tests) / sizeof(tests[0]);
	int			i;
	int			got;

	printf("\t🔵 Testing ft_atoi 🔵\n\tint ft_atoi(const char *str)\n\n");
	i = 0;
	while (i < n)
	{
		got = ft_atoi(tests[i].input);
		printf("Test %02d - %s\n", i + 1, tests[i].lable);
		printf("  input: \"%s\"\n", tests[i].input);
		printf("  yours:    [%d]\n  expected: [%d]  %s\n\n",
			got, tests[i].expected,
			got == tests[i].expected ? "✅ PASS" : "❌ FAIL");
		i++;
	}
}
static void	test_calloc(void)
{
	size_t	nmemb = 5;
	size_t	size = sizeof(int);
	int		*yours = (int *)ft_calloc(nmemb, size);
	int		*original = (int *)calloc(nmemb, size);

	printf("\t🔵 Testing ft_calloc 🔵\n\tvoid	*ft_calloc(size_t nmemb, size_t size)\n\n");
	printf("Test 01\nnmemb: %zu, size: %zu\n", nmemb, size);
	size_t i = 0;
	printf("yours[%zu]:    ", i);
	while (i < nmemb)
	{	
		printf("[%d] ", yours[i]);
		i++;
	}
	i = 0;
	printf("\nexpected[%zu]: ", i);
	while (i < nmemb)
	{
		printf("[%d] ", original[i]);
		i++;
	}
	printf("\n\n");
	free(yours);
	free(original);
}
static void	test_strdup(void)
{
	t_strdup_test	tests[] = {
		{"normal string", "The DRY rule: Don't Repeat Yourself.", "The DRY rule: Don't Repeat Yourself."},
		{"empty string",  "",                                      ""},
	};
	int				n = sizeof(tests) / sizeof(tests[0]);
	int				i;
	char			*got;

	printf("\t🔵 Testing ft_strdup 🔵\n\tchar *ft_strdup(const char *s)\n\n");
	i = 0;
	while (i < n)
	{
		got = ft_strdup(tests[i].input);
		printf("Test %02d - %s\n", i + 1, tests[i].lable);
		printf("  input: \"%s\"\n", tests[i].input);
		printf("  yours:    [%s]\n  expected: [%s]  %s\n\n",
			got ? got : "(null)", tests[i].expected,
			got && strcmp(got, tests[i].expected) == 0 ? "✅ PASS" : "❌ FAIL");
		free(got);
		i++;
	}
}
static void	test_substr(void)
{
	t_substr_test	tests[] = {
		{"normal",            "Malloc gives garbage", 13, 8,  "garbage"},
		{"start at 0",        "Malloc gives garbage", 0,  6,  "Malloc"},
		{"len = 0",           "Malloc gives garbage", 5,  0,  ""},
		{"start out of range","Malloc gives garbage", 50, 13, ""},
		{"len out of range",  "Malloc gives garbage", 13, 20, "garbage"},
		{"empty string",      "",                     0,  6,  ""},
		{"NULL string",       NULL,                   0,  6,  NULL},
	};
	int				n = sizeof(tests) / sizeof(tests[0]);
	int				i;
	char			*got;
	int				pass;

	printf("\t🔵 Testing ft_substr 🔵\n\tchar *ft_substr(char const *s, unsigned int start, size_t len)\n\n");
	i = 0;
	while (i < n)
	{
		got = ft_substr(tests[i].s, tests[i].start, tests[i].len);
		if (tests[i].expected == NULL)
			pass = (got == NULL);
		else
			pass = (got != NULL && strcmp(got, tests[i].expected) == 0);
		printf("Test %02d - %s\n", i + 1, tests[i].lable);
		printf("  s: \"%s\", start: %u, len: %zu\n",
			tests[i].s ? tests[i].s : "(null)", tests[i].start, tests[i].len);
		printf("  yours:    [%s]\n  expected: [%s]  %s\n\n",
			got ? got : "(null)",
			tests[i].expected ? tests[i].expected : "(null)",
			pass ? "✅ PASS" : "❌ FAIL");
		free(got);
		i++;
	}
}
static void	test_strjoin(void)
{
	t_strjoin_test	tests[] = {
		{"normal",         "Small pieces, ", "loosely joined.", "Small pieces, loosely joined."},
		{"empty s1",       "",               "loosely joined.", "loosely joined."},
		{"empty s2",       "Small pieces, ", "",               "Small pieces, "},
		{"both empty",     "",               "",               ""},
		{"s1 NULL",        NULL,             "loosely joined.", NULL},
		{"s2 NULL",        "Small pieces, ", NULL,             NULL},
		{"both NULL",      NULL,             NULL,             NULL},
		{"single chars",   "4",              "2",              "42"},
	};
	int				n = sizeof(tests) / sizeof(tests[0]);
	int				i;
	char			*got;
	int				pass;

	printf("\t🔵 Testing ft_strjoin 🔵\n\tchar *ft_strjoin(char const *s1, char const *s2)\n\n");
	i = 0;
	while (i < n)
	{
		got = ft_strjoin(tests[i].s1, tests[i].s2);
		if (tests[i].expected == NULL)
			pass = (got == NULL);
		else
			pass = (got != NULL && strcmp(got, tests[i].expected) == 0);
		printf("Test %02d - %s\n", i + 1, tests[i].lable);
		printf("  s1: \"%s\"  s2: \"%s\"\n",
			tests[i].s1 ? tests[i].s1 : "(null)",
			tests[i].s2 ? tests[i].s2 : "(null)");
		printf("  yours:    [%s]\n  expected: [%s]  %s\n\n",
			got ? got : "(null)",
			tests[i].expected ? tests[i].expected : "(null)",
			pass ? "✅ PASS" : "❌ FAIL");
		free(got);
		i++;
	}
}
static void	test_strtrim(void)
{
	const char		*s1 = "\t$6x* Trim the start and the end, set what to cut. *x2$\t";
	const char		*set = "\t$62x* ";
	const char		*no_trim = "Perfection is achieved not when there is nothing more to add, but when there is nothing left to take away.";
	const char		*all_trim = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789\t$62x*,. ";
	t_strtrim_test	tests[] = {
		{"trim both ends",      s1,      set,         "Trim the start and the end, set what to cut."},
		{"nothing to trim",     no_trim, set,         no_trim},
		{"all chars trimmed",   no_trim, all_trim,    ""},
		{"empty string",        "",      set,         ""},
		{"empty set",           s1,      "",          s1},
		{"NULL set",            s1,      NULL,        NULL},
		{"NULL string",         NULL,    set,         NULL},
		{"midtrap set",         s1,      "\t$62x* ,", "Trim the start and the end, set what to cut."},
	};
	int				n = sizeof(tests) / sizeof(tests[0]);
	int				i;
	char			*got;
	int				pass;

	printf("\t🔵 Testing ft_strtrim 🔵\n\tchar *ft_strtrim(char const *s1, char const *set)\n\n");
	i = 0;
	while (i < n)
	{
		got = ft_strtrim(tests[i].s1, tests[i].set);
		if (tests[i].expected == NULL)
			pass = (got == NULL);
		else
			pass = (got != NULL && strcmp(got, tests[i].expected) == 0);
		printf("Test %02d - %s\n", i + 1, tests[i].lable);
		printf("  yours:    [%s]\n  expected: [%s]  %s\n\n",
			got ? got : "(null)",
			tests[i].expected ? tests[i].expected : "(null)",
			pass ? "✅ PASS" : "❌ FAIL");
		free(got);
		i++;
	}
}
static void	test_ft_split(void)
{
	t_split_test	tests[] = {
		{"normal",           "Dexter is the bay harbor butcher", ' ', 6,
			{"Dexter", "is", "the", "bay", "harbor", "butcher", NULL}},
		{"empty string",     "",                                 ' ', 0,
			{NULL}},
		{"null char delim",  "Dexter is the bay harbor butcher", '\0', 1,
			{"Dexter is the bay harbor butcher", NULL}},
		{"no separator",     "Dexteristhebayharborbutcher",      ' ', 1,
			{"Dexteristhebayharborbutcher", NULL}},
		{"lead+trail sep",   "      Dexter is the bay harbor butcher      ", ' ', 6,
			{"Dexter", "is", "the", "bay", "harbor", "butcher", NULL}},
		{"multi sep",        "Dexter   is   the bay harbor    butcher",     ' ', 6,
			{"Dexter", "is", "the", "bay", "harbor", "butcher", NULL}},
		{"only separators",  ".........",                        '.', 0,
			{NULL}},
		{"tab separator",    "\tDexter\tis\tthe\tbay\tharbor\tbutcher\t",   '\t', 6,
			{"Dexter", "is", "the", "bay", "harbor", "butcher", NULL}},
		{"NULL string",      NULL,                               '\0', 0,
			{NULL}},
		{"delim==only char", "D",                               'D', 0,
			{NULL}},
		{"one-letter words", "D e x t e r",                    ' ', 6,
			{"D", "e", "x", "t", "e", "r", NULL}},
	};
	int				n = sizeof(tests) / sizeof(tests[0]);
	int				i;
	int				j;

	printf("\t🔵 Testing ft_split 🔵\n\tchar **ft_split(char const *s, char c)\n\n");
	i = 0;
	while (i < n)
	{
		char	**got = ft_split(tests[i].s, tests[i].c);
		int		count = 0;
		int		match = 1;

		printf("Test %02d - %s\n", i + 1, tests[i].lable);
		printf("  s: \"%s\", c: '%c'\n",
			tests[i].s ? tests[i].s : "(null)", tests[i].c);
		printf("  yours:\n");
		if (got == NULL)
			printf("    (null)\n");
		else
		{
			j = 0;
			while (got[j] != NULL)
			{
				printf("    [%d]: [%s]\n", j, got[j]);
				j++;
			}
			count = j;
		}
		if (count != tests[i].expected_count)
			match = 0;
		j = 0;
		while (match && tests[i].expected[j] != NULL && got && got[j] != NULL)
		{
			if (strcmp(got[j], tests[i].expected[j]) != 0)
				match = 0;
			j++;
		}
		printf("  count: got=%d expected=%d  %s\n\n",
			count, tests[i].expected_count, match ? "✅ PASS" : "❌ FAIL");
		if (got)
		{
			j = 0;
			while (got[j])
				free(got[j++]);
			free(got);
		}
		i++;
	}
}
static void	test_ft_itoa(void)
{
	t_itoa_test	tests[] = {
		{"positive",        42,          "42"},
		{"negative",        -42,         "-42"},
		{"zero",            0,           "0"},
		{"INT_MAX",         2147483647,  "2147483647"},
		{"INT_MIN",         -2147483648, "-2147483648"},
		{"negative one",    -1,          "-1"},
		{"trailing zeros",  100,         "100"},
	};
	int			n = sizeof(tests) / sizeof(tests[0]);
	int			i;
	char		*got;

	printf("\t🔵 Testing ft_itoa 🔵\n\tchar *ft_itoa(int n)\n\n");
	i = 0;
	while (i < n)
	{
		got = ft_itoa(tests[i].n);
		printf("Test %02d - %s\n", i + 1, tests[i].lable);
		printf("  n: %d\n", tests[i].n);
		printf("  yours:    [%s]\n  expected: [%s]  %s\n\n",
			got ? got : "(null)", tests[i].expected,
			got && strcmp(got, tests[i].expected) == 0 ? "✅ PASS" : "❌ FAIL");
		free(got);
		i++;
	}
}
static char	test_func_strmapi(unsigned int i, char c)
{
	(void)i;
	return (ft_toupper(c));
}
static void	test_strmapi(void)
{
	char	*s = "The variables are indepented, you could use bananas, but even passing the i it does for all characters the same action, is not much flexible. Personally kinda bummer.\n";
	char	*s_empty = "";

	printf("\t🔵 Testing ft_strmapi 🔵\n\tchar *ft_strmapi(char const *s, char (*f)(unsigned int, char))\n\n");
	char *result_ft_strmapi1 = ft_strmapi(s, test_func_strmapi);
	printf("Test 01 - normal string + function ft_toupper\ns: \"%s\"\n", s);
	printf("yours:    [%s]\n", result_ft_strmapi1);
	printf("expected: [%s]\n\n", "THE VARIABLES ARE INDEPENTED, YOU COULD USE BANANAS, BUT EVEN PASSING THE I IT DOES FOR ALL CHARACTERS THE SAME ACTION, IS NOT MUCH FLEXIBLE. PERSONALLY KINDA BUMMER.\n");
	char *result_ft_strmapi2 = ft_strmapi(s_empty, test_func_strmapi);
	printf("Test 02 - empty string\ns: \"%s\"\n", s_empty);
	printf("yours:    [%s]\n", result_ft_strmapi2);
	printf("expected: [%s]\n\n", "");
	char *result_ft_strmapi3 = ft_strmapi(s, NULL);
	printf("Test 03 - NULL function\ns: \"%s\"\n", s);
	printf("yours:    [%s]\n", result_ft_strmapi3 ? result_ft_strmapi3 : "(null)");
	printf("expected: [%s]\n\n", "(null)");
	char *result_ft_strmapi4 = ft_strmapi(NULL, test_func_strmapi);
	printf("Test 04 - NULL string\ns: %p\n", (void *)NULL);
	printf("yours:    [%s]\n", result_ft_strmapi4 ? result_ft_strmapi4 : "(null)");
	printf("expected: [%s]\n\n", "(null)");

	free(result_ft_strmapi1);
	free(result_ft_strmapi2);
	free(result_ft_strmapi3);
	free(result_ft_strmapi4);
}
static void	test_func_striteri(unsigned int i, char *c)
{
	(void)i;
	*c = ft_toupper(*c);
}
static void	test_striteri(void)
{
	char	s[] = "Striteri uses the pointer to modify the string in place. No return, no malloc, no copy. Just hopes it works. Reckless.\n";
	char	s_empty[1] = "";

	printf("\t🔵 Testing ft_striteri 🔵\n\tvoid ft_striteri(char *s, void (*f)(unsigned int, char*))\n\n");
	ft_striteri(s, test_func_striteri);
	printf("Test 01 - normal string + function ft_toupper with pointer\ns: \"%s\"\n", s);
	printf("yours:    [%s]\n", s);
	printf("expected: [%s]\n\n", "STRITERI USES THE POINTER TO MODIFY THE STRING IN PLACE. NO RETURN, NO MALLOC, NO COPY. JUST HOPES IT WORKS. RECKLESS.\n");
	ft_striteri(s_empty, test_func_striteri);
	printf("Test 02 - empty string\ns: \"%s\"\n", s_empty);
	printf("yours:    [%s]\n", s_empty);
	printf("expected: [%s]\n\n", "");
	ft_striteri(s, NULL);
	printf("Test 03 - NULL function\ns: \"%s\"\n", s);
	printf("yours:    [%s]\n", s);
	printf("expected: [%s]\n\n", s);
	ft_striteri(NULL, test_func_striteri);
	printf("Test 04 - NULL string\ns: %p\n", (void *)NULL);
	printf("yours:    [%p]\n", (void *)NULL);
	printf("expected: [%p]\n\n", (void *)NULL);
}
static void	test_putchar_fd(void)
{
	printf("\t🔵 Testing ft_putchar_fd 🔵\n\tvoid ft_putchar_fd(char c, int fd)\n\n");
	printf("Test 01 - print char to stdout\nchar: 'A', fd: 1\nyours:\n");
	fflush(stdout);
	ft_putchar_fd('A', 1);
	printf("\nexpected:\nA\n\n");
	fflush(stdout);
	fprintf(stderr, "Test 02 - print char to stderr\nchar: 'B', fd: 2\nyours:\n");
	ft_putchar_fd('B', 2);
	fprintf(stderr, "\nexpected:\nB\n\n");
	fflush(stderr);
}
static void	test_putstr_fd(void)
{
	printf("\t🔵 Testing ft_putstr_fd 🔵\n\tvoid ft_putstr_fd(char *s, int fd)\n\n");
	printf("Test 01 - print string to stdout\nstr: \"The end is near\", fd: 1\nyours:\n");
	fflush(stdout);
	ft_putstr_fd("The end is near", 1);
	printf("\nexpected:\nThe end is near\n\n");
	fflush(stdout);
	fprintf(stderr, "Test 02 - print string to stderr\nstr: \"Error message\", fd: 2\nyours:\n");
	ft_putstr_fd("Error message", 2);
	fprintf(stderr, "\nexpected:\nError message\n\n");
	fflush(stderr);
}
static void	test_putendl_fd(void)
{
	printf("\t🔵 Testing ft_putendl_fd 🔵\n\tvoid ft_putendl_fd(char *s, int fd)\n\n");
	printf("Test 01 - print string with newline to stdout\nstr: \"I want to break free\", fd: 1\nyours:\n");
	fflush(stdout);
	ft_putendl_fd("I want to break free", 1);
	printf("expected:\nI want to break free\\n\n\n");
	fflush(stdout);
	fprintf(stderr, "Test 02 - print string with newline to stderr\nstr: \"Error message with newline\", fd: 2\nyours:\n");
	ft_putendl_fd("Error message with newline", 2);
	fprintf(stderr, "expected:\nError message with newline\\n\n\n");
	fflush(stderr);
}
static void	test_putnbr_fd(void)
{
	printf("\t🔵 Testing ft_putnbr_fd 🔵\n\tvoid ft_putnbr_fd(int n, int fd)\n\n");
	printf("Test 01 - print positive number to stdout\nn: 12345, fd: 1\nyours:\n");
	fflush(stdout);
	ft_putnbr_fd(12345, 1);
	printf("\nexpected:\n12345\n\n");
	fflush(stdout);
	fprintf(stderr, "Test 02 - print negative number to stderr\nn: -67890, fd: 2\nyours:\n");
	ft_putnbr_fd(-67890, 2);
	fprintf(stderr, "\nexpected:\n-67890\n\n");
	fflush(stderr);
	printf("Test 03 - print zero to stdout\nn: 0, fd: 1\nyours:\n");
	fflush(stdout);
	ft_putnbr_fd(0, 1);
	printf("\nexpected:\n0\n\n");
	printf("Test 04 - print int max to stdout\nn: 2147483647, fd: 1\nyours:\n");
	fflush(stdout);
	ft_putnbr_fd(2147483647, 1);
	printf("\nexpected:\n2147483647\n\n");
	fflush(stdout);
	fprintf(stderr, "Test 05 - print int min to stderr\nn: -2147483648, fd: 2\nyours:\n");
	ft_putnbr_fd(-2147483648, 2);
	fprintf(stderr, "\nexpected:\n-2147483648\n\n");
	fflush(stderr);
}
void    del_noop(void *content)
{
    (void)content;
}
void str_toupper(void *content)
{
	char *str;
	int i;

	i = 0;
	str = (char *)content;
	while (str[i])
	{
		str[i] = ft_toupper(str[i]);
		i++;
	}
}

void test_ft_lstnew(t_list **names)
{
	t_list *null_node;

	null_node = ft_lstnew(NULL);
	*names = ft_lstnew("Carolina");
	printf("\n\t🔵 Testing ft_lstnew 🔵\n\tt_list *ft_lstnew(void *content)\n\n");
	printf("NULL content test\n");
	if (null_node)
	{
		printf("Node created with NULL content.\n");
		printf("Content: %p\n", null_node->content);
		printf("Next: %p\n", null_node->next);
	}
	printf("\n Creating list \"names\" str lists from node \"Carolina\"\n");
	if (names)
	{
		printf("Node created with content: %s\n", (char *)(*names)->content);
		printf("Next: %p\n", (*names)->next);
	}
	free(null_node);
}

void test_ft_lstadd_front(t_list **names)
{
	t_list *new_node = ft_lstnew("Ana");
	ft_lstadd_front(names, new_node);
	printf("\n\t\t🔵 Testing ft_lstadd_front 🔵\n\tvoid ft_lstadd_front(t_list **lst, t_list *new)\n\n");
	printf("Adding node with content \"Ana\" to the front of the list\n");
	if (names)
	{
		printf("New head content: %s\n", (char *)(*names)->content);
		printf("Next node content: %s\n", (char *)(*names)->next->content);
	}
}
void test_ft_lstadd_back(t_list **names, t_list *new)
{
	ft_lstadd_back(names, new);
	printf("\n\t\t🔵 Testing ft_lstadd_back 🔵\n\tvoid ft_lstadd_back(t_list **lst, t_list *new)\n\n");
	printf("Adding node with content \"%s\" to the back of the list\n", (char *)new->content);
	t_list *current = *names;
	while (current)
	{
		printf("Node content: %s\n", (char *)current->content);
		current = current->next;
	}
}
void test_ft_lstsize(t_list *names)
{
	int size = ft_lstsize(names);
	printf("\n\t\t🔵 Testing ft_lstsize 🔵\n\tint ft_lstsize(t_list *lst)\n\n");
	printf("Size of the list: %d\n", size);
}
void test_ft_lstlast(t_list *names)
{	
	t_list *last = ft_lstlast(names);
	printf("\n\t\t🔵 Testing ft_lstlast 🔵 \n\tt_list *ft_lstlast(t_list *lst)\n\n");
	if (last)
		printf("Last node content: %s\n", (char *)last->content);
}
void test_ft_lstmap(t_list *names, t_list *names_dup)
{
	printf("\n\t\t🔵 Testing ft_lstmap 🔵 \n\tt_list *ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))\n\n");
	printf("Mapping list \"names\" to a new list \"names_dup\" using ft_strdup\n");
	t_list *current_orig = names;
	t_list *current_dup = names_dup;
	while (current_orig && current_dup)
	{
		printf("Original content: %s | Mapped content: %s\n", (char *)current_orig->content, (char *)current_dup->content);
		current_orig = current_orig->next;
		current_dup = current_dup->next;
	}
}
void test_ft_lstiter(t_list *names, t_list *names_dup)
{
	printf("\n\t\t🔵 Testing ft_lstiter 🔵\n\tvoid ft_lstiter(t_list *lst, void (*f)(void *))\n\n");
	printf("Iterating over list \"names_dup\" and converting content to uppercase using ft_toupper\n");
	ft_lstiter(names_dup, str_toupper);
	t_list *current = names_dup;
	while (current)
	{
		printf("Node content after iteration: %s\n", (char *)current->content);
		current = current->next;
	}
	printf("\nIterating over original list \"names\" to show it remains unchanged\n");
	current = names;
	while (current)
	{
		printf("Original node content: %s\n", (char *)current->content);
		current = current->next;
	}
}
void test_ft_delone(void)
{
	t_list *disposable;

	disposable = ft_lstnew(ft_strdup("Disposable"));
	printf("\n\t\t🔵 Testing ft_lstdelone 🔵\n\tvoid ft_lstdelone(t_list *lst, void (*del)(void*))\n\n");
	printf("Created node with content: %s\n", (char *)disposable->content);
	ft_lstdelone(disposable, free);
	printf("Node deleted. Do the valgrind test to verify memory is properly freed.\n");
}
void test_ft_lstclear(t_list **names_dup, void (*del)(void *))
{
	t_list *current = *names_dup;

	printf("\n\n\t\t🔵 Testing ft_lstclear 🔵\n\tvoid ft_lstclear(t_list **lst, void (*del)(void*))\n\n");
	printf("Clearing list \"names_dup\" and freeing memory\n");
	printf("List size before clearing: %d\n", ft_lstsize(*names_dup));
	printf("List before clearing:\n");
	while (current)	{
		printf("Node content: %s\n", (char *)current->content);
		current = current->next;
	}
	ft_lstclear(names_dup, del);
	printf("List size after clearing: %d\n", ft_lstsize(*names_dup));
	if (*names_dup == NULL)
	{
		printf("List cleared successfully. Do the valgrind test to verify memory is properly freed.\n");
		printf("Command: valgrind --leak-check=full --show-leak-kinds=all ./test");
	}
	else
		printf("List not cleared properly.\n");
}
int	main(void)
{
	printf("\n\n🌟🌟🌟  WELCOME TO THE LIBFT TEST by ANA CHIOSSI 🌟🌟🌟\n");
	printf("    ⚠️   DISCLAIMER: I make mistakes.  ⚠️\n");
	test_isalpha();
	test_isdigit();
	test_isalnum();
	test_isascii();
	test_isprint();
	test_strlen();
	test_memset();
	test_bzero();
	test_memcpy();
	test_memmove();
	test_strlcpy();
	test_strlcat();
	test_toupper();
	test_tolower();
	test_strchr();
	test_strrchr();
	test_strncmp();
	test_memchr();
	test_strnstr();
	test_atoi();
	test_calloc();
	printf("\n\n 🌟   Milestone achieved!! PART 1 COMPLETE !   🌟\n\n");
	test_strdup();
	test_substr();
	test_strjoin();
	test_strtrim();
	test_ft_split();
	test_ft_itoa();
	test_strmapi();
	test_striteri();
	test_putchar_fd();
	test_putstr_fd();
	test_putendl_fd();
	test_putnbr_fd();
	printf("\n\n 🌟🌟  Milestone achieved!! PART 2 COMPLETE ! 🌟🌟\n\n\n\n\n");
	printf("😱 OH NO. ARE YOU READY FOR THE LISTS?? Mannagia...\n");
	printf("🎯 Linked lists. Pointers to pointers. malloc inside malloc.\n");
	printf("🔥 LETS DO THIS, GOOD LUCK  🔥  \n\n");
	t_list *names;
	t_list *names_dup;
	test_ft_lstnew(&names);
	test_ft_lstadd_front(&names);
	test_ft_lstadd_back(&names, ft_lstnew("de"));
	test_ft_lstadd_back(&names, ft_lstnew("Morais"));
	test_ft_lstadd_back(&names, ft_lstnew("Chiossi"));
	test_ft_lstsize(names);
	test_ft_lstlast(names);
	names_dup = ft_lstmap(names, ft_strdup, free);
	test_ft_lstmap(names, names_dup);
	ft_lstiter(names_dup, str_toupper);
	test_ft_lstiter(names, names_dup);
	test_ft_delone();
	test_ft_lstclear(&names_dup, free);
	test_ft_lstclear(&names, del_noop);
	printf("\n\n 🌟🌟🌟 CONGRATULATIONS!! YOU DID YOUR FIRST COMMON COURSE PROJECT!! LIBFT PART 3 COMPLETE ! 🌟🌟🌟\n\n\n\n\n");
	return (0);
}
