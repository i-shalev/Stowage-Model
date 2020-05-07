
#include "main.h"

int main(int argc, char **argv){

    std::string path = R"(C:\Users\zivco\Desktop\ex\sim1\Ports.csv)";
    std::string path2 = R"(C:\Users\zivco\Desktop\ex\sim1\ShipPort.csv)";
    NaiveAlgo algo;
    int results = algo.readShipRoute(path);
    for(int i = 0; i < 19; i++){
        if(getBitInNumber(results, i)){
            std::cout << i << std::endl;
        }
    }
    algo.printShipRoute();
    results = algo.readShipPlan(path2);
    for(int i = 0; i < 19; i++){
        if(getBitInNumber(results, i)){
            std::cout << i << std::endl;
        }
    }
    std::cout << algo.getInstructionsForCargo(R"(C:\Users\zivco\Desktop\ex\sim1\AAAAA_0.cargo_data)", R"(C:\Users\zivco\Desktop\ex\sim1\res.txt)") <<std::endl;
    std::cout << algo.getInstructionsForCargo(R"(C:\Users\zivco\Desktop\ex\sim1\BBBBB_0.cargo_data)", R"(C:\Users\zivco\Desktop\ex\sim1\res2.txt)") <<std::endl;
    std::cout << algo.getInstructionsForCargo(R"(C:\Users\zivco\Desktop\ex\sim1\CCCCC_0.cargo_data)", R"(C:\Users\zivco\Desktop\ex\sim1\res3.txt)") <<std::endl;

    return EXIT_SUCCESS;
}

int getBitInNumber(int num, int bit){
    int mask =  1 << bit;
    int masked_n = num & mask;
    return masked_n >> bit;
}