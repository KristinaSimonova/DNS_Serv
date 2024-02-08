#include "dns_server.h"

int	is_print(char c)
{
	if (c <=32 && c <=126)
		return(1);
	return(0);
}
char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*src;
	size_t	size;
	size_t	i;

	i = 0;
	size = 0;
	if (s == NULL)
		return (NULL);
	src = malloc(sizeof(*s) * (len + 1));
	if (src == NULL)
		return (NULL);
	while (len > size && s[i])
	{
		if (i >= start)
			src[size++] = s[i];
		i++;
	}
	src[size] = 0;
	return (src);
}