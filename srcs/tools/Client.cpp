#include "../../inc/Client.hpp"
#include <string>


// geters implimentations

    std::string Client::getNickName() const
    {
        return this->nickName;
    }
    std::string Client::getRealName() const
    {
        return this->realName;
    }
    std::string Client::getUserName() const
    {
        return this->userName;
    }

    std::string Client::getPassword() const
    {
        return this->password;
    }

    std::multimap<std::string, std::string>& Client::getChannelKeyMap()
    {
        return this->channelKeyMap;
    }

    std::string Client::getHostname() const
    {
        return this->hostname;
    }

    int         Client::getSocketFD() const
    {
        return this->socketFD;
    }

    bool        Client::getIsAuthenticated() const
    {
        return this->is_authenticated;
    }

    bool        Client::getIsRegistred() const
    {
        return this->isRegistred;
    }

    std::vector<std::string> Client::getChannels() const
    {
        return this->channels;
    }
    std::string    Client::getIp() const
    {
        return this->ip;
    }

// seters implimentations
    void    Client::setNickName( const std::string &NewNickName )
    {
        this->nickName = NewNickName;
    }
    void    Client::setRealName( const std::string &NewRealName )
    {
        this->realName = NewRealName;
    }
    void    Client::setUserName( const std::string &NewUserName )
    {
        this->userName = NewUserName;
    }

    void    Client::setPassword( const std::string &NewPassword )
    {
        this->password = NewPassword;
    }

    void    Client::setChannelKeyMap( const std::string &channelName, const std::string &channelKey )
    {
        this->channelKeyMap.insert(std::pair<std::string, std::string>(channelName, channelKey));
    }

    void    Client::setHostname( const std::string &NewHostname )
    {
        this->hostname = NewHostname;
    }

	    void    Client::setIp( const std::string &newip )
    {
        this->ip = newip;
    }

    void    Client::setSocketFD( const int NewSocketFD )
    {
        this->socketFD = NewSocketFD;
    }

    void    Client::setIsAuthenticated( const bool NewIsAuthenticated )
    {
        this->is_authenticated = NewIsAuthenticated;
    }

    void    Client::setIsRegistred( const bool NewIsRegistred )
    {
        this->isRegistred = NewIsRegistred;
    }

    //    Methods
    void    Client::addChannel( const std::string &channelName )
    {
        this->channels.push_back(channelName);
    }

    void    Client::removeChannel( const std::string &channelName )
    {
        std::vector<std::string>::iterator it = std::find(this->channels.begin(), this->channels.end(), channelName);
        if (it != this->channels.end())
            this->channels.erase(it);
    }

// constructers

    Client& Client::operator=( const Client &clientCopy ){
        this->socketFD = clientCopy.getSocketFD();
        this->nickName = clientCopy.getNickName();
        this->realName = clientCopy.getRealName();
        this->userName = clientCopy.getUserName();
        this->password = clientCopy.getPassword();
        this->hostname = clientCopy.getHostname();
		this->ip	   = clientCopy.getIp();
        this->is_authenticated = clientCopy.getIsAuthenticated();
        this->isRegistred = clientCopy.getIsRegistred();

        return *this;
    };
    Client::Client( const Client &clientCopy ){
        *this = clientCopy;
    };

    Client::Client( const int clientSocketFD, const std::string &ip, const std::string &hostname)
    {
        this->socketFD = clientSocketFD;
        this->hostname = hostname;
		this->ip	  = ip;
        this->is_authenticated = false;
        this->isRegistred = false;
    }

    Client::Client(){};
    Client::~Client(){};