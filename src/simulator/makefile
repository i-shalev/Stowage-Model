COMP = g++-9.3.0
#put all your object files here
OBJS = AlgorithmRegistrar.o AlgorithmRegistration.o ThreadPoolExecuter.o TasksProducer.o main.o
COMMON_OBJS = WeightBalanceCalculator.o ShipRoute.o ShipPlan.o Ship.o Files.o  Crane.o Port.o Floor.o Container.o 

#The executable filename DON'T CHANGE
EXEC = simulator
COMMON = ../common/
SIMULATOR = ./
CPP_COMP_FLAG = -std=c++2a -Wall -Wextra -Werror -pedantic-errors -DNDEBUG
CPP_LINK_FLAG = -pthread -ldl -export-dynamic -Wl,--whole-archive -lstdc++fs -Wl,--no-whole-archive#(-rdynamic)

$(EXEC): $(COMMON_OBJS) $(OBJS)
	$(COMP) $(COMMON_OBJS) $(OBJS) $(CPP_LINK_FLAG) -o $@ -lstdc++fs
#a rule for building a simple c++ source file
#use g++ -MM main.cpp to see dependencies
$(COMMON_OBJS):
	$(COMP) $(CPP_COMP_FLAG) -c $(COMMON)/$*.cpp -o $@
$(OBJS):
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp -o $@


clean:
	rm -f $(OBJS) $(COMMON_OBJS) $(EXEC)