
#include "main.h"

int main(int argc, char **argv){

    std::string path = R"(C:\Users\itay\Desktop\ex\sim1\Ports.csv)";
    std::string path2 = R"(C:\Users\itay\Desktop\ex\sim1\ShipPort.csv)";
    NaiveAlgo algo;

    std::cout << "start ReadShipRoute:" << std::endl;
    int results = algo.readShipRoute(path);
    for(int i = 0; i < 19; i++){
        if(getBitInNumber(results, i)){
            std::cout << i << std::endl;
        }
    }
    algo.printShipRoute();
    std::cout << " " << std::endl;

    std::cout << "start ReadShipPlan:" << std::endl;
    results = algo.readShipPlan(path2);
    for(int i = 0; i < 19; i++){
        if(getBitInNumber(results, i)){
            std::cout << i << std::endl;
        }
    }
    std::cout << " " << std::endl;

    std::cout << "start AAAAA_0.cargo_data:" << std::endl;
    parse(algo.getInstructionsForCargo(R"(C:\Users\itay\Desktop\ex\sim1\AAAAA_0.cargo_data)", R"(C:\Users\itay\Desktop\ex\sim1\res.txt)"));
    std::cout << " " << std::endl;
    std::cout << "start BBBBB_0.cargo_data:" << std::endl;
    parse(algo.getInstructionsForCargo(R"(C:\Users\itay\Desktop\ex\sim1\BBBBB_0.cargo_data)", R"(C:\Users\itay\Desktop\ex\sim1\res2.txt)"));
    std::cout << " " << std::endl;
    std::cout << "start CCCCC_0.cargo_data:" << std::endl;
    parse(algo.getInstructionsForCargo(R"(C:\Users\itay\Desktop\ex\sim1\CCCCC_0.cargo_data)", R"(C:\Users\itay\Desktop\ex\sim1\res3.txt)"));

    return EXIT_SUCCESS;
}
void parse(int num){
    for( int i=1; i<18; i++){
        if(getBitInNumber(num, i))
            std::cout << "bit "<< i << " is " << getBitInNumber(num, i) << std::endl;
    }
}
int getBitInNumber(int num, int bit){
    int mask =  1 << bit;
    int masked_n = num & mask;
    return masked_n >> bit;
}