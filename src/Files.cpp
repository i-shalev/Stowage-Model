//
// Created by itay on 29/03/2020.
//


#include <algorithm>
#include "Files.h"

bool getSizesShipPlan(const std::string &path, int &numFloors, int &length, int &width, int &numLines, std::vector<std::string>* errors) {
    std::ifstream fin;
    try{
        fin.open(path, std::ios::in);
    } catch (const std::exception& e) {
        errors->push_back("Error: Failed to open file");
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
                std::replace( line.begin(), line.end(), ',', '.');
                errors->push_back("Error: One of the parameters is not a number. in line: " + line + " (replace comma with .)");
//                std::cout << "Warning: One of the parameters is not a number, in line: " << line << std::endl;
                return false;
            }
        } else {
            std::replace( line.begin(), line.end(), ',', '.');
            errors->push_back("Error: Not enough parameters in the first line. in line: " + line + " (replace comma with .)");
//            std::cout << "ERROR: Not enough parameters in the first line, in line: " << line << std::endl;
            return false;
        }
    } else {
        errors->push_back("Error: Failed to read the line with the parameters");
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

bool readShipPlan(std::vector<std::vector<int>> &blocks, const std::string &path, std::vector<std::string> *errors) {
    std::ifstream fin;
    try{
        fin.open(path, std::ios::in);
    } catch (const std::exception& e) {
        errors->push_back("ERROR: Failed to open file");
//        std::cout << "ERROR: Failed to open file" << std::endl;
        return false;
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
                        blocks.at(i - 1).push_back(stoi(row.at(j)));
                    } catch (const std::exception& e) {
                        std::replace( line.begin(), line.end(), ',', '.');
                        errors->push_back("Warning: One of the parameters is not a number. in line: " + line + " (replace comma with .)");
//                        std::cout << "Warning: One of the parameters is not a number, in line: " << line << std::endl;
                        blocks.at(i - 1).push_back(-1);
                    }
                }
            } else {
                std::replace( line.begin(), line.end(), ',', '.');
                errors->push_back("Warning: Not enough parameters - expected 3 parameters per line. in line: " + line + " (replace comma with .)");
//                std::cout << "Warning: Not enough parameters - expected 3 parameters per line, in line: " << line << std::endl;
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

bool readShipPorts(std::vector<std::string> &ports, const std::string &path, std::vector<std::string> *errors) {
    std::ifstream fin;
    try{
        fin.open(path, std::ios::in);
    } catch (const std::exception& e) {
        errors->push_back("Error: Failed to open file" );
//        std::cout << "ERROR: Failed to open file" << std::endl;
        return false;
    }
    std::string line;
    while (getline(fin, line)) {
        if(! isCommentLine(line)) {
            ports.push_back(removeLeadingAndTrailingWhitespaces(line));
        }
    }
    fin.close();
    return true;
}

bool readPortContainers(Port *&port, const std::string &path, std::vector<std::string> *errors) {
    std::ifstream fin;
    try{
        fin.open(path, std::ios::in);
    } catch (const std::exception& e) {
        errors->push_back("Error: Failed to open file");
//        std::cout << "ERROR: Failed to open file" << std::endl;
        return false;
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
                std::replace( line.begin(), line.end(), ',', '.');
                errors->push_back("Warning: not all the information about container was given. in line: " + line + " (replace comma with .)");
//                std::cout << "Warning: not all the information about container was given, in line: " << line << std::endl;
            }
            else {
                try{
                   int weight = stoi(row[1]);
                    auto *container = new Container(weight, row[2], row[0], false);
                    if (container->getValid()) {
                        port->addContainer(container);
                    } else {
                        delete container;
                        std::replace( line.begin(), line.end(), ',', '.');
                        errors->push_back("Warning: ID or destination is not valid. in line: " + line + " (replace comma with .)");
//                        std::cout << "Warning: ID or destination is not valid , in line: " << line << std::endl;
                    }
                } catch (const std::exception& e) {
                    std::replace( line.begin(), line.end(), ',', '.');
                    errors->push_back("Warning: weight is not int. in line: " + line + " (replace comma with .)");
//                    std::cout << "Warning: weight is not int, in line: " << line << std::endl;
                    continue;
                }
            }
        }
    }
    fin.close();
    return true;
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