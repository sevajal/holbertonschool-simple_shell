#include "main.h"

void free_memory(char **tokens, char *token, char *string)
{
	int i;
	for (i = 0; tokens != NULL; i++)
		free(tokens[i]);
	free(tokens);
	free(token);
	free(string);

}

/**
 * main - interactive command line interpreter
 * @argc - number of arguments
 * @argv - arguments
 */

int main(void)
{       /*  int argc, char **argv[], extern char **environ; */
	char *string, **tokens, *token;
	size_t n = 0;
	ssize_t gl;
	pid_t fork_id, w_pid; /*w_pid, pid, ppid*/
	int i = 0;
	/*char *exe_envp[] = { "PATH=$PATH", NULL };*/

	do{
		string = NULL, token = NULL, tokens = NULL;
		n = 0, i = 0;

		if (write (STDOUT_FILENO, "($) ", 4) == -1) /* prints the prompt*/
			dprintf(STDERR_FILENO, "Can't write the stdout");

		/* ssize_t getline(char **restrict lineptr, size_t *restrict n, FILE *restrict stream); */
		gl = getline(&string, &n, stdin);/* read the line */
		if (gl == -1)
		{
			free_memory(tokens, token, string);
			if(feof(stdin))
				return (EXIT_SUCCESS);
			else
				return(EXIT_FAILURE); /* EXIT_SUCCESS or EXIT_FAILURE */
		}
		if ((token = malloc(sizeof(char) * strlen(string))) == NULL)
		{
			free_memory(tokens, token, string);
			return(EXIT_FAILURE); /* EXIT_SUCCESS or EXIT_FAILURE */
		}
		token = strtok(string, " ");
		while (token != NULL)
		{
			i++, token = strtok(NULL, " ");
			fflush(stdout);
		}
		if ((tokens = malloc((i + 1) * sizeof(char *))) == NULL)
		{
			free_memory(NULL, token, string);
			return(EXIT_FAILURE); /* EXIT_SUCCESS or EXIT_FAILURE */
		}
		token = strtok(string, " \t\n");
		i = 0;
		while (token != NULL)
		{
			tokens[i] = token;
			/*printf("%s\n", tokens[i]);*/
			i++;
			token = strtok(NULL, " ");
			fflush(stdout);
		}
		tokens[i] = NULL;

		fork_id = fork ();

		if (fork_id == -1)
		{
			free_memory(tokens, token, string);
			perror("./shell");
			exit(EXIT_FAILURE);
		}
		if (fork_id == 0)
		{
			if (execv(tokens[0], tokens) == -1)
			{
				free_memory(NULL, token, string);
				perror("./shell");
				exit(EXIT_FAILURE);
			}
		}
		if (fork_id != 0)
		{
			w_pid = wait(NULL);
			if (w_pid == -1)
			{
				free_memory(tokens, token, string);
				exit(EXIT_FAILURE);
			}
		}
	} while (1);

	free_memory(tokens, token, string);
	return (0);
}
