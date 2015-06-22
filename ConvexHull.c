#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <highgui.h>
#include "algorithm/heapsort.h"

/**
 * Build a convex hull for a given set of points
 * @author: Methusael Murmu
**/

#define ON_LINE 0x13
#define SECTION_LOWER 0x11
#define SECTION_UPPER 0x12

#define IMG_WIDTH 500
#define IMG_HEIGHT 500
#define SAMPLE_WIDTH 450
#define SAMPLE_HEIGHT 450
#define WINDOW_ID "ConvexHull"

#define USAGE_INFO "Usage: %s -n <number of sample points>"
#define SHOW_INFO(M, ...) \
    do { fprintf(stderr, "" M "\n", __VA_ARGS__); exit(0); } while(0)

typedef struct point {
    uint x, y;
} Point;

Point * generate_points(int n) {
    assert(n > 2);

    int i;
    int offset_x = (IMG_WIDTH - SAMPLE_WIDTH) / 2;
    int offset_y = (IMG_HEIGHT - SAMPLE_HEIGHT) / 2;
    Point *pts = (Point *) malloc(n * sizeof(Point));

    srand((unsigned int) time(NULL));
    for (i = 0; i < n; i++) {
        pts[i].x = (uint) (rand() % SAMPLE_WIDTH + offset_x + 1);
        pts[i].y = (uint) (rand() % SAMPLE_HEIGHT + offset_y + 1);
    }

    return pts;
}

/*
 * Comparison function to be used for sorting
 */
int compare_point(void *p1, void *p2) {
    return (*(Point *)p1).x - (*(Point *)p2).x;
}

/*
 * Sort points in ascending order along x-axis using heap sort.
 * Note: 
 * It's advisable to use the compact HEAP_SORT_ANY macro instead
 */
void sort_points(Point *pts, int n) {
    int i;
    // Pointer reference to given points (elements in pts_cpy)
    void **pref = (void **) malloc(n * sizeof(void *));
    // Copy of pts. Will be used as reference while sorting
    Point *pts_cpy = (Point *) malloc(n * sizeof(Point));

    for (i = 0; i < n; i++) {
        pts_cpy[i] = pts[i];
        pref[i] = (void *) &pts_cpy[i];
    }

    // Sort the pointer references using data from actual point
    // Use compare_point as the comparison function
    heap_sort(pref, n, compare_point);
    // Update pts to reflect the sorted references in pref
    for (i = 0; i < n; i++)
        pts[i] = *(Point *) pref[i];

    free(pref);
    free(pts_cpy);
}

/*
 * Determines if the point p lies in the upper or lower section of the
 * area divided by the line segment defined by points p1 and p2.
 *
 * Returns SECTION_LOWER, SECTION_UPPER or ON_LINE.
 * SECTION_UPPER:   Moving from p1 to p2, p lies to the left of the segment
 * SECTION_LOWER:   Moving from p1 to p2, p lies to the right of the segment
 * ON_LINE:         p lies on the line defined by p1 and p2
 */
int get_section(Point p1, Point p2, Point p) {
    int dx = p2.x - p1.x, dy = p2.y - p1.y;
    int get_section_slope(int, int, Point, Point);

    return get_section_slope(dx, dy, p1, p);
}

/*
 * Determines if the point p lies in the upper or lower section of the
 * area divided by the line defined by the slope dy/dx and point p1.
 */
int get_section_slope(int dx, int dy, Point p1, Point p) {
    int res = (p.y - p1.y) * dx - (p.x - p1.x) * dy;

    return  res < 0 ? SECTION_UPPER :
            res > 0 ? SECTION_LOWER : ON_LINE;
}

/*
 * Builds the convex hull for the given set of points.
 * Uses a slightly modified version of Graham Scan.
 *
 * pts: The set of points whose convex hull is to be computed
 * n : Number of points in pts
 * sorted: Indicates if pts is already sorted or not
 * nsz: Integer pointer whose value is set to the number of points in the convex hull
 */
Point * build_hull(Point *pts, int n, int sorted, int *nsz) {
    if (!sorted) sort_points(pts, n);

    // sz = number of points in the hull
    // hsz = number of points in the upper hull
    int i, sz = 2, hsz;
    int dx = pts[n-1].x - pts[0].x;
    int dy = pts[n-1].y - pts[0].y;
    Point *hull = (Point *) malloc(n * sizeof(Point));
    hull[0] = pts[0]; hull[1] = pts[1];

    // Compute hull for upper section
    for (i = 2; i < n; i++) {
        if (get_section_slope(dx, dy, pts[0], pts[i]) == SECTION_LOWER)
            continue;

        while (sz > 1 && get_section(hull[sz-2], hull[sz-1], pts[i]) != SECTION_LOWER)
            sz--;
        hull[sz++] = pts[i];
    }

    // Append the 2nd last point and increment sz
    hull[sz] = pts[n-2];
    hsz = sz++;
    // Compute hull for lower section
    for (i = n - 3; i > -1; i--) {
        if (get_section_slope(dx, dy, pts[0], pts[i]) == SECTION_UPPER)
            continue;

        while (sz > hsz && get_section(hull[sz-2], hull[sz-1], pts[i]) != SECTION_LOWER)
            sz--;
        hull[sz++] = pts[i];
    }

    // Leftmost point is included twice; remove it
    sz--; *nsz = sz;
    if (sz != n)
        hull = (Point *) realloc(hull, sz * sizeof(Point));

    return hull;
}

void draw_hull(IplImage *img, Point *pts, int np, Point *hull, int nh) {
    int i;

    // Draw the points
    for (i = 0; i < np; i++) {
        cvRectangle(
            img,
            cvPoint(pts[i].x - 1, pts[i].y - 1),
            cvPoint(pts[i].x + 1, pts[i].y + 1),
            CV_RGB(0, 0, 0xFF), 1, 4, 0
        );
    }

    // Draw the edges connecting the points in the hull
    for (i = 1; i < nh; i++) {
        cvLine(
            img,
            cvPoint(hull[i-1].x, hull[i-1].y),
            cvPoint(hull[i].x, hull[i].y),
            CV_RGB(0xFF, 0, 0), 1, 4, 0
        );
    }

    // Draw the final edge connecting back to the first point
    cvLine(
        img,
        cvPoint(hull[0].x, hull[0].y),
        cvPoint(hull[nh-1].x, hull[nh-1].y),
        CV_RGB(0xFF, 0, 0), 1, 4, 0
    );
}

int main(int argc, char *argv[]) {
    if (argc < 3)
        SHOW_INFO(USAGE_INFO, argv[0]);

    // npoint = Number of points
    // nhullp = Number of points in hull
    int idx, npoint = 0, nhullp;
    while((idx = getopt(argc, argv, "hn:")) != -1) {
        switch (idx) {
            case 'n':
                npoint = strtoul(optarg, NULL, 10);
                if (!npoint)
                    SHOW_INFO("Invalid parameter to 'n': %s\n", optarg);
                break;

            default:
                SHOW_INFO(USAGE_INFO, argv[0]);
        }
    }

    Point *pts = generate_points(npoint);
    Point *hull = build_hull(pts, npoint, 0, &nhullp);

    IplImage *img = cvCreateImage(
        cvSize(IMG_WIDTH, IMG_HEIGHT),
        IPL_DEPTH_8U,
        3
    );
    cvZero(img);
    draw_hull(img, pts, npoint, hull, nhullp);

    cvNamedWindow(WINDOW_ID, CV_WINDOW_AUTOSIZE);
    cvShowImage(WINDOW_ID, img);

    // Press ESC to close window
    while ((cvWaitKey(20) & 0xFF) != 27);

    free(pts);
    free(hull);
    cvReleaseImage(&img);
    cvDestroyWindow(WINDOW_ID);

    return EXIT_SUCCESS;
}
