#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(void)
{
    // 1. 카메라 장치 열기
    VideoCapture cap(0);
    if (!cap.isOpened()) {
        cerr << "Camera open failed!" << endl;
        return -1;
    }

    int width = cvRound(cap.get(CAP_PROP_FRAME_WIDTH));
    int height = cvRound(cap.get(CAP_PROP_FRAME_HEIGHT));
    double fps = cap.get(CAP_PROP_FPS);
    if (fps < 1.0) fps = 30.0; // FPS 값이 유효하지 않을 경우 기본값 설정
    int delay = cvRound(1000 / fps);

    VideoWriter output;
    bool isRecording = false; // 녹화 상태를 나타내는 플래그

    cout << "------------------------------------------" << endl;
    cout << " 사용 방법: " << endl;
    cout << " s 키: 녹화 시작" << endl;
    cout << " e 키: 녹화 종료 및 프로그램 종료" << endl;
    cout << "------------------------------------------" << endl;

    Mat frame;
    while (true) {
        cap >> frame;
        if (frame.empty()) break;

        // 녹화 중일 때만 프레임을 파일에 저장
        if (isRecording) {
            output << frame;
            // 녹화 중임을 알리는 텍스트를 화면에 표시 (선택 사항)
            putText(frame, "REC ●", Point(20, 30), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255), 2);
        }

        imshow("Camera", frame);

        int key = waitKey(delay);

        // 's' 또는 'S'를 누르면 녹화 시작
        if ((key == 's' || key == 'S') && !isRecording) {
            output.open("output.avi", VideoWriter::fourcc('D', 'I', 'V', 'X'), fps, Size(width, height));
            if (!output.isOpened()) {
                cerr << "File open failed!" << endl;
                return -1;
            }
            isRecording = true;
            cout << "녹화를 시작합니다..." << endl;
        }
        // 'e' 또는 'E'를 누르면 녹화 종료 및 프로그램 종료
        else if (key == 'e' || key == 'E') {
            if (isRecording) {
                cout << "녹화를 종료하고 저장합니다." << endl;
            }
            break;
        }
    }

    // 자원 해제 (output.release()가 호출되어야 파일이 정상 저장됨)
    cap.release();
    output.release();
    destroyAllWindows();

    return 0;
}