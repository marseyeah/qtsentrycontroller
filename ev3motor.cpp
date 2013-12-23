#include "ev3motor.h"

#include "ev3.h"
#include "ev3command.h"

Ev3Motor::Ev3Motor(Ev3 *ev3, Port port)
    : QObject()
    , m_ev3(ev3)
    , m_port(port)
    , m_power(0)
{
}

Ev3Motor::~Ev3Motor()
{
    setPower(0);
}

void Ev3Motor::setPower(int power)
{
    power = qBound(-100, power, 100);

    if (m_power == power)
        return;

    setPolarity(-1);

//    Ev3Command cmd;
//    cmd.addOpCode(opOUTPUT_POWER);
//    cmd.addParameter(qint8(0x00));
//    cmd.addParameter(qint8(m_port));
//    cmd.addParameter(qint8(power));
//    m_ev3->sendCommand(cmd.data());

    char command[7];
    command[0] = opOUTPUT_POWER;
    command[1] = 0x81;
    command[2] = 0x00;
    command[3] = 0x81;
    command[4] = m_port;
    command[5] = 0x81;
    command[6] = power;
    m_ev3->sendCommand(QByteArray::fromRawData(command, 7));

    if (power == 0)
        stop();
    else if (m_power == 0)
        start();

    m_power = power;
    emit powerChanged();
}

void Ev3Motor::start()
{
//    Ev3Command cmd;
//    cmd.addOpCode(opOUTPUT_START);
//    cmd.addParameter(qint8(0x00));
//    cmd.addParameter(qint8(m_port));
//    m_ev3->sendCommand(cmd.data());

    char command[5];
    command[0] = opOUTPUT_START;
    command[1] = 0x81;
    command[2] = 0x00;
    command[3] = 0x81;
    command[4] = m_port;
    m_ev3->sendCommand(QByteArray::fromRawData(command, 5));

}

void Ev3Motor::stop()
{
//    Ev3Command cmd;
//    cmd.addOpCode(opOUTPUT_STOP);
//    cmd.addParameter(qint8(0x00));
//    cmd.addParameter(qint8(m_port));
//    cmd.addParameter(qint8(0x01)); //brake
//    m_ev3->sendCommand(cmd.data());

    char command[7];
    command[0] = opOUTPUT_STOP;
    command[1] = 0x81;
    command[2] = 0x00;
    command[3] = 0x81;
    command[4] = m_port;
    command[5] = 0x81;
    command[6] = 0x01; // brake
    m_ev3->sendCommand(QByteArray::fromRawData(command, 7));

}

void Ev3Motor::setPolarity(int p)
{
    char command[7];
    command[0] = opOUTPUT_POLARITY;
    command[1] = 0x81;
    command[2] = 0x00;
    command[3] = 0x81;
    command[4] = m_port;
    command[5] = 0x81;
    command[6] = p > 0 ? qint8(1) : qint8(-1);
    m_ev3->sendCommand(QByteArray::fromRawData(command, 7));
}
