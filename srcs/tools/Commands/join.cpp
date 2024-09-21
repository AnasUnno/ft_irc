#include "../../../inc/Tools.hpp"


void Tools::addClientToChannel(Client &client, const std::string &channelName )
{
	bool isOperator = false;
	bool isDone = false;

	if (ChannelNameValidation(channelName) == true)
	{
		if (this->channels.find(channelName) == this->channels.end())
		{
			isOperator = true;
			this->createChannel(channelName);
		}
		channels[channelName].addClientToChannel(client, isOperator, isDone);
		if (isDone)
		{
			client.addChannel(channelName);
			std::map<std::string, int> clients = this->channels[channelName].getclientInfo();
			std::map<std::string, int> operators = this->channels[channelName].getOperatorsInfo();
			for (std::map<std::string, int>::iterator it = clients.begin(); it != clients.end(); it++)
			{
				std::string UserIdentifier = ":" + client.getNickName() + "!~" + client.getUserName() + "@" + client.getIp() + " JOIN :" + channelName + "\r\n";
				if (it->first == client.getNickName())
				{
					UserIdentifier += ":" + client.getHostname() + " 353 " + client.getNickName() + " = " + channelName + " :" + UserIdentifierMsgMaker(channelName);
					UserIdentifier += ":" + client.getHostname() + " 366 " + client.getNickName() + " " + channelName + " :End of /NAMES list.\r\n";
				}
				send(it->second, UserIdentifier.c_str(), UserIdentifier.size(), 0);
			}
			for (std::map<std::string, int>::iterator it = operators.begin(); it != operators.end(); it++)
			{
				std::string UserIdentifier = ":" + client.getNickName() + "!" + client.getUserName() + "@" + client.getIp() + " JOIN :" + channelName + "\r\n";
				if (it->first == client.getNickName())
				{
					UserIdentifier += ":" + client.getHostname() + " 353 " + client.getNickName() + " = " + channelName + " :" + UserIdentifierMsgMaker(channelName);
					UserIdentifier += ":" + client.getHostname() + " 366 " + client.getNickName() + " " + channelName + " :End of /NAMES list.\r\n";
				}
				send(it->second, UserIdentifier.c_str(), UserIdentifier.size(), 0);
			}
		}
	}
	else
	{
		this->ResponseMsg = ":" + client.getHostname() + " 476 " + client.getNickName() + " " + channelName + " :Bad Channel Name\r\n";
		send(client.getSocketFD(), this->ResponseMsg.c_str(), this->ResponseMsg.size(), 0);
	}
}

void Tools::join(Client &client, const std::string &channel_name)
{
	std::string channel_nametmp = channel_name;
	std::stringstream channel_nameSide(getnextWord(channel_nametmp));
	std::string channelTmp;
	std::string channel_keytmp;
	std::stringstream channel_keySide(getnextWord(channel_nametmp));

	if (channel_nameSide.str().empty())
	{
		this->ResponseMsg = ":" + client.getHostname() + " 461 " + client.getNickName() + " JOIN :Not enough parameters\r\n";
		send(client.getSocketFD(), this->ResponseMsg.c_str(), this->ResponseMsg.size(), 0);
		return;
	}

	for (size_t i = 0; std::getline(channel_nameSide, channelTmp, ',') ; i++)
	{
		std::getline(channel_keySide, channel_keytmp, ',');
		if (channel_keytmp.empty())
			channel_keytmp = "";
		client.setChannelKeyMap(channelTmp, channel_keytmp);
	}

	
	for (std::multimap<std::string, std::string>::iterator it = client.getChannelKeyMap().begin(); it != client.getChannelKeyMap().end(); it++)
		this->addClientToChannel(client, it->first);

	client.getChannelKeyMap().clear();
}