/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kzerri <kzerri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 16:18:59 by kzerri            #+#    #+#             */
/*   Updated: 2024/07/31 16:13:39 by kzerri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# define RESET       "\x1b[0m"
# define BLACK       "\x1b[30m"
# define RED         "\x1b[31m"
# define GREEN       "\x1b[32m"
# define YELLOW      "\x1b[33m"
# define BLUE        "\x1b[34m"
# define MAGENTA     "\x1b[35m"
# define CYAN        "\x1b[36m"
# define WHITE       "\x1b[37m"
# define BOLDBLACK   "\x1b[1m\x1b[30m"
# define BOLDRED     "\x1b[1m\x1b[31m"
# define BOLDGREEN   "\x1b[1m\x1b[32m"
# define BOLDYELLOW  "\x1b[1m\x1b[33m"
# define BOLDBLUE    "\x1b[1m\x1b[34m"
# define BOLDMAGENTA "\x1b[1m\x1b[35m"
# define BOLDCYAN    "\x1b[1m\x1b[36m"
# define BOLDWHITE   "\x1b[1m\x1b[37m"
# define BACKLOG	1024
# define BUFFER		1024
# define FDS		200
# define VALID		true

#include <cerrno>
#include <cstring>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <netdb.h>
# include <poll.h>
# include <iostream>
# include <cstdlib>
# include <cctype>
# include <vector>
# include <map>
# include <fcntl.h>
# include <cctype>
# include "Tools.hpp"


class Server
{
	private:
		Tools				tools;
		unsigned short		Port;
		std::string			Password;
		int					set_opt;
		int					listen_sd;
		int					on;
		int					main_sd;
		struct sockaddr_in	server_addr;
		struct sockaddr_in	client_addr;
		socklen_t			client_addr_len;
		char				buffer[BUFFER];
		struct pollfd		fd;
		std::vector<struct pollfd>	fds;
		// int					nfds;
		int					timeout;
		int					poll_status;
		int					ex_status;
		// int					current_clients;
		std::string			input;
		bool				client_disconnected;
		bool				end_server;
		bool				squeeze_array;
		char				hostname[1024];
		std::string			com;
		char*				ip;
		std::string			leTemp;
	public:
		Server(void);
		~Server(void);
		Server &operator=(const Server &Obj);
		Server(const std::string &port, const std::string &password);
		bool	PortAndPassword(const std::string &port, const std::string &password);
		void	StartSocket(void);
		void	BindAddressToTheSocket(void);
		void	SetSocketForListeningState(void);
		void	StartListeningAndServeTheIncomingConnections(void);
		void	AcceptIncomingConnections(void);
		void	ServeClient(int index);
		void	DecreaseClientsNumber(void);
		void	CleanUpFds(void);
};
