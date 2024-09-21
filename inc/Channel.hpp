#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <sys/socket.h>
#include "Client.hpp"


// 

class Channel
{
	private:
		std::string						Name;
		std::string						topic;
		std::map<std::string, int>		clientInfo; // name clients
		std::map<std::string, int>		OperatorsInfo; // name operators
		std::map<std::string, int>		inviatedClients;
		std::string						key; // channel's pasword
		unsigned int					usersLimit;// capacity of channel
		bool							hasKey; //channel requires key to join
		bool							invitOnly; // cannot join unless invited
		bool							restOnTopic;//restriction to ops on changing the topic.
		bool							isUsersLimited;//is there a limite for channel capacity.
	
	public:
		// geters
			bool						getIsKey();
			bool						getIsInviteOnly();
			bool						getRestOnTopic();
			bool						getIsUsersLimited();
			std::string&				getTopic();
			std::string					getKey();
			std::string 				getName() const;
			std::map<std::string, int>& getclientInfo();
			std::map<std::string, int>& getOperatorsInfo();
			std::map<std::string, int>& getInviatedClients();
			unsigned int 				getUSersLimit();
	
		// seters
			void	setName( const std::string &NewName );
			void	setInviteOnly(bool flag);
			void	setRestOnTopic(bool flag);
			void	setKey(std::string pass, bool flag);
			void	setUserLimit(unsigned int userLimit, bool flag);
			void	setTopic(std::string str);
			void	sendResponseToAllClients();

		//	Methods
			void	addClientToChannel( Client& client, bool isOperator, bool &isDone, bool isInvite = false);
			void	addClientToInviatedList( const Client &client , std::string invitedClientName, bool &isDone);
			void	removeClient( Client& client, bool &isDone);
			void	removeClient( const std::string &clientNickName, bool &isDone);
			void	removeClientFromInviatedList( const std::string &clientNickName, bool &isDone);
			bool	findInChannel( const std::string &clientNickName , bool &isOperator);

	//	Constructor
		Channel& operator = ( const Channel &channelCopy );
		Channel( const Channel &channelCopy );
		Channel( const std::string &channelName, const std::string &key );
		Channel( const std::string &channelName );
		Channel();
		~Channel();
};

#endif