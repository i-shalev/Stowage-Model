COMP = g++-8.3.0

SHARED_OBJS = _319088373_a.so _319088373_b.so
COMMON_OBJS = WeightBalanceCalculator.o ShipRoute.o ShipPlan.o Ship.o Files.o  Crane.o Port.o Floor.o Container.o 
OBJS = NaiveAlgo.o NaiveAlgoWithTrick.o
CPP_COMP_FLAG = -std=c++2a -Wall -Wextra -Werror -pedantic-errors -DNDEBUG -fPIC
CPP_LINK_FLAG = -shared
COMMON = ../Common/

all: $(SHARED_OBJS)



$(COMMON_OBJS):
	$(COMP) $(CPP_COMP_FLAG) -c $(COMMON)$*.cpp
 
$(OBJS):
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp

_319088373_a.so: NaiveAlgo.o $(COMMON_OBJS) NaiveAlgo.o
	$(COMP) $(CPP_LINK_FLAG) -o $@ $^

_319088373_b.so: NaiveAlgoWithTrick.o $(COMMON_OBJS) NaiveAlgoWithTrick.o
	$(COMP) $(CPP_LINK_FLAG) -o $@ $^
	
clean:
	rm -f $(OBJS) $(COMMON_OBJS) $(SHARED_OBJS)
