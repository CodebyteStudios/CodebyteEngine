#
# Compiler configuration
#

ifeq ($(OS),Windows_NT)
    CCFLAGS += -D WIN32
    ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
        # TODO
    endif
    ifeq ($(PROCESSOR_ARCHITECTURE),x86)
        # TODO
    endif
else
    UNAME_S := $(shell uname -s)
    # Linux
    ifeq ($(UNAME_S),Linux)
        C_INCLUDES = -Iinclude/ -Idependencies/ -I/usr/include -I/usr/local/include
        C_LIBDIR = -Ldependencies/libs/ -L/usr/lib -L/usr/local/lib
        C_LIBS = -lGLEW -lGLU -lGL -lglfw3 -lBulletDynamics -lBulletCollision -lLinearMath -ldl -lX11 -lXxf86vm -lXrandr -lpthread -lXi
    endif
    # Mac
    ifeq ($(UNAME_S),Darwin)
        C_INCLUDES = -Iinclude/ -Idependencies/ -I/usr/include -I/usr/local/include
        C_LIBDIR = -Ldependencies/libs/ -L/usr/lib -L/usr/local/lib
        C_LIBS = -lGLEW -lglfw3 -lBulletDynamics -lBulletCollision -lLinearMath -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo
    endif
endif

C_OBJS = $(shell find dependencies/src/*.cpp)
C_OBJS += $(shell find src/*.cpp)

CXX      = g++
CXXFLAGS = -Wall $(C_INCLUDES) -std=c++11
LDFLAGS  = $(C_LIBS) -DGLEW_STATIC

TARGET = bin/engine
SRCS   = $(C_OBJS)
OBJS   = $(SRCS:.cpp=.o)
DEPS   = $(SRCS:.cpp=.depends)

#
# Targets
#

.PHONY: clean all

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(C_LIBDIR) -o $@ $(OBJS) dependencies/duktape.o  $(LDFLAGS)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.depends: %.cpp
	$(CXX) -M $(CXXFLAGS) $< > $@

clean:
	rm -f $(OBJS) $(DEPS) $(TARGET)
	
run:
	make all
	(cd bin; ./engine)

-include $(DEPS)
