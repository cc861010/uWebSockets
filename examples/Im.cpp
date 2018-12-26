#include "App.h"
#include <list>
#include <iostream>

std::list<uWS::WebSocket<false, true> *> clients;

int main() {

    /* ws->getUserData returns one of these */
    struct PerSocketData {};

    /* Very simple WebSocket echo server */
    uWS::App().ws<PerSocketData>("/*", {
        /* Settings */
        .compression = uWS::SHARED_COMPRESSOR,
        .maxPayloadLength = 16 * 1024,
        /* Handlers */
        .open = [](auto *ws, auto *req) {
            clients.push_front(ws);
            std::cout << "new client, push front";
        },
        .message = [](auto *ws, std::string_view message, uWS::OpCode opCode) {
            for (auto it = clients.begin(); it != clients.end(); it++){
                (*it)->send(message, opCode);
            }
            std::cout << "message:" << message <<"\n";
        },
        .drain = [](auto *ws) {
            /* Check getBufferedAmount here */
            std::cout << "drain" <<"\n";
        },
        .ping = [](auto *ws) {
            std::cout << "ping" <<"\n";
        },
        .pong = [](auto *ws) {
            std::cout << "pong" <<"\n";
        },
        .close = [](auto *ws, int code, std::string_view message) {
            std::cout << "close, and remove closed client" << std::endl;
            clients.remove(ws);
        }
    }).listen(9001, [](auto *token) {
        if (token) {
            std::cout << "Listening on port " << 9001 << std::endl;
        }
    }).run();
}
