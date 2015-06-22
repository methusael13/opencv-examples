#include <cv.h>
#include <time.h>
#include <stdio.h>
#include <assert.h>
#include <highgui.h>

#define DISP_DELAY 33
#define MAX_COUNTER 1000
#define WINDOW_ID "DisplayCamera"

int main(int argc, char *argv[]) {
    time_t start_t, end_t;
    IplImage *frame = NULL;
    CvCapture *capture = NULL;
    
    double fps;
    int counter = 0;

    if (argc == 1)
        capture = cvCreateCameraCapture(-1);
    else
        capture = cvCreateFileCapture(argv[1]);

    assert(capture != NULL);
    cvNamedWindow(WINDOW_ID, CV_WINDOW_AUTOSIZE);
    time(&start_t);
    
    while (1) {
        frame = cvQueryFrame(capture);
        if (!frame) break;

        counter++;
        time(&end_t);
        fps = counter / difftime(end_t, start_t);
        cvShowImage(WINDOW_ID, frame);

        if (counter > 30)
            printf("\x1b[1;1H\x1b[J\nFPS: %.2F\n", fps);
        if (counter > MAX_COUNTER) {
            counter = 0;
            time(&start_t);
        }
        
        if ((cvWaitKey(DISP_DELAY) & 255) == 27)
            break;
    }

    printf("\n");
    cvReleaseCapture(&capture);
    cvDestroyWindow(WINDOW_ID);

    return 0;
}
