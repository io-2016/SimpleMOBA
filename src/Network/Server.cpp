#include "Server.hpp"
const int MAXLEN = 128;

Server::Server(QQuickItem* parent) : QQuickItem(parent) {
  m_socket.bind();

  connect(&m_socket, &QUdpSocket::readyRead, this, &Server::readData);
  qDebug() << m_socket.localAddress() << m_socket.localPort();
}

void Server::readData() {
  qDebug() << "got some data";

  char buffer[MAXLEN];
  QHostAddress address;
  quint16 port;
  m_socket.readDatagram(buffer, MAXLEN, &address, &port);

  qDebug() << "received from" << address << ":" << port;
  qDebug() << buffer;
}
