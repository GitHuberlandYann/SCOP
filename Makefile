NAME = scop
OBJS_DIR = Objs
SRCS_DIR = Sources

FILES = main parsing utils Exception Face Material Scop

SRCS = $(addprefix $(SRCS_DIR)/, $(addsuffix .cpp, $(FILES)))

OBJS = $(addprefix $(OBJS_DIR)/, $(addsuffix .o, $(FILES)))

# ===---===---===---===---===---===---===---===---===---===---===---===---

ifeq ($(shell uname), Linux)
MLX_DIR = mlx_linux
MLX_PATH = ${MLX_DIR}/libmlx.a
MLX = -lmlx_linux -Lmlx_linux -L/usr/lib -Imlx_linux -lXext -lX11 -lm -lz $(MLX_PATH)
INCLUDES = -I Includes -I mlx_linux
else
MLX_DIR = mlx
MLX_PATH = ${MLX_DIR}/libmlx.a
MLX = -lmlx -Lmlx -framework OpenGl -framework AppKit $(MLX_PATH)
INCLUDES = -I Includes -I mlx
endif

# ===---===---===---===---===---===---===---===---===---===---===---===---

CC = c++
CPPFLAGS = -Wall -Wextra -Werror -O3
SAN = -fsanitize=address -g
MATH = -lm

# ===---===---===---===---===---===---===---===---===---===---===---===---

all: $(OBJS_DIR) $(NAME)

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

$(NAME): $(OBJS) $(MLX_PATH)
	$(CC) $(CPPFLAGS) $(SAN) $(OBJS) -o $(NAME) $(MLX) $(MATH)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp
	$(CC) $(CPPFLAGS) $(SAN) $(INCLUDES) -c $< -o $@

$(MLX_PATH):
	@make -C $(MLX_DIR)

clean:
	#make clean -C $(MLX_DIR)
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
