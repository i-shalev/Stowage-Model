//
// Created by itay on 29/03/2020.
//


#include <algorithm>
#include "Files.h"

// if return false - indicate if we failed to open file or there was an error on the first line of the ShipPlan file. (2^3)
bool getSizesShipPlan(const std::string &path, int &numFloors, int &length, int &width, int &numLines, std::vector<std::string>* errors) {
    std::ifstream fin;

    try{
        fin.open(path, std::ios::in);
    } catch (const std::exception& e) {
//        errors->push_back("Error: Failed to open file");
//        std::cout << "ERROR: Failed to open file" << std::endl;
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

// return vector (bool0, bool1). bool0 - indicates 2^2.
//                               bool1 - indicates 2^3.
//  TODO add check for 2^0, 2^1 when create the ship, all other errors are handle here so can skip without report it.
std::vector<bool> readShipPlan(std::vector<std::vector<int>> &blocks, const std::string &path, std::vector<std::string> *errors) {
    std::vector<bool> results = {false, false};
    std::ifstream fin;
    try{
        fin.open(path, std::ios::in);
    } catch (const std::exception& e) {
//        errors->push_back("ERROR: Failed to open file");
//        std::cout << "ERROR: Failed to open file" << std::endl;
        results[1] = true;
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
                for(int j=0; j < 3; j++){
                    try {
                        int num = stoi(row.at(j));
                        blocks.at(i - 1).push_back(num);
                        if(num < 0){
                            results[0] = true;
                        }
                    } catch (const std::exception& e) {
                        results[0] = true;
//                        std::replace( line.begin(), line.end(), ',', '.');
//                        errors->push_back("Warning: One of the parameters is not a number. in line: " + line + " (replace comma with .)");
//                        std::cout << "Warning: One of the parameters is not a number, in line: " << line << std::endl;
                        blocks.at(i - 1).push_back(-1);
                    }
                }
            } else {
                results[0] = true;
//                std::replace( line.begin(), line.end(), ',', '.');
//                errors->push_back("Warning: Not enough parameters - expected 3 parameters per line. in line: " + line + " (replace comma with .)");
//                std::cout << "Warning: Not enough parameters - expected 3 parameters per line, in line: " << line << std::endl;
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
std::vector<bool> readShipPorts(std::vector<std::string> &ports, const std::string &path, std::vector<std::string> *errors) {
    std::ifstream fin;
    std::vector<bool> results = {false, false, false, false};
    int count = 0;
    std::string lastPort = "";
    try{
        fin.open(path, std::ios::in);
    } catch (const std::exception& e) {
//        errors->push_back("Error: Failed to open file" );
//        std::cout << "ERROR: Failed to open file" << std::endl;
        results[2] = true;
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
                    results[0] = true;
                }
            } else {
                // illegal port name - 2^6
                results[1] = true;
            }
        }
    }

    if(count == 0) {
        // no legal ports at all - 2^7
        results[2] = true;
    } else if (count == 1) {
        // only 1 valid port - 2^8
        results[3] = true;
    }
    fin.close();
    return results;
}

bool isLegalPortName(std::string portName){
    if(portName.length() != 5)
        return false;
    for(int i = 0; i < 5; i++) {
        char ch = portName.at(i);
        if (!(ch >= 'A' && ch <= 'Z'))
            return false;
    }
    return true;
}

// return vector (bool0, bool1, bool2, bool3). bool0 - indicates 2^12.
//                                             bool1 - indicates 2^13.
//                                             bool2 - indicates 2^14.
//                                             bool3 - indicates 2^15.
//                                             bool4 - indicates 2^16.
std::vector<bool> readPortContainers(Port *&port, const std::string &path, std::vector<std::string> *errors) {
    std::ifstream fin;
    std::vector<bool> results = {false, false, false, false};
    try{
        fin.open(path, std::ios::in);
    } catch (const std::exception& e) {
//        errors->push_back("Error: Failed to open file");
//        std::cout << "ERROR: Failed to open file" << std::endl;
        results[4] = true;
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
                results[1] = true;
                if(row.empty()){
                    // row empty - 2^12 and 2^13 and 2^14
                    results[0] = true;
                    results[2] = true;
                } else {
                    // ID exist
                    if(!isLegalId(row[0])){
                        // illegal ID - 2^15
                        results[3] = true;
                    }
                    if(row.size() == 1){
                        // only ID - 2^12
                        results[0] = true;
                    } else {
                        // ID and weight
                        if(getWeightIfLegal(row[1]) < 0){
                            // weight is invalid - 2^12
                            results[0] = true;
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
                    results[0] = true;
                    legalLine = false;
                }

                //check ID
                if(!isLegalId(row[0])){
                    // illegal ID - 2^15
                    results[3] = true;
                    legalLine = false;
                }

                //check dest port
                if(!isLegalPortName(row[0])){
                    // illegal port name - 2^6
                    results[1] = true;
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
    return line.size() == 0 or line.at(0) == '#';
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