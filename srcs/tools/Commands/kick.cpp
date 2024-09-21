#include "../../../inc/Tools.hpp"

void	Tools::kick(Client &client, const std::string &channel_name)
{
	bool		isDone = false;
	std::string channel_nameCopy = channel_name;
	std::string channel = getnextWord(channel_nameCopy);

	std::stringstream clientSide(getnextWord(channel_nameCopy));
	std::string clientName;

	std::vector<std::string> clientNames;

	for (size_t i = 0; std::getline(clientSide, clientName, ','); i++)
		clientNames.push_back(clientName);
	if (channel.empty() || clientNames.size() == 0)
	{
		this->ResponseMsg = ":" + client.getHostname() + " 461 " + client.getNickName() + " KICK :Not enough parameters\r\n";
		send(client.getSocketFD(), this->ResponseMsg.c_str(), this->ResponseMsg.size(), 0);
		return;
	}
	for (size_t i = 0; i < clientNames.size(); i++)
	{
		if (this->channels.find(channel) != this->channels.end() && this->channels[channel].getOperatorsInfo().find(client.getNickName()) != this->channels[channel].getOperatorsInfo().end())
		{
			if (this->channels[channel].getclientInfo().find(clientNames.at(i)) != this->channels[channel].getclientInfo().end() || this->channels[channel].getOperatorsInfo().find(clientNames.at(i)) != this->channels[channel].getOperatorsInfo().end())
			{
				this->ResponseMsg = ":" + client.getNickName() + "!" + client.getUserName() + "@" + client.getIp() + " KICK " + channel + " " + clientNames.at(i) + " :" + client.getNickName() + "\r\n";
				for (std::map<std::string, int>::iterator it = this->channels[channel].getclientInfo().begin(); it != this->channels[channel].getclientInfo().end(); it++)
						send(it->second, this->ResponseMsg.c_str(), this->ResponseMsg.size(), 0);
				for (std::map<std::string, int>::iterator it = this->channels[channel].getOperatorsInfo().begin(); it != this->channels[channel].getOperatorsInfo().end(); it++)
						send(it->second, this->ResponseMsg.c_str(), this->ResponseMsg.size(), 0);
				this->channels[channel].removeClient(clientNames.at(i), isDone);
				if (isDone)
				{
					if (this->channels[channel].getclientInfo().size() == 0 && this->channels[channel].getOperatorsInfo().size() == 0)
						this->channels.erase(channel);
				}
			}
			else
			{
				this->ResponseMsg = ":" + client.getHostname() + " 403 " + client.getNickName() + " " + clientNames.at(i) + " :No such nick\r\n";
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