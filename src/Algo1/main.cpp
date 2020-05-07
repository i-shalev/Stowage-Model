
#include "main.h"

int main(int argc, char **argv){

    std::string path1 = R"(C:\Users\itay\Desktop\input\example1\Algo 2 better than 1\Ports.csv)";
    std::string path2 = R"(C:\Users\itay\Desktop\input\example1\Algo 2 better than 1\ShipPlan.csv)";
    NaiveAlgo algo = NaiveAlgo();
    int results = algo.readShipRoute(path1);
    for(int i = 0; i < 19; i++){
        if(getBitInNumber(results, i)){
            std::cout << i << std::endl;
        }
    }

    results = algo.readShipPlan(path2);
    for(int i = 0; i < 19; i++){
        if(getBitInNumber(results, i)){
            std::cout << i << std::endl;
        }
    }


//    algo.printShipRoute();

    return EXIT_SUCCESS;
}

int getBitInNumber(int num, int bit){
    int mask =  1 << bit;
    int masked_n = num & mask;
    return masked_n >> bit;
}