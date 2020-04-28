#include "minishell.h"

/* Check if char is a space */
int 	is_space(char a)
{
	if (a == '\f' || a == '\n' || a == '\r' ||
		a == '\t' || a == '\v' || a == ' ')
		return (1);
	return (0);
}

/* Find end of a quoted argument ('' or "")
if no other quote is found, return 0 and handle
it by making an empty string
PARAM: 1) the char *input from GNL
	   2) the index of the first quote
RETURN: index right after the quote or 0 if we didn't
find a closing quote */
int 	pass_quote(char *str, int start)
{
	int i;

	i = start + 1;
	while (str[i])
	{
		if (str[i] == str[start])
			return (i + 1);
		i++;
	}
	return (0);
}

/* Return the end of each split argument, either
look for a second quote or for a space
PARAM: 1) the char *input from GNL
	   2) what we identified as the index of the 
	   	start of a valid string
RETURN: index after second quote or after 
non-isspace character */
int 	find_string_end(char *src, int start)
{
	int end;

	if (src[start] == '\"' || src[start] == '\'')
		end = pass_quote(src, start);
	else
	{
		end = start;
		while (src[end] && src[end] != '\'' 
			&& src[end] != '\"' && is_space(src[end]) == 0)
			end++;
	}
	return (end);
}

/* Find the number of strings in our split_input
PARAM: the char *input from GNL
RETURN: the number of strings we'll need in our 
**split_input */
int		get_input_size(char *input)
{
	int i;
	int size;

	i = 0;
	size = 0;
	while (input[i])
	{
		while (input[i] && (is_space(input[i]) == 1))
			i++;
		if (is_space(input[i]) == 0)
		{
			size++;
			i = find_string_end(input, i);
			if (i == 0)
				return (-1);
		}
	}
	return (size);
}

/* Free the **split_input we stored our input in
PARAMS : 1) our **split_input, 
		 2) its size (either the
place where malloc crashed or -1 to indicate 
the array's full length until NULL)
RETURN: NULL, to return NULL if malloc crashes in 
clean_input */
void	*free_array(char **array, int max)
{
	int i;

	if (max == -1)
	{
		max = 0;
		while (array[max] != NULL)
			max++;
	}
	i = -1;
	while (++i < max)
		free(array[i]);
	free(array);
	return (NULL);
}

/*split the input while ignoring useless spaces, 
and handle the open quote situation.
PARAM: 1) the char *input from GNL
	   2) our destination **split_input, already malloc'd 
	   3) an int, for the sake of space and holy norminette
RETURN: our gorgeous **split_input*/
char 	**clean_input(char *input, char **split_input, int size)
{
	int i;
	int j;
	int k;

	i = 0;
	j = -1;
	while (input[i] && is_space(input[i]) == 1)
			i++;
	while (input[i])
	{
		size = find_string_end(input, i) - i;
		if (!(split_input[++j] = malloc(sizeof(char) *
			(size + 1))))
			return (free_array(split_input, j));
		k = 0;
		while (k < size)
			split_input[j][k++] = input[i++];
		split_input[j][k] = '\0';
		while (input[i] && is_space(input[i]) == 1)
			i++;
	}
	split_input[++j] = NULL;
	return (split_input);
}

/*just filling an array that we'll use to compare with
our strings*/
int		init_commands(char **cmds)
{
	cmds[0] = "echo";
	cmds[1] = "cd";
	cmds[2] = "pwd";
	cmds[3] = "export";
	cmds[4] = "unset";
	cmds[5] = "env";
	cmds[6] = "exit";
	cmds[7] = "$";
	cmds[8] = NULL;
	return (0);
}

/*Getting the size that we'll need to malloc our 
**split_input, mallocing split_input.
Filling up our split_input nicely and without random
spaces.
Initializing the commands array*/
int		general_parser(char *input)
{
	char *cmds[9];
	char **split_input;
	int size;

	if ((size = get_input_size(input)) == -1)
		ft_printf("send to stdin\n");
	else
	{
		if (!(split_input = malloc(sizeof(char *) * (size + 1))))
			return (-1);//PROTECT IT IN MAIN
		split_input = clean_input(input, split_input, size);
		if (split_input == NULL)
			return (-1); //PROTECT IT IN MAIN
		init_commands(cmds);
		
		//print test
		int i = -1;
		while (split_input[++i] != NULL)
			ft_printf("split_input[%d] [%s]\n", i, split_input[i]);
		
		/*send split input to other functions from there to 
		check for commands, exe, env...*/

		free_array(split_input, -1);
	}
	return (0);
}