/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <anade-mo@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 19:42:37 by gsantoni          #+#    #+#             */
/*   Updated: 2026/06/21 23:42:53 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

t_node	*new_node(int value)
{
	t_node	*node;

	node = (t_node *)malloc(sizeof(t_node));
	if (!node)
		return (NULL);
	node->value = value;
	node->next = NULL;
	return (node);
}

t_stack	*new_stack(void)
{
	t_stack	*stack;

	stack = (t_stack *)malloc(sizeof(t_stack));
	if (!stack)
		return (NULL);
	stack->top = NULL;
	stack->bottom = NULL;
	stack->size = 0;
	return (stack);
}

void	free_stack(t_stack *stack)
{
	t_node	*temp;

	if (!stack)
		return ;
	if (!stack->size)
	{
		free(stack);
		return ;
	}
	temp = stack->top;
	stack->top = temp->next;
	stack->size -= 1;
	free(temp);
	free_stack(stack);
}

void	print_stack(t_stack *stack)
{
	t_node	*temp;

	if (!stack->size)
		return ;
	temp = stack->top;
	while (temp)
	{
		ft_printf("%d ", temp->value);
		temp = temp->next;
	}
	ft_printf("\n");
}
