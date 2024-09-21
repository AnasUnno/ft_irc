HEADERS     =   inc/server.hpp inc/Channel.hpp inc/Client.hpp inc/Tools.hpp

NAME        =   ircserv

CC          =   c++

CFLAGS      =    -Wall -Wextra -Werror -std=c++98 

RM          =   rm -rf

SRCS        =   srcs/main.cpp srcs/server/server.cpp srcs/tools/Channel.cpp srcs/tools/Client.cpp srcs/tools/Tools.cpp srcs/tools/Commands/PassNickUser.cpp srcs/tools/Commands/Mode.cpp srcs/tools/Commands/Topic.cpp srcs/tools/Commands/invite.cpp \
			srcs/tools/Commands/join.cpp srcs/tools/Commands/kick.cpp srcs/tools/Commands/part.cpp srcs/tools/Commands/privmsg.cpp srcs/tools/Commands/quit.cpp srcs/tools/Commands/bot.cpp

OBJS        = $(SRCS:.cpp=.o)

COLOR_RESET =   \033[0m
COLOR_CYAN  =   \033[1;36m
COLOR_GREEN =   \033[1;32m
COLOR_YELLOW=   \033[1;33m

$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $(NAME)
	@echo "$(COLOR_GREEN)./ircserv executable has been successfully created ✅$(COLOR_RESET)"

all: $(NAME)

%.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

m : $(NAME)

clean:
	@echo "$(COLOR_YELLOW)Cleaning object files...$(COLOR_RESET)"
	$(RM) $(OBJS)
	@echo "$(COLOR_GREEN)Object files have been successfully cleaned ✅$(COLOR_RESET)"

fclean: clean
	@echo "$(COLOR_YELLOW)Cleaning executable...$(COLOR_RESET)"
	$(RM) $(NAME)
	@echo "$(COLOR_GREEN)Executable has been successfully cleaned ✅$(COLOR_RESET)"

re: fclean all

.PHONY: all clean fclean re
