#ifndef SERVER_HPP
#define SERVER_HPP
#include <QQuickItem>
#include <QUdpSocket>

class Server : public QQuickItem {
 private:
  Q_OBJECT

  QUdpSocket m_socket;

  void readData();

 protected:
 public:
  Server(QQuickItem* = nullptr);
};

#endif  // SERVER_HPP
