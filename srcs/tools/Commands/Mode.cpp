#include "../../../inc/Tools.hpp"
#include <exception>
#include <map>
#include <stdexcept>
#include <sys/socket.h>
#include <utility>
#include <vector>

void	Tools::Mode(Client& client, const std::string& bufferline)
{
	std::vector<std::string> tokens;
	std::string message;
	std::string	args = bufferline;
	cleanString(args);
	fillVector(args, tokens);
	if (tokens.size() < 1)
	{
		message = ":" + this->hName + " 461 " + client.getNickName() + " MODE  :Not enough parameters\r\n";
		send(client.getSocketFD(), message.c_str(), message.size(), 0);
		return;
	}
	std::map<std::string, Channel>::iterator it = channels.find(tokens[0]);
	if (it == channels.end())
	{
		message = ":" + this->hName + " 403 " + client.getNickName() + " " + tokens[0] + " :No such channel\r\n";
		send(client.getSocketFD(), message.c_str(), message.size(), 0);
		return;
	}
	if (checkClientInChannel(client.getNickName(), it->second) == -1)
	{
		message = ":" + this->hName + " 442 " + client.getNickName() + " " + it->second.getName() + " :You're not on that channel\r\n";
		send(client.getSocketFD(), message.c_str(), message.size(), 0);
		return;
	}
	if (tokens.size() == 1)
		printChannelInfo(client, it->second);
	else
	{
		if (checkClientInChannel(client.getNickName(), it->second) == 1)
			ModeFunctionality(client, it->second, tokens);
		else
		{
			message = ":" + this->hName + " 482 " +  client.getNickName() + " " + tokens[0] + " :You're not channel operator\r\n";
			send(client.getSocketFD(), message.c_str(), message.size(), 0);
			return;
		}
	}
}

void	Tools::printChannelInfo(Client &client, Channel& channel)
{
	std::string infos = "+";

	if (channel.getIsInviteOnly())
		infos += "i";
	if (channel.getRestOnTopic())
		infos += "t";
	if (channel.getIsKey())
		infos += "k";
	if (channel.getIsUsersLimited())
		infos += "l";
	std::string tmp = ":" + this->hName + " 324 " + client.getNickName() + " " + channel.getName() + " " + infos + "\r\n";
	send(client.getSocketFD(), tmp.c_str() , tmp.size(), 0);
}

int	Tools::checkClientInChannel(std::string clientName, Channel& channel)
{
	std::map<std::string, int> clientInfo = channel.getclientInfo();
	std::map<std::string, int>::iterator itClient = clientInfo.find(clientName);
	if (itClient != clientInfo.end())
		return 0;// 0 if he is a client
	clientInfo  = channel.getOperatorsInfo();
	itClient = clientInfo.find(clientName);
	if (itClient != clientInfo.end())
		return 1;// 1 if he  is an operator
	return -1;//if he is not a client nor operator
}

void	Tools::ModeFunctionality(Client& client, Channel& channel, std::vector<std::string>& tokens)
{
	size_t count = 2;
	std::string	message;
	std::string sign = "+";
	bool flag = true;


	for (int i = 0 ; tokens[1][i]; i++)
	{
		if (tokens[1][i] == '+')
		{
			sign = "+";
			flag = true;
		}
		else if (tokens[1][i] == '-')
		{
			sign = "-";
			flag = false;
		}
		switch (tokens[1][i])
		{
			case 'i' :
				channel.setInviteOnly(flag);
				message = ":" + client.getNickName() + "!" + client.getUserName() + "@" + client.getIp() + " MODE " + channel.getName() + " " + sign + "i\r\n";
				sendResponseToClients(channel, message);
				break;

			case 't' :
				channel.setRestOnTopic(flag);
				message = ":" + client.getNickName() + "!" + client.getUserName() + "@" + client.getIp() + " MODE " + channel.getName() + " " + sign + "t\r\n";
				sendResponseToClients(channel, message);
				break;

			case 'k':
				if (!flag)
				{
					channel.setKey("none", flag);
					message = ":" + client.getNickName() + "!" + client.getUserName() + "@" + client.getIp() + " MODE " + channel.getName() + " " + sign + "k\r\n";
					sendResponseToClients(channel, message);
				}
				else
				{
					if (tokens.size() > count)
					{
						channel.setKey(tokens[count], flag);
						message = ":" + client.getNickName() + "!" + client.getUserName() + "@" + client.getIp() + " MODE " + channel.getName() + " " + sign + "k\r\n";
						sendResponseToClients(channel, message);
						count++;
					}
					else
					{
						message = ":" + this->hName + " 461 " + client.getNickName() + " MODE :Not enough parameters\r\n";
						send(client.getSocketFD(), message.c_str(), message.size(), 0);
					}
				}
				break;

			case 'o':
				if (tokens.size() > count)
				{
					std::map<std::string, int>& Operators = channel.getOperatorsInfo();
					std::map<std::string, int>::iterator it = Operators.find(tokens[count]);

					std::map<std::string, int>& clients_map = channel.getclientInfo();
					std::map<std::string, int>::iterator it_client = clients_map.find(tokens[count]);
					if (it != Operators.end())
					{
						if (!flag)
						{
							clients_map.insert(std::make_pair(it->first, it->second));
							Operators.erase(it);
							message = ":" + client.getNickName() + "!" + client.getUserName() + "@" + client.getIp() + " MODE " + channel.getName() + " " + sign + "o "  + tokens[count] + "\r\n";
							sendResponseToClients(channel, message);
						}
					}
					else
					{
						if (flag)
						{
							if (it_client != clients_map.end())
							{
								Operators.insert(std::make_pair(it_client->first, it_client->second));
								clients_map.erase(it_client);
								message = ":" + client.getNickName() + "!" + client.getUserName() + "@" + client.getIp() + " MODE " + channel.getName() + " " + sign + "o " + tokens[count] + "\r\n";
								sendResponseToClients(channel, message);
							}
							else
							{
								message = ":" + this->hName + " 401 " + client.getNickName() + " " + tokens[count] + " No such nick/channel\r\n";
								send(client.getSocketFD(), message.c_str(), message.size(), 0);
							}
						}
					}
					count++;
				}
				else
				{
					message = ":" + this->hName + " 461 " + client.getNickName() + " MODE :Not enough parameters\r\n";
					send(client.getSocketFD(), message.c_str(), message.size(), 0);
				}
				break;

			case 'l':
				if (!flag)
				{
					channel.setUserLimit(0, flag);
					message = ":" + client.getNickName() + "!" + client.getUserName() + "@" + client.getIp() + " MODE " + channel.getName() + " " + sign + "l\r\n";
					sendResponseToClients(channel, message);
				}
				else
				{
					if (tokens.size() > count)
					{
						if (tokens[count].find_first_not_of("0123456789") == std::string::npos && tokens[count].size() < 4)
						{
							channel.setUserLimit(atoi(tokens[count].c_str()), flag);
							message = ":" + client.getNickName() + "!" + client.getUserName() + "@" + client.getIp() + " MODE " + channel.getName() + " " + sign + "l " + tokens[count] + "\r\n";
							sendResponseToClients(channel, message);
						}
						else
						{
							message = ":" + this->hName + " 461 " + client.getNickName() + " MODE :Not enough parameters\r\n";
							send(client.getSocketFD(), message.c_str(), message.size(), 0);
						}
						count++;
					}
					else
					{
						message = ":" + this->hName + " 461 " + client.getNickName() + " MODE :Not enough parameters\r\n";
						send(client.getSocketFD(), message.c_str(), message.size(), 0);
					}
						
				}
				break;

			default:
				if (tokens[1][i] != '+' && tokens[1][i] != '-')
				{
					message = ":" + this->hName + " 472 " + client.getNickName() + " " + tokens[1][i] + " :is unknown mode char to me\r\n";
					send(client.getSocketFD(), message.c_str(), message.size(), 0);
				}
		}
	}
}

void	Tools::sendResponseToClients(Channel& chan, std::string& response)
{
	for(std::map<std::string, int>::iterator it = chan.getOperatorsInfo().begin(); it != chan.getOperatorsInfo().end(); it++)
		send(it->second, response.c_str(), response.size(), 0);

	for(std::map<std::string, int>::iterator it = chan.getclientInfo().begin(); it != chan.getclientInfo().end(); it++)
		send(it->second, response.c_str(), response.size(), 0);
}
