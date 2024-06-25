/*
 *  Description:    A test program for the cube solver.
 *
 *  Author(s):      Nictheboy Li <nictheboy@outlook.com>
 *
 *  Last Updated:   2024-06-25
 *
 */

#include <filesystem>
#include <fstream>
#include "../src/cube_solve.hpp"

constexpr int test_count = 20;

std::pair<std::string, int64_t> run_and_get_result(std::string cube_description, bool multi_thread) {
    IHandleQueueSystem<PCubeTask>* pSystem =
        multi_thread
            ? (IHandleQueueSystem<PCubeTask>*)new HandleQueueSystemMultiThread<PCubeTask>()
            : (IHandleQueueSystem<PCubeTask>*)new HandleQueueSystemSingleThread<PCubeTask>();
    CubeHandler* pHandler = new CubeHandler(5, false, false);
    PCubeTask pTask = new CubeTask(Cube(cube_description), std::vector<CubeAction>());

    std::chrono::time_point start = std::chrono::system_clock::now();
    pSystem->Solve(pTask, *pHandler);
    std::chrono::time_point end = std::chrono::system_clock::now();

    std::string result = pHandler->GetActionDescription();
    delete pHandler;
    delete pSystem;
    return std::make_pair(result, std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
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
    std::pair<std::string, int64_t> result = run_and_get_result(test_input, multi_thread);
    std::stringstream ss(result.first);
    while (!ss.eof()) {
        std::string line;
        std::getline(ss, line);
        if (line == expected_output) {
            printf("Test %d passed: %ldms\n", test_index, result.second);
            return true;
        }
    }
    printf("Expected: %s\n", expected_output.c_str());
    printf("Actual:\n");
    printf("%s\n", result.first.c_str());
    return false;
}

int main() {
    bool has_failed = false;
    printf("Running tests for single thread...\n");
    for (int i = 1; i <= test_count; i++) {
        if (!run_test_for(i, false)) {
            has_failed = true;
            printf("Test %d failed\n", i);
        }
    }
    printf("Running tests for multi thread...\n");
    for (int i = 1; i <= test_count; i++) {
        if (!run_test_for(i, true)) {
            has_failed = true;
            printf("Test %d failed\n", i);
        }
    }
    return has_failed ? -1 : 0;
}
