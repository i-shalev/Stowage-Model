COMP = g++-8.3.0
#put all your object files here
OBJS = AlgorithmRegistrar.o AlgorithmRegistration.o main.o
COMMON_OBJS = WeightBalanceCalculator.o ShipRoute.o ShipPlan.o Ship.o Files.o  Crane.o Port.o Floor.o Container.o 

#The executable filename DON'T CHANGE
EXEC = simulator
COMMON = ../Common/
SIMULATOR = ./
CPP_COMP_FLAG = -std=c++2a -Wall -Wextra -Werror -pedantic-errors -DNDEBUG
CPP_LINK_FLAG = -ldl -export-dynamic #(-rdynamic)

$(EXEC): $(COMMON_OBJS) $(OBJS)
	$(COMP) $(COMMON_OBJS) $(OBJS) $(CPP_LINK_FLAG) -o $@
#a rule for building a simple c++ source file
#use g++ -MM main.cpp to see dependencies
$(COMMON_OBJS):
	$(COMP) $(CPP_COMP_FLAG) -c $(COMMON)$*.cpp
$(OBJS):
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp


clean:
	rm -f $(OBJS) $(COMMON_OBJS) $(EXEC)