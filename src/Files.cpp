//
// Created by itay on 29/03/2020.
//

#include <dirent.h>
#include <sys/stat.h>
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

    while(getline(fin, line)) {
        if(!isCommentLine(line)) {
            break;
        }
    }

    if (!isCommentLine(line)) {
        numLines = 1;
        stringstream s(line);
        while (getline(s, word, ',')) {
            row.push_back(removeLeadingAndTrailingWhitespaces(word));
        }

        if (row.size() >= 3) {
            try{
                numFloors = stoi(row[0]);
                length = stoi(row[1]);
                width = stoi(row[2]);
            } catch (const std::exception& e) {
                std::cout << "Warning: One of the parameters is not a number, in line: " << line << std::endl;
                return false;
            }
        } else {
            std::cout << "ERROR: Not enough parameters in the first line, in line: " << line << std::endl;
            return false;
        }
    } else {
        std::cout << "ERROR: Failed to read the line with the parameters" << std::endl;
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
        if(isCommentLine(line)) {
            continue;
        }
        if (i != 0)
        {
            row.clear();
            stringstream s(line);
            while (getline(s, word, ',')) {
                row.push_back(removeLeadingAndTrailingWhitespaces(word));
            }
            if (row.size() >= 3) {
                for(int j=0; j < 3; j++){
                    try {
                        blocks.at(i - 1).push_back(stoi(word));
                    } catch (const std::exception& e) {
                        std::cout << "Warning: One of the parameters is not a number, in line: " << line << std::endl;
                        blocks.at(i - 1).push_back(-1);
                    }
                }
            } else {
                std::cout << "Warning: Not enough parameters - expected 3 parameters per line, in line: " << line << std::endl;
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
        if(! isCommentLine(line)) {
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
        if(! isCommentLine(line)) {
            ports.push_back(removeLeadingAndTrailingWhitespaces(line));
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
        if(! isCommentLine(line)) {
            stringstream s(line);
            row.clear();
            while (getline(s, word, ',')) {
                row.push_back(removeLeadingAndTrailingWhitespaces(word));
            }
            if(row.size() < 3) {
                std::cout << "Warning: not all the information about container was given, in line: " << line << std::endl;
            }
            else {
                try{
                   int weight = stoi(row[1]);
                    auto *container = new Container(weight, row[2], row[0], false);
                    if (container->getValid()) {
                        port->addContainer(container);
                    } else {
                        delete container;
                        std::cout << "Warning: ID or destination is not valid , in line: " << line << std::endl;
                    }
                } catch (const std::exception& e) {
                    std::cout << "Warning: weight is not int, in line: " << line << std::endl;
                    continue;
                }
            }
        }
    }
    fin.close();
    return true;
}

string removeLeadingAndTrailingWhitespaces(string line) {
    const char* t = " \t\n\r\f\v";
    line.erase(0, line.find_first_not_of(t));
    line.erase(line.find_last_not_of(t) + 1);
    return line;
}

bool isCommentLine(string line) {
    if(line.empty())
        return true;
    line = removeLeadingAndTrailingWhitespaces(line);
    return line.at(0) == '#';
}

bool hasEnding (std::string const &fullString, std::string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

void stringToCharStar(char* chatStar, string str) {
    for (int i = 0; i < str.size(); i++) {
        chatStar[i] = str.at(i);
    }
    chatStar[str.size()] = '\0';
}

bool isFile(const char* path) {
    struct stat buf;
    stat(path, &buf);
    return S_ISREG(buf.st_mode);
}

void getCargoData(const char *path, vector<string>& res){
    DIR *dir = opendir(path);
    struct dirent *entry = readdir(dir);
    string name = "";
    while (entry != NULL)
    {
        name = entry->d_name;
        string pathString(path);
        char fullPath[strlen(path)+name.size()+2];

        for (int i = 0; i < strlen(path); i++) {
            fullPath[i] = path[i];
        }
        fullPath[strlen(path)] = '\\';

        for (int i = 0; i < name.size(); i++) {
            fullPath[i+strlen(path)+1] = name.at(i);
        }
        fullPath[strlen(path)+name.size()+1] = '\0';

        if(hasEnding(name, "cargo_data")) {
            if(isFile(fullPath)) {
                res.push_back(name.substr(0, name.size() - 11));
            }
        }
        entry = readdir(dir);
    }

    closedir(dir);
}

