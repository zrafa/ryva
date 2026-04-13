
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <signal.h>
#include <sys/time.h>

#define BAUD B115200

int fd_global = -1;

// Timestamp en microsegundos
long long timestamp_us() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (long long)tv.tv_sec * 1000000LL + tv.tv_usec;
}

// Handler Ctrl+C
void handle_sigint(int sig) {
    if (fd_global != -1) {
        close(fd_global);
    }
    printf("\nCerrado.\n");
    exit(0);
}

// Configurar puerto serial
int configurar_serial(int fd) {
    struct termios tty;

    if (tcgetattr(fd, &tty) != 0) {
        perror("tcgetattr");
        return -1;
    }

    cfsetispeed(&tty, BAUD);
    cfsetospeed(&tty, BAUD);

    tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;
    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;

    // Modo raw limpio
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_iflag &= ~(INLCR | ICRNL | IGNCR);
    tty.c_lflag = 0;
    tty.c_oflag = 0;

    tty.c_cc[VMIN]  = 1;
    tty.c_cc[VTIME] = 1; // 100 ms timeout

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        perror("tcsetattr");
        return -1;
    }

    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s /dev/ttyUSB0\n", argv[0]);
        return 1;
    }

    signal(SIGINT, handle_sigint);

    int fd = open(argv[1], O_RDONLY | O_NOCTTY);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    fd_global = fd;

    if (configurar_serial(fd) != 0) {
        close(fd);
        return 1;
    }

    uint8_t b;
    uint8_t frame[9];

    while (1) {
        // Buscar header 0x59 0x59
        if (read(fd, &b, 1) <= 0) continue;
        if (b != 0x59) continue;

        if (read(fd, &b, 1) <= 0) continue;
        if (b != 0x59) continue;

        frame[0] = 0x59;
        frame[1] = 0x59;

        // Leer resto del frame
        int read_bytes = 0;
        while (read_bytes < 7) {
            int n = read(fd, frame + 2 + read_bytes, 7 - read_bytes);
            if (n > 0) read_bytes += n;
        }

        uint16_t distancia = frame[2] | (frame[3] << 8);
        long long ts = timestamp_us();

        printf("%lld\t%u\n", ts, distancia);
        fflush(stdout);
    }

    return 0;
}
