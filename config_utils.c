#include "dns_server.h"

void	read_config(const char *filename, struct Config *config)
{
	FILE	*file;
	char	line[256];
	char	*key;
	char	*value;
	int i;

	i = 0;

	file = fopen(filename, "r");
	if (file == NULL)
	{
		perror("Failed to open configuration file");
		exit(1);
	}
	while (fgets(line, sizeof(line), file))
	{
		key = strtok(line, " ");
		value = strtok(NULL, " ");
		if (strcmp(key, "upstream_server:") == 0)
		{
			strcpy(config->upstream_server, value);
		}
		else if (strcmp(key, "blacklist:") == 0)
		{
			while ((strcmp(key, "upstream_server:") != 0) && (strcmp(key, "response") != 0) &&  value != NULL)
			{
				strcpy(config->blacklist[i], value);
				i++;
				value = strtok(NULL, " ");
			}
		}
		else if (strcmp(key, "response:") == 0)
			strcpy(config->response, value);
	}
	fclose(file);
}

int	check_blacklist(char *domain, struct Config *config)
{
	int i;
	char	*str;
	char	*str2;

	i = 0;
	str = ft_substr(domain, 1, strlen(domain));
	str2 = str;
	while (*str2)
	{
		if (is_print(*str2))
			*str2 = '.';
		str2++;
	}
	while (i < 4)
	{
		if (strcmp(str, config->blacklist[i]) == 0)
		{
			printf("Blacklisted domain: %s\n", config->blacklist[i]);
			free(str);
			return (1);
		}
		i++;
	}
	free(str);
	return (0);
}

/*	Prints config file for testing purposes */

// void	print_config(struct Config *conf)
// {
// 	printf("responce: %s\n", conf->response);
// 	printf("upstream_server: %s\n", conf->upstream_server);
// 	for (int i = 0; i < 5; i++)
// 	{
// 		printf("black_list: %s\n", conf->blacklist[i]);
// 	}
// }