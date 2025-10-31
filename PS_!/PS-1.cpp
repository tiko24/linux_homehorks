#include <iostream>
#include <fcntl.h>
#include <chrono>
#include <unistd.h>
#include <vector>
#include <string>
#include <sys/wait.h>

void do_exec(std::vector<char*> cmd) {
    auto start = std::chrono::high_resolution_clock::now();
    
    pid_t a = fork();
    if(a < 0) {
        std::cerr << "fork failed";
        return;
    } else if (a == 0) {
        // Child process - convert vector to char* array for execvp
        int errorcode = execvp(cmd[0], cmd.data());
        if (errorcode < 0) {
            std::cerr << "Execution failed\n";
            exit(1);
        }
    } else {
        // Parent process
        int stat;
        int errorcode = waitpid(a, &stat, 0);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        if (errorcode < 0) {
            std::cerr << "waitpid failed\n";
        } else {
            std::cout << "Command executed in " << duration.count() << " ms\n";
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Invalid command\n";
        exit(1);
    }
    
    std::vector<char*> cmds;
    for(int i = 1; i < argc; ++i) {
        cmds.push_back(argv[i]);
    }
    cmds.push_back(nullptr);
    
    do_exec(cmds);

    return 0;
}
