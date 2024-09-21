#include "../../inc/Channel.hpp"

// geters
	std::string Channel::getName() const 
	{
		return this->Name;
	}

	std::map<std::string, int>& Channel::getclientInfo()
	{
		return this->clientInfo;
	}

	std::map<std::string, int>& Channel::getOperatorsInfo()
	{
		return this->OperatorsInfo;
	}

	std::map<std::string, int>& Channel::getInviatedClients()
	{
		return this->inviatedClients;
	}

	bool	Channel::getIsKey()
	{
		return this->hasKey;
	}

	std::string Channel::getKey()
	{
		return this->key;
	}

	bool	Channel::getIsInviteOnly()
	{
		return this->invitOnly;
	}

	bool	Channel::getRestOnTopic()
	{
		return this->restOnTopic;
	}

	bool	Channel::getIsUsersLimited()
	{
		return this->isUsersLimited;
	}

	unsigned int Channel::getUSersLimit()
	{
		return (this->usersLimit);
	}
	std::string&	Channel::getTopic()
	{
		return (this->topic);
	}

// seters
	void        Channel::setName( const std::string &NewName )
	{
		if (!NewName.empty())
			this->Name = NewName;
	}


	void	Channel::setInviteOnly(bool flag)
	{
		invitOnly = flag;
	}

	void	Channel::setRestOnTopic(bool flag)
	{
		restOnTopic = flag;
	}

	void	Channel::setKey(std::string pass, bool flag)
	{
		if (flag)
			this->key = pass;
		hasKey = flag;
	}

	void	Channel::setUserLimit(unsigned int usersLimit, bool flag)
	{
		if (flag)
			this->usersLimit = usersLimit;
		this->isUsersLimited = flag;
	}

	void	Channel::setTopic(std::string Topic)
	{
		this->topic = Topic;
	}

// Methods

	bool		Channel::findInChannel( const std::string &clientNickName, bool &isOperator)
	{		
		if (this->OperatorsInfo.find(clientNickName) != this->OperatorsInfo.end())
			return true;
		if (!isOperator && this->clientInfo.find(clientNickName) != this->clientInfo.end())
			return true;
		return false;
	}

	void		Channel::addClientToChannel( Client& client, bool isOperator, bool &isDone, bool isInvite )
	{
		std::string msg;
		(void)isInvite;

		if (!this->findInChannel(client.getNickName(), isOperator))
		{
			if (this->inviatedClients.find(client.getNickName()) == this->inviatedClients.end() && this->invitOnly)
			{
				msg = ":" + client.getHostname() + " 473 " + client.getNickName() + " " + this->Name + " :Cannot join channel (+i)" + "\r\n";
				send(client.getSocketFD(), msg.c_str(), msg.size(), 0);
				return;
			}
			else if (this->inviatedClients.find(client.getNickName()) == this->inviatedClients.end() && this->hasKey && client.getChannelKeyMap().find(this->Name)->second != this->key)
			{
				msg = ":" + client.getHostname() + " 475 " + client.getNickName() + " " + this->Name + " :Cannot join channel (+k)" + "\r\n";
				send(client.getSocketFD(), msg.c_str(), msg.size(), 0);
				return;
			}
			else if (this->isUsersLimited && (this->clientInfo.size() + this->OperatorsInfo.size() >= this->usersLimit))
			{
				msg = ":" + client.getHostname() + " 471 " + client.getNickName() + " " + this->Name + " :Cannot join channel (+l)" + "\r\n";
				send(client.getSocketFD(), msg.c_str(), msg.size(), 0);
				return;
			}
			else if (isOperator && this->OperatorsInfo.find(client.getNickName()) == this->OperatorsInfo.end())
			{
				this->OperatorsInfo[client.getNickName()] = client.getSocketFD();
				isDone = true;
			}
			else if (this->OperatorsInfo.find(client.getNickName()) == this->OperatorsInfo.end() && this->clientInfo.find(client.getNickName()) == this->clientInfo.end())
			{
				this->clientInfo[client.getNickName()] = client.getSocketFD();
				isDone = true;
			}
		}
		else
		{
			msg = ":" + client.getHostname() + " 443 " + client.getNickName() + " " + this->Name + " :is already on channel" + "\r\n";
			send(client.getSocketFD(), msg.c_str(), msg.size(), 0);
		}
	}

	void	Channel::addClientToInviatedList( const Client &client , std::string invitedClientName, bool &isDone )
	{
		if (this->inviatedClients.find(invitedClientName) == this->inviatedClients.end())
		{
			this->inviatedClients[invitedClientName] = client.getSocketFD();
			isDone = true;
		}
		else
		{
			std::string msg = ":" + client.getHostname() + " 443 " + client.getNickName() + " " + this->Name + " :is already Invited" + "\r\n";
			send(client.getSocketFD(), msg.c_str(), msg.size(), 0);
		}
	}

	void	Channel::removeClientFromInviatedList( const std::string &clientNickName, bool &isDone)
	{
		if (this->inviatedClients.find(clientNickName) != this->inviatedClients.end())
		{
			this->inviatedClients.erase(clientNickName);
			isDone = true;
		}
	}

	void	Channel::removeClient( const std::string &clientNickName, bool &isDone)
	{
		if (this->clientInfo.find(clientNickName) != this->clientInfo.end())
		{
			this->clientInfo.erase(clientNickName);
			isDone = true;
		}
		else if (this->OperatorsInfo.find(clientNickName) != this->OperatorsInfo.end())
		{
			this->OperatorsInfo.erase(clientNickName);
			isDone = true;
		}
		if (this->inviatedClients.find(clientNickName) != this->inviatedClients.end())
		{
			this->inviatedClients.erase(clientNickName);
			isDone = true;
		}
	}

	void	Channel::removeClient( Client& client, bool &isDone)
	{
		if (this->clientInfo.find(client.getNickName()) == this->clientInfo.end() && this->OperatorsInfo.find(client.getNickName()) == this->OperatorsInfo.end())
		{
			std::string msg = ":" + client.getHostname() + " 441 " + client.getNickName() + " " + this->Name + " :They aren't on that channel" + "\r\n";
			send(client.getSocketFD(), msg.c_str(), msg.size(), 0);
		}
		else if (this->clientInfo.find(client.getNickName()) != this->clientInfo.end())
		{
			this->clientInfo.erase(client.getNickName());
			isDone = true;
		}
		else if (this->OperatorsInfo.find(client.getNickName()) != this->OperatorsInfo.end())
		{
			this->OperatorsInfo.erase(client.getNickName());
			isDone = true;
		}
	}

// Constructor

	Channel& Channel::operator = ( const Channel &channelCopy )
	{
		this->Name = channelCopy.Name;
		this->clientInfo = channelCopy.clientInfo;
		this->OperatorsInfo = channelCopy.OperatorsInfo;
		this->hasKey = channelCopy.hasKey;
		this->key = channelCopy.key;
		this->usersLimit = channelCopy.usersLimit;
		this->invitOnly = channelCopy.invitOnly;
		this->restOnTopic = channelCopy.restOnTopic;
		return *this;
	}

	Channel::Channel( const Channel &channelCopy )
	{
		*this = channelCopy;
	}

	Channel::Channel( const std::string &channelName, const std::string &key )
	{
		this->Name = channelName;
		this->key = key;
		this->hasKey = true;
	}

	Channel::Channel( const std::string &channelName )
	{
		this->Name = channelName;
		this->hasKey = false;
		this->usersLimit = 20;
		this->invitOnly = false;
		this->restOnTopic = false;
		this->isUsersLimited = false;
	}

	Channel::Channel()
	{
		this->hasKey = false;
	}

	Channel::~Channel()
	{
		this->clientInfo.clear();
	}