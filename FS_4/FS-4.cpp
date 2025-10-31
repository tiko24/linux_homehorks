#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <iostream>
#include <algorithm>

void setup_input(int arg_count, char** arg_values)
{
    if (arg_count != 2) {
        printf("Usage: %s <input_file>\n", arg_values[0]);
        exit(1);
    }

    int file_descriptor = open(arg_values[1], O_RDONLY);
    if (file_descriptor < 0) {
        perror("open");
        exit(1);
    }

    dup2(file_descriptor, STDIN_FILENO);
    close(file_descriptor);
}

int main(int argc, char** argv)
{
    setup_input(argc, argv);

    std::string line;
    std::cin >> line;

    std::string line_reversed = line;
    std::reverse(line_reversed.begin(), line_reversed.end());

    std::cout << line_reversed << std::endl;

    return 0;
}

