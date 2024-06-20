SRCDIR      = ./src/
INCDIR      = ./src/includes/
SRCNAME     = main.cpp \
			  grid.cpp \
			  simulation.cpp
SRCS        = $(addprefix $(SRCDIR), $(SRCNAME))

OBJS        = $(SRCS:.cpp=.o)

CC          = g++
CFLAGS      = -std=c++11 -Wall -Wextra
RL          = -I ~/clibs/raylib/src/
LRL         = -L ~/clibs/raylib/src/ -lraylib -framework CoreFoundation -framework IOKit -framework Cocoa -framework CoreGraphics

CXXFLAGS    = $(CFLAGS) $(DEFINES) $(RL)

NAME        = GameOfLife

all:        $(NAME)                 ## Build the project

$(NAME):    $(OBJS)
	$(CC) $(CXXFLAGS) -o $(NAME) $(OBJS) -I $(INCDIR) ${LRL}

$(SRCDIR)%.o: $(SRCDIR)%.cpp
	$(CC) $(CXXFLAGS) -c -I $(INCDIR) $< -o $@

clean:                              ## Remove all object files
	rm -f $(OBJS)

fclean:     clean                   ## Remove all object files and the executable
	rm -f $(NAME)

re:         fclean all              ## Rebuild the project

x:          all clean

help:                               ## List all commands
            @grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

.PHONY:     all clean fclean re x help
