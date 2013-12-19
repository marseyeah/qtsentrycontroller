#include "ev3.h"

#include <QDebug>

#define UNLOCK_MESSAGE "GET /target?sn=%1 VMTP1.0\nProtocol: EV3"

Ev3::Ev3()
    : QObject()
    , m_connectionState(Disconnected)
{
    m_broadcast = new QUdpSocket;
    QObject::connect(m_broadcast, SIGNAL(readyRead()), this, SLOT(processBroadcastDatagram()));

    m_connection = new QTcpSocket;
    QObject::connect(m_connection, &QTcpSocket::stateChanged, this, &Ev3::updateState);
    QObject::connect(m_connection, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(updateError(QAbstractSocket::SocketError)));
}

Ev3::~Ev3()
{
    disconnect();
    m_connection->deleteLater();
    m_broadcast->deleteLater();
}

void Ev3::searchAndConnect()
{
    if (m_connectionState > Disconnected)
        return;

    setState(Searching);

    m_serialNumber.clear();
    m_broadcast->bind(3015);
}

void Ev3::processBroadcastDatagram()
{
    clearError();

    QHostAddress address;
    quint16 udpPort = -1;

    while (m_broadcast->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(m_broadcast->pendingDatagramSize());
        m_broadcast->readDatagram(datagram.data(), datagram.size(), &address, &udpPort);
        qDebug() << "Broadcast from" << address.toString() << "on port" << udpPort;
        m_serialNumber = datagram.mid(15, 12);
    }

    // Send handshake
    m_broadcast->writeDatagram(QByteArray("1"), address, udpPort);
    m_broadcast->close();

    // Establish connection
    setState(Connecting);
    qDebug() << "Connecting to" << address.toString() << "(SN=" << m_serialNumber << ") on port 5555";
    m_connection->connectToHost(address, 5555);
}

bool Ev3::unlockEv3()
{
    qDebug() << "Sending unlock message";
    m_connection->write(QString(QLatin1String(UNLOCK_MESSAGE)).arg(m_serialNumber).toLatin1());
    if (m_connection->waitForReadyRead(1000) && m_connection->read(16).startsWith("Accept")) {
        qDebug() << "Connection confirmed";
       return true;
    } else {
        qDebug() << "Unlock failed";
        return false;
    }
}

void Ev3::disconnect()
{
    m_serialNumber.clear();

    if (m_connectionState == Searching) {
        m_broadcast->close();
        setState(Disconnected);
        return;
    }

    if (m_connectionState != Connected)
        return;

    m_connection->disconnectFromHost();
}

Ev3::ConnectionState Ev3::connectionState() const
{
    return m_connectionState;
}

void Ev3::setState(ConnectionState state)
{
    if (m_connectionState == state)
        return;

    if (state == Connected) {
        if (!unlockEv3()) {
            m_connectionState = Disconnected;
            emit connectionStateChanged();
            m_error = QStringLiteral("Could not unlock EV3.");
            emit errorChanged();
            return;
        }
    }

    m_connectionState = state;
    emit connectionStateChanged();
}

void Ev3::updateState(QAbstractSocket::SocketState state)
{
    switch (state) {
    case QAbstractSocket::UnconnectedState:
        setState(Disconnected);
        break;
    case QAbstractSocket::HostLookupState:
    case QAbstractSocket::ConnectingState:
        setState(Connecting);
        break;
    case QAbstractSocket::ConnectedState:
    case QAbstractSocket::BoundState:
        setState(Connected);
        clearError();
        break;
    case QAbstractSocket::ClosingState:
        setState(Disconnecting);
        break;
    default:
        setState(Unknown);
    }
}

void Ev3::updateError(QAbstractSocket::SocketError error)
{
    switch (error) {
    case QAbstractSocket::ConnectionRefusedError:
        m_error = QStringLiteral("Connection refused.");
        break;
    case QAbstractSocket::RemoteHostClosedError:
        m_error = QStringLiteral("Ev3 closed the connection.");
        break;
    case QAbstractSocket::HostNotFoundError:
        m_error = QStringLiteral("Host not found.");
        break;
    case QAbstractSocket::SocketAccessError:
        m_error = QStringLiteral("Socket access error.");
        break;
    case QAbstractSocket::SocketResourceError:
        m_error = QStringLiteral("Socket resource error.");
        break;
    case QAbstractSocket::SocketTimeoutError:
        m_error = QStringLiteral("Socket operation timed out.");
        break;
    case QAbstractSocket::DatagramTooLargeError:
        m_error = QStringLiteral("Datagram too large.");
        break;
    case QAbstractSocket::NetworkError:
        m_error = QStringLiteral("Network error.");
        break;
    case QAbstractSocket::AddressInUseError:
        m_error = QStringLiteral("Address already in use.");
        break;
    case QAbstractSocket::SocketAddressNotAvailableError:
        m_error = QStringLiteral("Address not available.");
        break;
    case QAbstractSocket::UnsupportedSocketOperationError:
        m_error = QStringLiteral("Unsupported socket operation.");
        break;
    case QAbstractSocket::UnfinishedSocketOperationError:
        m_error = QStringLiteral("Unfinished socket operation.");
        break;
    case QAbstractSocket::OperationError:
        m_error = QStringLiteral("Operation error.");
        break;
    case QAbstractSocket::TemporaryError:
        m_error = QStringLiteral("Temporary error.");
        break;
    case QAbstractSocket::UnknownSocketError:
        m_error = QStringLiteral("Unknown error.");
        break;
    default:
        return;
    }

    emit errorChanged();
}

void Ev3::clearError()
{
    if (m_error.isEmpty())
        return;

    m_error.clear();
    emit errorChanged();
}
