#include "../../../inc/Tools.hpp"

void	Tools::part(Client &client, const std::string &channel_name)
{
	std::string channel_nameCopy = channel_name;
	std::stringstream channel_nameSide(getnextWord(channel_nameCopy));
	bool		isDone = false;
	std::vector<std::string> channelsArgs;

	while (std::getline(channel_nameSide, channel_nameCopy, ','))
		channelsArgs.push_back(channel_nameCopy);

	channel_nameCopy = getnextWord(channel_nameCopy);
	for (size_t i = 0; i < channelsArgs.size(); i++)
	{
		if (this->channels.find(channelsArgs.at(i)) != this->channels.end())
		{
			Channel &channel = findChannel(channelsArgs.at(i));
			channel.removeClient(client, isDone);
			if (isDone)
			{
				client.removeChannel(channelsArgs.at(i));
				this->checkIfOperatorNeedToBeInChannel(channel);
				this->ResponseMsg = ":" + client.getNickName() + "!" + client.getUserName() + "@" + client.getIp() + " PART " + channelsArgs.at(i) + "\r\n";
				std::map<std::string, int> clients = channel.getclientInfo();
				std::map<std::string, int> operators = channel.getOperatorsInfo();
				for (std::map<std::string, int>::iterator it = clients.begin(); it != clients.end(); it++)
					send(it->second, this->ResponseMsg.c_str(), this->ResponseMsg.size(), 0);
				for (std::map<std::string, int>::iterator it = operators.begin(); it != operators.end(); it++)
					send(it->second, this->ResponseMsg.c_str(), this->ResponseMsg.size(), 0);
				send(client.getSocketFD(), this->ResponseMsg.c_str(), this->ResponseMsg.size(), 0);
			}
			if (channel.getOperatorsInfo().size() == 0 && channel.getclientInfo().size() == 0)
				removeChannel(channelsArgs.at(i));
		}
		else
		{
			this->ResponseMsg = ":" + client.getHostname() + " 403 " + client.getNickName() + " " + channelsArgs.at(i) + " :No such channel\r\n";
			send(client.getSocketFD(), this->ResponseMsg.c_str(), this->ResponseMsg.size(), 0);
		}
	}
}