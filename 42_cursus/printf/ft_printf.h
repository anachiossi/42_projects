/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 18:09:47 by anade-mo          #+#    #+#             */
/*   Updated: 2026/06/13 13:58:50 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* The include file <stdarg.h> declares a type va_list and defines three macros
		for stepping through a list of arguments
		whose number and types are not known to the called function.
	typedef va_list -> defines the type, not a function.
		void va_start(va_list ap, last);
				// inicialize the va_list
		whatver_type va_arg(va_list ap, whatever_type);
					// will return the same type you sent!
					// return the last known parameter.
					// left to right.
		void va_end(va_list ap);
				// free the list. inside same function that started.
				// leave the list undefined.
		void va_copy(va_list dest, va_list src)
				// we are not using this now.
*/

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <stddef.h>
# include <unistd.h>

typedef struct s_flags
{
	int		space;
	char	padchar;
	int		left;
	int		showsign;
	int		prefix;
	int		width;
	int		precision;
}			t_flags;
// printf.c
int			ft_printf(const char *format, ...);
int			ft_handle_text(const char *format, int i);
int			ft_handle_conversion(const char *format, int *i, va_list ap);
int			ft_return_check(int ret_pad, int ret_write);
size_t		ft_strlen(const char *s);
// printf_parsers
t_flags		ft_printf_parser(const char *format, int *i);
t_flags		ft_parse_flags(const char *format, int *i, t_flags info);
t_flags		ft_parse_width(const char *format, int *i, t_flags info);
t_flags		ft_parse_precision(const char *format, int *i, t_flags info);
int			ft_dispatch(char specifier, t_flags info, va_list ap);
// printf_strings
int			ft_putchar_count(char c);
int			ft_putchar_pad(int len, char c);
int			ft_print_char(int c, t_flags info);
int			ft_print_str(char *s, t_flags info);
// int			ft_print_custom(t_flags info);
// printf_numbers
int			ft_print_nbr(int n, t_flags info);
int			ft_print_nbr_content(int n, int nbrlen, int precise_len,
				t_flags info);
int			ft_print_nbr_base(unsigned long nbr, char *base, int nbrlen);
int			ft_nbrlen_base(unsigned long nbr, char *base);
int			ft_print_unsigned(unsigned int nbr, t_flags info);
// printf_pointers
int			ft_print_ptr(void *p, t_flags info);
int			ft_print_ptr_content(unsigned long nbr, int nbrlen, t_flags info,
				int is_pointer);
int			ft_print_hex(unsigned int nbr, t_flags info, int is_upper);
int			ft_print_hex_content(unsigned long nbr, int nbrlen, t_flags info,
				int is_upper);
int			ft_calc_precise_len(unsigned long nbr, char *base, t_flags info,
				int *nbrlen);
// printf_flags
int			ft_apply_precision(int nbrlen, t_flags info);
int			ft_apply_sign(long n, t_flags info);
int			ft_apply_padding(int len, t_flags info);
int			ft_apply_prefix(int is_upper, int is_pointer, t_flags info);
// printf_float
void		ft_float_round(double f, int prec, long *ip, long *fp);
int			ft_float_frac(long frac, int precision);
int			ft_print_float(double f, t_flags info);

#endif
