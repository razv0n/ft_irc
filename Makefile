NAME = ircserv

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SRC_DIR = src
INC_DIR = includes
CMD_DIR = cmd

SRC = main.cpp\
      $(SRC_DIR)/server.cpp \
      $(SRC_DIR)/client.cpp \
      $(SRC_DIR)/Channel.cpp \
	  $(CMD_DIR)/invite.cpp\
	  $(CMD_DIR)/join.cpp\
	  $(CMD_DIR)/kick.cpp\
	  $(CMD_DIR)/mode.cpp\
	  $(CMD_DIR)/nick.cpp\
	  $(CMD_DIR)/part.cpp\
	  $(CMD_DIR)/pass.cpp\
	  $(CMD_DIR)/ping.cpp\
	  $(CMD_DIR)/prvmsg.cpp\
	  $(CMD_DIR)/quit.cpp\
	  $(CMD_DIR)/topic.cpp\
	  $(CMD_DIR)/user.cpp\

	  

OBJ = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJ)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
