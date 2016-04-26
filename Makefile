CXXFLAGS = -g3 -O0 -Wall -std=c++0x
OBJS = hexTools.o naiveAES.o printTools.o stateTools.o AESSubroutines.o

default: $(OBJS) main.o
	$(CXX) -o main $(OBJS) main.o $(CXXFLAGS)