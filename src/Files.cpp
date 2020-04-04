//
// Created by itay on 29/03/2020.
//

#include <fstream>
#include <sstream>
#include <istream>
#include "Files.h"

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
        try{
            numFloors = stoi(row[0]);
            length = stoi(row[1]);
            width = stoi(row[2]);
        } catch (const std::exception& e) {
            std::cout << "Warning: One of the parameters is not a number" << std::endl;
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
                try {
                    blocks.at(i - 1).push_back(stoi(word));
                } catch (const std::exception& e) {
                    std::cout << "Warning: One of the parameters is not a number" << std::endl;
                    blocks.at(i - 1).push_back(-1);
                }
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