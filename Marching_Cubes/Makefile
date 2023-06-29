NAME = march
OBJS_DIR = Objs
SRCS_DIR = Sources

FILES = main MarchingCube utils

SRCS = $(addprefix $(SRCS_DIR)/, $(addsuffix .cpp, $(FILES)))

OBJS = $(addprefix $(OBJS_DIR)/, $(addsuffix .o, $(FILES)))

# ===---===---===---===---===---===---===---===---===---===---===---===---

CC = c++
CPPFLAGS = -Wall -Wextra -Werror -O3
SAN = -fsanitize=address -g
INCLUDES = -I Includes
MATH = -lm

# ===---===---===---===---===---===---===---===---===---===---===---===---

all: $(OBJS_DIR) $(NAME)

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

$(NAME): $(OBJS)
	$(CC) $(CPPFLAGS) $(SAN) -std=c++17 $(OBJS) -o $(NAME) $(MATH) -lstdc++

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp
	$(CC) $(CPPFLAGS) $(SAN) $(INCLUDES) -std=c++17 -c $< -o $@

clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
