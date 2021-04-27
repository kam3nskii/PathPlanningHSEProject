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

                mission.run(10);
                //mission.startSearch();
                // mission.saveLog(mission.sr_LPAstar, "_LPAStarTMP", 0); //
                // mission.saveLog(mission.sr_LPAstar, "_LPAStar", 1); //
                // mission.saveLog(mission.sr_Astar, "_AStar", 2); //
                //return mission.test(1); // 3
            }
        }
    }
    return 0;
}
