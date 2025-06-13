BIN := cg1 # set executable name
LIBS := -lm # libs

UNAME_S := $(shell uname -s)
CXXFLAGS := 
CFLAGS := 
CUSTOM_CFLAGS := -std=c11 -Wall -Wextra\
				 -Werror -Wpedantic -Wno-unused-parameter\
				 -Wno-unused-variable \
				 -I./inc/ 
DEFAULT_COMPILER := gcc

CPP = $(DEFAULT_COMPILER) $(CXXFLAGS) 

SRC_MODULES := $(sort $(dir $(wildcard ./src/*/)))
SRC_MODULES := $(SRC_MODULES:./src/%/=%)

SRC_DIRS := $(addprefix src/, $(SRC_MODULES)) src
OBJ_DIRS := $(addprefix build/, $(SRC_MODULES))
SOURCE := $(foreach sdir,$(SRC_DIRS),$(wildcard $(sdir)/*.c)) 

HEAD := $(wildcard inc/*.h) 

OBJECTS = $(SOURCE:src/%.c=build/%.o)


.PHONY = all clean uninstall reinstall 

ifdef NDEBUG
CUSTOM_CFLAGS += -DNDEBUG
else
CUSTOM_CFLAGS += -DDEBUG
endif

ifeq ($(UNAME_S), Linux) #LINUX
	ECHO_MESSAGE = "Linux"
	LIBS += -lGL `pkg-config --libs glfw3 glew` -lXrandr

	CXXFLAGS += `pkg-config --cflags glfw3 glew` 
	CFLAGS = $(CXXFLAGS)
	DEFAULT_COMPILER := gcc
endif

ifeq ($(UNAME_S), Darwin) #APPLE
	ECHO_MESSAGE = "Mac OS X"
	LIBS += -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo 
	LIBS += -L/usr/local/lib -L/opt/local/lib -L/opt/homebrew/lib 

	CXXFLAGS += -I/usr/local/include -I/opt/local/include -I/opt/homebrew/include
	CFLAGS = $(CXXFLAGS)
	DEFAULT_COMPILER := clang
endif


all: $(SRC_DIRS) $(OBJ_DIRS) inc/ $(OBJECTS) $(BIN) 

$(BIN): $(SOURCE) $(OBJECTS) $(HEAD)
	$(CPP) $(CUSTOM_CFLAGS) -o $(BIN) $(OBJECTS) $(LIBS) 

build/%.o: src/%.c $(HEAD)
	@mkdir -p build 
	$(CPP) $(CUSTOM_CFLAGS) -c $< -o $@

uninstall: clean
	@rm -f $(BIN)

clean:
	@rm -drf build 
	@rm -f $(wildcard *.o)

reinstall: uninstall $(BIN)

$(SRC_DIRS) $(OBJ_DIRS) inc/ : 
	@mkdir -p $@

