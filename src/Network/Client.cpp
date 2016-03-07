#include "Client.hpp"
#include "Entities/World.hpp"
#include "Entities/Player.hpp"
#include <QQuickView>
#include <QQmlEngine>
#include <QNetworkInterface>
#include <sstream>
const int MAXLEN = 1 << 10;

ClientWorker::ClientWorker(Client* client)
    : QObject(nullptr), m_client(client), m_udpsocket(this) {
  connect(&m_udpsocket, &QUdpSocket::readyRead, this,
          &ClientWorker::readMessage);

  connect(client, &Client::udpPortChanged, this,
          &ClientWorker::onUdpPortChanged);
  connect(client, &Client::sendMessage, this, &ClientWorker::sendMessage);
  connect(client, &Client::peerAdded, this, &ClientWorker::onPeerAdded);
  connect(client, &Client::peerRemoved, this, &ClientWorker::onPeerRemoved);
  connect(client, &Client::sendToAll, this, &ClientWorker::sendToAll);
}

ClientWorker::~ClientWorker() { sendToAll("DISCONNECT"); }

void ClientWorker::readMessage() {
  static char buffer[MAXLEN] = {};
  QHostAddress address;
  quint16 port;
  m_udpsocket.readDatagram(buffer, MAXLEN, &address, &port);

  qDebug() << "received msg from" << address << port;
  qDebug() << buffer;
  qDebug() << "\n";
  emit gotMessage(buffer, address.toString(), port);
}

void ClientWorker::onUdpPortChanged() {
  m_udpsocket.abort();
  if (m_udpsocket.bind(client()->udpPort()))
    qDebug() << "bound to port" << client()->udpPort();
  else
    qDebug() << "failed to bind" << client()->udpPort();
}

void ClientWorker::sendMessage(QString message, QString ip, quint16 port) {
  m_udpsocket.writeDatagram(message.toLocal8Bit(), QHostAddress(ip), port);
}

void ClientWorker::sendToAll(QString message) {
  for (Address peer : m_peer)
    sendMessage(message, peer.m_address.toString(), peer.m_port);
}

void ClientWorker::onPeerAdded(QString ip, quint16 port) {
  m_peer.insert({QHostAddress(ip), port});
}

void ClientWorker::onPeerRemoved(QString ip, quint16 port) {
  m_peer.erase(m_peer.find({QHostAddress(ip), port}));
}

Client::Client(QQuickItem* parent)
    : QQuickItem(parent), m_world(), m_udpPort(), m_worker(this) {
  m_worker.moveToThread(&m_workerThread);
  m_workerThread.start();

  connect(&m_worker, &ClientWorker::gotMessage, this, &Client::parseMessage);
  connect(this, &Client::worldChanged, this, &Client::onWorldChanged);
}

Client::~Client() {
  m_workerThread.quit();
  m_workerThread.wait();
}

void Client::setWorld(World* world) {
  if (m_world == world) return;
  m_world = world;
  emit worldChanged();
}

void Client::setUdpPort(uint port) {
  if (m_udpPort == port) return;
  m_udpPort = port;
  emit udpPortChanged();
}

void Client::join(QString ip, uint port) {
  std::stringstream message;
  message << "JOIN ";
  message << QHostAddress(ip).toIPv4Address() << " ";
  message << port << " ";

  assert(world());
  assert(world()->player());
  // world()->player()->dump(message, 0, 0);

  emit sendMessage(message.str().c_str(), ip, port);
}

void Client::parseMessage(QString message, QString ip, quint16 port) {
  std::stringstream stream(message.toStdString());
  std::string operationType;
  stream >> operationType;

  std::string rest = stream.str();
  rest = std::string(rest.begin() + stream.tellg() + 1, rest.end());

  QString name(operationType.c_str());

  if (name.contains("JOIN"))
    joinMessageReceived(rest.c_str(), ip, port);
  else if (name.contains("POSITION"))
    playerPositionMessageReceived(rest.c_str(), ip, port);
  else if (name.contains("NEWPLAYER"))
    newPlayerMessageReceived(rest.c_str());
  else if (name.contains("DISCONNECT"))
    disconnectMessageReceived(ip, port);
}

void Client::onWorldChanged() {
  if (world()) {
    onPlayerChanged();
    // connect(world(), &World::playerChanged, this, &Client::onPlayerChanged);
  }
}

void Client::onPlayerChanged() {
  /*if (world()->player()) {
      connect(world()->player(), &Player::xChanged,
              this, &Client::onPlayerPositionChanged);
      connect(world()->player(), &Player::yChanged,
              this, &Client::onPlayerPositionChanged);
  }*/
}

void Client::onPlayerPositionChanged() {
  std::stringstream message;
  // message << "POSITION " << world()->player()->x() << " " <<
  // world()->player()->y();

  emit sendToAll(message.str().c_str());
}

Player* Client::loadPlayer(QString) {
  /*QQmlComponent* component = new QQmlComponent(qmlEngine(this), this);
  component->setData(data.toLocal8Bit(), QUrl("qrc:/qml/Entities/"));

  QObject* object = component->beginCreate(QQmlEngine::contextForObject(this));
  object->setParent(world());
  component->completeCreate();

  Player* player = static_cast<Player*>(object);
  player->setParentItem(world());
  player->setBodyType(QBody::Static);

  return player;*/
  return 0;
}

void Client::sendNewPlayerInfo(Address address, Player* /*player*/,
                               QHostAddress ip, quint16 port) {
  std::stringstream stream;
  stream << "NEWPLAYER ";
  stream << address.m_address.toIPv4Address() << " ";
  stream << address.m_port << " ";
  // player->dump(stream, 0, 0);

  emit sendMessage(stream.str().c_str(), ip.toString(), port);
}

void Client::joinMessageReceived(QString message, QString ip, quint16 port) {
  for (auto it = m_player.begin(); it != m_player.end(); it++)
    sendNewPlayerInfo(it->first, it->second, QHostAddress(ip), port);

  std::stringstream msgstream(message.toLocal8Bit().constData());
  uint myAddress, myPort;
  msgstream >> myAddress >> myPort;
  sendNewPlayerInfo({QHostAddress(myAddress), quint16(myPort)},
                    world()->player(), QHostAddress(ip), port);

  std::string playerDescription = msgstream.str();
  playerDescription =
      std::string(playerDescription.begin() + msgstream.tellg() + 1,
                  playerDescription.end());

  std::stringstream stream;
  stream << "NEWPLAYER ";
  stream << QHostAddress(ip).toIPv4Address() << " ";
  stream << port << " ";

  emit sendToAll((stream.str().c_str() + playerDescription).c_str());
  emit peerAdded(ip, port);

  qDebug() << "joined player referring to" << ip << port;

  Address address = {QHostAddress(ip), port};
  delete m_player[address];
  m_player[address] = loadPlayer(playerDescription.c_str());
}

void Client::newPlayerMessageReceived(QString message) {
  std::stringstream stream(message.toLocal8Bit().constData());
  uint ip;
  quint16 port;
  stream >> ip >> port;

  std::string data = stream.str();
  data = std::string(data.begin() + stream.tellg() + 1, data.end());

  emit peerAdded(QHostAddress(ip).toString(), port);

  qDebug() << "created player refering to " << QHostAddress(ip) << port;

  Address address = {QHostAddress(ip), port};
  delete m_player[address];
  m_player[address] = loadPlayer(data.c_str());
}

void Client::playerPositionMessageReceived(QString message, QString ip,
                                           quint16 port) {
  std::stringstream stream(message.toLocal8Bit().constData());
  qreal x, y;
  stream >> x >> y;

  Player* player = m_player[{QHostAddress(ip), port}];
  player->setTransform(QPointF(x, y), player->rotation());
}

void Client::disconnectMessageReceived(QString ip, quint16 port) {
  qDebug() << "bye: " << ip << port;
  auto it = m_player.find({QHostAddress(ip), port});
  if (it != m_player.end()) {
    delete it->second;
    m_player.erase(it);

    emit peerRemoved(ip, port);
  }
}
