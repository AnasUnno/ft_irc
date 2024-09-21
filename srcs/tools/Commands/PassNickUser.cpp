#include "../../../inc/Tools.hpp"
#include <cctype>
#include <cstddef>
#include <cstdio>
#include <ctime>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <vector>

void	Tools::checkRegisterProcess(Client &client)
{
	if (!client.getNickName().empty() && !client.getUserName().empty() && client.getIsAuthenticated())
	{
		client.setIsRegistred(true);
		std::string message = ":" + this->hName + " 001 " + client.getNickName() + " :Welcome to the " + this->hName + " Network, " + client.getNickName() + "!" + client.getUserName() + "@" + this->hName + "\r\n";
		send(client.getSocketFD(), message.c_str(), message.size(), 0);
		message = ":" + this->hName + " 002 " + client.getNickName() + " :Your host is ircserver, running version 0.1\r\n";
		send(client.getSocketFD(), message.c_str(), message.size(), 0);
		message = ":" + this->hName + " 003 " + client.getNickName() + " :This server was created " + this->serverCreationTime + "\r\n";
		send(client.getSocketFD(), message.c_str(), message.size(), 0);
		message = ":" + this->hName + " 004 " + client.getNickName() + " ircserver 0.1 NONE itkol kol" + "\r\n";
		send(client.getSocketFD(), message.c_str(), message.size(), 0);
	}
}

void	Tools::cleanString(std::string& args)
{
	size_t pos = args.find('\r');
	if (pos != std::string::npos)
		args.erase(pos, 2);
	else
	{
		pos = args.find('\n');
		if (pos != std::string::npos)
			args.erase(pos, 1);
	}
	pos = args.find_last_not_of("\t ");
	if (pos != std::string::npos)
		args.erase(pos + 1);
	else
		args.clear();
}


void	Tools::fillVector(std::string input, std::vector<std::string>& vec)
{
	size_t pos = input.find('\n');
	if (pos != std::string::npos)
		input.erase(pos, 1);
	std::istringstream stream(input);
	std::string word;
	while (stream >> word)
		vec.push_back(word);
}

bool	validName(std::string name)
{
	for (size_t i = 0; i < name.size(); i++)
	{
		if (!std::isalnum(name[i]))
			return false;
	}
	return true;
}

void	Tools::Pass( Client &client, const std::string& line)
{
	std::vector<std::string> tokens;
	std::string args = line;
	std::string nickInputCommand;
	std::string message;

	size_t pos = args.find('\r');
	if (pos != std::string::npos)
	{
		args.erase(pos, 2);
		nickInputCommand = args.substr(pos, args.size() - 1);
	}
	if (!nickInputCommand.empty())
		args.erase(pos, nickInputCommand.size());
	if (client.getIsAuthenticated())
	{
		message = ":" + this->hName + " 462 *" + client.getNickName() + " :You may not reregister\r\n";
		send(client.getSocketFD(), message.c_str(), message.size(), 0);
		return;
	}
	fillVector(args, tokens);
	if (tokens.size() < 1)
	{
		message = ":" + this->hName + " 461 * : PASS :Not enough parameters\r\n";
		send(client.getSocketFD(), message.c_str(), message.size(), 0);
		return ;
	}
	if (tokens[0] == this->password)
	{ 
		client.setIsAuthenticated(true);
		if (nickInputCommand.empty())
			return;
	}
	else
	{
		message = ":" + this->hName + " 464 * :Password incorrect\r\n";
		send(client.getSocketFD(), message.c_str(), message.size(), 0);
		return;
	}
	authentication(client, nickInputCommand);
}

void	Tools::Nick( Client &client, const std::string& line)
{
	std::vector<std::string> tokens;
	std::string args = line;
	std::string userInputCommand;
	std::string message;

	size_t pos = args.find('\r');
	if (pos != std::string::npos)
	{
		args.erase(pos, 2);
		userInputCommand = args.substr(pos, args.size() - 1);
	}
	else
	{
		pos = args.find('\n');
		if (pos != std::string::npos)
			args.erase(pos, 1);
	}
	if (!userInputCommand.empty())
		args.erase(pos, userInputCommand.size());
	if (!client.getIsAuthenticated())
	{
		message = ":" + this->hName + "  451 " + client.getNickName() + " :You have not registered\r\n";
		send(client.getSocketFD(), message.c_str(), message.size(), 0);
		return;
	}
	if (!client.getNickName().empty())
	{
		message = ":" + this->hName + " 481 " + client.getNickName() + " :Permission Denied - You're not allowed to change your nickname\r\n";
		send(client.getSocketFD(), message.c_str(), message.size(), 0);
		return;
	}
	fillVector(args, tokens);
	if (tokens.size() < 1)
	{
		message = ":" + this->hName + " 431 * :No nickname given\r\n";
		send(client.getSocketFD(), message.c_str(), message.size(), 0);
		return;
	}
	if (tokens.size() != 1)
	{
		std::string name = tokens[0];
		for (size_t i = 1; i < tokens.size(); i++)
			name += " " + tokens[i];
		message = ":" + this->hName + " 432 " + name + " :Erroneus nickname\r\n";
		send(client.getSocketFD(), message.c_str(), message.size(), 0);
		return;
	}
	if (!validName(tokens[0]))
	{
		message = ":" + this->hName + " 432 " + tokens[0] +  " :Erroneus nickname\r\n";
		send(client.getSocketFD(), message.c_str(), message.size(), 0);
		return;
	}
	std::map<int, Client>::iterator it = listenClients.begin();
	while (it != listenClients.end())
	{
		if (it->second.getNickName() == tokens[0])
		{
			message = ":" + this->hName + " 433 * " + tokens[0] + " :Nickname is already in use\r\n";
			send(client.getSocketFD(), message.c_str(), message.size(), 0);
			return;
		}
		it++;
	}
	client.setNickName(tokens[0]);
	if (!userInputCommand.empty())
	{
		authentication(client, userInputCommand);
		return;
	}
	checkRegisterProcess(client);
}

void	getUserArgs(std::string input, std::vector<std::string>& vec)
{
	std::stringstream stream(input);
	std::string word;
	int i = 0;
	while (i < 3 && stream)
	{
		stream >> word;
		vec.push_back(word);
		i++;
	}
	if (stream.peek() == EOF)
		return;
	std::string str;
	stream >> str;
	if (!str.empty() && str != ":")
		vec.push_back(str);
}

void	Tools::User( Client &client,const std::string& command)
{
	std::vector<std::string> tokens;
	std::string part1;
	std::string part2;
	std::string message;
	std::string	args = command;

	cleanString(args);
	if (!client.getIsAuthenticated())
	{
		message = ":" + this->hName + " 451 " + client.getNickName() + " :You have not registered\r\n";
		send(client.getSocketFD(), message.c_str(), message.size(), 0);
		return;
	}
	if (!client.getUserName().empty())
	{
		std::string tempoNick = "*";
		if (!client.getNickName().empty())
			tempoNick = client.getNickName();
		message = ":" + this->hName + " 481 " + tempoNick + " :Permission Denied - You're not allowed to change your username\r\n";
		send(client.getSocketFD(), message.c_str(), message.size(), 0);
		return;
	}
	if (client.getIsRegistred())
	{
		message = ":" + this->hName + " 462 " + client.getNickName() + " :You may not reregister\r\n";
		send(client.getSocketFD(), message.c_str(), message.size(), 0);
		return;
	}
	getUserArgs(args, tokens);
	if (tokens.size() < 4)
	{
		message = ":" + this->hName + " 461 " + client.getNickName() + " USER :Not enough parameters\r\n";
		send(client.getSocketFD(), message.c_str(), message.size(), 0);
		return;
	}
	std::map<int, Client>::iterator it = listenClients.begin();
	while (it != listenClients.end())
	{
		if (it->second.getUserName() == tokens[0])
		{
			message = ":" + this->hName + " 433 * " + tokens[0] + " :Username is already in use\r\n";
			send(client.getSocketFD(), message.c_str(), message.size(), 0);
			return;
		}
		it++;
	}
	client.setUserName(tokens[0]);
	checkRegisterProcess(client);
}

void	Tools::NickChange(Client &client,const std::string& line)
{
	std::string args = line;
	std::vector<std::string> tokens;
	std::string message;

	cleanString(args);
	fillVector(args, tokens);
	if (tokens.size() < 1)
	{
		message = ":" + this->hName + " 431 " + client.getNickName() + " : No nickname given\r\n";
		send(client.getSocketFD(), message.c_str(), message.size(), 0);
		return;
	}
	if (tokens.size() != 1)
	{
		std::string name = tokens[0];
		for (size_t i = 1; i < tokens.size(); i++)
			name += " " + tokens[i];
		message = ":" + this->hName + " 432 " + client.getNickName() + " " + name + " :Erroneus nickname\r\n";
		send(client.getSocketFD(), message.c_str(), message.size(), 0);
		return;
	}
	if (client.getNickName() == tokens[0])
		return;
	if (!validName(tokens[0]))
	{
		message = ":" + this->hName + " 432 " + tokens[0] +  " :Erroneus nickname\r\n";
		send(client.getSocketFD(), message.c_str(), message.size(), 0);
		return;
	}
	std::map<int, Client>::iterator it = listenClients.begin();
	while (it != listenClients.end())
	{
		if (it->second.getNickName() == tokens[0] && client.getSocketFD() != it->first)
		{
			message = ":" + this->hName + " 433 " + client.getNickName() + " " + tokens[0] + " :Nickname is already in use\r\n";
			send(client.getSocketFD(), message.c_str(), message.size(), 0);
			return;
		 }
		it++;
	}
	std::string oldNick = client.getNickName();
	changeNickChannels(oldNick, tokens[0]);
	client.setNickName(tokens[0]);
	message = ":" + oldNick + "!" + client.getUserName() + "@" + client.getIp() + " NICK :" + client.getNickName() + "\r\n";
	send(client.getSocketFD(), message.c_str(), message.size(), 0);
}

void	Tools::changeNickChannels(std::string& oldNick, std::string& newNick)
{
	for (std::map<std::string, Channel>::iterator it = channels.begin(); it != channels.end(); it++)
	{
		std::map<std::string, int>& clients = it->second.getclientInfo();
		for (std::map<std::string, int>::iterator iterClient = clients.begin(); iterClient != clients.end(); iterClient++)
		{
			if (iterClient->first == oldNick)
			{
				int socketFD = iterClient->second;
				clients.erase(iterClient);
				clients[newNick] = socketFD;
				return;
			}
		}
		std::map<std::string, int>& ops = it->second.getOperatorsInfo();
		for (std::map<std::string, int>::iterator iterClient = ops.begin(); iterClient != ops.end(); iterClient++)
		{
			if (iterClient->first == oldNick)
			{
				int socketFD = iterClient->second;
				ops.erase(iterClient);
				ops[newNick] = socketFD;
				return;
			}
		}
	}
}

void	Tools::authentication(Client& client, const std::string& bufferLine)
{

	std::string command;
	std::string cmdTmp(bufferLine);
	command = getnextWord(cmdTmp);
	toUpper(command);
	void	(Tools::*funcPtr[3]) (Client& client, const std::string &arg) = {&Tools::Pass, &Tools::Nick, &Tools::User};
	std::string funcs[3] = {"PASS", "NICK", "USER"};
	for (int i = 0; i < 3; i++)
	{
		if (command == funcs[i])
			return ((this->*funcPtr[i])(client, cmdTmp));
	}
	std::string tempoNick = "*";
	if (!client.getNickName().empty())
		tempoNick = client.getNickName();
	std::string tmp = ":" + this->hName + " 451 " + tempoNick + " :You have not registered\r\n";
	send(client.getSocketFD(), tmp.c_str(), tmp.size(), 0);
}
