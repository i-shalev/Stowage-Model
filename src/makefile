COMP = g++-8.3.0
OBJS = main.o  NaiveAlgo.o  Balancer.o  Container.o  Crane.o  Files.o  Floor.o  Port.o  Ship.o  ShipPlan.o  ShipRoute.o AlgoRunner.o NaiveAlgoWithTrick.o 
EXEC = ex1
CPP_COMP_FLAG = -std=c++2a -Wall -Wextra -Werror -pedantic-errors -DNDEBUG
CPP_LINK_FLAG = -lstdc++fs

$(EXEC): $(OBJS)
	$(COMP) $(OBJS) $(CPP_LINK_FLAG)  -o $@
$(OBJS):
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
clean:
	rm -f $(OBJS) $(EXEC)make