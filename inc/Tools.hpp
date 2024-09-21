#ifndef TOOLS_HPP
#define TOOLS_HPP

#include <iostream>
#include <sstream>
#include <cctype>
#include <vector>
#include "Channel.hpp"
#include <cstddef>
#include <map>
#include <sys/socket.h>
#include <unistd.h>

class Tools{
	private:
		std::string						bufferLine;
		bool							cmdisValide;
		std::map<int, Client>			listenClients; // non authenticated clients, yet
		std::map<std::string, Channel>	channels;
		std::string						password;
		std::string						ResponseMsg;
		std::string						hName;
		std::string						serverCreationTime;

	public:
		//	geters
			const	std::string&	getBufferLine()		const;
			bool					getCmdisValide()	const;
			Client&					getClient( const	int	socketFD );

		//	seters
			void					setBufferLine( const 	std::string	&NewBufferLine );
			void					setCmdisValide( const 	bool		NewCmdisValide );

		//	Methods
			std::string				runProccess( const	std::string& bufferLine, const	int	socketFD, std::string &password);
			void					toUpper( std::string &str );
			std::string 			cmdIdentityAndRunIt( Client	&client, const	std::string&	cmd);

		//authentication
			void		fillVector(std::string input, std::vector<std::string>& vec);
			void		checkRegisterProcess(Client &client);
			void		authentication(Client& client, const std::string& bufferLine);
			void		Pass(Client	&client, const std::string&	pass);
			void		Nick( Client &client, const std::string& line);
			void		User(Client	&client, const std::string&	command);
			void		NickChange(Client &client,const std::string& line);
			void		changeNickChannels(std::string& oldNick, std::string& newNick);
			void		cleanString(std::string& args);

		//Commands
			void		Topic(Client&	client,		const	std::string&	bufferline);
			void		Mode(Client&	client,		const	std::string&	bufferline);
			void		quit( Client	&client,	const	std::string&	channel_name);
			void		privmsg( Client	&client,	const	std::string&	args );
			void		join( Client	&client,	const	std::string&	args );
			void		part( Client	&client,	const	std::string&	args );
			void		kick( Client 	&client,	const 	std::string		&channel_name);
			void		invite( Client 	&client,	const 	std::string		&channel_name);
			void		bot( Client		&client,	const	std::string&	args );
		// tools
			int			checkClientInChannel(std::string clientName, Channel& channel);
			void		printChannelInfo(Client& client, Channel& channel);
			void		ModeFunctionality(Client& client, Channel& channel, std::vector<std::string>& tokens);
			void		sendResponseToClients(Channel& chan, std::string& response);
			void		removeClientFromListensClients(const int socketFD);
			void		browdcast( const	std::string& msg, Channel &channel, const std::string &clientName);
			std::string	UserIdentifierMsgMaker( const	std::string	&channelName );
			void		leaveAllChannels( Client &client);
			void		checkIfOperatorNeedToBeInChannel(Channel& channel);
			std::string	getnextWord( std::string &str );
			bool		ChannelNameValidation( const	std::string &channelName );
			bool		ChannelNameHasSpace( const	std::string &channelName );
			Channel&	findChannel( const	std::string &channelName );
			void		createChannel( const 	std::string &channelName);
			int			findClient( const	std::string &clientNickName );
			bool		isItchannelOrclient( const	std::string &channelNameOrNick );
			void		sendMsgToClient( Client &client,	const	std::string &msg, const std::string &clientNickName );
			void		sendMsgToChannel( Client &client,	const	std::string &msg, const std::string &channelName );
			void		createClient( const	int	socketFD, const std::string &ip, const std::string &hostname, const std::string& leTemp);
			void		removeChannel( const	std::string &channelName );
			void		addClientToChannel( Client	&client,	const	std::string&	channelName );

	//	Constructor
		Tools( const	std::string &bufferLine );
		Tools();
		~Tools();
};


#endif


/*

*/