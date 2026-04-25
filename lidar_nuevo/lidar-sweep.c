

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>

// ---------------- SERIAL ----------------

int configurar_serial(int fd) {
    struct termios tty;

    tcgetattr(fd, &tty);

    cfsetospeed(&tty, B115200);
    cfsetispeed(&tty, B115200);

    tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;

    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;

    tty.c_lflag = 0;
    tty.c_iflag = 0;
    tty.c_oflag = 0;

    tty.c_cc[VMIN]  = 1;
    tty.c_cc[VTIME] = 1;

    tcsetattr(fd, TCSANOW, &tty);
    return 0;
}






// Verifica checksum
int paquete_valido(uint8_t *buf) {
    uint32_t sum = 0;

    for (int i = 0; i < 6; i++)
        sum += buf[i];

    return ((sum % 255) == buf[6]);
}

// Lee hasta encontrar un paquete válido
int leer_paquete_valido(int fd, uint8_t *buf) {
    int count = 0;

    while (1) {
        uint8_t byte;
        int n = read(fd, &byte, 1);

        if (n <= 0)
            continue;

        // Llenar buffer inicial
        if (count < 7) {
            buf[count++] = byte;
        } else {
            // Shift a la izquierda
            for (int i = 0; i < 6; i++)
                buf[i] = buf[i + 1];

            buf[6] = byte;
        }

        // Cuando tenemos 7 bytes, validar
        if (count == 7) {
            if (paquete_valido(buf)) {
                return 1; // paquete válido encontrado
            }
        }
    }
}
// ---------------- MAIN ----------------


int main() {
    int fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    configurar_serial(fd);

    char cmd2[] = "LR03\r\n";
    write(fd, cmd2, strlen(cmd2));
    printf("Comando LR enviado\n");
    char buf2[9];
    read(fd, buf2, 9);
    sleep(2);

    char cmd3[] = "MS02\r\n";
    write(fd, cmd3, strlen(cmd3));
    printf("Comando MS enviado. Esperamos 10 segundos...\n");
    read(fd, buf2, 9);
    sleep(10);

    // 🔴 Comando correcto: CR + LF
    char cmd[] = "DS\r\n";

    write(fd, cmd, strlen(cmd));
    printf("Comando DS enviado. Esperando 2 segundos...\n");
    sleep(2);

    strncpy(cmd, "DX\r\n", 5);
    write(fd, cmd, strlen(cmd));
    printf("Comando DX enviado. Esperando 2 segundos...\n");
    sleep(2);

    strncpy(cmd, "DS\r\n", 5);
    write(fd, cmd, strlen(cmd));
    printf("Comando DS enviado. Esperando 2 segundos...\n");
    sleep(2);

    // -------- HEADER (7 bytes) --------
    uint8_t header[7];
    int total = 0;

    read(fd, header, 7);

    printf("Header: ");
    for (int i = 0; i < 7; i++) {
        printf("%02X ", header[i]);
    }
    printf("\n");

    // -------- DATA LOOP --------
    uint8_t buf[7];

    float prom = 0;
    int n = 0;
    int falta_imprimir = 0;
    while (1) {
        int total = 0;

	leer_paquete_valido(fd, buf);

    	// read(fd, buf, 7);
        // -------- Parseo --------

        uint8_t sync = buf[0];

        //uint16_t az_raw = (buf[1] << 8) | buf[2];
        uint16_t az_raw = (buf[2] << 8) | buf[1];

        //uint16_t dist = (buf[3] << 8) | buf[4];
        uint16_t dist = (buf[4] << 8) | buf[3];
        uint8_t signal = buf[5];
        uint8_t checksum = buf[6];

        // Convertir azimuth a grados (según Sweep: valor / 16.0)
        float angle = az_raw / 16.0f;
   	// angle is transmitted as fixed point integer with scaling factor of 16
    	//float angulo = (az_raw * 1000 / 16.0f);
	float angulo = 1.0f * ((float)(az_raw >> 4) + ((az_raw & 15) / 16.0f));

        // -------- Validar checksum --------
        uint32_t sum = 0;
        for (int i = 0; i < 6; i++) sum += buf[i];
        uint8_t calc_checksum = sum % 255;

        if (calc_checksum != checksum) {
            printf("Checksum ERROR\n");
            continue;
        }

        // -------- Mostrar --------
	//printf("Angle: %.2f deg | Distance: %d cm | Signal: %d | Sync: %d\n",
        //      angulo, dist, signal, (sync & 0x01));
        //	fflush(stdout);
	//
	// if ((angulo < 11) || (angulo > 350)) {
	if ((angulo < 3) || (angulo > 358)) {
        	printf("Angle: %.2f deg | Distance: %d cm \n",
               angulo, dist);
		prom += dist;
		n++;

        	fflush(stdout);
		falta_imprimir = 1;
	} else if (falta_imprimir) {
        	printf("Promedio: %.2f cm \n", prom/n);
        	fflush(stdout);
		prom = 0;
		n = 0;
		falta_imprimir = 0;
	}
    }

    close(fd);
    return 0;
}

