NAME = prog

TMP = objs

CXX = c++

AR = ar rcs

INCLUDES = -I./data_structures

CXXFLAGS = -std=c++17 -Wall -Wextra #-Werror

SRCS = $(wildcard *.cpp)

OBJS = $(patsubst %.cpp, ./$(TMP)/%.o, $(SRCS))

RM = rm -fr

HEADER = $(wildcard *.hpp) $(wildcard data_structures/*.hpp)

./$(TMP)/%.o: %.cpp $(HEADER) Makefile
	$(CXX) $(INCLUDES) $(CXXFLAGS) -o $@ -c $<

all: $(NAME)

$(NAME): $(TMP) $(OBJS)
	$(CXX) $(INCLUDES) $(CXXFLAGS) $(OBJS) -o $(NAME)

$(TMP):
	@mkdir $(TMP)

clean:
	$(RM) $(OBJS_DIR)
	$(RM) $(TMP)

fclean: clean
	$(RM) $(NAME)

re:	fclean all

.PHONY: all clean fclean re bonus