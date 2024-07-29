#include <stddef.h>

#include "command.h"
#include "parser.h"
#include "wrappers.h" // memory

void	pop_subshell_stack(t_subshell_stack **stack)
{
	t_subshell_stack	*head;

	if (!stack || !*stack)
		return ;
	head = *stack;
	*stack = (*stack)->next;
	free(head);
}

void	free_subshell_stack(t_subshell_stack **stack)
{
	if (!stack)
		return ;
	while (*stack)
		pop_subshell_stack(stack);
}

void	push_subshell_stack(t_command **root, t_command **start, \
		t_subshell_stack **stack)
{
	t_subshell_stack	*head;

	if (!stack)
		return ;
	head = xmalloc(sizeof(t_subshell_stack));
	if (!head)
	{
		free_subshell_stack(stack);
		return ;
	}
	head->root = root;
	head->start = start;
	head->next = *stack;
	*stack = head;
}
