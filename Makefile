VERSION 	=4
CXX			= g++
DEBUG 		= -g
CXXFLAGS	= -Wall -Wextra -Wpedantic
SOURCES	 	= $(wildcard ./code/Src/*.cpp)
INCLUDES  	= -I ./code/Inc
LIB_NAMES 	=
LIB_PATH 	= -lm
OBJ			=$(patsubst %.cpp, %.o, $(SOURCES))

#links
list_$(VERSION): $(OBJ) ./code/Src/list.o
	@mkdir -p progect
	$(CXX) $(OBJ) $(LIB_PATH) $(LIB_NAMES) -o ./progect/list_$(VERSION)
	@rm -rf $(OBJ) ./code/Src/list.o
	
#compile
%.o: %.cpp
	$(CXX) $(INCLUDES) $(DEBUG) -c $(CXXFLAGS) $< -o $@


.PHONY:start
start: 
	./progect/list_$(VERSION)

.PHONY:clear
clear:
	@echo "Remove linked and compiled files......"
	rm -rf $(OBJ) $(TARGET) progect 
