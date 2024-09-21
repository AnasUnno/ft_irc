/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araji-af <araji-af@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 12:34:10 by araji-af          #+#    #+#             */
/*   Updated: 2024/07/20 17:38:36 by araji-af         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/Tools.hpp"
#include <cstddef>
#include <sstream>
#include <string>
#include <vector>

void	getTopicArgs(std::string input, std::vector<std::string>& vec)
{
	std::stringstream stream(input);
	std::string word;
	std::string topic;

	stream >> word;
	if (!word.empty())
		vec.push_back(word);
	if (stream.peek() != EOF)
		std::getline(stream, word);
	else
		word.clear();
	if (!word.empty())
	{
		size_t pos = word.find_first_not_of("\t ");
		if (pos != std::string::npos)
			topic = word.substr(pos);
		else
			topic = word;
		if (topic[0] == ':' && topic != ":")
			topic.erase(0, 1);
		vec.push_back(topic);
	}
}

void	Tools::Topic(Client& client, const std::string& bufferline)
{
	std::vector<std::string> tokens;
	std::string args;
	std::string channelName;
	std::string message;

	args = bufferline;
	cleanString(args);
	getTopicArgs(args, tokens);
	if (tokens.size() < 1)
	{
		message = ":" + this->hName + " 461 " + client.getNickName() + " TOPIC :Not enough parameters\r\n";
		send(client.getSocketFD(), message.c_str(), message.size(), 0);
		return;
	}
	channelName = tokens[0];
	std::map<std::string, Channel>::iterator it = channels.find(channelName);
	if (it == channels.end())
	{
		message = ":" + this->hName + " 403 " + client.getNickName() + " " + channelName + " :No such channel\r\n";
		send(client.getSocketFD(), message.c_str(), message.size(), 0);
		return;
	}
	int clientPos = checkClientInChannel(client.getNickName(), it->second);
	if (clientPos == -1)
	{
		message = ":" + this->hName + " 442 " + client.getNickName() + " " + channelName + " :You're not on that channel\r\n";
		send(client.getSocketFD(), message.c_str(), message.size(), 0);
		return;
	}
	if (tokens.size() == 1)
	{
		if (it->second.getTopic().empty())
		{
			message = ":" + this->hName + " 331 " + client.getNickName() + " " + channelName + " :No topic is set\r\n";
			send(client.getSocketFD(), message.c_str(), message.size(), 0);
		}
		else
		{
			message = ":" + this->hName + " 332 " + client.getNickName() + " " + channelName + " :" + it->second.getTopic() + "\r\n";
			send(client.getSocketFD(), message.c_str(), message.size(), 0);
		}
		return;
	}
	else
	{
		if (it->second.getRestOnTopic())
		{
			if (clientPos == 1)
			{
				if (tokens[1] == ":")
					it->second.setTopic("");
				else
					it->second.setTopic(tokens[1]);
				message = ":" + client.getNickName() + "!" + client.getUserName() + "@" + client.getIp() + " TOPIC " + channelName + " :" + it->second.getTopic() + "\r\n";
				sendResponseToClients(it->second, message);
			}
			else
			{
				message = ":" + this->hName + " 482 " + client.getNickName() + " " + it->second.getName() + " :You're not channel operator\r\n";
				send(client.getSocketFD(), message.c_str(), message.size(), 0);
			}
		}
		else
		{
		 	if (tokens[1] == ":")
				it->second.setTopic("");
			else
				it->second.setTopic(tokens[1]);
			message = ":" + client.getNickName() + "!" + client.getUserName() + "@" + client.getIp() + " TOPIC " + channelName + " :" + it->second.getTopic() + "\r\n";
			sendResponseToClients(it->second, message);
		}
	}
}
