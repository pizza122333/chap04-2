#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(void)
{
    // 1. 동영상 파일 열기
    VideoCapture cap("stopwatch.avi");
    if (!cap.isOpened()) {
        cerr << "에러: stopwatch.avi 파일을 찾을 수 없습니다. 폴더 위치를 확인하세요!" << endl;
        return -1;
    }

    // 원본 영상의 속성 가져오기
    int width = cvRound(cap.get(CAP_PROP_FRAME_WIDTH));
    int height = cvRound(cap.get(CAP_PROP_FRAME_HEIGHT));
    double fps = cap.get(CAP_PROP_FPS);
    int fourcc = VideoWriter::fourcc('X', 'V', 'I', 'D'); // 코덱 설정
    int delay = cvRound(1000 / fps);

    // 2. 동영상 저장을 위한 VideoWriter 객체 생성
    VideoWriter outputVideo("output.avi", fourcc, fps, Size(width, height));

    if (!outputVideo.isOpened()) {
        cerr << "에러: 결과 파일을 저장할 수 없습니다." << endl;
        return -1;
    }

    Mat frame, result;

    while (true) {
        cap >> frame; // 프레임 읽기
        if (frame.empty()) break;

        // 원본 복사본 생성 (처리 후 영상용)
        result = frame.clone();

        // 3. 라인 그리기 (예: 좌상단에서 우하단으로 대각선)
        // 요청하신 특정 '라인 그림'이 있다면 아래 Point 좌표를 수정하세요.
        line(result, Point(50, 50), Point(width - 50, height - 50), Scalar(0, 0, 255), 3);
        line(result, Point(width - 50, 50), Point(50, height - 50), Scalar(0, 255, 0), 3);

        // 4. 결과 영상 파일 저장
        outputVideo << result;

        // 5. 원본과 결과 영상 화면 출력
        imshow("Original Video", frame);
        imshow("Processed Video (Lines)", result);

        // 6. 'q' 또는 'Q' 키를 누르면 종료
        int key = waitKey(delay);
        if (key == 'q' || key == 'Q') {
            break;
        }
    }

    // 리소스 해제
    cap.release();
    outputVideo.release();
    destroyAllWindows();

    cout << "작업 완료. output.avi 파일이 저장되었습니다." << endl;

    return 0;
}