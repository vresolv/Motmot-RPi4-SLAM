#ifndef CAMERA_MANAGER_HPP
#define CAMERA_MANAGER_HPP

#include <lccv.hpp>

#include <QThread>
#include <QObject>
#include <QDebug>

#include <opencv2/opencv.hpp>

class cameraManager : public QThread {
    Q_OBJECT
public:
    explicit cameraManager(QObject *parent = nullptr);
    ~cameraManager();
    void startCam(int width, int height, int framerate);
    void stopCam();
    void run() override;

signals:
    void sendCurrentFrame(cv::Mat frame);

private:
    cv::Mat currentFrame;
    lccv::PiCamera cam;
    bool isRunning = false;
};

#endif // CAMERA_MANAGER_HPP