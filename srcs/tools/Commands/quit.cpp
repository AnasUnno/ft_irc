
#include "../../../inc/Tools.hpp"

void Tools::leaveAllChannels(Client &client)
{
	bool isDone = false;
	std::vector<std::string> channels = client.getChannels();
	if (channels.size() == 0)
	{
		this->ResponseMsg = ":" + client.getNickName() + "!~" + client.getUserName() + "@" + client.getIp() + " QUIT Quit:Bye for now!\r\n";
		return;
	}
	for (size_t i = 0; i < channels.size(); i++)
	{
		Channel &channel = findChannel(channels[i]);
		channel.removeClient(client.getNickName(), isDone);
		if (isDone)
		{
			this->ResponseMsg = ":" + client.getNickName() + "!~" + client.getUserName() + "@" + client.getIp() + " QUIT Quit: " + channel.getName() + " Bye for now!\r\n";
			this->browdcast(this->ResponseMsg, channel, client.getNickName());
		}
		this->checkIfOperatorNeedToBeInChannel(channel);
		if (channel.getOperatorsInfo().size() == 0)
			removeChannel(channels[i]);
	}

}

void Tools::quit(Client &client, const std::string &channel_name)
{
	Client clientCopy = client;
	(void)channel_name;
	leaveAllChannels(client);
	this->removeClientFromListensClients(client.getSocketFD());
	this->ResponseMsg = ":" + clientCopy.getNickName() + "!~" + clientCopy.getUserName() + "@" + clientCopy.getIp() + " QUIT Quit:Bye for now!\r\n";
	send(clientCopy.getSocketFD(), this->ResponseMsg.c_str(), this->ResponseMsg.size(), 0);
}