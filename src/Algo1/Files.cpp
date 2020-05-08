//
// Created by itay on 29/03/2020.
//


#include <algorithm>
#include "Files.h"

// if return false - indicate if we failed to open file or there was an error on the first line of the ShipPlan file. (2^3)
bool getSizesShipPlan(const std::string &path, int &numFloors, int &length, int &width, int &numLines) {
    std::ifstream fin;

    try{
        fin.open(path, std::ios::in);
        if(!fin.is_open()){
            throw std::exception();
        }
    } catch (const std::exception& e) {
        return false;
    }

    std::vector<std::string> row;
    std::string line, word;

    while(getline(fin, line)) {
        if(!isCommentLine(line)) {
            break;
        }
    }

    if (!isCommentLine(line)) {
        numLines = 1;
        std::stringstream s(line);
        while (getline(s, word, ',')) {
            row.push_back(removeLeadingAndTrailingWhitespaces(word));
        }

        if (row.size() >= 3) {
            try{
                numFloors = stoi(row[0]);
                length = stoi(row[1]);
                width = stoi(row[2]);
            } catch (const std::exception& e) {
//                std::replace( line.begin(), line.end(), ',', '.');
//                errors->push_back("Error: One of the parameters is not a number. in line: " + line + " (replace comma with .)");
//                std::cout << "Warning: One of the parameters is not a number, in line: " << line << std::endl;
                return false;
            }
        } else {
//            std::replace( line.begin(), line.end(), ',', '.');
//            errors->push_back("Error: Not enough parameters in the first line. in line: " + line + " (replace comma with .)");
//            std::cout << "ERROR: Not enough parameters in the first line, in line: " << line << std::endl;
            return false;
        }
    } else {
//        errors->push_back("Error: Failed to read the line with the parameters");
//        std::cout << "ERROR: Failed to read the line with the parameters" << std::endl;
       return false;
    }

    while(getline(fin, line)) {
        if(!isCommentLine(line)) {
            numLines++;
        }
    }

    fin.close();
    return true;
}

// return vector (bool0, bool1, bool2, bool3, bool4). bool0 - indicates 2^2.
//                                                    bool1 - indicates 2^3.
//                                                    bool2 - indicates 2^0.
//                                                    bool3 - indicates 2^1.
//                                                    bool4 - indicates 2^4.
std::vector<bool> *readShipPlanInFiles(std::vector<std::vector<int>> &blocks, const std::string &path,
        int numFloors, int length, int width) {
    auto* results = new std::vector<bool>{false, false, false, false, false};
    // set to save the X,Y we already saw
    std::map<std::tuple<int, int>, int> map;

    std::ifstream fin;
    try{
        fin.open(path, std::ios::in);
        if(!fin.is_open()){
            throw std::exception();
        }
    } catch (const std::exception& e) {
        results->at(1) = true;
        return results;
    }

    std::vector<std::string> row;
    std::string line, word, temp;
    int i = 0;

    while (getline(fin, line)) {
        if(isCommentLine(line)) {
            continue;
        }
        if (i != 0)
        {
            row.clear();
            std::stringstream s(line);
            while (getline(s, word, ',')) {
                row.push_back(removeLeadingAndTrailingWhitespaces(word));
            }
            if (row.size() >= 3) {
                bool legalLine = true;
                int x0 = 0, x1 = 0, x2 = 0;
                try {
                    x0 = stoi(row.at(0));
                    x1 = stoi(row.at(1));
                    x2 = stoi(row.at(2));

                    if(x0 < 0 or x0 >= length or x1 < 0 or x1 >= width){
                        results->at(3) = true;
                        legalLine = false;
                    }
                    if(x2 < 0 or x2 >= numFloors){
                        results->at(2) = true;
                        legalLine = false;
                    }
                    auto res = map.find(std::tuple<int, int>(x0, x1));
                    if(!map.empty() && res != map.end()){
                        if(res->second == x2){
                            results->at(0) = true;
                        }
                        else{
                            results->at(4) = true;
                            return results;
                        }
                    } else {
                        map[std::tuple<int, int>(x0, x1)] = x2;
                    }
                } catch (const std::exception& e) {
                    results->at(0) = true;
                    legalLine = false;
                }

                if(legalLine) {
                    blocks.at(i - 1).push_back(x0);
                    blocks.at(i - 1).push_back(x1);
                    blocks.at(i - 1).push_back(x2);
                } else {
                    // ToDo replace that with i-- (i believe it will work just do that after all work)
                    blocks.at(i - 1).push_back(-1);
                    blocks.at(i - 1).push_back(-1);
                    blocks.at(i - 1).push_back(-1);
                }
            } else {
                results->at(0) = true;
                // ToDo replace that with i-- (i believe it will work just do that after all work)
                blocks.at(i - 1).push_back(-1);
                blocks.at(i - 1).push_back(-1);
                blocks.at(i - 1).push_back(-1);
            }
        }
        i++;
    }
    fin.close();
    return results;
}

// return vector (bool0, bool1, bool2, bool3). bool0 - indicates 2^5.
//                                             bool1 - indicates 2^6.
//                                             bool2 - indicates 2^7.
//                                             bool3 - indicates 2^8.
std::vector<bool> *readShipPorts(std::vector<std::string> &ports, const std::string &path) {
    std::ifstream fin;
    auto* results = new std::vector<bool>{false, false, false, false};
    int count = 0;
    std::string lastPort = "";
    try{
        fin.open(path, std::ios::in);
        if(!fin.is_open()){
            throw std::exception();
        }
    } catch (const std::exception& e) {
//        errors->push_back("Error: Failed to open file" );
//        std::cout << "ERROR: Failed to open file" << std::endl;
        results->at(2) = true;
        return results;
    }
    std::string line;
    while (getline(fin, line)) {
        if(! isCommentLine(line)) {
            std::string lineAfterRemoval = removeLeadingAndTrailingWhitespaces(line);
            if(isLegalPortName(lineAfterRemoval)){
                if(lineAfterRemoval != lastPort) {
                    ports.push_back(lineAfterRemoval);
                    count++;
                    lastPort = lineAfterRemoval;
                } else {
                    // two or more consecutive ports - 2^5
                    results->at(0) = true;
                }
            } else {
                // illegal port name - 2^6
                results->at(1) = true;
            }
        }
    }

    if(count == 0) {
        // no legal ports at all - 2^7
        results->at(2) = true;
    } else if (count == 1) {
        // only 1 valid port - 2^8
        results->at(3) = true;
    }
    fin.close();
    return results;
}

bool isLegalPortName(std::string portName){
    if(portName.length() != 5)
        return false;
    for(int i = 0; i < 5; i++) {
        char ch = portName.at(i);
        if (!((ch >= 'A' and ch <= 'Z') or (ch >= 'a' && ch <= 'z') ))
            return false;
    }
    return true;
}

// return vector (bool0, bool1). bool0 - indicates 2^14.
//                               bool1 - indicates 2^16.
std::vector<bool> * readPortContainers(Port *port, const std::string &path) {
    std::ifstream fin;
    auto* results = new std::vector<bool>{false, false};
    try{
        fin.open(path, std::ios::in);
        if(!fin.is_open()){
            throw std::exception();
        }
    } catch (const std::exception& e) {
        results->at(1) = true;
        return results;
    }
    std::vector<std::string> row;
    std::string line, word, temp;

    while (getline(fin, line)) {
        if(! isCommentLine(line)) {
            std::string id = "-1", dest = "-1", weight = "-1";
            std::stringstream s(line);
            row.clear();
            while (getline(s, word, ',')) {
                row.push_back(removeLeadingAndTrailingWhitespaces(word));
            }

            if(row.empty() or row.at(0).length() != 11){
                // id cant be read 2^14
                results->at(0) = true;
                continue;
            }

            id = row.at(0);
            if(row.size() >= 3) {
                weight = row.at(1);
                dest = row.at(2);
            } else if(row.size() == 2) {
                weight = row.at(1);
            }

            auto *container = new Container(getWeightIfLegal(weight), dest, id, false);
            port->addContainer(container);
        }
    }
    fin.close();
    return results;
}

// return vector (bool0, bool1, bool2, bool3). bool0 - indicates 2^12.
//                                             bool1 - indicates 2^13.
//                                             bool2 - indicates 2^14.
//                                             bool3 - indicates 2^15.
//                                             bool4 - indicates 2^16.
std::vector<bool> * readPortContainersOld(Port *port, const std::string &path) {
    std::ifstream fin;
    auto* results = new std::vector<bool>{false, false, false, false, false};
    try{
        fin.open(path, std::ios::in);
        if(!fin.is_open()){
            throw std::exception();
        }
    } catch (const std::exception& e) {
//        errors->push_back("Error: Failed to open file");
//        std::cout << "ERROR: Failed to open file" << std::endl;
        results->at(4) = true;
        return results;
    }
    std::vector<std::string> row;
    std::string line, word, temp;

    while (getline(fin, line)) {
        if(! isCommentLine(line)) {
            std::stringstream s(line);
            row.clear();
            while (getline(s, word, ',')) {
                row.push_back(removeLeadingAndTrailingWhitespaces(word));
            }
            if(row.size() < 3) {
                // dest missing - 2^13
                results->at(1) = true;
                if(row.empty()){
                    // row empty - 2^12 and 2^13 and 2^14
                    results->at(0) = true;
                    results->at(2) = true;
                } else {
                    // ID exist
                    if(!isLegalId(row[0])){
                        // illegal ID - 2^15
                        results->at(3) = true;
                    }
                    if(row.size() == 1){
                        // only ID - 2^12
                        results->at(3) = true;
                    } else {
                        // ID and weight
                        if(getWeightIfLegal(row[1]) < 0){
                            // weight is invalid - 2^12
                            results->at(0) = true;
                        }
                    }
                }
            }
            else {
                bool legalLine = true;

                //check weight
                int weight = getWeightIfLegal(row[1]);
                if(weight < 0) {
                    // weight is invalid - 2^12
                    results->at(0) = true;
                    legalLine = false;
                }

                //check ID
                if(!isLegalId(row[0])){
                    // illegal ID - 2^15
                    results->at(3) = true;
                    legalLine = false;
                }

                //check dest port
                if(!isLegalPortName(row[2])){
                    // illegal port name - 2^6
                    results->at(1) = true;
                    legalLine = false;
                }

                if(legalLine) {
                    auto *container = new Container(weight, row[2], row[0], false);
                    if (container->getValid()) {
                        port->addContainer(container);
                    } else {
                        delete container;
                    }
                }
            }
        }
    }
    fin.close();
    return results;
}

int getWeightIfLegal(std::string weight){
    try{
        int _weight = stoi(weight);
        return _weight;
    } catch (const std::exception& e) {
        return -1;
    }
}

bool isLegalId(std::string id){
    int result = 0;
    int mul = 1;
    int digitCheck;
    char ch;
    if(id.length() != 11)
        return false;
    if(id.at(3)!='U' && id.at(3)!='J' && id.at(3)!='Z')
        return false;
    for(int i = 0; i < 4; i++)
    {
        ch = id.at(i);
        if(!(ch >= 'A' && ch <= 'Z'))
            return false;
        result += mul * letterToInt(ch);
        mul *= 2;
    }
    for(int i = 4; i < 10; i++)
    {
        ch = id.at(i);
        if(!(ch >= '0' && ch <= '9'))
            return false;
        result += mul * (ch - '0');
        mul *= 2;
    }
    digitCheck = result % 11;
    if(digitCheck == 10)
        digitCheck = 0;
    return digitCheck == (id.at(10) - '0');
}

std::string removeLeadingAndTrailingWhitespaces(std::string line) {
    const char* t = " \t\n\r\f\v";
    line.erase(0, line.find_first_not_of(t));
    line.erase(line.find_last_not_of(t) + 1);
    return line;
}

bool isCommentLine(std::string line) {
    if(line.empty())
        return true;
    line = removeLeadingAndTrailingWhitespaces(line);
    return line.empty() or line.at(0) == '#';
}

bool hasEnding (std::string const &fullString, std::string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

void stringToCharStar(char* chatStar, std::string str) {
    for (size_t i = 0; i < str.size(); i++) {
        chatStar[i] = str.at(i);
    }
    chatStar[str.size()] = '\0';
}

bool isFile(const char* path) {
    struct stat buf;
    stat(path, &buf);
    return S_ISREG(buf.st_mode);
}

int isDirectory(const char *path) {
    struct stat statbuf;
    if (stat(path, &statbuf) != 0)
        return 0;
    return S_ISDIR(statbuf.st_mode);
}

void getCargoData(const char *path, std::vector<std::string>& res){
    DIR *dir = opendir(path);
    struct dirent *entry = readdir(dir);
    std::string name;
    while (entry != nullptr)
    {
        name = entry->d_name;
        std::string pathString(path);
        char* fullPath = (char *)(malloc((strlen(path)+name.size()+2) * sizeof(char)));
        for (size_t i = 0; i < strlen(path); i++) {
            fullPath[i] = path[i];
        }
        fullPath[strlen(path)] = '/';

        for (size_t i = 0; i < name.size(); i++) {
            fullPath[i+strlen(path)+1] = name.at(i);
        }
        fullPath[strlen(path)+name.size()+1] = '\0';

        if(hasEnding(name, "cargo_data")) {
            if(isFile(fullPath)) {
                res.push_back(name.substr(0, name.size() - 11));
            }
        }
        entry = readdir(dir);
        delete fullPath;
    }

    closedir(dir);
}

std::vector<std::string>* getDirsFromRootDir(const std::string &pathToDir) {
    auto* dirs = new std::vector<std::string>();
    char* path = (char *)(malloc((pathToDir.size() + 1) * sizeof(char)));
    stringToCharStar(path, pathToDir);

    DIR *dir = opendir(path);
    struct dirent *entry = readdir(dir);
    std::string name;
    while (entry != nullptr)
    {
        name = entry->d_name;
        entry = readdir(dir);
        if(name == "." or name == "..") {
            continue;
        }
        std::string pathString(path);
        char* fullPath = (char *)(malloc((strlen(path) + name.size() + 2) * sizeof(char)));
        for (size_t i = 0; i < strlen(path); i++) {
            fullPath[i] = path[i];
        }
        fullPath[strlen(path)] = '/';

        for (size_t i = 0; i < name.size(); i++) {
            fullPath[i + strlen(path) + 1] = name.at(i);
        }
        fullPath[strlen(path) + name.size() + 1] = '\0';

        if(isDirectory(fullPath)) {
            dirs->push_back(fullPath);
        }

        delete fullPath;
    }
    delete path;
    closedir(dir);
    return dirs;
}

void writeToFile(const std::string& filename, const std::string& data) {
    std::ofstream outfile;
    outfile.open(filename, std::ios_base::app);
    outfile << data;
    outfile.close();
}

void emptyFile(const std::string& filename){
    std::ofstream outfile;
    outfile.open(filename);
    outfile.close();
}

std::vector<std::string>* getDirsNamesFromRootDir(const std::string &pathToDir) {

        auto* dirs = new std::vector<std::string>();
        char* path = (char *)(malloc((pathToDir.size() + 1) * sizeof(char)));
        stringToCharStar(path, pathToDir);

        DIR *dir = opendir(path);
        struct dirent *entry = readdir(dir);
        std::string name;
        while (entry != nullptr)
        {
            name = entry->d_name;
            entry = readdir(dir);
            if(name == "." or name == "..") {
                continue;
            }
            std::string pathString(path);
            char* fullPath = (char *)(malloc((strlen(path) + name.size() + 2) * sizeof(char)));
            for (size_t i = 0; i < strlen(path); i++) {
                fullPath[i] = path[i];
            }
            fullPath[strlen(path)] = '/';

            for (size_t i = 0; i < name.size(); i++) {
                fullPath[i + strlen(path) + 1] = name.at(i);
            }
            fullPath[strlen(path) + name.size() + 1] = '\0';

            if(isDirectory(fullPath)) {
                dirs->push_back(name);
            }

            delete fullPath;
        }
        delete path;
        closedir(dir);
        return dirs;
}