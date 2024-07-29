#include <stdbool.h>

bool	is_escaped_in_dquotes(char c)
{
	return (c == '\n' || c == '"' || c == '$' || c == '\\');
}
