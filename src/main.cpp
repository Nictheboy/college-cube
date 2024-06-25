#include "cube_solve.hpp"

std::string GetAllStdin() {
    std::string all_stdin;
    std::string line;
    while (std::getline(std::cin, line)) {
        all_stdin += line;
        all_stdin += "\n";
    }
    return all_stdin;
}

int main() {
    const int max_depth = 5;
    IHandleQueueSystem<PCubeTask>* pSystem = new HandleQueueSystemSingleThread<PCubeTask>();
    // IHandleQueueSystem<PCubeTask>* pSystem = new HandleQueueSystemMultiThread<PCubeTask>();
    CubeHandler* pHandler = new CubeHandler(max_depth, false, false);
    PCubeTask pTask = new CubeTask(Cube(GetAllStdin()), std::vector<CubeAction>());
    std::cout << "Start solving..." << std::endl;

    std::chrono::time_point start = std::chrono::system_clock::now();
    pSystem->Solve(pTask, *pHandler);
    std::chrono::time_point end = std::chrono::system_clock::now();
    std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;

    if (pHandler->IsSolved())
        std::cout << pHandler->GetActionDescription() << std::endl;
    else
        std::cout << "No solution within " << max_depth << " steps" << std::endl;
    delete pHandler;
    delete pSystem;
    return 0;
}
