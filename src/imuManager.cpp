#include <imuManager.hpp>

imuManager::imuManager(QObject *parent) : QThread(parent) {}

imuManager::~imuManager() {}

void imuManager::startImu(QString port) {

    serialImu.setPortName(port);
    serialImu.setBaudRate(QSerialPort::Baud9600);
    serialImu.setDataBits(QSerialPort::Data8);
    serialImu.setParity(QSerialPort::NoParity);
    serialImu.setStopBits(QSerialPort::OneStop);
    serialImu.setFlowControl(QSerialPort::NoFlowControl);

    if (serialImu.open(QIODevice::ReadOnly))
        qDebug() << "Serial port Imu opened " << port;
    else {
        qDebug() << "Cannot open Serial port Imu " << port;
        return;
    }

    QObject::connect(&serialImu, &QSerialPort::readyRead, [&]() {
        QByteArray data = serialImu.readAll();
        qDebug() << "Data Rec Imu: " << QString::fromUtf8(data);
	emit sendCurrentImuFrame(QString::fromUtf8(data));
    });

    isRunning = true;  
}

void imuManager::stopImu() {
    if (serialImu.isOpen())
        serialImu.close();

    isRunning = false;
}

void imuManager::run() {
    exec();
}
