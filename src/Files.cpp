//
// Created by itay on 29/03/2020.
//

#include "Files.h"
#include <stdio.h>
#include <dirent.h>

bool getSizesShipPlan(const string &path, int &numFloors, int &length, int &width, int &numLines) {
    ifstream fin;
    try{
        fin.open(path, ios::in);
    } catch (const std::exception& e) {
        std::cout << "ERROR: Failed to open file" << std::endl;
        return false;
    }

    vector<string> row;
    string line, word;
    int i = 0;

    if (getline(fin, line)) {
        numLines = 1;
        stringstream s(line);
        while (getline(s, word, ',')) {
            row.push_back(word);
        }

        if (row.size() >= 3) {
            try{
                numFloors = stoi(row[0]);
                length = stoi(row[1]);
                width = stoi(row[2]);
            } catch (const std::exception& e) {
                std::cout << "Warning: One of the parameters is not a number" << std::endl;
                return false;
            }
        } else {
                std::cout << "ERROR: Not enough parameters in the first line" << std::endl;
                return false;
        }

    } else {
        std::cout << "ERROR: Failed to read line" << std::endl;
       return false;
    }

    while(getline(fin, line)) {
        numLines++;
    }

    fin.close();
    return true;
}

bool readShipPlan(vector<vector<int>>& blocks, const string& path) {
    ifstream fin;
    try{
        fin.open(path, ios::in);
    } catch (const std::exception& e) {
        std::cout << "ERROR: Failed to open file" << std::endl;
        return false;
    }

    vector<string> row;
    string line, word, temp;
    int i = 0;

    while (getline(fin, line)) {
        if (i != 0)
        {
            row.clear();
            stringstream s(line);
            while (getline(s, word, ',')) {
                row.push_back(word);
            }
            if (row.size() >= 3) {
                for(int j=0; j < 3; j++){
                    try {
                        blocks.at(i - 1).push_back(stoi(word));
                    } catch (const std::exception& e) {
                        std::cout << "Warning: One of the parameters is not a number" << std::endl;
                        blocks.at(i - 1).push_back(-1);
                    }
                }
            } else {
                std::cout << "Warning: Not enough parameters - expected 3 parameters per line" << std::endl;
                blocks.at(i - 1).push_back(-1);
                blocks.at(i - 1).push_back(-1);
                blocks.at(i - 1).push_back(-1);
            }
        }
        i++;
    }
    fin.close();
    return true;
}

bool getNumberOfNonEmtpyLines(const string &path, int &numLines){
    ifstream fin;
    try{
        fin.open(path, ios::in);
    } catch (const std::exception& e) {
        std::cout << "ERROR: Failed to open file" << std::endl;
        return false;
    }

    numLines = 0;
    string line;

    while(getline(fin, line)) {
        if(!line.empty()) {
            numLines++;
        }
    }
    fin.close();
    return true;
}

bool readShipPorts(vector<string>& ports, const string& path) {
    ifstream fin;
    try{
        fin.open(path, ios::in);
    } catch (const std::exception& e) {
        std::cout << "ERROR: Failed to open file" << std::endl;
        return false;
    }

    string line;
    while (getline(fin, line)) {
        if(!line.empty()) {
            ports.push_back(line);
        }
    }
    fin.close();
    return true;
}

bool readPortContainers(Port*& port, const string& path) {
    ifstream fin;
    try{
        fin.open(path, ios::in);
    } catch (const std::exception& e) {
        std::cout << "ERROR: Failed to open file" << std::endl;
        return false;
    }
    vector<string> row;
    string line, word, temp;

    while (getline(fin, line)) {
        if(!line.empty()) {
            stringstream s(line);
            row.clear();
            while (getline(s, word, ',')) {
                row.push_back(word);
            }
            if(row.size() < 3) {
                std::cout << "Warning: not all the information about container was given" << std::endl;
            }
            else {
                try{
                   int weight = stoi(row[1]);
                    auto *container = new Container(weight, row[2], row[0], false);
                    if (container->getValid()) {
                        port->addContainer(container);
                    } else {
                        delete container;
                        std::cout << "Warning: ID or destination is not valid " << std::endl;
                    }
                } catch (const std::exception& e) {
                    std::cout << "Warning: weight is not int" << std::endl;
                    continue;
                }
            }
        }
    }
    fin.close();
    return true;
}

bool hasEnding (std::string const &fullString, std::string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

void getCargoData(const char *path, vector<string>& res){
    DIR *dir = opendir(path);
    struct dirent *entry = readdir(dir);
    string name = "";
    while (entry != NULL)
    {
        name = entry->d_name;
        if(hasEnding(name, "cargo_data")) {
            res.push_back(name.substr(0, name.size() - 11));
        }
        entry = readdir(dir);
    }

    closedir(dir);

}