#include <zmq.hpp>

#include <opencv2/opencv.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>

int main() {
    zmq::context_t context(1);

    zmq::socket_t requester(context, ZMQ_REQ);
    requester.connect("ipc:///tmp/zmq_ipc");

    int frameCount = 0;
    double totalLatency = 0;
    auto start_time = std::chrono::high_resolution_clock::now();

    while (true) {

        std::string requestMessage = "GET_DATA";
        requester.send(zmq::buffer(requestMessage), zmq::send_flags::none);

        zmq::message_t reply;
        requester.recv(reply, zmq::recv_flags::none);
        std::string replyStr(static_cast<char*>(reply.data()), reply.size());

        auto delimiterPos = replyStr.find("|");
        if (delimiterPos != std::string::npos) {
            std::string sizeStr = replyStr.substr(0, delimiterPos);
            size_t frameSize = std::stoul(sizeStr);
            std::string frameEncoded = replyStr.substr(delimiterPos + 1, frameSize);
            std::string timestampStr = replyStr.substr(delimiterPos + 1 + frameSize);

            double serverTimestamp = std::stod(timestampStr);

            std::vector<uchar> buffer(frameEncoded.begin(), frameEncoded.end());
            cv::Mat frame = cv::imdecode(buffer, cv::IMREAD_COLOR);

            if (!frame.empty()) {
                cv::imwrite("output.jpeg", frame);
            } else {
                std::cout << "Error decoding frame" << std::endl;
            }

            auto now = std::chrono::system_clock:: now();
            double now_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();

            double delay_ms = (now_ns - serverTimestamp) / 1e6;

            totalLatency += delay_ms;
            frameCount++;

            if (frameCount == 50) {
                auto end_time = std::chrono::high_resolution_clock::now();
                double elapsedTime = std::chrono::duration<double>(end_time - start_time).count();

                double avgLatency = totalLatency / 50;
                double frameRate = frameCount / elapsedTime;

                std::cout << "Avg Latency (ms): " << avgLatency << std::endl;
                std::cout << "Frame Rate (fps): " << frameRate << std::endl;

                frameCount = 0;
                totalLatency = 0;
                start_time = std::chrono::high_resolution_clock::now();
            }
        } 
        else {
            //std::cout << "Invalid Data Format Received: " << replyStr << std::endl;
        }
    }

    return 0;
}