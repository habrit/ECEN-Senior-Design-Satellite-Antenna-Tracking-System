#include "Functions.cpp"


int main() {
    // Open the serial port
    int serialPort = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY);

    if (serialPort == -1) {
        std::cerr << "Failed to open serial port" << std::endl;
        return 1;
    }

    // Configure the serial port
    struct termios serialConfig;
    tcgetattr(serialPort, &serialConfig);
    cfsetispeed(&serialConfig, B9600);  // Set baud rate to 9600
    cfsetospeed(&serialConfig, B9600);
    serialConfig.c_cflag &= ~PARENB;    // Disable parity bit
    serialConfig.c_cflag &= ~CSTOPB;    // Set one stop bit
    serialConfig.c_cflag &= ~CSIZE;     // Clear data size bits
    serialConfig.c_cflag |= CS8;        // Set data size to 8 bits
    tcsetattr(serialPort, TCSANOW, &serialConfig);

    // Set the hex values
    unsigned char hexValues[5] = {0x43, 0x97, 0x00, 0x00, 0x01};

    // Send the hex values
    ssize_t bytesWritten = write(serialPort, hexValues, sizeof(hexValues));
    if (bytesWritten == -1) {
        std::cerr << "Failed to write to serial port" << std::endl;
        close(serialPort);
        return 1;
    }

    // Display any responses it sends back
    char response[100];
    ssize_t bytesRead = read(serialPort, response, sizeof(response));
    if (bytesRead == -1) {
        std::cerr << "Failed to read from serial port" << std::endl;
        close(serialPort);
        return 1;
    }
    response[bytesRead] = '\0';
    std::cout << response << std::endl;

    // Close the serial port
    close(serialPort);

    return 0;
}
