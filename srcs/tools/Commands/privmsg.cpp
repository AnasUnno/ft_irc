#include "../../../inc/Tools.hpp"

bool	Tools::isItchannelOrclient(const std::string &channelNameOrNick)
{
	if (channelNameOrNick[0] == '#')
		return true;
	return false;
}

void	Tools::sendMsgToChannel( Client &client, const std::string &msg, const std::string &channelName)
{

	if (this->channels.find(channelName) != this->channels.end())
	{
		std::map<std::string, int> clients = this->channels[channelName].getclientInfo();
		std::map<std::string, int> operators = this->channels[channelName].getOperatorsInfo();

		if (clients.find(client.getNickName()) == clients.end() && operators.find(client.getNickName()) == operators.end())
		{
			this->ResponseMsg = ":" + client.getNickName() + " 442 " + client.getNickName() + " " + channelName + " :You're not on that channel\r\n";
			send(client.getSocketFD(), this->ResponseMsg.c_str(), this->ResponseMsg.size(), 0);
			return;
		}
		if (msg.empty())
		{
			this->ResponseMsg = ":" + client.getHostname() + " 412 " + client.getNickName() + " :No text to send\r\n";
			send(client.getSocketFD(), this->ResponseMsg.c_str(), this->ResponseMsg.size(), 0);
			return;
		}
		this->ResponseMsg = ":" + client.getNickName() + "!~" + client.getUserName() + "@" + client.getIp() + " PRIVMSG " + channelName + " :" + msg + "\r\n";
		browdcast( this->ResponseMsg, this->channels[channelName], client.getNickName() );
	}
	else
	{
		this->ResponseMsg = ":" + client.getNickName() + " 403 " + client.getNickName() + " " + channelName + " :No such channel\r\n";
		send(client.getSocketFD(), this->ResponseMsg.c_str(), this->ResponseMsg.size(), 0);
	}
}

void	Tools::sendMsgToClient(Client &client, const std::string &msg, const std::string &clientNickName)
{
	int clientSocketFD = this->findClient(clientNickName);
	if (clientSocketFD == -1)
	{
		this->ResponseMsg =":" + client.getHostname() + " 401 " + client.getNickName() + " " + clientNickName + " :No such nick\r\n";
		send(client.getSocketFD(), this->ResponseMsg.c_str(), this->ResponseMsg.size(), 0);
		return;
	}
	if (msg.empty())
	{
		this->ResponseMsg = ":" + client.getHostname() + " 412 " + client.getNickName() + " :No text to send\r\n";
		send(client.getSocketFD(), this->ResponseMsg.c_str(), this->ResponseMsg.size(), 0);
		return;
	}
	if (client.getNickName() == clientNickName)
		return;
	this->ResponseMsg = ":" + client.getNickName() + "!~" + client.getUserName() + "@" + client.getIp() + " PRIVMSG " + clientNickName + " :" + msg + "\r\n";
	send(clientSocketFD, this->ResponseMsg.c_str(), this->ResponseMsg.size(), 0);
}

void	Tools::privmsg(Client &client, const std::string &arg)
{
	std::string argCopy = arg;
	std::stringstream clientsSideOrChannels(this->getnextWord(argCopy));
	std::string clientOrChannelTmp;
	std::vector<std::string> clientsOrChannels;
	this->cleanString(argCopy);
	std::string message = argCopy;
	while (std::getline(clientsSideOrChannels, clientOrChannelTmp, ',') || std::getline(clientsSideOrChannels, clientOrChannelTmp, ','))
		clientsOrChannels.push_back(clientOrChannelTmp);

	if (message[0] == ':')
		message.erase(0, 1);
	for (size_t i = 0; i < clientsOrChannels.size(); i++)
	{
		if (isItchannelOrclient(clientsOrChannels[i]))
			sendMsgToChannel(client, message, clientsOrChannels[i]);
		else
			sendMsgToClient(client, message, clientsOrChannels[i]);
	}
}
