#include <iostream>
#include <cstdlib>
#include <ctime>
#include "../../../inc/Tools.hpp"

// Function to print a random wisdom
void Tools::bot( Client &client, const std::string &args )
{
    std::string tmp = args;

    if (!this->getnextWord(tmp).empty())
    {
        this->ResponseMsg = ":BOT!@ PRIVMSG BOT :Invalid command. Usage: /bot\r\n";
        send(client.getSocketFD(), this->ResponseMsg.c_str(), this->ResponseMsg.size(), 0);
        return;
    }

    std::vector<std::string> wisdoms;
    wisdoms.push_back("The journey of a thousand miles begins with a single step. — Lao Tzu");
    wisdoms.push_back("In the end, we will remember not the words of our enemies, but the silence of our friends. — Martin Luther King Jr.");
    wisdoms.push_back("The only true wisdom is in knowing you know nothing. — Socrates");
    wisdoms.push_back("Do not dwell in the past, do not dream of the future, concentrate the mind on the present moment. — Buddha");
    wisdoms.push_back("It does not matter how slowly you go as long as you do not stop. — Confucius");
    wisdoms.push_back("Knowing yourself is the beginning of all wisdom. — Aristotle");
    wisdoms.push_back("The only limit to our realization of tomorrow is our doubts of today. — Franklin D. Roosevelt");
    wisdoms.push_back("Happiness is not something ready-made. It comes from your own actions. — Dalai Lama");
    wisdoms.push_back("In the midst of chaos, there is also opportunity. — Sun Tzu");
    wisdoms.push_back("What you do today can improve all your tomorrows. — Ralph Marston");

    std::srand(std::time(0));
    int randomIndex = std::rand() % wisdoms.size();
    std::string intro = "Wisdom of the day: ";
	this->ResponseMsg = ":BOT!@" + client.getIp() + " PRIVMSG BOT" + " :" + intro + wisdoms[randomIndex] + "\r\n";
    send(client.getSocketFD(), this->ResponseMsg.c_str(), this->ResponseMsg.size(), 0);
}
