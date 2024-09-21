/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araji-af <araji-af@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 20:09:44 by kzerri            #+#    #+#             */
/*   Updated: 2024/07/16 21:08:24 by araji-af         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../inc/server.hpp"

int main(int ac, char **av)
{
	signal(SIGPIPE, SIG_IGN);
	if (ac == 3)
	{
		std::cout << GREEN << " _    _  _____  _      _____  _____ ___  ___ _____   _____  _____   _____ ______  _____" << RESET << std::endl;
    	std::cout << GREEN << "| |  | ||  ___|| |    /  __ \\|  _  ||  \\/  ||  ___| |_   _||  _  | |_   _|| ___ \\/  __ \\" << RESET << std::endl;
    	std::cout << GREEN << "| |  | || |__  | |    | /  \\/| | | || .  . || |__     | |  | | | |   | |  | |_/ /| /  \\/" << RESET << std::endl;
    	std::cout << GREEN << "| |/\\| ||  __| | |    | |    | | | || |\\/| ||  __|    | |  | | | |   | |  |    / | |    " << RESET << std::endl;
    	std::cout << GREEN << "\\  /\\  /| |___ | |____| \\__/\\\\ \\_/ /| |  | || |___    | |  \\ \\_/ /  _| |_ | |\\ \\ | \\__/\\" << RESET << std::endl;
    	std::cout << GREEN << " \\/  \\/ \\____/ \\_____/ \\____/ \\___/ \\_|  |_\\____/    \\_/   \\___/   \\___/ \\_| \\_| \\_____/" << RESET << std::endl;
	    std::cout << GREEN << "                                                                                       " << RESET << std::endl;
		try
		{
			Server Server(av[1], av[2]);
		}
		catch(const std::exception& e)
		{
			std::cerr << RED << e.what() << RESET << std::endl;
			exit(-1);
		}
	}
	else
		std::cout << RED << "ERROR: YOU SHOULD SPECIFY 3 ARGUMENTS" << RESET << std::endl;
}
