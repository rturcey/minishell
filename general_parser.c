#include "./LIBFTPRINTFGNL/libft.h"
#include <stdlib.h>

//check if char is a space
int 	is_space(char a)
{
	if (a == '\f' || a == '\n' a == '\r' ||
		a == '\t' || a == '\v' || a == ' ')
		return (1);
	return (0);
}

//find end of a quoted argument ('' or "")
int 	pass_quote(char *str, int start, char quote_type)
{
	int i;

	i = start + 1;
	while (str[i])
	{
		if (str[i] == quote_type)
			return (i);
		i++;
	}
	return (start);
}

/*cut the spaces at the end of the argument, 
clean each argv and modify them directly*/
void 	clean_string(char *dest, char *src, int start)
{
	int i;

	i = 0;
	if (src[start] == '\"' || src[start] == '\'')
		end = pass_quote(src, start, src[start]);
	else
	{
		end = start;
		while (src[end] && is_space(src[end]) == 0)
			end++;
	}
	while (src[start] && start < end)
		dest[i++] = src[start++];
	dest[i] = '\0';
}

/*stock a copy of each argv to be able to cut out spaces
without making a mess. cut out spaces at the start, the
end will be cut in clean_string*/
int 	skip_spaces(int ac, char **av, int *compteur)
{
	char *stock;
	int i;
	int j;

	i = -1;
	while (++i < ac)
	{
		if (!(stock = ft_strdup(av[i])))
			return (-1);
		j = -1;
		while (av[i][j] && is_space(av[i][j]) == 1)
			j++;
		clean_string(av[i], stock, j);
		free(stock);
	}
	return (0);
}

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

int		general_parser(char *input)
{
	char *cmds[9];

	init_commands(cmds);
	
	return (0);

}

int main()
{
	general_parser(argc, argv, );
	return (0);
}