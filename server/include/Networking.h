#include <asio.hpp>
#include <cstdlib>
#include <iostream>
#include <map>
#include <memory>
#include <utility>

#ifndef __NETWORKING__
#define __NETWORKING__
using asio::ip::tcp;

struct Message {
  int sessionId;
  std::string msg;
};

class NetworkMessages {
 public:
  NetworkMessages();
  void add(int id, std::string message);
  std::vector<Message> getAll();

 private:
  std::mutex m;
  std::vector<Message> arr;
};

class Session : public std::enable_shared_from_this<Session> {
 public:
  Session(tcp::socket, NetworkMessages*);
  void start();
  // Write to the client.
  void send(char*, int);
  bool ready = false;

 private:
  // Keeps reading messages. The lambda function it creates will call do_read to
  // trigger another read.
  void read();

  tcp::socket socket_;
  enum { max_length = 1024 };
  char read_data_[max_length];
  int sessionId;
  NetworkMessages* dataStorage;

  static int getNextSessionId();
};

class Server {
 public:
  Server(asio::io_service&, short, NetworkMessages*);

 private:
  // Accept a connection request and create a new session.
  void do_accept();

  tcp::acceptor acceptor_;
  tcp::socket socket_;
  // The storage for all sessions to store incoming messages.
  NetworkMessages* dataStorage;
};

class Networking {
 public:
  static NetworkMessages* recvData;
  // std::thread requires the created thread to not to be out of scope to
  // continue running. Returning the std::thread does not work either.
  static std::thread serverThread;

  static std::map<int, Session*> allSessions;

  // initServerNetworking
  // This function will create an async server that can establish a TCP
  // connection with clients and maintains them. It will collect the inputs and
  // put into a shared vector. short port: The port number.
  static void initServerNetworking(short);
};
#endif