#ifndef EV3_H
#define EV3_H

#include <QObject>
#include <QTcpSocket>
#include <QUdpSocket>

#define EV3_UDP_PORT 3015
#define EV3_TCP_PORT 5555

class Ev3 : public QObject
{
    Q_OBJECT
    Q_ENUMS(ConnectionState)
    Q_PROPERTY(ConnectionState state READ connectionState NOTIFY connectionStateChanged)
    Q_PROPERTY(QString error READ error NOTIFY errorChanged)
public:
    enum ConnectionState {
        Unknown,
        Disconnected,
        Disconnecting,
        Searching,
        Connected,
        Connecting
    };

    Ev3();
    ~Ev3();

    ConnectionState connectionState() const;
    QString error() const { return m_error; }

Q_SIGNALS:
    void connectionStateChanged();
    void errorChanged();

public Q_SLOTS:
    void searchAndConnect();
    void disconnect();

private Q_SLOTS:
    void updateState(QAbstractSocket::SocketState state);
    void updateError(QAbstractSocket::SocketError error);
    void processBroadcastDatagram();

private:
    bool unlockEv3();
    void clearError();
    void setState(ConnectionState state);

    QString m_serialNumber;

    ConnectionState m_connectionState;
    QString m_error;

    QUdpSocket *m_broadcast;
    QTcpSocket *m_connection;

};

#endif // EV3_H
