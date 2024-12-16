#ifndef IMU_MANAGER_HPP
#define IMU_MANAGER_HPP

#include <QThread>
#include <QObject>
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>

#include <iostream>

class imuManager : public QObject {
    Q_OBJECT
public:
    imuManager();
    ~imuManager();

public slots:    
    void startImu(QString port);
    void stopImu();

signals:
    void sendCurrentImuFrame(const QString &frame);

private:
    std::unique_ptr<QSerialPort> serialImu;
};

#endif // IMU_MANAGER_HPP
