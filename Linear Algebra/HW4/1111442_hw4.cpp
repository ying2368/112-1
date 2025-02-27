#include<opencv2/opencv.hpp>
#include<iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <vector>
#include <cmath>
#include <fstream>
using namespace std;
using namespace cv;

struct Car {
    Scalar color;
    Point current;
    vector<Point> path;
    int f;

    Car(Scalar color, Point current, Point point)
        :color(color), current(current),f(0) { path.push_back(point); }
};

bool similar1(const Scalar& color1, const Scalar& color2)
{
    return abs(color1[0] - color2[0]) <= 5 && abs(color1[1] - color2[1]) <= 5 && abs(color1[2] - color2[2]) <= 5;
}

int similar2(Point p1, Point p2, int car_width, int car_height)
{
    return abs(p1.x - p2.x) < car_width && abs(p1.y - p2.y) < car_height;
}

int exist(vector<Car> cars, Scalar color, Point point, int car_width, int car_height)
{
    for (int i = 0; i < cars.size(); i++)
        if (similar1(cars[i].color, color) && similar2(cars[i].current, point, car_width, car_height) && cars[i].f != 1)
            return i;

    return -1;
}

string rgbToHex(Scalar  color)
{
    int blue = static_cast<int>(color[0]);
    int green = static_cast<int>(color[1]);
    int red = static_cast<int>(color[2]);
    stringstream ss;
    ss << setfill('0') << setw(2) << hex << red;
    ss << setfill('0') << setw(2) << hex << green;
    ss << setfill('0') << setw(2) << hex << blue;

    return ss.str();
}

string convert(string s)
{
    for (int i = 0; i < s.size(); i++)
        if (s[i]<'0'||s[i]>'9')
            s[i] -= 32;
    return s;
}

double route(vector<Point> path)
{
    double r = 0;
    for (int i = 1; i < path.size(); i++)
    {
        r += sqrt((path[i].x - path[i - 1].x) * (path[i].x - path[i - 1].x) + (path[i].y - path[i - 1].y) * (path[i].y - path[i - 1].y));
    }
    return r;
}

int main(int argc, char* argv[])
{
    VideoCapture cap(argv[1]);       
    vector<Mat> video;
    Mat frame;

    for (int i = 0; i < 90; ++i) {
        cap >> frame;
        if(i%2 == 0)                        // 每兩幀取一幀
            video.push_back(frame.clone());
    }

    vector<Car> cars;
    int width = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    int height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    int car_width = 0, car_height = 0;
    for (int i = 0; i < 45; ++i)
    {
        Mat grayImage;
        cvtColor(video[i], grayImage, COLOR_BGR2GRAY);

        Mat binaryImage;
        threshold(grayImage, binaryImage, 100, 255, THRESH_BINARY);

        vector<vector<Point>> contours;
        findContours(binaryImage, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
        for (const auto& contour : contours) {
            Rect boundingRect = cv::boundingRect(contour);
            double area = contourArea(contour);

            if (area > 1000)
            {
                int maxx = 0, maxy = 0, minx = width, miny = height;
                for (int i = 0; i < contour.size(); i++)
                {
                    if (contour[i].x > maxx) maxx = contour[i].x;
                    if (contour[i].x < minx) minx = contour[i].x;
                    if (contour[i].y > maxy) maxy = contour[i].y;
                    if (contour[i].y < miny) miny = contour[i].y;
                }

                Point centroid((maxx + minx) / 2, (maxy + miny) / 2);
                Point front((maxx + minx) / 2, maxy);
                Scalar color = video[i].at<Vec3b>(centroid.y, centroid.x);
                if (maxx - minx > car_width) 
                    car_width = maxx - minx;
                if (maxy - miny > car_height) 
                    car_height = maxy - miny;
                int index = exist(cars, color, centroid, car_width, car_height);
                if (index == -1)
                {
                    cars.push_back(Car(color, centroid, front));
                    circle(video[i], centroid, 5, Scalar(0, 255, 255), -1);
                    circle(video[i], front, 5, Scalar(0, 255, 255), -1);

                }
                else
                {
                    if(centroid.y >= cars[index].current.y)
                        if (maxy - miny > car_height - 5 || miny < height / 2)
                            cars[index].path.push_back(front);
                    else if (centroid.y < cars[index].current.y)
                        if (maxy - miny > car_height - 5 || miny > height / 2)
                            cars[index].path.push_back(front);
                    cars[index].current = centroid;
                    cars[index].f = 0;
                }

            }
        }

        for (int j = 0; j < cars.size(); j++)
        {
            if (cars[j].f == -1)
                cars[j].f = 1;
            if (cars[j].f == 0)
                cars[j].f = -1;
        }
    }

    vector<Car> result;
    for (int i = 0; i < cars.size(); i++)
    {
        int j = 0;
        for (; j < result.size(); j++)
            if (rgbToHex(cars[i].color) < rgbToHex(result[j].color))
                break;
        result.insert(result.begin() + j, cars[i]);
    }

    ofstream outfile(argv[2]);
    for (int i = 0; i < result.size(); i++)
    {
        if (result[i].path.size() > 1)
        {
            outfile << "#" << convert(rgbToHex(result[i].color)) << " " << setprecision(2) << fixed << route(result[i].path) << endl;
            cout << "#" << convert(rgbToHex(result[i].color)) << " " << setprecision(2) << fixed << route(result[i].path) << endl;
        }
    }

    cap.release();
    outfile.close();
    return 0;
}