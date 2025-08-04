#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>  // Add this line for mkfifo

#define ADDER_PIPE "adder_pipe"
#define SUBTRACTOR_PIPE "subtractor_pipe"
#define MULTIPLIER_PIPE "multiplier_pipe"
#define DIVIDER_PIPE "divider_pipe"

void create_pipes() {
    mkfifo(ADDER_PIPE, 0666);
    mkfifo(SUBTRACTOR_PIPE, 0666);
    mkfifo(MULTIPLIER_PIPE, 0666);
    mkfifo(DIVIDER_PIPE, 0666);
}

pid_t create_subprocess(const char *program, const char *pipe_name) {
    pid_t pid = fork();
    if (pid == 0) {
        execl(program, program, pipe_name, NULL);
        exit(EXIT_SUCCESS);
    } else if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    return pid;
}

int main() {
    create_pipes();
    // Create subprocesses
    pid_t adder_pid = create_subprocess("./adder", ADDER_PIPE);
    pid_t subtractor_pid = create_subprocess("./subtractor", SUBTRACTOR_PIPE);
    pid_t multiplier_pid = create_subprocess("./multiplier", MULTIPLIER_PIPE);
    pid_t divider_pid = create_subprocess("./divider", DIVIDER_PIPE);

    while (1) {
        // Display transaction menu
        printf("1- Addition\n2- Subtraction\n3- Multiplication\n4- Division\n5- Exit\n");
        int choice;
        scanf("%d", &choice);
        if (choice == 5) {
            // Received exit command, cleanup and exit
            int exit_code = 5;
            write(open(ADDER_PIPE, O_WRONLY), &exit_code, sizeof(int));
            write(open(SUBTRACTOR_PIPE, O_WRONLY), &exit_code, sizeof(int));
            write(open(MULTIPLIER_PIPE, O_WRONLY), &exit_code, sizeof(int));
            write(open(DIVIDER_PIPE, O_WRONLY), &exit_code, sizeof(int));

            // Wait for all subprocesses to finish
            waitpid(adder_pid, NULL, 0);
            waitpid(subtractor_pid, NULL, 0);
            waitpid(multiplier_pid, NULL, 0);
            waitpid(divider_pid, NULL, 0);

            break;
        }
        // Get user input, send command and data to the selected subprocess
        int num1, num2;
        printf("Enter two numbers: ");
        scanf("%d %d", &num1, &num2);
        // Open pipes for communication with the selected subprocess
        int pipe_fd;
        switch (choice) {
            case 1:
                pipe_fd = open(ADDER_PIPE, O_WRONLY);
                break;
            case 2:
                pipe_fd = open(SUBTRACTOR_PIPE, O_WRONLY);
                break;
            case 3:
                pipe_fd = open(MULTIPLIER_PIPE, O_WRONLY);
                break;
            case 4:
                pipe_fd = open(DIVIDER_PIPE, O_WRONLY);
                break;
            default:
                fprintf(stderr, "Invalid choice\n");
                continue;
        }
        // Send command (operation code) to the selected subprocess
        write(pipe_fd, &choice, sizeof(int));
        // Send data (numbers) to the selected subprocess
        write(pipe_fd, &num1, sizeof(int));
        write(pipe_fd, &num2, sizeof(int));
        // Close the write end of the pipe
        close(pipe_fd);
        // Open pipes for reading the result from the subprocess
        switch (choice) {
            case 1:
                pipe_fd = open(ADDER_PIPE, O_RDONLY);
                break;
            case 2:
                pipe_fd = open(SUBTRACTOR_PIPE, O_RDONLY);
                break;
            case 3:
                pipe_fd = open(MULTIPLIER_PIPE, O_RDONLY);
                break;
            case 4:
                pipe_fd = open(DIVIDER_PIPE, O_RDONLY);
                break;
            default:
                fprintf(stderr, "Invalid choice\n");
                continue;
        }
        // Read the result from the subprocess
        int result;
        read(pipe_fd, &result, sizeof(int));
        // Print the result
        printf("Result: %d\n", result);
        // Close the read end of the pipe
        close(pipe_fd);
    }

    // Cleanup and close pipes
    unlink(ADDER_PIPE);
    unlink(SUBTRACTOR_PIPE);
    unlink(MULTIPLIER_PIPE);
    unlink(DIVIDER_PIPE);

    return 0;
}
