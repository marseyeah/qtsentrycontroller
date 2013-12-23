#include "ev3command.h"

Ev3Command::Ev3Command()
    : m_stream(&m_data, QIODevice::WriteOnly)
{
}

void Ev3Command::addOpCode(OP opCode)
{
    m_stream << qint8(opCode);
}

void Ev3Command::addParameter(qint8 p)
{
    m_stream << qint8(0x81) << p;
}

void Ev3Command::addParameter(qint16 p)
{
    m_stream << qint8(0x82) << p;
}

void Ev3Command::addParameter(qint32 p)
{
    m_stream << qint8(0x83) << p;
}
