/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araji-af <araji-af@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 02:05:37 by kzerri            #+#    #+#             */
/*   Updated: 2024/08/03 11:00:22 by araji-af         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/server.hpp"
#include <ctime>

Server::Server(void)
{}

Server::~Server(void)
{}

Server &Server::operator=(const Server &Obj)
{
	(void)Obj;
	return (*this);
}

bool	Server::PortAndPassword(const std::string &port, const std::string &password)
{
	long long	p;

	if (port.empty() || password.empty())
		throw std::invalid_argument("The two or one of the passed arguments is empty!");
	if (port.find_first_not_of("0123456789") != std::string::npos || *port.begin() == '0')
		throw std::invalid_argument("Incorrect port number!\nPlease specify a number between 1024 and 65,535");
	p = atol(port.c_str());
	if (p < 1024 || p > 65535)
		throw std::invalid_argument("Incorrect port number!\nPlease specify a number between 1024 and 65,535");
	for (int i = 0; password[i]; i++)
	{
		if (isspace(password[i]))
			throw std::invalid_argument("Incorrect password!\nPlease your password should not contain any white space");
	}
	this->Port = p;
	this->Password = password;
	return (true);
}

void	Server::CleanUpFds(void)
{
	for (unsigned int i = 0; i < fds.size(); i++)
			close(fds[i].fd);
}

void	Server::DecreaseClientsNumber(void)
{
	
	for (unsigned long i = 0; i < fds.size(); i++)
	{
		if (fds[i].fd == -1)
			fds.erase(fds.begin() + i);
	}
}

void	Server::StartSocket(void)
{
	listen_sd = socket(PF_INET, SOCK_STREAM, 0);
	if (listen_sd < 0)
		throw std::runtime_error("socket() system call failed!");
	on = 1;
	set_opt = setsockopt(listen_sd, SOL_SOCKET, SO_NOSIGPIPE, &on, sizeof(on));
	if (set_opt == -1)
	{
		close(listen_sd);
		throw std::runtime_error("setsockopt() system call failed!");
	}
	set_opt = setsockopt(listen_sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	if (set_opt < 0)
	{
		close(listen_sd);
		throw std::runtime_error("setsockopt() system call failed!");
	}
	if (fcntl(listen_sd, F_SETFL, O_NONBLOCK) < 0)
	{
		close(listen_sd);
		throw std::runtime_error("fcntl() system call failed!");
	}
}

void	Server::BindAddressToTheSocket(void)
{
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = PF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(this->Port);

	if (bind(listen_sd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		close(listen_sd);
		throw std::runtime_error("bind() system call failled!");
	}
}

void	Server::SetSocketForListeningState(void)
{
	if (listen(listen_sd, BACKLOG) < 0)
	{
		close(listen_sd);
		throw std::runtime_error("listen() system call failed!");
	}
	fd.fd = listen_sd;
	fd.events = POLLIN;
	fds.push_back(fd);
	timeout = -1;
}

void	Server::ServeClient(int index)
{
	client_disconnected = false;
	while (true)
	{
		bzero(buffer, sizeof(buffer));
		ex_status = recv(fds[index].fd, buffer, sizeof(buffer), 0);
		if (ex_status > 512)
		{
			send(fds[index].fd, "Error: Message too long\n", 24, 0);
			continue ;
		}
		if (ex_status < 0)
		{
			if (errno != EWOULDBLOCK)
			{
				std::cerr << RED << "recv() system call failed!" << RESET << std::endl;
				client_disconnected = true;
			}
			break ;
		}
		if (!ex_status) 
		{
			client_disconnected = true;
			break ;
		}
		buffer[ex_status] = 0;
		input += buffer;
		if (*(input.end() - 1) != '\n')
		{
			continue ;
		}
		com = this->tools.runProccess(input, fds[index].fd, this->Password);
		com == "QUIT" ?	client_disconnected = true : client_disconnected;
		if (ex_status < 0)
		{
			std::cerr << RED <<"send() system call failed!" << RESET << std::endl;
			client_disconnected = true;
			break ;
		}
		input.clear();
	}
}

void	Server::AcceptIncomingConnections(void)
{
	client_disconnected = false;
	std::cout << BOLDGREEN << "Incoming client..." << RESET << std::endl;
	do
	{
		client_addr_len = sizeof(client_addr);
		main_sd = accept(listen_sd,  (struct sockaddr *)&client_addr, &client_addr_len);
		if (main_sd < 0)
		{
			if (errno != EWOULDBLOCK)
			{
				std::cerr << RED << "accept() system call failed!" << RESET << std::endl;
				end_server = true;
			}
			break ;
		}
		ip = inet_ntoa(client_addr.sin_addr);
    	memset(hostname, 0, sizeof(hostname));
    	gethostname(hostname, sizeof(hostname));
		tools.createClient(main_sd, ip, hostname, leTemp);
		
		fd.fd = main_sd;
		fd.events = POLLIN;
		fds.push_back(fd);
		std::cout << GREEN << " - client " << fds.back().fd << " accepted ✅ " << RESET << std::endl;
		std::cout << GREEN << " - client " << fds.back().fd << " connected to IRCserver successfully ✅" << RESET << std::endl;
	} while (main_sd != -1);
}

void	Server::StartListeningAndServeTheIncomingConnections(void)
{
	client_disconnected = false;
	for (unsigned long i = 0; i < fds.size(); i++)
	{
		if (fds[i].revents == 0)
			continue ;
		if (fds[i].revents & POLLIN)
		{
			if (fds[i].fd == listen_sd)
			{
				Server::AcceptIncomingConnections();
			}
			else
			{
				Server::ServeClient(i);
			}
		}
		else
			client_disconnected = true;
		if (client_disconnected)
		{
			std::cerr << BOLDRED << "client " << fds[i].fd  << " disconnected!" << RESET << std::endl;
			this->tools.quit(this->tools.getClient(fds[i].fd), "");
			close(fds[i].fd);
			fds[i].fd = -1;
			squeeze_array = true;
			break ;
		}
	}
	if (squeeze_array)
		Server::DecreaseClientsNumber();
}

Server::Server(const std::string &port, const std::string &password)
{
	time_t t = time(0);
	this->leTemp = ctime(&t);
	if (PortAndPassword(port, password) == VALID)
	{
		Server::StartSocket();
		Server::BindAddressToTheSocket();
		Server::SetSocketForListeningState();
		end_server = false;
		std::cout << BOLDGREEN << "Waiting for connections..." << RESET << std::endl;
		while (!end_server)
		{
			poll_status = poll(&fds[0], fds.size(), timeout);
			if (poll_status < 0)
			{
				Server::CleanUpFds();
				throw std::runtime_error("poll() system call failed");
			}
			Server::StartListeningAndServeTheIncomingConnections();
		}
	}
	Server::CleanUpFds();
}
