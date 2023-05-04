#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/select.h>

int main() {
    const char* devicePath = "/dev/ttyUSB0";  // Replace with the appropriate TTY device path
    int baudRate = B115200;  // Replace with the appropriate baud rate

    // Open the TTY device
    int ttyFD = open(devicePath, O_RDWR);
    if (ttyFD == -1) {
        perror("Error opening TTY device");
        return 1;
    }

    // Configure the TTY device
    struct termios ttyConfig;
    if (tcgetattr(ttyFD, &ttyConfig) != 0) {
        perror("Error getting TTY device attributes");
        close(ttyFD);
        return 1;
    }
    cfsetospeed(&ttyConfig, baudRate);
    cfsetispeed(&ttyConfig, baudRate);
    ttyConfig.c_cflag |= (CLOCAL | CREAD);
    ttyConfig.c_cflag &= ~PARENB;
    ttyConfig.c_cflag &= ~CSTOPB;
    ttyConfig.c_cflag &= ~CSIZE;
    ttyConfig.c_cflag |= CS8;
    ttyConfig.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    ttyConfig.c_oflag &= ~OPOST;
    ttyConfig.c_cc[VMIN] = 0;
    ttyConfig.c_cc[VTIME] = 10;
    if (tcsetattr(ttyFD, TCSANOW, &ttyConfig) != 0) {
        perror("Error setting TTY device attributes");
        close(ttyFD);
        return 1;
    }

    // Prepare the file descriptor set for select()
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    FD_SET(ttyFD, &fds);

    // Read from and write to the TTY device
    char buffer[255];
    ssize_t numBytes;
    while (1) {
        // Wait for activity on either stdin or ttyFD
        int maxFD = (STDIN_FILENO > ttyFD) ? STDIN_FILENO : ttyFD;
        int activity = select(maxFD + 1, &fds, NULL, NULL, NULL);
        if (activity < 0) {
            perror("Error in select");
            break;
        }

        // Check for activity on stdin
        if (FD_ISSET(STDIN_FILENO, &fds)) {
            // Read from stdin
            if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                perror("Error reading from stdin");
                break;
            }

            // Write to TTY device
            ssize_t numBytesWritten = write(ttyFD, buffer, strlen(buffer));
            if (numBytesWritten < 0) {
                perror("Error writing to TTY device");
                break;
            }
        }

        // Check for activity on ttyFD
        if (FD_ISSET(ttyFD, &fds)) {
            // Read from TTY device
            numBytes = read(ttyFD, buffer, sizeof(buffer));
            if (numBytes > 0) {
                printf("Received: %.*s", (int)numBytes, buffer);
            }
        }
    }

    // Close the TTY device
    close(ttyFD);

    return 0;
}