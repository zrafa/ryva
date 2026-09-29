// Publicador B: publica velocidad a 2 Hz (ZeroMQ PUB en el puerto 5557)
#include <zmq.h>
#include <chrono>
#include <thread>
#include <sstream>
#include <iomanip>
#include <iostream>

int main()
{
    void *contexto = zmq_ctx_new();
    void *publicador = zmq_socket(contexto, ZMQ_PUB);
    zmq_bind(publicador, "tcp://*:5557");

    float valor = 100.0f;
    while (true)
    {
        std::ostringstream oss;
        oss << "velocidad " << std::fixed << std::setprecision(2) << valor;
        std::string mensaje = oss.str();

        zmq_send(publicador, mensaje.c_str(), mensaje.size(), 0);
        std::cout << "B publico -> " << mensaje << std::endl;

        valor += 1.0f;
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // 2 Hz
    }
}
