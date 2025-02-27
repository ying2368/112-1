#include<opencv2/opencv.hpp>
#include<iostream>
#include<fstream>
#include<vector>
#include<sstream>

using namespace std;
using namespace cv;

double windowing(double wl, double ww, double h)
{
	if (h <= wl - ww / 2)
		return 0;
	if (h > wl + ww / 2)
		return 255;
	return 255 * ((double)(h - wl) / ww + 0.5);
}

double convert(double a)
{
	if (a > 90)
		return a - 180;
	if (a < -90)
		return a + 180;
	return a;
}

void load(string imagePath, vector<vector<double>> &pixelData)
{
	ifstream image_csv(imagePath);
	if (!image_csv)
		cerr << "Failed to open CSV file." << endl;

	string line;
	while (getline(image_csv, line))
	{
		istringstream lineStream(line);
		string value;
		vector<double> rowData;

		while (getline(lineStream, value, ','))
		{
			double pixelValue = stod(value);
			rowData.push_back(pixelValue);
		}
		pixelData.push_back(rowData);
	}
	image_csv.close();
}

void Rescaling(double slope, double intercept, vector<vector<double>> &pixelData)
{
	for (int i = 0; i < 512; i++)
		for (int j = 0; j < 512; j++)
			pixelData[i][j] = pixelData[i][j] * slope + intercept;
}

void Windowing(double level, double width, vector<vector<double>> &pixelData)
{
	for (int i = 0; i < 512; i++)
		for (int j = 0; j < 512; j++)
			pixelData[i][j] = windowing(level, width, pixelData[i][j]);
}

void save(string fileName, vector<vector<double>> pixelData)
{
	Mat image(512, 512, CV_8UC1);
	for (int i = 0; i < 512; i++)
		for (int j = 0; j < 512; j++)
			image.at<uint8_t>(i, j) = static_cast<uint8_t>(floor(pixelData[i][j]));

	imwrite(fileName, image);
	cout << "Image saved as " << fileName << "." << endl;
}

void judgeImage(string img1, string img2)
{
	Mat image1 = imread(img1);
	Mat image2 = imread(img2);

	if (image1.empty() || image2.empty())
		cerr << "Failed to load images." << endl;

	if (image1.size() != image2.size())
	{
		cerr << "Image dimensions are different." << endl;
		return;
	}

	Mat diffImage(image1.size(), CV_8UC3, Scalar(0, 0, 0)); // 初始為黑色
	bool error = false;

	for (int i = 0; i < image1.rows; ++i)
		for (int j = 0; j < image1.cols; ++j)
			if (image1.at<Vec3b>(i, j) != image2.at<Vec3b>(i, j))
			{
				diffImage.at<Vec3b>(i, j) = Vec3b(0, 0, 255); // 紅色表示差異
				error = true;
			}

	if (error)
	{
		cout << img1 << "," << img2 << " is defferent." << endl;
		imwrite("difference.png", diffImage);
	}
}

int main(int argc, char* argv[])
{
	string inputPath = argv[1], outputPath1 = argv[2], outputPath2 = argv[3], anglePath = argv[4];
	ifstream input_txt(inputPath);
	string infoPath, imagePath, maskPath;
	int level1, width1, level2, width2;

	getline(input_txt, infoPath);
	getline(input_txt, imagePath);
	getline(input_txt, maskPath);
	input_txt >> level1 >> width1 >> level2 >> width2;
	input_txt.close();

	ifstream info_csv(infoPath);
	double slope, intercept;
	string line;
	while (getline(info_csv, line))
	{
		istringstream lineStream(line);
		string value;

		getline(lineStream, value, ',');
		if (value == "Rescale Slope")
		{
			getline(lineStream, value, ',');
			slope = stod(value);
		}
		if (value == "Rescale Intercept")
		{
			getline(lineStream, value, ',');
			intercept = stod(value);
		}
	}
	info_csv.close();

	vector<vector<double>> imagePixelData;
	load(imagePath, imagePixelData);
	Rescaling(slope, intercept, imagePixelData);
	Windowing(level1, width1,imagePixelData);
	save(outputPath1, imagePixelData);

	vector<vector<double>> image2PixelData;
	load(imagePath, image2PixelData);
	Rescaling(slope, intercept, image2PixelData);
	Windowing(level2, width2,image2PixelData);
	//save("03.png", image2PixelData);

	vector<vector<double>> maskPixelData;
	load(maskPath, maskPixelData);

	Mat brainMask(512, 512, CV_8UC1, Scalar(0));
	for (int i = 0; i < 512; ++i)
		for (int j = 0; j < 512; ++j)
			if (maskPixelData[i][j] == 3)
				brainMask.at<uint8_t>(i, j) = uint8_t(255);

	imwrite("05.png", brainMask);
	//cout << "Image saved as 05.png." << endl;

	Rescaling((double)255 / 16, 0, maskPixelData);
	//save("01.png", maskPixelData);

	Mat mergedImage(512, 512, CV_8UC3);
	for (int i = 0; i < 512; i++)
		for (int j = 0; j < 512; j++)
			mergedImage.at<Vec3b>(i, j) = Vec3b(
				static_cast<uint8_t>(floor(maskPixelData[i][j])),
				static_cast<uint8_t>(floor(imagePixelData[i][j])),
				static_cast<uint8_t>(floor(image2PixelData[i][j]))
			);

	imwrite("04.png", mergedImage);
	//cout << "Image saved as 04.png." << endl;
	
	Mat contourImage(brainMask.size(), CV_8UC1, Scalar(0)); //輪廓
	vector<vector<Point>> contours;   
	findContours(brainMask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	int num1 = 0, num2 = 0;
	double low = 512, high = 0, middle = 256;
	for (size_t i = 0; i < contours.size(); i++)
	{
		double area = contourArea(contours[i]);
		//cout << area << endl;
		if (area > 100)
		{
			vector<Point> p = contours[i];
			for (int j = 0; j < p.size(); j++)
			{
				if (p[j].y < low)
				{
					num1 = i;
					low = p[j].y;
				}
					
				if (p[j].y > high)
				{
					num2 = i;
					high = p[j].y;
				}
			}
		}
	}
	
	drawContours(contourImage, contours, static_cast<int>(num1), Scalar(255), 2);
	drawContours(contourImage, contours, static_cast<int>(num2), Scalar(255), 2);
	imwrite("contourImage.png", contourImage);

	vector<Vec4i> lines;
	HoughLinesP(contourImage, lines, 1, CV_PI / 180, 40, 0, 10); 
	//cout << lines.size() << endl;
	if (lines.size() < 20)
		HoughLinesP(contourImage, lines, 1, CV_PI / 180, 35, 0, 10);
	//cout << lines.size() << endl;
	
	double totalAngle = 0.0;
	for (size_t i = 0; i < lines.size(); i++) 
	{
		Vec4i line = lines[i];
		double x1 = line[0], y1 = line[1], x2 = line[2], y2 = line[3];
		double angleInRadians = atan2(y2 - y1, x2 - x1);
		double angleInDegrees = (double)angleInRadians * 180.0 / CV_PI + 90;
		//cout << "傾斜角度: " << convert(angleInDegrees) << endl;
		totalAngle += convert(angleInDegrees);
	}

	double averageAngle = totalAngle / lines.size();
	//cout << "Average Angle: " << averageAngle << " degrees" << endl;

	if (num1 != num2)
	{
		totalAngle = 0.0;
		int goodLine = 0;
		for (size_t i = 0; i < lines.size(); i++) {
			Vec4i line = lines[i];
			double x1 = line[0], y1 = line[1], x2 = line[2], y2 = line[3];
			double angleInRadians = atan2(y2 - y1, x2 - x1);  	// 弧度
			double angleInDegrees = (double)angleInRadians * 180.0 / CV_PI + 90;  	
			angleInDegrees = convert(angleInDegrees);

			if (angleInDegrees > averageAngle - 5 && angleInDegrees < averageAngle + 5)
			{
				totalAngle += angleInDegrees;
				goodLine++;
				//cout << "Good: " << angleInDegrees << endl;
			}
		}

		averageAngle = totalAngle / goodLine;
	}

	cout << "Average Angle: " << averageAngle << " degrees" << endl;

	Point2f center(256, 256);
	Mat rotationMatrix = getRotationMatrix2D(center, averageAngle, 1);
	Mat rotatedImage;
	warpAffine(mergedImage, rotatedImage, rotationMatrix, mergedImage.size(), INTER_LINEAR, BORDER_CONSTANT, Scalar(0));
	imwrite(outputPath2, rotatedImage);

	ofstream angle_txt(anglePath);
	angle_txt << averageAngle << endl;
	angle_txt.close();

	return 0;
}
