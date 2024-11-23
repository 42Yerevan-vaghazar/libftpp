NAME = prog

TMP = objs

CXX = c++

AR = ar rcs

INCLUDES = -I./src

CXXFLAGS = -std=c++17 -Wall -Wextra #-fsanitize=address #-Werror

SRCS1 = $(wildcard *.cpp)

OBJS = $(patsubst %.cpp, ./$(TMP)/%.o, $(SRCS1))

SRCS2 = $(wildcard src/*.cpp)

OBJS += $(patsubst src/%.cpp, ./$(TMP)/src/%.o, $(SRCS2))

RM = rm -fr

HEADER = $(wildcard *.hpp) $(wildcard src/*.hpp)

all: $(NAME)

./$(TMP)/%.o: %.cpp $(HEADER) Makefile
	$(CXX) $(INCLUDES) $(CXXFLAGS) -o $@ -c $<

$(NAME): $(TMP) $(OBJS)
	$(CXX) $(INCLUDES) $(CXXFLAGS) $(OBJS) -o $(NAME)

$(TMP):
	@mkdir -p $(TMP)/src

clean:
	$(RM) $(OBJS_DIR)
	$(RM) $(TMP)

fclean: clean
	$(RM) $(NAME)

re:	fclean all

.PHONY: all clean fclean re bonus