# **************************************************************************** #
#                                                                              #
#    Makefile                                                                  #
#                                                                              #
#    By: mowardan                                                              #
#                                                                              #
#    TCP Server Learning Project - Preparation for ft_irc @ 1337               #
#                                                                              #
# **************************************************************************** #

NAME		= server

CXX			= c++
CXXFLAGS	= -Wall -Wextra -Werror -std=c++98

SRCS		= main.cpp
OBJS		= $(SRCS:.cpp=.o)

# Colors
GREEN		= \033[0;32m
YELLOW		= \033[0;33m
BLUE		= \033[0;34m
RED			= \033[0;31m
RESET		= \033[0m

all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(BLUE)Linking $(NAME)...$(RESET)"
	@$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)
	@echo "$(GREEN)✓ $(NAME) compiled successfully!$(RESET)"

%.o: %.cpp
	@echo "$(YELLOW)Compiling $<...$(RESET)"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@echo "$(RED)Cleaning object files...$(RESET)"
	@rm -f $(OBJS)
	@echo "$(GREEN)✓ Clean done!$(RESET)"

fclean: clean
	@echo "$(RED)Removing $(NAME)...$(RESET)"
	@rm -f $(NAME)
	@echo "$(GREEN)✓ Full clean done!$(RESET)"

re: fclean all

run: $(NAME)
	@echo "$(GREEN)Starting server on port 55000...$(RESET)"
	@./$(NAME)

.PHONY: all clean fclean re run
