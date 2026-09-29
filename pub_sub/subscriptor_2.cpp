
#include <zmq.h>
#include <cstring>
#include <string>
#include <iostream>
#include <thread>
#include <mutex>

std::mutex candado;
std::string topico, valor;   // el dato compartido: siempre el último recibido

void hiloSuscriptor()
{
    void *contexto = zmq_ctx_new();
    void *sub = zmq_socket(contexto, ZMQ_SUB);

    zmq_connect(sub, "tcp://localhost:5556");
    zmq_connect(sub, "tcp://localhost:5557");
    zmq_connect(sub, "tcp://localhost:5558");

    zmq_setsockopt(sub, ZMQ_SUBSCRIBE, "velocidad", 9);
    zmq_setsockopt(sub, ZMQ_SUBSCRIBE, "posicion", 8);

    char buffer[256];
    while (true)
    {
        int n = zmq_recv(sub, buffer, sizeof(buffer) - 1, 0);
        if (n < 0) continue;
        buffer[n] = '\0';

        std::string msg(buffer);
        auto pos = msg.find(' ');
        std::string t = msg.substr(0, pos);
        std::string v = msg.substr(pos + 1);

        candado.lock();
        topico = t;
        valor  = v;
        candado.unlock();
    }
}

int main()
{
    std::thread hilo(hiloSuscriptor);

    while (true)
    {
        // --- el main hace sus cosas ---
        // ...

        // --- en algún punto necesita el dato actual ---
        candado.lock();
        std::string t = topico;
        std::string v = valor;
        candado.unlock();

        // --- usa t y v como quiera ---
        std::cout << "[" << t << "] valor: " << v << std::endl;
    }

    hilo.join();
}
