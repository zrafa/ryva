// Publicador C: publica posicion x,y a 1 Hz (ZeroMQ PUB en el puerto 5558)
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
    zmq_bind(publicador, "tcp://*:5558");

    float x = 0.0f, y = 0.0f;
    while (true)
    {
        std::ostringstream oss;
        oss << "posicion " << std::fixed << std::setprecision(2) << x << "," << y;
        std::string mensaje = oss.str();

        zmq_send(publicador, mensaje.c_str(), mensaje.size(), 0);
        std::cout << "C publico -> " << mensaje << std::endl;

        x += 1.0f;
        y += 2.0f;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // 1 Hz
    }
}
