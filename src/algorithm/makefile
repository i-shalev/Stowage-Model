COMP = g++-9.3.0

SHARED_OBJS = _319088373_a.so _319088373_b.so
COMMON_OBJS = WeightBalanceCalculator.o ShipRoute.o ShipPlan.o Ship.o Files.o  Crane.o Port.o Floor.o Container.o 
OBJS = _319088373_a.o _319088373_b.o
CPP_COMP_FLAG = -std=c++2a -Wall -Wextra -Werror -pedantic-errors -DNDEBUG -fPIC -fvisibility=hidden
CPP_LINK_FLAG = -shared
COMMON = ../common/

all: $(SHARED_OBJS)



$(COMMON_OBJS):
	$(COMP) $(CPP_COMP_FLAG) -c $(COMMON)$*.cpp
 
$(OBJS):
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp

_319088373_a.so: _319088373_a.o $(COMMON_OBJS) _319088373_a.o
	$(COMP) $(CPP_LINK_FLAG) -o $@ $^

_319088373_b.so: _319088373_b.o $(COMMON_OBJS) _319088373_b.o
	$(COMP) $(CPP_LINK_FLAG) -o $@ $^
	
clean:
	rm -f $(OBJS) $(COMMON_OBJS) $(SHARED_OBJS)
