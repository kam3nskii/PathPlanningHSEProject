#include "mission.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Error! Pathfinding task file (XML) is not specified!" << std::endl;
        return 0;
    }

    Mission mission(argv[1]);

    std::cout << argv[1] << std::endl;
    std::cout << "Parsing the map from XML:" << std::endl;

    if (!mission.getMap()) {
        std::cout << "Incorrect map! Program halted!" << std::endl;
    } else {
        std::cout << "Map OK!" << std::endl
                  << "Parsing configurations (algorithm, log) from XML:" << std::endl;
        if (!mission.getConfig()) {
            std::cout << "Incorrect configurations! Program halted!" << std::endl;
        } else {
            std::cout << "Configurations OK!" << std::endl << "Creating log channel:" << std::endl;

            if (!mission.createLog()) {
                std::cout << "Log chanel has NOT been created! Program halted!" << std::endl;
            } else {
                std::cout << "Log chanel has been created!\n" << std::endl;
                mission.createEnvironmentOptions();

                if (mission.options.searchtype == CN_SP_ST_ASTAR) {
                    mission.startSearchAstar();
                } else if (mission.options.searchtype == CN_SP_ST_LPASTAR) {
                    mission.runLPAstar(5);
                } else if (mission.options.searchtype == CN_SP_ST_DSTARLITE) {
                    mission.mainDstarLite();
                }
            }
        }
    }
    return 0;
}


//get data
// int main(int argc, char* argv[]) {
//     if (argc < 2) {
//         std::cerr << "Error! Pathfinding task file (XML) is not specified!" << std::endl;
//         return 0;
//     }

//     int iterations = 15;

//     {
//         // Mission mission(argv[1]);
//         // if (!mission.getMap()) {
//         //     std::cerr << "Incorrect map! Program halted!" << std::endl;
//         //     return -1;
//         // }
//         // if (!mission.getConfig()) {
//         //     std::cerr << "Incorrect configurations! Program halted!" << std::endl;
//         //     return -1;
//         // }
//         // if (!mission.createLog()) {
//         //     std::cerr << "Log chanel has not been created! Program halted!" << std::endl;
//         //     return -1;
//         // }
//         // mission.createEnvironmentOptions();
//         // mission.createSearch();
//         // mission.startSearchLPAstar();
//         // SearchResult resultLPA = mission.getLPAstarSearchResult();
//         // SearchResult resultA = mission.getAstarSearchResult();
//         // if (mission.checkResult(resultLPA, resultA.pathlength) == -1) {
//         //     return 1;
//         // }
//         // if (mission.testLPAstar(iterations)) {
//         //     return 1;
//         // }
//     }
//     {
//         Mission mission(argv[1]);
//         if (!mission.getMap()) {
//             std::cerr << "Incorrect map! Program halted!" << std::endl;
//             return -1;
//         }
//         if (!mission.getConfig()) {
//             std::cerr << "Incorrect configurations! Program halted!" << std::endl;
//             return -1;
//         }
//         if (!mission.createLog()) {
//             std::cerr << "Log chanel has not been created! Program halted!" << std::endl;
//             return -1;
//         }
//         mission.createEnvironmentOptions();
//         mission.createSearch();
//         mission.startSearchDstarLite();
//         SearchResult resultDstar = mission.getDstarLiteSearchResult();
//         SearchResult resultA = mission.getAstarSearchResult();
//         if (mission.checkResult(resultDstar, resultA.pathlength) == -1) {
//             return 1;
//         }
//         if (mission.testDstarLite(iterations)) {
//             return 1;
//         }
//     }
// }
