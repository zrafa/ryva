// Subscriptor: escucha velocidad y posicion, imprime topico y valor de cada mensaje
#include <zmq.h>
#include <cstring>
#include <string>
#include <iostream>

int main()
{
    void *contexto = zmq_ctx_new();
    void *subscriptor = zmq_socket(contexto, ZMQ_SUB);

    // Se conecta a los 3 publicadores
    zmq_connect(subscriptor, "tcp://localhost:5556"); // A
    zmq_connect(subscriptor, "tcp://localhost:5557"); // B
    zmq_connect(subscriptor, "tcp://localhost:5558"); // C

    // Subscripcion a los dos topicos (por prefijo del mensaje)
    zmq_setsockopt(subscriptor, ZMQ_SUBSCRIBE, "velocidad", strlen("velocidad"));
    zmq_setsockopt(subscriptor, ZMQ_SUBSCRIBE, "posicion", strlen("posicion"));

    char buffer[256];
    while (true)
    {
        int nbytes = zmq_recv(subscriptor, buffer, sizeof(buffer) - 1, 0);
        if (nbytes < 0) continue;
        buffer[nbytes] = '\0';

        std::string mensaje(buffer);
        auto espacio = mensaje.find(' ');
        std::string topico = mensaje.substr(0, espacio);
        std::string valor = mensaje.substr(espacio + 1);

        std::cout << "[" << topico << "] valor: " << valor << std::endl;
    }
}
