#include "Networking.h"

#include <asio.hpp>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <thread>
#include <utility>
using asio::ip::tcp;

NetworkMessages::NetworkMessages() : arr(std::vector<Message>()), m{} {}

void NetworkMessages::add(int id, std::string message) {
  m.lock();
  arr.push_back(Message{id, message});
  m.unlock();
}

std::vector<Message> NetworkMessages::getAll() {
  m.lock();

  std::vector<Message> newVector;
  newVector = arr;
  arr = std::vector<Message>();

  m.unlock();
  return newVector;
}

int Session::getNextSessionId() {
  // Note: Can also be implemented as a pool of numbers to give out so that only
  // [0, 3] will appear in sessionId, which allows for easier player assignment.
  for (int i = 0; i < 4; i++) {
    if (Networking::allSessions[i] == NULL) {
      return i;
    }
  }
  // static int nextSessionId = 0;  // Static variables only initialize once.
  // return nextSessionId++;
  return 0;
}

Session::Session(tcp::socket socket, NetworkMessages* dataPtr)
    : socket_(std::move(socket)),
      dataStorage(dataPtr),
      sessionId(getNextSessionId()) {
  asio::ip::tcp::no_delay option(true);
  socket_.set_option(option);
  Networking::allSessions[this->sessionId] = this;
}

void Session::start() { read(); }

void Session::read() {
  auto self(shared_from_this());
  socket_.async_read_some(
      asio::buffer(read_data_, max_length),
      [this, self](asio::error_code ec, std::size_t length) {
        if (!ec) {
          this->dataStorage->add(this->sessionId,
                                 std::string(this->read_data_));

          read();
        } else {
          std::cerr << "Error: " << ec.message() << std::endl;
          Networking::allSessions[this->sessionId] = NULL;
        }
      });
}

void Session::send(std::string msg) {
  if (this == NULL) {
    // Check if the session has disconnected or not.
    return;
  }
  auto self(shared_from_this());
  socket_.async_write_some(
      asio::buffer(msg, msg.length()),
      [this, self](asio::error_code ec, std::size_t /*length*/) {
        if (!ec) {
        } else {
          std::cerr << "Error: " << ec.message() << std::endl;
        }
      });
}

Server::Server(asio::io_service& io_service, short port,
               NetworkMessages* dataPtr)
    : acceptor_(io_service, tcp::endpoint(tcp::v4(), port)),
      socket_(io_service),
      dataStorage(dataPtr) {
  do_accept();
}

void Server::do_accept() {
  acceptor_.async_accept(socket_, [this](asio::error_code ec) {
    if (!ec) {
      std::make_shared<Session>(std::move(socket_), dataStorage)->start();
    }
    do_accept();
  });
}

std::map<int, Session*> Networking::allSessions;
std::thread Networking::serverThread;
NetworkMessages* Networking::recvData = new NetworkMessages();
void Networking::initServerNetworking(short port) {
  // Define a Lambda Expression
  auto startServerThreadFunc = [](short port, NetworkMessages* dataPtr) {
    try {
      asio::io_service ioService;

      Server s{ioService, port, dataPtr};

      ioService.run();
    } catch (std::exception& e) {
      std::cerr << "Exception: " << e.what() << "\n";
    }
  };
  Networking::serverThread = std::thread(startServerThreadFunc, port, recvData);
}