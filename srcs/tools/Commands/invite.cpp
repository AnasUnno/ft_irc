#include "../../../inc/Tools.hpp"

void	Tools::invite(Client &client, const std::string &channel_name)
{
	std::string channel_nameCopy = channel_name;
	std::stringstream clientName(getnextWord(channel_nameCopy));
	std::string clientNameTmp;
	std::vector<std::string> clientNames;
	std::string channel = getnextWord(channel_nameCopy);
	bool isDone = false;

	while (std::getline(clientName, clientNameTmp, ','))
		clientNames.push_back(clientNameTmp);

	for (size_t i = 0; clientNames.size() > i; i++)
	{
		if (this->channels.find(channel) != this->channels.end() && this->channels[channel].getOperatorsInfo().find(client.getNickName()) != this->channels[channel].getOperatorsInfo().end())
		{
			if (clientNames.at(i).empty() || channel.empty())
			{
				this->ResponseMsg = ":" + client.getHostname() + " 461 " + client.getNickName() + " INVITE :Not enough parameters\r\n";
				send(client.getSocketFD(), this->ResponseMsg.c_str(), this->ResponseMsg.size(), 0);
				return;
			}
			else if (this->listenClients.find(this->findClient(clientNames.at(i))) == this->listenClients.end())
			{
				this->ResponseMsg = ":" + client.getHostname() + " 401 " + client.getNickName() + " " + clientNames.at(i) + " :No such nick/channel\r\n";
				send(client.getSocketFD(), this->ResponseMsg.c_str(), this->ResponseMsg.size(), 0);
				return;
			}
			if (this->channels[channel].getclientInfo().find(clientNames.at(i)) == this->channels[channel].getclientInfo().end() && this->channels[channel].getOperatorsInfo().find(clientNames.at(i)) == this->channels[channel].getOperatorsInfo().end())
			{
				this->channels[channel].addClientToInviatedList(client, clientNames.at(i), isDone);
				if (isDone)
				{
					this->ResponseMsg = ":" + client.getNickName() + "!" + client.getUserName() + "@" + client.getIp() + " INVITE " + clientNames.at(i) + " :" + channel + "\r\n";
					send(this->listenClients[this->findClient(clientNames.at(i))].getSocketFD(), this->ResponseMsg.c_str(), this->ResponseMsg.size(), 0);
					this->ResponseMsg = ":" + client.getHostname() + " 341 " + client.getNickName() + " " + clientNames.at(i) + " :" + channel + "\r\n";
					send(client.getSocketFD(), this->ResponseMsg.c_str(), this->ResponseMsg.size(), 0);
				}
			}
			else
			{
				this->ResponseMsg = ":" + client.getHostname() + " 443 " + client.getNickName() + " " + clientNames.at(i) + " :is already in channel\r\n";
				send(client.getSocketFD(), this->ResponseMsg.c_str(), this->ResponseMsg.size(), 0);
			}
		}
		else
		{
			if (this->channels.find(channel) == this->channels.end())
				this->ResponseMsg = ":" + client.getHostname() + " 403 " + client.getNickName() + " " + channel + " :No such channel\r\n";
			else
				this->ResponseMsg = ":" + client.getHostname() + " 482 " + client.getNickName() + " " + channel + " :You must be a channel half-operator\r\n";
			send(client.getSocketFD(), this->ResponseMsg.c_str(), this->ResponseMsg.size(), 0);
		}
	}
}
