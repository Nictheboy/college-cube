#include <filesystem>
#include <fstream>
#include "../src/cube_solve.hpp"

constexpr int test_count = 20;

std::string run_and_get_result(std::string cube_description, bool multi_thread) {
    IHandleQueueSystem<PCubeTask>* pSystem =
        multi_thread ? new HandleQueueSystemMultiThread<PCubeTask>() : new HandleQueueSystemMultiThread<PCubeTask>();
    CubeHandler* pHandler = new CubeHandler(5, false, false);
    PCubeTask pTask = new CubeTask(Cube(cube_description), std::vector<CubeAction>());

    pSystem->Solve(pTask, *pHandler);

    std::string result = pHandler->GetActionDescription();
    delete pHandler;
    delete pSystem;
    return result;
}

bool run_test_for(int test_index, bool multi_thread) {
    std::string test_input;
    std::string expected_output;
    std::string test_source_code_path = std::string(__FILE__).substr(0, std::string(__FILE__).find_last_of("/\\"));
    std::string test_file_path = (std::filesystem::path(test_source_code_path) / "cases" / (std::to_string(test_index) + ".txt")).string();
    std::ifstream test_file(test_file_path);
    if (!test_file.is_open()) {
        printf("Test %d: test data file not found\n", test_index);
        return false;
    }
    for (int i = 0; i < 24; i++) {
        std::string line;
        std::getline(test_file, line);
        test_input += line;
        test_input += "\n";
    }
    for (int i = 0; i < 5; i++) {
        std::string action;
        test_file >> action;
        expected_output += action;
        expected_output += " ";
    }
    expected_output.pop_back();
    std::string result = run_and_get_result(test_input, multi_thread);
    std::stringstream ss(result);
    while (!ss.eof()) {
        std::string line;
        std::getline(ss, line);
        if (line == expected_output)
            return true;
    }
    printf("Expected: %s\n", expected_output.c_str());
    printf("Actual:\n");
    printf("%s\n", result.c_str());
    return false;
}

int main() {
    bool has_failed = false;
    printf("Running tests for single thread...\n");
    for (int i = 1; i <= test_count; i++) {
        if (run_test_for(i, false)) {
            printf("Test %d passed\n", i);
        } else {
            has_failed = true;
            printf("Test %d failed\n", i);
        }
    }
    printf("Running tests for multi thread...\n");
    for (int i = 1; i <= test_count; i++) {
        if (run_test_for(i, true)) {
            printf("Test %d passed\n", i);
        } else {
            has_failed = true;
            printf("Test %d failed\n", i);
        }
    }
    return has_failed ? -1 : 0;
}
