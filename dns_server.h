#ifndef DNS_SERVER_H
#define DNS_SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define CONFIG_FILE "config.txt"
#define PORT 8080

struct Config {
	char	upstream_server[16];
	char	blacklist[10][32];
	char	response[16];
};

typedef struct			s_dns_header
{
	unsigned short		id;
 
	unsigned char		rd :1;
	unsigned char		tc :1;
	unsigned char		aa :1;
	unsigned char		opcode :4;
	unsigned char		qr :1;

	unsigned char		rcode :4;
	unsigned char		z :3;
	unsigned char		ra :1;

	unsigned short		q_count;
	unsigned short		ans_count;
	unsigned short		auth_count;
	unsigned short		add_count;
}					t_dns_header;

/*		utils.c		*/
int		is_print(char c);
char	*ft_substr(char const *s, unsigned int start, size_t len);

/*		config_utils.c		*/
void	read_config(const char *filename, struct Config *config);
int		check_blacklist(char *domain, struct Config *config);
// void	print_config(struct Config *conf);

#endif