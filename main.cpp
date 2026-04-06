#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(void)
{
    // 1. 카메라 장치 열기 (0번 기본 카메라)
    VideoCapture cap(0);
    if (!cap.isOpened()) {
        cerr << "Camera open failed!" << endl;
        return -1;
    }

    // 카메라의 프레임 너비, 높이, FPS 가져오기
    int width = cvRound(cap.get(CAP_PROP_FRAME_WIDTH));
    int height = cvRound(cap.get(CAP_PROP_FRAME_HEIGHT));
    double fps = cap.get(CAP_PROP_FPS);

    // 카메라 FPS가 0으로 반환되는 경우를 대비해 기본값(30) 설정
    if (fps < 1.0) fps = 30.0;
    int delay = cvRound(1000 / fps);

    // 2. 동영상 저장을 위한 VideoWriter 설정
    // DIVX 코덱을 사용하며, 원본 사이즈 그대로 저장합니다.
    VideoWriter output("output.avi", VideoWriter::fourcc('D', 'I', 'V', 'X'), fps, Size(width, height));

    if (!output.isOpened()) {
        cerr << "File open failed!" << endl;
        return -1;
    }

    cout << "녹화를 시작합니다. 'q'를 누르면 종료됩니다." << endl;

    Mat frame;
    while (true) {
        cap >> frame; // 카메라로부터 프레임 읽기
        if (frame.empty()) break;

        // 3. 프레임 저장
        output << frame;

        // 4. 화면 출력 (현재 촬영 중인 영상 확인)
        imshow("Recording...", frame);

        // 5. 'q' 또는 'Q' 키를 누르면 종료
        int key = waitKey(delay);
        if (key == 'q' || key == 'Q') {
            break;
        }
    }

    // 자원 해제
    cap.release();
    output.release();
    destroyAllWindows();

    cout << "녹화가 완료되었습니다. output.avi 파일을 확인하세요." << endl;

    return 0;
}