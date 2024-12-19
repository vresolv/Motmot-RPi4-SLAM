#include <zmq.hpp>

#include <lccv.hpp>

#include <opencv2/opencv.hpp>

#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <mutex>
#include <deque>

struct FrameData {
    cv::Mat frame;
    double timestamp;
};

std::deque<FrameData> dataQueue;
const size_t MAX_QUEUE_SIZE = 10;
std::mutex dataMutex;

void addToQueue(const FrameData& newData) {
    std::lock_guard<std::mutex> lock(dataMutex);

    if (dataQueue.size() >= MAX_QUEUE_SIZE) {
        dataQueue.pop_front();
    }

    dataQueue.push_back(newData);
}

void request_handler_function(zmq::context_t &context) {
    zmq::socket_t worker(context, ZMQ_REP);
    worker.connect("inproc://workers");

    while (true) {
        zmq::message_t request;
        worker.recv(request, zmq::recv_flags::none);

        std::string requestStr(static_cast<char*>(request.data()), request.size());

        if (strcmp(requestStr.c_str(), "GET_DATA") == 0) {

            FrameData dataToSend;
            bool hasData = false;

            {
                std::lock_guard<std::mutex> lock(dataMutex);
                if (!dataQueue.empty()) {
                    dataToSend = dataQueue.front();
                    hasData = true;
                }
            }


            if (hasData) {
                std::vector<uchar> buffer;
                if (cv::imencode(".jpg", dataToSend.frame, buffer)) {
                    
                    {
                        std::lock_guard<std::mutex> lock(dataMutex);
                        if (!dataQueue.empty()) {
                            dataQueue.pop_front();
                        }
                    }

                    std::string serializedData;
                    serializedData.append(std::to_string(buffer.size()));
                    serializedData.append("|");
                    serializedData.append(reinterpret_cast<const char*>(buffer.data()), buffer.size());
                    serializedData.append(std::to_string(dataToSend.timestamp));

                    worker.send(zmq::buffer(serializedData), zmq::send_flags::none);
                }
                else {
                    std::cout << "Failed to Encode Data" << std::endl;
                }
            } 
            else {
                worker.send(zmq::buffer("NO_DATA"), zmq::send_flags::none);
            }
        } 
        else {
            worker.send(zmq::buffer("INVALID_REQUEST"), zmq::send_flags::none);
        }
    }
}

void camera_function() {
    lccv::PiCamera cam;

    cam.options->video_width=1280;
    cam.options->video_height=720;
    cam.options->framerate=15;
    cam.options->verbose=true;

    cv::Mat currentFrame(cam.options->video_height, cam.options->video_width, CV_8UC3);

    cam.startVideo();

    while(true) {
        if(!cam.getVideoFrame(currentFrame, 1000))
            std::cout << "Cam Timeout error" << std::endl;
        else {
            auto now = std::chrono::system_clock:: now();
            auto now_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
            
            FrameData newData;
            newData.frame = currentFrame.clone();
            newData.timestamp = now_ns;

            addToQueue(newData);

            cv::imwrite("output.jpeg", currentFrame);
        }
    }
}

int main() {

    zmq::context_t context(1);

    zmq::socket_t router(context, ZMQ_ROUTER);
    router.bind("ipc:///tmp/zmq_ipc");
    
    zmq::socket_t dealer(context, ZMQ_DEALER);
    dealer.bind("inproc://workers");

    std::thread camera_thread(camera_function);

    const int numWorkers = 4;
    std::vector<std::thread> workers;
    for (int i = 0; i < numWorkers; i++) {
        workers.emplace_back(request_handler_function, std::ref(context));
    }

    zmq::proxy(router, dealer);

    camera_thread.join();

    for (auto& worker : workers) {
        worker.join();
    }

    return 0;
}