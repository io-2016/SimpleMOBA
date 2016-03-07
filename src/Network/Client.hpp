#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <QQuickItem>
#include <QUdpSocket>
#include <QTcpSocket>
#include <QThread>
#include <map>
#include <set>

class World;
class Client;
class Player;

struct Address {
  QHostAddress m_address;
  quint16 m_port;

  inline bool operator<(const Address& a) const {
    if (m_address == a.m_address)
      return m_address.toIPv4Address() < a.m_address.toIPv4Address();
    return m_port < a.m_port;
  }
};

class ClientWorker : public QObject {
 private:
  Q_OBJECT

  Client* m_client;
  QUdpSocket m_udpsocket;
  QTcpSocket m_tcpsocket;
  std::set<Address> m_peer;

  void readMessage();
  void onUdpPortChanged();
  void sendMessage(QString message, QString ip, quint16 port);
  void sendToAll(QString message);
  void onPeerAdded(QString, quint16);
  void onPeerRemoved(QString, quint16);

 protected:
 public:
  ClientWorker(Client*);
  ~ClientWorker();

  inline Client* client() { return m_client; }

 signals:
  void gotMessage(QString message, QString ip, quint16 port);
};

class Client : public QQuickItem {
 private:
  Q_OBJECT

  Q_PROPERTY(World* world READ world WRITE setWorld NOTIFY worldChanged)
  Q_PROPERTY(uint udpPort READ udpPort WRITE setUdpPort NOTIFY udpPortChanged)

  World* m_world;
  uint m_udpPort;

  ClientWorker m_worker;
  QThread m_workerThread;
  std::map<Address, Player*> m_player;

  void onWorldChanged();
  void onPlayerChanged();
  void onPlayerPositionChanged();

  Player* loadPlayer(QString data);

  void sendNewPlayerInfo(Address, Player*, QHostAddress ip, quint16 port);

  void parseMessage(QString, QString, quint16);
  void joinMessageReceived(QString message, QString ip, quint16 port);
  void newPlayerMessageReceived(QString message);
  void playerPositionMessageReceived(QString message, QString ip, quint16 port);
  void disconnectMessageReceived(QString ip, quint16 port);

 protected:
 public:
  Client(QQuickItem* = nullptr);
  ~Client();

  inline World* world() const { return m_world; }
  void setWorld(World*);

  inline uint udpPort() const { return m_udpPort; }
  void setUdpPort(uint);

  Q_INVOKABLE void join(QString ip, uint port);

 signals:
  void worldChanged();
  void udpPortChanged();
  void sendMessage(QString message, QString ip, uint port);
  void peerAdded(QString ip, quint16 port);
  void peerRemoved(QString ip, quint16 port);
  void sendToAll(QString message);
};

#endif  // CLIENT_HPP
