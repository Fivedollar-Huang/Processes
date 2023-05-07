#include <iostream>
#include <fstream>
#include <sstream>

#include "src/structures.hpp"
#include "src/functions.hpp"
#include "src/handleFile.hpp"

int totalFile;
bool started;

std::pair<int, int> getParam(std::string str){
    std::pair<int,int> result;
    result.first = -1;
    result.second = -1;
    std::stringstream ss(str);
    std::string s;
    std::getline(ss, s, ' ');
    if(std::getline(ss, s, ' ')){
        // for(char c : s){
        //     if(!isdigit(c)){
        //         std::cout << "*--param must be digit, found: " << 
        //             c << " instead "<< std::endl;
        //         return result;
        //     }
        // }
        result.first = std::stoi(s);

        if(std::getline(ss, s, ' ')){
            // for(int i = 0; i < s.size(); i++){
            //     if(!isdigit(s[i])){
            //         std::cout << "*--param must be digit, found: " << 
            //             s[i] << " instead" << std::endl;
            //         return result;
            //     }
            // }
            result.second = std::stoi(s);
        }
    }
    return result;
}

void readUserInput(std::string userInput){
    std::pair<int, int> param = getParam(userInput);
    switch(userInput[0]){ 
        // Only check for first character, all inputs are assumed to be valid;
        case 'i':
            started = true;
            std::cout << "*Reading Command: in" << std::endl;
            totalFile++;
            init();
            break;
        case 'c':
            std::cout << "*Reading Command: cr" << std::endl;
            create(param.first);
            break;
        case 'd':
            std::cout << "*Reading Command: de" << std::endl;
            destroy(param.first);
            break;
        case 'r':
            if(userInput[1] == 'l'){
                std::cout << "*Reading Command: rl" << std::endl;
                release(param.first, param.second);
            }
            else if(userInput[1] == 'q'){
                std::cout << "*Reading Command: rq" << std::endl;
                request(param.first, param.second);
            }
            else{
                std::cout << "*Invalid Command" << std::endl;
            }
            break;
        case 't':
            std::cout << "*Reading Command: to" << std::endl;
            timeout();
            break;
        case 'p':
            printReadyList();
            break;
        default:
            std::cout << "*Invalid Command: " << userInput << std::endl;
            break;
    }

}

int main(int argc, char const *argv[])
{    
    std::string userInput = "None";
    
    initOutputFile();
    if(argc == 1){
        std::cout << "Accepting User Input" << std::endl;
        while(true){
            getline(std::cin, userInput);
            if(userInput[0] == 'e') break;
            readUserInput(userInput);
        }
    }
    else{
        std::cout << "Reading File \"" << argv[1] << "\"" << std::endl;
        std::ifstream f(argv[1]);
        totalFile = 0;
        started = false;
        while(getline(f, userInput)){
            readUserInput(userInput);
        }
    }
    closeOutputFile();
    std::cout << "Exiting Program" << std::endl;

    return 0;
}
