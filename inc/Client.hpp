#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include<sstream>
#include <string>
#include <vector>
#include <deque>
#include <map>
#include <algorithm>
#include <cctype>

#include <iostream>

// class Tools;

class Client
{
	private:
		std::string nickName;
		std::string realName;
		std::string userName;
		std::string password;
		std::multimap<std::string, std::string> channelKeyMap;
		std::string hostname;
		std::string ip;
		int         socketFD;
		std::vector<std::string> channels;

		bool        is_authenticated;
		bool        isRegistred;
	public:
		// geters
			std::string getNickName()			const;
			std::string getRealName()			const;
			std::string getUserName()			const;
			std::string getPassword()			const;
			std::multimap<std::string, std::string>& getChannelKeyMap();
			int			getChannelKeyMapSize() const;
			std::string getHostname()			const;
			std::string getIp()					const;
			int         getSocketFD()			const;
			bool        getIsAuthenticated()	const;
			bool        getIsRegistred()		const;
			std::vector<std::string> getChannels() const;
		// seters
			void        setNickName(		const std::string	&NewNickName );
			void        setRealName(		const std::string	&NewRealName );
			void        setUserName(		const std::string	&NewUserName );
			void		setPassword(		const std::string	&NewPassword );
			void		setChannelKeyMap(	const std::string	&channelName, const std::string &channelKey );
			void		setHostname(		const std::string	&NewHostname );
			void		setIp(				const std::string	&NewIp       );
			void        setSocketFD(		const int			NewSocketFD );
			void        setIsAuthenticated(	const bool			NewIsAuthenticated );
			void        setIsRegistred(		const bool			NewIsRegistred );
		//	Methods
			void		addChannel(			const std::string	&channelName );
			void		removeChannel(		const std::string	&channelName );
	
	
	//	Constructor
		Client& operator = ( const Client &clientCopy );
		Client( const Client &clientCopy );
		Client( int socketFD, const std::string &ip, const std::string &hostname);
		Client();
		~Client();

	void	sendReply(std::string& reply);
	void	authentication();
};

#endif