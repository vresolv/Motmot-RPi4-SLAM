#ifndef CAMERA_MANAGER_HPP
#define CAMERA_MANAGER_HPP

#include <lccv.hpp>

#include <QThread>
#include <QObject>
#include <QDebug>
#include <QMetaType>

#include <iostream>

#include <opencv2/opencv.hpp>

class cameraManager : public QObject {
    Q_OBJECT
public:
    cameraManager();
    ~cameraManager();

public slots:
    void startCam(int width, int height, int framerate); 
    void stopCam();

signals:
    void sendCurrentFrame(cv::Mat frame);

private:
    std::unique_ptr<lccv::PiCamera> cam;
    bool isRunning = false;
};

#endif // CAMERA_MANAGER_HPP
