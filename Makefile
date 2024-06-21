
SRCDIR      = ./src/
INCDIR      = ./src/includes/
SRCNAME     = main.cpp \
              grid.cpp \
              simulation.cpp \
              keybinds.cpp	\
			  MainApp.cpp
SRCS        = $(addprefix $(SRCDIR), $(SRCNAME))
OBJS        = $(SRCS:.cpp=.o)
CC          = g++
CFLAGS      = -std=c++11 -Wall -Wextra -g3
RAYLIB      = ~/clibs/raylib/src/
CXXFLAGS    = $(CFLAGS) -I $(INCDIR) -I $(RAYLIB)
NAME        = GameOfLife

# Detect OS
UNAME_S := $(shell uname -s)

# Conditional flags for macOS
ifeq ($(UNAME_S), Darwin)
    LDFLAGS += -L $(RAYLIB) -lraylib -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
else
    LDFLAGS += -L $(RAYLIB) -lraylib
endif

all:        $(NAME)                 ## Build the project

$(NAME):    $(OBJS)
	$(CC) $(OBJS) $(CXXFLAGS) $(LDFLAGS) -o $(NAME)

$(SRCDIR)%.o: $(SRCDIR)%.cpp
	$(CC) $(CXXFLAGS) -c $< -o $@

clean:                              ## Remove all object files
	rm -f $(OBJS)

fclean:     clean                   ## Remove all object files and the executable
	rm -f $(NAME)

re:         fclean all              ## Rebuild the project

x:          all clean

help:                               ## List all commands
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) |
