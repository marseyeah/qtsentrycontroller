#ifndef EV3MOTOR_H
#define EV3MOTOR_H

#include <QObject>

class Ev3;

class Ev3Motor : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int power READ power WRITE setPower NOTIFY powerChanged)
public:
    enum Port {
        PortA = 0x01,
        PortB = 0x02,
        PortC = 0x04,
        PortD = 0x08
    };

    explicit Ev3Motor(Ev3 *ev3, Port port);
    ~Ev3Motor();

    int power() const { return m_power; }
    void setPower(int power);

Q_SIGNALS:
    void powerChanged();

private:
    void start();
    void stop();
    void setPolarity(int p);

    Ev3 *m_ev3;
    Port m_port;
    int m_power;
};

#endif // EV3MOTOR_H
