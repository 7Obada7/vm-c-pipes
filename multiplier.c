#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s pipe_name\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    

    while (1) {
    const char *pipe_name = argv[1];
    int pipe_fd = open(pipe_name, O_RDONLY);
        int command;
        read(pipe_fd, &command, sizeof(int));

        if (command == 5) {
            // Received exit command, cleanup and exit
            close(pipe_fd);
            exit(EXIT_SUCCESS);
        }

        int num1, num2;

        // Read numbers from the pipe
        read(pipe_fd, &num1, sizeof(int));
        read(pipe_fd, &num2, sizeof(int));

        // Perform the calculation (multiplication)
        int result = num1 * num2;

        // Close and reopen the pipe for writing
        close(pipe_fd);
        pipe_fd = open(pipe_name, O_WRONLY);

        // Write the result back to the pipe
        write(pipe_fd, &result, sizeof(int));
        close(pipe_fd);
    }

    return 0;
}

