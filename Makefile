# Comamad Miscs

NAME	= webserv
C_COMP	= c++
C_FLAG	= -Wall -Werror -Wextra -std=c++98
C_MDIR	= mkdir -p
C_REMO	= rm -rf
C_PUTS	= printf

# Dirs

D_HDRS	= -I ./includes/
D_SRCS	= src/
D_OBJS	= objs/

# Sources & Objects

DS_MAIN	= main/
DS_CLAS	= classes/
DS_UTIL	= utils/

N_MAIN	=	main.cpp
N_CLAS	=	ServerConfig.cpp		\
			Location.cpp			\
			ServerConfigArray.cpp	\
			Server.cpp				\
			Cluster.cpp				\
			HttpRequest.cpp			\
			HttpResponse.cpp		\
			RequestProcessor.cpp	\
			Log.cpp
N_UTIL	=	ws_inet_pton.cpp			\
			ws_inet_addr.cpp			\
			ws_inet_ntoa.cpp			\
			httpStatusCodes.cpp			\
			stp_utils.cpp

F_SRCS	=	$(addprefix $(D_SRCS)$(DS_MAIN), $(N_MAIN))	\
			$(addprefix $(D_SRCS)$(DS_CLAS), $(N_CLAS))	\
			$(addprefix $(D_SRCS)$(DS_UTIL), $(N_UTIL))
N_OBJS	=	$(N_MAIN:.cpp=.opp)	\
			$(N_CLAS:.cpp=.opp)	\
			$(N_UTIL:.cpp=.opp)
F_OBJS	= $(addprefix $(D_OBJS), $(N_OBJS))

# JsonHandler
D_JSON	=	./CppJsonHandler/
N_JSON	=	jsonHandler.a
F_JSON	=	$(addprefix $(D_JSON), $(N_JSON))

# Colors
P_WHITE = \033[0m
P_LBLUE = \033[38;5;32m
P_DBLUE = \033[38;5;21m
P_PRPLE = \033[38;5;93m
P_NCYAN = \033[38;5;37m
P_MAGEN = \033[38;5;99m
P_GREEN = \033[38;5;40m
P_LGREN = \033[38;5;42m
P_NWINE = \033[38;5;88m

# Mandatory Rules

all			: $(NAME)

$(NAME)		: $(F_JSON) $(D_OBJS) $(F_OBJS)
	@$(C_PUTS) "\n\t$(P_NCYAN)Finishing binary..: $(P_LBLUE)$(NAME)$(P_WHITE)\n"
	@$(C_COMP) $(C_FLAG) $(D_HDRS) -o $@ $(F_OBJS) $(F_JSON)
	@$(C_PUTS) "$(P_GREEN)Done!$(P_WHITE)\n"

$(D_OBJS)%.opp	: $(D_SRCS)/*/%.cpp
	@$(C_PUTS) "\t$(P_PRPLE)Generating $(NAME)..: %-33.33s\r $(P_WHITE)" $@
	@$(C_COMP) $(C_FLAG) $(D_HDRS) -c $< -o $@

$(D_OBJS)	:
	@$(C_PUTS) "$(P_LGREN)Starting $(NAME) compilation...$(P_WHITE)"
	@$(C_PUTS) "\n\t$(P_MAGEN)Creating $(NAME) objects directory...$(P_WHITE)\n"
	@$(C_MDIR) $(D_OBJS)

$(F_JSON)	:
	@clear
	@$(MAKE) --no-print-directory -C $(D_JSON)

clean		:
	@$(MAKE) clean --no-print-directory -C $(D_JSON)
	@$(C_PUTS) "$(P_NWINE)Cleaning $(NAME) objects...$(P_WHITE)\n"
	@${C_REMO} $(D_OBJS)

fclean		:
	@$(MAKE) fclean --no-print-directory -C $(D_JSON)
	@$(C_PUTS) "$(P_NWINE)Cleaning $(NAME) objects...$(P_WHITE)\n"
	@${C_REMO} $(D_OBJS)
	@$(C_PUTS) "$(P_NWINE)Deleting $(NAME)...$(P_WHITE)\n"
	@${C_REMO} $(NAME)

re		: fclean all

# Test

test	: $(NAME)
	@./$(NAME)

retest	: re
	@./$(NAME)

val		:
	@valgrind --leak-check=full --show-leak-kinds=all ./$(NAME)

valtest	: $(NAME) val

# Phony

.PHONY: all clean fclean re val