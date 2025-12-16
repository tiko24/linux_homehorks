#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

int n_th_prime(int n) {
    int count = 0;
    int num = 1;
    while (count < n) {
        num++;
        bool prime = true;
        for (int i = 2; i * i <= num; i++) {
            if (num % i == 0) {
                prime = false;
                break;
            }
        }
        if (prime) {
            count++;
        }
    }
    return num;
}

int main() {
    int fd_p[2];
    int fd_q[2];

    pipe(fd_p);
    pipe(fd_q);

    int pid = fork();

    if (pid == 0) {
        close(fd_p[1]);
        close(fd_q[0]);

        int m;
        read(fd_p[0], &m, sizeof(m));

        int res = n_th_prime(m);
        write(fd_q[1], &res, sizeof(res));

        close(fd_p[0]);
        close(fd_q[1]);
    }
    else {
        close(fd_p[0]);
        close(fd_q[1]);

        int m;
        std::cin >> m;

        write(fd_p[1], &m, sizeof(m));

        int result;
        read(fd_q[0], &result, sizeof(result));

        std::cout << result << std::endl;

        close(fd_p[1]);
        close(fd_q[0]);
        wait(nullptr);
    }

    return 0;
}