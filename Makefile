NAME = libftpp.a

OBJS_DIR = objs

LIB_DIR = lib

CXX = c++

AR = ar rcs

INCLUDES = -I./src

CXXFLAGS = -std=c++17 -Wall -Wextra -Werror

SRCS1 = $(wildcard *.cpp)

OBJS = $(patsubst %.cpp, ./$(OBJS_DIR)/%.o, $(SRCS1))

SRCS2 = $(wildcard src/*.cpp)

OBJS += $(patsubst src/%.cpp, ./$(OBJS_DIR)/src/%.o, $(SRCS2))

RM = rm -fr

HEADER = $(wildcard *.hpp) $(wildcard src/*.hpp)

all: $(NAME)

./$(OBJS_DIR)/%.o: %.cpp $(HEADER) Makefile
	$(CXX) $(INCLUDES) $(CXXFLAGS) -o $@ -c $<

$(NAME): $(OBJS_DIR) $(OBJS) $(LIB_DIR)
	$(AR) $(LIB_DIR)/$(NAME) $(OBJS)

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)/src

$(LIB_DIR):
	@mkdir -p $(LIB_DIR)/src

clean:
	$(RM) $(OBJS_DIR)

fclean: clean
	$(RM) $(NAME)

re:	fclean all

.PHONY: all clean fclean re bonus