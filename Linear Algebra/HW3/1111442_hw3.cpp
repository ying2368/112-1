#include<opencv2/opencv.hpp>
#include<iostream>
#include<algorithm>
#include<cmath>
#include<fstream>

using namespace std;
using namespace cv;

bool mark(Mat image, double area)
{
    int black = 0, white = 0;
    for (int i = 3; i < image.rows - 3; i++)
        for (int j = 3; j < image.cols - 3; j++)
        {
            if (image.at<uint8_t>(i, j) == 0)
                black++;
            if (image.at<uint8_t>(i, j) == 255)
                white++;
        }

    return black - white > 30;
}

int main(int argc, char* argv[])
{
    // Mat srcImage = imread("input7.jpg");
    Mat srcImage = imread(argv[1]);
    Mat grayImage;
    cvtColor(srcImage, grayImage, COLOR_BGR2GRAY);

    Mat binaryImage;
    threshold(grayImage, binaryImage, 70, 255, THRESH_BINARY);

    vector<vector<Point>> contours;
    findContours(binaryImage, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

    vector<vector<Point>> filteredContours;
    for (const auto& contour : contours) {
        Rect boundingRect = cv::boundingRect(contour);  // 获取外接矩形

        double aspectRatio = static_cast<double>(boundingRect.width) / boundingRect.height;
        double area = contourArea(contour);

        if (area > 50 && area < 600 && aspectRatio > 0.6 && aspectRatio < 1.4)
        {
            filteredContours.push_back(contour);
            rectangle(srcImage, boundingRect, Scalar(0, 255, 255), 2); //在图像上绘制外接矩形
        }
    }

    // 寻找定位点
    Point2f topLeft(500, 700), topRight, bottomLeft, bottomRight;
    for (const auto& contour : filteredContours) {
        for (const auto& point : contour) {
            if (topLeft.x + topLeft.y > point.x + point.y)    // 左上
                topLeft = point;
            if (topRight.x - topRight.y < point.x - point.y)     // 右上
                topRight = point;
            if (bottomLeft.x - bottomLeft.y > point.x - point.y)    // 左下
                bottomLeft = point;
            if (bottomRight.x + bottomRight.y < point.x + point.y)    // 右下
                bottomRight = point;
        }
    }

    // 在图像上标出定位点
    circle(srcImage, topLeft, 5, Scalar(255, 255, 0), -1);
    circle(srcImage, topRight, 5, Scalar(255, 255, 0), -1);
    circle(srcImage, bottomLeft, 5, Scalar(255, 255, 0), -1);
    circle(srcImage, bottomRight, 5, Scalar(255, 255, 0), -1);

    // 定义透视变换矩阵
    Point2f srcPoints[4] = { topLeft, topRight, bottomLeft, bottomRight };
    Point2f dstPoints[4] = { Point2f(0, 0), Point2f(210 * 5, 0), Point2f(0, 297 * 5), Point2f(210 * 5, 297 * 5) };
    Mat perspectiveMatrix = getPerspectiveTransform(srcPoints, dstPoints);

    Mat transformedImage;   // 应用透视变换
    warpPerspective(srcImage, transformedImage, perspectiveMatrix, Size(210 * 5, 297 * 5));

    /*imwrite("Binary Image.jpg", binaryImage);
    imwrite("Contours and Points.jpg", srcImage);*/
    //imshow("Transformed Image", transformedImage);

    // 定義要裁剪的區域
    Rect roiRect(80, 530, 50, 800); // (x, y, width, height)
    Rect roiRect2(220, 530, 730, 800); // (x, y, width, height)
    Mat LabelArea = transformedImage(roiRect).clone(); // 標註區域
    Mat swipeArea = transformedImage(roiRect2).clone(); // 畫卡區域

    vector<int> height;
    contours.clear();
    filteredContours.clear();
    cvtColor(LabelArea, grayImage, COLOR_BGR2GRAY);
    threshold(grayImage, binaryImage, 105, 255, THRESH_BINARY);
    findContours(binaryImage, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

    for (const auto& contour : contours) {
        Rect boundingRect = cv::boundingRect(contour);

        double area = contourArea(contour);
        if (area > 80 && area < 300)
        {
            filteredContours.push_back(contour);
            rectangle(LabelArea, boundingRect, Scalar(0, 255, 0), 2); // 在图像上绘制外接矩形

            height.push_back(contour[0].y);
            circle(LabelArea, contour[0], 5, Scalar(255, 0, 255), -1);
        }
    }

    sort(height.begin(), height.end());

    cout << "==================================" << endl;
    cout << "height.size(): " << height.size() << endl;

    contours.clear();
    filteredContours.clear();
    int width[12] = { 33,90,150,207,266,322,380,438,495,553,610,670 };
    int count[24]{};
    int ans[24]{};

    cvtColor(swipeArea, grayImage, COLOR_BGR2GRAY);
    threshold(grayImage, binaryImage, 123, 255, THRESH_BINARY);
    findContours(binaryImage, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

    Mat binaryImage2;
    threshold(grayImage, binaryImage2, 128, 255, THRESH_BINARY);

    for (const auto& contour : contours) {
        Rect boundingRect = cv::boundingRect(contour);  // 获取外接矩形
        double aspectRatio = static_cast<double>(boundingRect.width) / boundingRect.height;
        double area = contourArea(contour);
        if (area > 100 && area < 300 && aspectRatio < 3 && aspectRatio > 1.5)
        {
            filteredContours.push_back(contour);
            rectangle(swipeArea, boundingRect, Scalar(255, 255, 0), 2); // 在图像上绘制外接矩形
            Mat croppedImage = binaryImage2(boundingRect);

            if (mark(croppedImage, area))
            {
                Point2f topLeft = contour[0];
                for (const auto& point : contour)
                    if (topLeft.x + topLeft.y > point.x + point.y)
                        topLeft = point;

                int x = topLeft.x, y = topLeft.y;
                for (int i = 0; i < 24; i++)
                {
                    if (abs(y - height[i]) < 7)
                    {
                        count[i]++;
                        for (int j = 0; j < 12; j++)
                            if (abs(x - width[j]) < 12)
                                ans[i] = j;
                    }
                }

                //cout << x << " " << y << endl;
                circle(swipeArea, topLeft, 5, Scalar(255, 0, 255), -1);
            }

        }
    }

    ofstream outfile(argv[2]);
    for (int i = 0; i < 24; i++)
    {
        if (count[i] == 0)
            outfile << "X";
        else if (count[i] > 1)
            outfile << "M";
        else
        {
            if (ans[i] == 9)
                outfile << 0;
            else if (ans[i] == 10)
                outfile << "A";
            else if (ans[i] == 11)
                outfile << "B";
            else
                outfile << ans[i] + 1;
        }
    }
    outfile << endl;

    outfile.close();
    imwrite("binaryImage Image.jpg", binaryImage2);
    imwrite("Result Image.jpg", swipeArea);
    imshow("Result Image", swipeArea);
    waitKey(0);

    return 0;
}

