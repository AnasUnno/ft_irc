#include "../../inc/Tools.hpp"

//  geters
const std::string &Tools::getBufferLine() const
{
	return this->bufferLine;
}

bool Tools::getCmdisValide() const
{
	return this->cmdisValide;
}

Client &Tools::getClient(const int socketFD)
{
	return this->listenClients[socketFD];
}

//  seters
void Tools::setBufferLine(const std::string &NewBufferLine)
{
	this->bufferLine = NewBufferLine;
}

void Tools::setCmdisValide(const bool NewCmdisValide)
{
	this->cmdisValide = NewCmdisValide;
}

void Tools::toUpper(std::string &str)
{
	for (size_t i = 0; i < str.size(); i++)
		str[i] = std::toupper(str[i]);
}

bool Tools::ChannelNameHasSpace(const std::string &channelName)
{
	if (channelName.find(' ') != std::string::npos)
		return true;

	return false;
}

bool Tools::ChannelNameValidation(const std::string &channelName)
{
	std::string allowedCharacters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	if (channelName[0] != '#' || channelName.length() < 2 || channelName.size() > 200 || ChannelNameHasSpace(channelName) == true \
		|| channelName.substr(1).find_first_not_of(allowedCharacters) != std::string::npos)
		return false;
	
	return true;
}

void	Tools::checkIfOperatorNeedToBeInChannel(Channel& channel)
{
	std::map<std::string, int> &operators = channel.getOperatorsInfo();
	std::map<std::string, int> &clients = channel.getclientInfo();
	if (operators.size() == 0 && clients.size() > 0)
	{
		operators.insert(*clients.begin());
		clients.erase(clients.begin()->first);
	}
}

void	Tools::browdcast( const std::string &msg, Channel &channel, const std::string &clientName )
{
	std::map<std::string, int> clients = channel.getclientInfo();
	std::map<std::string, int> operators = channel.getOperatorsInfo();

	for (std::map<std::string, int>::iterator it = clients.begin(); it != clients.end(); it++)
		if (it->first != clientName)
			send(it->second, msg.c_str(), msg.size(), 0);
	for (std::map<std::string, int>::iterator it = operators.begin(); it != operators.end(); it++)
		if (it->first != clientName)
			send(it->second, msg.c_str(), msg.size(), 0);
}

std::string	Tools::UserIdentifierMsgMaker( const std::string &channelName )
{
	std::map<std::string, int> clients = this->channels[channelName].getclientInfo();
	std::map<std::string, int> operators = this->channels[channelName].getOperatorsInfo();

	std::string msg;

	for (std::map<std::string, int>::iterator it = clients.begin(); it != clients.end(); it++)
		msg += it->first + " ";
	for (std::map<std::string, int>::iterator it = operators.begin(); it != operators.end(); it++)
		msg += "@" + it->first + " ";
	msg += "\r\n";
	return msg;
}

void	Tools::removeClientFromListensClients(const int socketFD)
{
	if (this->listenClients.find(socketFD) != this->listenClients.end())
		this->listenClients.erase(socketFD);
}

void	Tools::createChannel(const std::string &channelName)
{
	if (this->channels.find(channelName) != this->channels.end())
		return;
	else
	{
		Channel channel(channelName);
		this->channels[channelName] = channel;
	}
}

Channel &Tools::findChannel( const std::string &channelName )
{
	if (this->channels.find(channelName) != this->channels.end())
		return this->channels[channelName];
	return this->channels[channelName];
}

std::string Tools::getnextWord(std::string &str)
{
	std::stringstream spliterLineCmd(str);
	std::string nextWord;
	size_t findPos = 0;

	if (str.find_first_not_of(" \t\n\r") == std::string::npos)
	{
		str.clear();
		return str;
	}

	if (str.empty())
		return str;

	spliterLineCmd >> nextWord;
	findPos = str.find(nextWord);
	if (findPos != std::string::npos)
		str.erase(0, findPos + nextWord.size() + 1);
	else
		str.erase(0, findPos + nextWord.size());
	return nextWord;
}

void	Tools::removeChannel(const std::string &channelName)
{
	if (this->channels.find(channelName) != this->channels.end())
		this->channels.erase(channelName);
}

int		Tools::findClient(const std::string &clientNickName)
{
	for (std::map<int, Client>::iterator it = this->listenClients.begin(); it != this->listenClients.end(); it++)
	{
		if (it->second.getNickName() == clientNickName)
			return it->first;
	}
	return -1;
}

std::string Tools::cmdIdentityAndRunIt(Client &client, const std::string &cmdLine)
{
	std::string cmdLineCopy = cmdLine;
	std::string cmdIdentity;
	std::string CMD[12] = {"PASS", "NICK", "USER", "JOIN", "PART", "KICK","PRIVMSG", "QUIT", "INVITE", "MODE", "TOPIC", "BOT"};
	void (Tools::*funcPtr[12])(Client &client, const std::string &arg) = {&Tools::Pass, &Tools::NickChange, &Tools::User, &Tools::join, &Tools::part, &Tools::kick,&Tools::privmsg, &Tools::quit, &Tools::invite, &Tools::Mode, &Tools::Topic, &Tools::bot};


	cmdIdentity = this->getnextWord(cmdLineCopy);
	this->toUpper(cmdIdentity);

	if (cmdIdentity == "PONG")
		return cmdIdentity;
	for (int i = 0; i < 12; i++)
	{
		if (cmdIdentity == CMD[i])
		{
			(this->*funcPtr[i])(client, cmdLineCopy);
			return cmdIdentity;
		}
	}
	this->ResponseMsg = ":" + client.getHostname() + " 421 " + client.getNickName() + " " + cmdIdentity + " :Unknown command\r\n";
	send(client.getSocketFD(), this->ResponseMsg.c_str(), this->ResponseMsg.size(), 0);

	return cmdIdentity;
}

void Tools::createClient(const int socketFD, const std::string &ip, const std::string &hostname, const std::string& leTemp)
{
	Client client = Client(socketFD, ip, hostname);
	this->listenClients[socketFD] = client;
	this->hName = hostname;
	this->serverCreationTime = leTemp;
	this->serverCreationTime.erase(this->serverCreationTime.find('\n'), 1);
}

std::string Tools::runProccess(const std::string &bufferLine, const int clientSocketFD, std::string &password)
{
	std::string bufferLineCopy = bufferLine;
	std::string cmdIdentity = this->getnextWord(bufferLineCopy);
	this->toUpper(cmdIdentity);

	if (cmdIdentity == "QUIT" || cmdIdentity.empty())
		return cmdIdentity;
	try
	{
		this->password = password;
		if (!this->listenClients[clientSocketFD].getIsAuthenticated() || !this->listenClients[clientSocketFD].getIsRegistred())
			authentication(listenClients[clientSocketFD], bufferLine);
		else
		{
			std::string cmdIdentity = this->cmdIdentityAndRunIt(listenClients[clientSocketFD], bufferLine);
			if (cmdIdentity.empty())
				return cmdIdentity;
		}
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
	}
	return cmdIdentity;
}

Tools::Tools(const std::string &bufferLine)
{
	this->bufferLine = bufferLine;
	this->cmdisValide = true;
};

Tools::Tools(){};

Tools::~Tools(){};