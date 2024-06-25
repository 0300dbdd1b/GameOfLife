# Variables
SRCDIR      = ./src/
INCDIR      = ./src/includes/
SRCNAME     = main.cpp \
              grid.cpp \
              simulation.cpp \
			  MainApp.cpp

SRCS        = $(addprefix $(SRCDIR), $(SRCNAME))
OBJS        = $(SRCS:.cpp=.o)
CC          = g++
CFLAGS      = -std=c++11 -Wall -Wextra -g3
RAYLIB_DIR  = ./extern/raylib
RAYLIB_SRC  = $(RAYLIB_DIR)/src
CXXFLAGS    = $(CFLAGS) -I $(INCDIR) -I $(RAYLIB_SRC)
NAME        = GameOfLife

# Detect OS
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Darwin)
    LDFLAGS += -L $(RAYLIB_SRC) -lraylib -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
else ifeq ($(OS), Windows_NT)
    LDFLAGS += -L $(RAYLIB_SRC) -lraylib -lopengl32 -lgdi32 -lwinmm -static
else
    LDFLAGS += -L $(RAYLIB_SRC) -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
endif

# Targets
all: raylib $(NAME) ## Build the project

raylib: ## Download and compile raylib if not already done
	@if [ ! -d "$(RAYLIB_DIR)" ]; then \
		echo "Cloning raylib..."; \
		git clone --depth 1 https://github.com/raysan5/raylib.git $(RAYLIB_DIR); \
	fi
	@if [ ! -f "$(RAYLIB_SRC)/libraylib.a" ]; then \
		echo "Compiling raylib..."; \
		cd $(RAYLIB_SRC) && make PLATFORM=PLATFORM_DESKTOP; \
	fi

rebuild_raylib: ## Clean and recompile raylib
	@if [ -d "$(RAYLIB_DIR)" ]; then \
		echo "Recompiling raylib..."; \
		cd $(RAYLIB_SRC) && make clean && make PLATFORM=PLATFORM_DESKTOP; \
	else \
		$(MAKE) raylib; \
	fi

$(NAME): $(OBJS)
	$(CC) $(OBJS) $(CXXFLAGS) $(LDFLAGS) -o $(NAME)

$(SRCDIR)%.o: $(SRCDIR)%.cpp
	$(CC) $(CXXFLAGS) -c $< -o $@

clean: ## Remove all object files
	rm -f $(OBJS)

fclean: clean ## Remove all object files and the executable
	rm -f $(NAME)

re: fclean all ## Rebuild the project

x: all clean

help: ## List all commands
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-15s\033[0m %s\n", $$1, $$2}'
