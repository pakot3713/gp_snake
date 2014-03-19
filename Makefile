CXX= g++
CXXFLAGS= -O3
OBJS = main.o gp.o gene.o snake.o

gp_snake : $(OBJS)
	g++ -o $@ $(OBJS)

clean :
	rm -f $(OBJS)	
