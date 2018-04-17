NAME = humanGL
CC = clang++

LIB_GLFW_NAME = glfw-3.2.1

SRC_PATH = ./src/
OBJ_PATH = ./obj/
LIB_PATH = ./lib/
INC_PATH = ./include/ $(LIB_PATH)$(LIB_GLFW_NAME)/include/

CC_FLGS = -Werror -Wextra -Wall -std=c++11
CC_LIBS = -lglfw3 -framework AppKit -framework OpenGL -framework IOKit -framework CoreVideo

SRC_NAME = main.cpp Env.cpp
OBJ_NAME = $(SRC_NAME:.cpp=.o)
LIB_NAME = $(LIB_GLFW_NAME)/src

SRC = $(addprefix $(SRC_PATH), $(SRC_NAME))
OBJ = $(addprefix $(OBJ_PATH), $(OBJ_NAME))
INC = $(addprefix -I,$(INC_PATH))
LIB = $(addprefix -L$(LIB_PATH),$(LIB_NAME))

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CC_FLGS) $(LIB) $(INC) $(OBJ) $(CC_LIBS) -o $(NAME)

$(OBJ_PATH)%.o: $(SRC_PATH)%.cpp
	mkdir -p $(OBJ_PATH)
	$(CC) $(CC_FLGS) $(INC) -o $@ -c $<

clean:
	rm -fv $(OBJ)
	rm -rf $(OBJ_PATH)

fclean: clean
	rm -fv $(NAME)

re: fclean all
