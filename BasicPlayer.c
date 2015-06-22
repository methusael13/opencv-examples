/*
 * Basic video player without any audio or skipping support
 * @author: Methusael Murmu
 */
 
#include <stdio.h>
#include <highgui.h>

/* Press ESC to exit */
#define EXIT_KEY_CODE 27
#define FRAME_DURATION 33
#define WINDOW_ID "BasicPlayer"

int         frame_idx = 0;
CvCapture  *capture   = NULL;

void onTrackbarEvent(int pos) {
    cvSetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES, pos);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <video file>\n", argv[0]);
        return -1;
    }

    int frames;
    capture = cvCreateFileCapture(argv[1]);
    frames = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_COUNT);
    cvNamedWindow(WINDOW_ID, CV_WINDOW_AUTOSIZE);

    if (frames) {
        cvCreateTrackbar(
            "Trackbar", WINDOW_ID,
            &frame_idx, frames, onTrackbarEvent
        );
    }

    char key;
    IplImage *frame = NULL;
    while (1) {
        frame = cvQueryFrame(capture);
        if (!frame) break;

        cvShowImage(WINDOW_ID, frame);
        if ((cvWaitKey(FRAME_DURATION) & 0xFF) == EXIT_KEY_CODE)
        	break;
    }

    cvReleaseCapture(&capture);
    cvDestroyWindow(WINDOW_ID);

    return 0;
}
