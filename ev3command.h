#ifndef EV3COMMAND_H
#define EV3COMMAND_H

#include <QDataStream>
#include "bytecodes.h"

class Ev3Command
{
public:
    Ev3Command();

    void addOpCode(OP opCode);
    void addParameter(qint8 p);
    void addParameter(qint16 p);
    void addParameter(qint32 p);

    const QByteArray &data() const { return m_data; }

private:
    QByteArray m_data;
    QDataStream m_stream;
};

#endif // EV3COMMAND_H
