
#include "main.h"

int main(){
    simulate( R"(C:\Users\zivco\Desktop\ex)");
}

int simulate(const string &pathToDir) {

    auto* ship = createShip(pathToDir);
    if(ship == nullptr) {
        delete ship;
        return EXIT_FAILURE;
    }
    ship->getPlan().printShipPlan();
    Algo alg(ship);
    while(!ship->finishRoute()){
        std::cout << "enter to port "<<ship->getCurrentDestination() << std::endl;
        alg.getInstructionForCargo(pathToDir +  R"(\instructions.txt)");
        Crane c1(ship);
        c1.executeOperationList(pathToDir +  R"(\instructions.txt)");
        ship->moveToNextPort();
        std::cout << "Moving to the next destination" << std::endl;
    }

    delete ship;
    return EXIT_SUCCESS;
}

bool handleNameOfFile (const string& fileName, string& portName, int & indexNumber) {
    vector<string> elems;
    stringstream s(fileName);
    string word;
    while(getline(s, word, '_')) {
        elems.push_back(word);
    }
    if(elems.size() != 2) {
        std::cout << "Warning: the file name: " << fileName << " is not legal." << std::endl;
        return false;
    }
    portName = elems[0];
    try {
        indexNumber = stoi(elems[1]);
    } catch (const std::exception& e) {
        std::cout << "Warning: the file name: " << fileName << " is not legal." << std::endl;
        return false;
    }
    return true;
}

map<string, int>* createMapOfPortAndNumberOfVisits(vector<string>* portList) {
    auto* mapPortVisits = new map<string, int>();
    for(const auto& port : *portList) {
        auto res = mapPortVisits->find(port);
        int ans;
        if(res == mapPortVisits->end()) {
            ans = 0;
        } else {
            ans = res->second;
            mapPortVisits->erase(port);
        }
        mapPortVisits->insert({port, ans+1});
    }
    return mapPortVisits;
}

ShipPlan* createShipPlan(const string& pathToShipPlan) {
    int numFloors=0 , length=0, width=0, numLines;
    if(! getSizesShipPlan(pathToShipPlan, numFloors, length, width, numLines)) {
        return nullptr;
    }

    // create the ShipPlanVector
    auto* blocks = new vector<vector<int>>(numLines-1);
    if(! readShipPlan(*blocks, pathToShipPlan)) {
        return nullptr;
    }

    auto* shipPlan = new ShipPlan(numFloors, length, width, *blocks);
    delete blocks;
    return  shipPlan;
}

ShipRoute* createShipRoute(const string &pathToShipPorts) {
    auto* ports = new vector<string>();
    readShipPorts(*ports, pathToShipPorts);
    auto* shipRoute = new ShipRoute(ports);

    delete ports;
    return shipRoute;
}

Ship* createShip(const string &pathToDir){


    auto* shipPlan = createShipPlan(pathToDir + R"(\ShipPort.csv)");
    auto* shipRoute = createShipRoute(pathToDir + R"(\Ports.csv)");
    auto* mapPortVisits = createMapOfPortAndNumberOfVisits(shipRoute->getDstList());

    if(shipPlan == nullptr or shipRoute == nullptr or shipRoute->getRouteLength() == 0){
        return nullptr;
    }

    map<string, Port*>* portNameToPortMap = createPortNameToPortMap(pathToDir, mapPortVisits, shipRoute->getDstList()->at(shipRoute->getDstList()->size()-1));

    auto* ship = new Ship(shipRoute, shipPlan, portNameToPortMap);

    return ship;
}

map<string, Port*>* createPortNameToPortMap(const string &pathToDir, map<string, int>* mapPortVisits, const string& lastPort) {

    auto* mapPortNameToPort = new map<string, Port*>();

    addPortsWithFileToMap(pathToDir, mapPortVisits, mapPortNameToPort);

    addPortsWithNoFileToMap(mapPortVisits, lastPort, pathToDir, mapPortNameToPort);

    return mapPortNameToPort;
}

void addPortsWithNoFileToMap(map<string, int> *mapPortVisits, const string& lastPort, const string &path, map<string, Port*>* mapPortNameToPort) {

    for(const auto& elem : *mapPortVisits)
    {
        for(int i = 0; i < elem.second; i++)
        {
            string port_index = elem.first + "_" + to_string(i);
            auto res = mapPortNameToPort->find(port_index);
            if(!mapPortNameToPort->empty() && res!=mapPortNameToPort->end()){
                continue;
            }

            if(lastPort == elem.first and i == elem.second - 1){
                // the last port - dont need to do anything for now
            } else {
                std::cout << "Warning: the file " << elem.first + "_" + to_string(i) << ".cargo_data is missing." << std::endl;
            }
            Port *port = new Port(elem.first, i);
            mapPortNameToPort->insert({port_index, port});
        }
    }
}

void addPortsWithFileToMap(const string &pathToDir, map<string, int> *mapPortVisits, map<string, Port*>* mapPortNameToPort) {
    char pathToDirChar[pathToDir.size()+1];
    stringToCharStar(pathToDirChar, pathToDir);
    vector<string> namesOfFilesEndsWithCargoData;
    getCargoData(pathToDirChar, namesOfFilesEndsWithCargoData);

    int indexNumber;
    string portName;

    for (const auto& name: namesOfFilesEndsWithCargoData) {
        if (handleNameOfFile(name, portName, indexNumber)) {
            auto res = mapPortVisits->find(portName);
            if(res !=  mapPortVisits->end() and res->second > indexNumber) {
                Port *port = new Port(portName, indexNumber);
                if (readPortContainers(port, pathToDir + '\\' + name + R"(.cargo_data)")) {
                    mapPortNameToPort->insert({name, port});
                }
            } else {
                std::cout << "Warning: the file " << name << ".cargo_data is not necessary" << std::endl;
            }
        }
    }
}