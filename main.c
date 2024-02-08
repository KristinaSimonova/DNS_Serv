#include "dns_server.h"

static int	get_socket(void)
{
	int						socketfd;
	int						enable;
	struct sockaddr_in		server;

	socketfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (socketfd == -1)
	{
		perror("Error_socet\n");
		exit (1);
	}
	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;

	server.sin_port = htons(PORT);

	enable = 1;
	if (setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) == -1)
	{
		printf("%d %d %d %d\n", socketfd, SOL_SOCKET, SO_REUSEADDR, enable);
		close(socketfd);
		perror("Unable to set socket option\n");
		exit (1);
	}
	if (bind(socketfd, (struct sockaddr *)&server, sizeof(server)) == -1)
	{
		perror("Unable to bind\n");
		exit (1);
	}
	return (socketfd);
}

static void	refused_send(int sockfd, char *buffer, int recive_b, struct sockaddr_in *client)
{
	t_dns_header 				*dns_h;
	socklen_t 					clnt_adrs_len;
	int							send_byte;

	clnt_adrs_len = sizeof(*client);

	dns_h = (t_dns_header *)buffer;
	dns_h->qr = 1;
	dns_h->rcode = 5;
	dns_h->ans_count = 0;

	if ((send_byte = sendto(sockfd,  buffer, recive_b, 0, (struct sockaddr *)client, clnt_adrs_len)) == -1)
	{
		printf("sendto\n");
		exit(1);
	}
}

static int	resend_query(struct Config *conf, char *buffer, int bytes_received)
{
	int		new_socket;
	struct sockaddr_in dest;
	socklen_t	dest_size;

	new_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if (new_socket == -1)
	{
		perror("Could not create socket\n");
		exit(1);
	}
	dest_size = sizeof(dest);
	memset(&dest, 0, sizeof(dest));
	dest.sin_family = AF_INET;
	dest.sin_addr.s_addr = inet_addr(conf->upstream_server);
	dest.sin_port = htons(53);

	if (sendto(new_socket, buffer, bytes_received, 0, (struct sockaddr *)&dest, dest_size) == -1)
	{
		perror("Could not send request\n");
		exit(1);
	}
	bytes_received = recvfrom(new_socket, buffer, 1024, 0, (struct sockaddr*)&dest, &dest_size);
	if (bytes_received < 0)
	{
		perror("Could not receive response\n");
		exit(1);
	}
	if (close(new_socket) == -1)
	{
		perror("Unable to close socket\n");
		exit(1);
	}
	return (bytes_received);
}

void	send_query(struct Config *conf, int socketfd)
{
	int		bytes_received;
	int		n;
	int		bytes_send;
	char	buffer[1024];
	struct	sockaddr_in	client;
	socklen_t	size_client;
	pid_t	pid;

	size_client = sizeof(client);

	while (1)
	{
		bzero(buffer, 1024);

		bytes_received = recvfrom(socketfd, buffer, 1024, 0, (struct sockaddr*)&client, &size_client);
		if (bytes_received < 0)
		{
			perror("Could not receive request\n");
			exit (1);
		}
		pid = fork();
		if (pid == -1)
		{
			perror("Unable to fork\n");
			exit (1);
		}
		if (pid == 0)
		{
			if (check_blacklist(&buffer[12], conf) == 1)
				{
					refused_send(socketfd, buffer, bytes_received, &client);
					printf("%s\n", conf->response);
				}
			else
			{
				n = resend_query(conf, buffer, bytes_received);
				bytes_send = sendto(socketfd, buffer, n, 0, (struct sockaddr *)&client, size_client);
				if (bytes_send < 0)
				{
					perror("Could not send response\n");
					exit(1);
				}
			}
			close(socketfd);
			exit(0);
		}
	}
	
}

int main(void)
{
	int				socketfd;
	struct Config	config;


	socketfd = get_socket();
	read_config(CONFIG_FILE, &config);
	// print_config(&config);
	send_query(&config, socketfd);
	return (0);
}
