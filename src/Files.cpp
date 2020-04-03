//
// Created by itay on 29/03/2020.
//

#include <fstream>
#include <sstream>
#include <istream>
#include "Files.h"
#include <string.h>

void getSizesShipPlan(const string& path, int& numFloors, int& length, int& width) {
    ifstream fin;
    fin.open(path, ios::in);

    vector<string> row;
    string line, word;
    int i = 0;

    if (getline(fin, line)) {
//        std::cout << "loop1" << std::endl;

        vector<vector<int>> floorBlocks;
        row.clear();
//        std::cout << "line:" << line << std::endl;

        // used for breaking words
        stringstream s(line);

        // read every column data of a row and
        // store it in a string variable, 'word'
        while (getline(s, word, ',')) {
            // add all the column data
            // of a row to a vector
            row.push_back(word);
        }

            numFloors = stoi(row[0]);
            length = stoi(row[1]);
            width = stoi(row[2]);

    }

    fin.close();
}

void readShipPlan(vector<vector<vector<int>>>& allBlocks, const string& path) {
    ifstream fin;
    fin.open(path, ios::in);



    vector<string> row;
    string line, word, temp;
    int i = 0;

    while (getline(fin, line)) {
//        std::cout << "loop1" << std::endl;
        i++;
        vector<vector<int>> floorBlocks;
        row.clear();
//        std::cout << "line:" << line << std::endl;

        // used for breaking words
        stringstream s(line);

        // read every column data of a row and
        // store it in a string variable, 'word'
        while (getline(s, word, ',')) {
            // add all the column data
            // of a row to a vector
            row.push_back(word);
        }
        if (i != 1)
        {

//            std::cout << "i != 1" << std::endl;
//            std::cout << "row size = " << row.size() << std::endl;
            for (int j = 0 ; j < row.size() ; j+=2) {
                word = row.at(j);
//                std::cout << " : " << word << std::endl;
                if(word.empty()) {
                    continue;
                }

                vector<int> block;
//                block.push_back(stoi(word.substr(1, word.size())));
//                std::cout << "try to push to floor: " << i-2 << std::endl;
//                std::cout << "at place: " << j/2 << std::endl;
//                std::cout << "first: " << stoi(word.substr(1, word.size())) << std::endl;
                allBlocks.at(i-2).at(j/2).at(0) = (stoi(word.substr(1, word.size())));
//                std::cout << "Done!" << std::endl;
                word = row.at(j+1);

//                std::cout << "second: " << stoi(word.substr(0, word.size() - 1)) << std::endl;
                allBlocks.at(i-2).at(j/2).at(1) = (stoi(word.substr(0, word.size() - 1)));
//                std::cout << "Done!" << std::endl;
//                block.push_back(stoi(word.substr(0, word.size() - 1)));
//                    std::cout << "else - j = " << j << std::endl;
//                    std::cout << word << std::endl;
//                    std::cout << "int: " << word.substr(0, word.size() - 1) << std::endl;
//                    std::cout << "int: " << stoi(word.substr(0, word.size() - 1)) << std::endl;
//                    std::cout << "first: " << word.substr(0, word.size() - 2) << std::endl;
//                    std::cout << "second: " << word.substr(1, word.size() - 1) << std::endl;

//                allBlocks.at(i-1).at(j/2).push_back(1);
            }
//            exit(0);
//            allBlocks.push_back(floorBlocks);
        }
    }
    fin.close();
//    return allBlocks;
}