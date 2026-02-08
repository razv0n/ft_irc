CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SERVER = server
CLIENT = client

SERVER_SRC = server.cpp
CLIENT_SRC = client.cpp

all: $(SERVER) $(CLIENT)

$(SERVER): $(SERVER_SRC)
	$(CXX) $(CXXFLAGS) -o $(SERVER) $(SERVER_SRC)

$(CLIENT): $(CLIENT_SRC)
	$(CXX) $(CXXFLAGS) -o $(CLIENT) $(CLIENT_SRC)

clean:
	rm -f $(SERVER) $(CLIENT)

fclean: clean

re: fclean all

.PHONY: all clean fclean re
