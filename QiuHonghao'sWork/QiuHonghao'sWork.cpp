#include <iostream>   
#include <math.h>
#include <opencv2/core/core.hpp>   
#include <opencv2/highgui/highgui.hpp>   

using namespace std;  //省去屏幕输出函数cout前的std::
using namespace cv;   //省去函数前面加cv::的必要性

//排序
void sort( double a[])
{
	for (int k = 0; k < 9; k++)
	{
		for (int m = 0; m < 9; m++)
		{
			if (a[k] > a[m])
			{
				double temp;
				temp = a[k], a[k] = a[m], a[m] = temp;
			}
		}

	}
}
//灰度线性变换
void GrayLinerTransform( )
{
  
    char imageName[] = "C:\\Users\\江袅\\Desktop\\1.bmp";
    Mat oringinimag = imread(imageName, IMREAD_COLOR);   // 读入图片 

	if (oringinimag.empty())     // 判断文件是否正常打开  
	{
		fprintf(stderr, "Can not load image %s\n", imageName);
		waitKey(6000);  // 等待6000 ms后窗口自动关闭   
		return;
	}
	imshow("oringinimag", oringinimag);  // 显示图片 
	waitKey();

	int height = oringinimag.rows;
	int width = oringinimag.cols;
	
	
	//将图片转化为灰度图像
	Mat imgGray;
	imgGray.create(height, width, CV_8UC1);//CV_8UC1 8为无符号整型 单通道
	Vec3b* poringinimag; 
	uchar* pimgGray;
	for (int i = 0; i < height; i++) {
		poringinimag = oringinimag.ptr<Vec3b>(i);
		pimgGray = imgGray.ptr<uchar>(i);
		for (int j = 0; j < width; j++) {
			pimgGray[j] = poringinimag[j][0] * 0.114 + poringinimag[j][1] * 0.587 + poringinimag[j][2] * 0.299;//一个心理学公式			
		}

	}
     imshow("灰度图像", imgGray);
	 waitKey();
	//做线性拉伸变换
	 double k, b;
	 cout << "请输入线性变换的k和b" << endl;
	 cin >> k >> b;
	Mat imgRes;
	imgRes.create(height, width, CV_8UC1);
	uchar* pimgRes;
	for (int i = 0; i < height; i++) {
		pimgRes = imgRes.ptr<uchar>(i);
		pimgGray = imgGray.ptr<uchar>(i);
		for (int j = 0; j < width; j++) {
			pimgRes[j] = pimgGray[j] *k+b;
		}

	}

	imshow("线性变换图像", imgRes);
	waitKey();
	imwrite("C:\\Users\\江袅\\Desktop\\灰度线性变换后图像.bmp", imgRes);
}
//高通滤波
void HighPassFilter()
{
	char imageName[] = "C:\\Users\\江袅\\Desktop\\1.bmp";
	Mat Oringinimag = imread(imageName, IMREAD_COLOR);   // 读入图片 

	if (Oringinimag.empty())     // 判断文件是否正常打开  
	{
		fprintf(stderr, "Can not load image %s\n", imageName);
		waitKey(6000);  // 等待6000 ms后窗口自动关闭   
		return;
	}
	imshow("oringinimag", Oringinimag);  // 显示图片 
	waitKey();

	int height = Oringinimag.rows;
	int width = Oringinimag.cols;

	Mat AfterFilterring;
	AfterFilterring.create(height, width, CV_8UC3);//CV_8UC1 8为无符号整型 单通道

	Vec3b* poringinimag;
	Vec3b* pAfterFilterring;
	
	//采用边缘复制，只对中间部分滤波
	for (int i = 0; i < height; ) {
		poringinimag = Oringinimag.ptr<Vec3b>(i);
		pAfterFilterring = AfterFilterring.ptr<Vec3b>(i);
		for (int j = 0; j < width; j++) {
			pAfterFilterring[j][0] = poringinimag[j][0];
			pAfterFilterring[j][1] = poringinimag[j][1];
			pAfterFilterring[j][2] = poringinimag[j][2];
			i = i + height - 1;
		}

	}
	for (int i = 0; i < height; i++) {
		poringinimag = Oringinimag.ptr<Vec3b>(i);
		pAfterFilterring = AfterFilterring.ptr<Vec3b>(i);
		for (int j = 0; j < width; ) {
			pAfterFilterring[j][0] = poringinimag[j][0];
			pAfterFilterring[j][1] = poringinimag[j][1];
			pAfterFilterring[j][2] = poringinimag[j][2];
			j = j + width - 1;
		}

	}
	//采用拉普拉斯增强算子
	double HighPasscore[3][3] =
	{ {0.0,-1.0,0.0},
	  {-1.0,5.0,-1.0},
	  {0.0,-1.0,0.0} };

	for (int i = 1; i < height - 1; i++) {
		for (int j = 1; j < width - 1; j++) {
			for (int k = 0; k < 3; k++) {
				AfterFilterring.at<Vec3b>(i, j)[k] = (
					Oringinimag.at<Vec3b>(i - 1, j - 1)[k] * HighPasscore[0][0]
					+ Oringinimag.at<Vec3b>(i - 1, j)[k] * HighPasscore[0][1]
					+ Oringinimag.at<Vec3b>(i - 1, j + 1)[k] * HighPasscore[0][2]
					+ Oringinimag.at<Vec3b>(i, j - 1)[k] * HighPasscore[1][0]
					+ Oringinimag.at<Vec3b>(i, j)[k] * HighPasscore[1][1]
					+ Oringinimag.at<Vec3b>(i, j + 1)[k] * HighPasscore[1][2]
					+ Oringinimag.at<Vec3b>(i + 1, j - 1)[k] * HighPasscore[2][0]
					+ Oringinimag.at<Vec3b>(i + 1, j)[k] * HighPasscore[2][1]
					+ Oringinimag.at<Vec3b>(i + 1, j + 1)[k] * HighPasscore[2][2]
					);
			}
		}
	}
	imshow("高通滤波后图像", AfterFilterring);  // 显示图片 
	waitKey();

	imwrite("C:\\Users\\江袅\\Desktop\\高通滤波处理后图像.bmp", AfterFilterring);
}
//低通滤波
void LowPassFilter()
{
	char imageName[] = "C:\\Users\\江袅\\Desktop\\1.bmp";
	Mat Oringinimag = imread(imageName, IMREAD_COLOR);   // 读入图片 

	if (Oringinimag.empty())     // 判断文件是否正常打开  
	{
		fprintf(stderr, "Can not load image %s\n", imageName);
		waitKey(6000);  // 等待6000 ms后窗口自动关闭   
		return;
	}
	imshow("oringinimag", Oringinimag);  // 显示图片 
	waitKey();

	int height = Oringinimag.rows;
	int width = Oringinimag.cols;

	Mat AfterFilterring;
	AfterFilterring.create(height, width, CV_8UC3);//CV_8UC1 8为无符号整型 单通道

	Vec3b* poringinimag;
	Vec3b* pAfterFilterring;

	//采用边缘复制，只对中间部分滤波
	for (int i = 0; i < height; ) {
		poringinimag = Oringinimag.ptr<Vec3b>(i);
		pAfterFilterring = AfterFilterring.ptr<Vec3b>(i);
		for (int j = 0; j < width; j++) {
			pAfterFilterring[j][0] = poringinimag[j][0];
			pAfterFilterring[j][1] = poringinimag[j][1];
			pAfterFilterring[j][2] = poringinimag[j][2];
			i = i + height - 1;
		}

	}
	for (int i = 0; i < height; i++) {
		poringinimag = Oringinimag.ptr<Vec3b>(i);
		pAfterFilterring = AfterFilterring.ptr<Vec3b>(i);
		for (int j = 0; j < width; ) {
			pAfterFilterring[j][0] = poringinimag[j][0];
			pAfterFilterring[j][1] = poringinimag[j][1];
			pAfterFilterring[j][2] = poringinimag[j][2];
			j = j + width - 1;
		}

	}
	
	double LowPasscore[3][3] =
	{ {0.0625,0.125,0.0625},
	  {0.125,0.25,0.125},
	  {0.0625,0.125,0.0625} };

	for (int i = 1; i < height - 1; i++) {
		for (int j = 1; j < width - 1; j++) {
			for (int k = 0; k < 3; k++) {
				AfterFilterring.at<Vec3b>(i, j)[k] = (
					Oringinimag.at<Vec3b>(i - 1, j - 1)[k] * LowPasscore[0][0]
					+ Oringinimag.at<Vec3b>(i - 1, j)[k] * LowPasscore[0][1]
					+ Oringinimag.at<Vec3b>(i - 1, j + 1)[k] * LowPasscore[0][2]
					+ Oringinimag.at<Vec3b>(i, j - 1)[k] * LowPasscore[1][0]
					+ Oringinimag.at<Vec3b>(i, j)[k] * LowPasscore[1][1]
					+ Oringinimag.at<Vec3b>(i, j + 1)[k] * LowPasscore[1][2]
					+ Oringinimag.at<Vec3b>(i + 1, j - 1)[k] * LowPasscore[2][0]
					+ Oringinimag.at<Vec3b>(i + 1, j)[k] * LowPasscore[2][1]
					+ Oringinimag.at<Vec3b>(i + 1, j + 1)[k] * LowPasscore[2][2]
					);
			}
		}
	}
	imshow("低通滤波后图像", AfterFilterring);  // 显示图片 
	waitKey();

	imwrite("C:\\Users\\江袅\\Desktop\\低通滤波处理后图像.bmp", AfterFilterring);

}
//中值滤波
void MedianFilter()
{
	char imageName[] = "C:\\Users\\江袅\\Desktop\\中值滤波.png";
	Mat oringinimag = imread(imageName, IMREAD_COLOR);   // 读入图片 

	if (oringinimag.empty())     // 判断文件是否正常打开  
	{
		fprintf(stderr, "Can not load image %s\n", imageName);
		waitKey(6000);  // 等待6000 ms后窗口自动关闭   
		return;
	}
	imshow("oringinimag", oringinimag);  // 显示图片 
	waitKey();

	int height = oringinimag.rows;
	int width = oringinimag.cols;

	
	Mat AfterFilterring;
	AfterFilterring.create(height, width, CV_8UC3);//CV_8UC1 8为无符号整型 单通道

	Vec3b* poringinimag;
	Vec3b* pAfterFilterring;
	//采用边缘复制，只对中间部分滤波
	for (int i = 0; i < height; ) {
		poringinimag = oringinimag.ptr<Vec3b>(i);
		pAfterFilterring = AfterFilterring.ptr<Vec3b>(i);
		for (int j = 0; j < width; j++) {
			pAfterFilterring[j][0] = poringinimag[j][0];
			pAfterFilterring[j][1] = poringinimag[j][1];
			pAfterFilterring[j][2] = poringinimag[j][2];
			i = i + height - 1;
		}

	}
	for (int i = 0; i < height; i++) {
		poringinimag = oringinimag.ptr<Vec3b>(i);
		pAfterFilterring = AfterFilterring.ptr<Vec3b>(i);
		for (int j = 0; j < width; ) {
			pAfterFilterring[j][0] = poringinimag[j][0];
			pAfterFilterring[j][1] = poringinimag[j][1];
			pAfterFilterring[j][2] = poringinimag[j][2];
			j = j + width - 1;
		}

	}
	//M.at<Vec3b>(i, j)[0]  直接访问
	for (int i = 1; i < height-1; i++) {
	   for (int j = 1; j < width-1;j++ ) {
		   for (int k=0;k<3;k++){
		double a[9];
		a[0] = oringinimag.at<Vec3b>(i - 1, j - 1)[k];
		a[1] = oringinimag.at<Vec3b>(i - 1, j    )[k];
		a[2] = oringinimag.at<Vec3b>(i - 1, j + 1)[k];
		a[3] = oringinimag.at<Vec3b>(i    , j - 1)[k];
		a[4] = oringinimag.at<Vec3b>(i    , j    )[k];
		a[5] = oringinimag.at<Vec3b>(i    , j + 1)[k];
		a[6] = oringinimag.at<Vec3b>(i + 1, j - 1)[k];
		a[7] = oringinimag.at<Vec3b>(i + 1, j    )[k];
		a[8] = oringinimag.at<Vec3b>(i + 1, j + 1)[k];
		sort(a);
		AfterFilterring.at<Vec3b>(i, j)[k] = a[4];
		   }
	   }
	}
	imshow("中值滤波后图像", AfterFilterring);  // 显示图片 
	waitKey();

	imwrite("C:\\Users\\江袅\\Desktop\\中值滤波处理后图像.bmp", AfterFilterring);
}
//几合处理
void GeometricProcessing() {
	char imageName[] = "C:\\Users\\江袅\\Desktop\\1.bmp";
	Mat Oringinimag = imread(imageName, IMREAD_COLOR);   // 读入图片 

	if (Oringinimag.empty())     // 判断文件是否正常打开  
	{
		fprintf(stderr, "Can not load image %s\n", imageName);
		waitKey(6000);  // 等待6000 ms后窗口自动关闭   
		return;
	}
	imshow("Oringinimag", Oringinimag);  // 显示图片 
	waitKey();

	int height = Oringinimag.rows;
	int width = Oringinimag.cols;
	NEW:
	int choice = 0;
	cout << "请选择希望的功能 1 平移  2 缩放  3 旋转  0 退出\n";
	cin >> choice;

	if ((choice < 0) && (choice > 3))
	{
		cout << "请正确输入要实现的功能！\n";
		goto NEW;
	}
	else if (choice == 0)
		goto END;
	else if (choice == 1)
	{
		//平移
		int x0 = 0, y0 = 0;
		cout << "请输入水平方向上要移动的距离（左移为正右移为负）：";
		cin >> x0;
		cout << "请输入竖直方向上要移动的距离（上移为正下移为负）：";
		cin >> y0;
		Mat ImgMove;
		ImgMove.create(height, width, CV_8UC3);
		for (int i = 0; i < height; i++)
		{
			int x = i + x0;
			for (int j = 0; j < width; j++)
			{
				int y = j + y0;
				if (0 < x && x < height && 0 < y && y < width)
				{
					for (int k = 0; k < 3; k++)
						ImgMove.at<Vec3b>(i , j)[k] = Oringinimag.at<Vec3b>(x , y)[k];
				}
				else
				{
					for (int k = 0; k < 3; k++)
						ImgMove.at<Vec3b>(i, j)[k] = 0;
				}
			}
		}
		imshow("原图", Oringinimag);
		imshow("平移", ImgMove);
		waitKey();
		imwrite("C:\\Users\\江袅\\Desktop\\平移图像.bmp", ImgMove);
		goto END;
	}

	else if (choice == 2)
	{
		//缩放
		double r;
		cout << "请输入要缩放到原图的多少？（小数表示）：";
		cin >> r;
		//本身是向下取整，加0.5达到四舍五入
		int newheight = height * r + 0.5;//新高
		int newwidth = width * r + 0.5;//新宽

		Mat ImgChange;
		ImgChange.create(newheight, newwidth, CV_8UC3);
		for (int i = 0; i < newheight; i++)
		{
			int i0 = i / r + 0.5;
			for (int j = 0; j < newwidth; j++)
			{
				int j0 = j / r + 0.5;
				if (0 <= i0 && i0 < height && 0 <= j0 && j0 < width)
				{
					for (int k = 0; k < 3; k++)
						ImgChange.at<Vec3b>(i, j)[k] = Oringinimag.at<Vec3b>(i0, j0)[k];
					
				}
				else
				{
					for (int k = 0; k < 3; k++)
					ImgChange.at<Vec3b>(i, j)[k] = 0;
				}
			}
		}
		
		
	imshow("原图", Oringinimag);
	imshow("缩放", ImgChange);
	waitKey();
	imwrite("C:\\Users\\江袅\\Desktop\\缩放图像.bmp", ImgChange);
    goto END;
	}

	else if (choice == 3)
	{   //旋转
	    float angle0;
		cout << "请输入旋转角度（顺时针）：";
		cin >> angle0;
		double angle, PI = 3.1415926;
		angle = PI * angle0 / 180;
		int newwidth;//旋转后的宽度
		int newheight;//旋转后的高度
		float  fSrcX1, fSrcY1, fSrcX2, fSrcY2, fSrcX3, fSrcY3, fSrcX4, fSrcY4;
		//计算原图像四个角的坐标(以图像中心为坐标系原点)
		fSrcX1 = -(width - 1) / 2;fSrcY1 =  (height - 1) / 2;
		fSrcX2 =  (width - 1) / 2;fSrcY2 =  (height - 1) / 2;
		fSrcX3 = -(width - 1) / 2;fSrcY3 = -(height - 1) / 2;
		fSrcX4 =  (width - 1) / 2;fSrcY4 = -(height - 1) / 2;

		float fDstX1, fDstY1, fDstX2, fDstY2, fDstX3, fDstY3, fDstX4, fDstY4;
		//以图像中心为原点旋转后四个角的坐标
		fDstX1 = cos(angle) * fSrcX1 + sin(angle) * fSrcY1;
		fDstY1 = -sin(angle) * fSrcX1 + cos(angle) * fSrcY1;
		fDstX2 = cos(angle) * fSrcX2 + sin(angle) * fSrcY2;
		fDstY2 = -sin(angle) * fSrcX2 + cos(angle) * fSrcY2;
		fDstX3 = cos(angle) * fSrcX3 + sin(angle) * fSrcY3;
		fDstY3 = -sin(angle) * fSrcX3 + cos(angle) * fSrcY3;
		fDstX4 = cos(angle) * fSrcX4 + sin(angle) * fSrcY4;
		fDstY4 = -sin(angle) * fSrcX4 + cos(angle) * fSrcY4;
		//旋转后图像大小参数
		newwidth = max(fabs(fDstX4 - fDstX1), fabs(fDstX3 - fDstX2)) + 0.5;
		newheight = max(fabs(fDstY4 - fDstY1), fabs(fDstY3 - fDstY2)) + 0.5;
		//两个常数避免后续计算
		float f1, f2;
		f1 = -0.5 * (newwidth - 1) * cos(angle) - 0.5 * (newheight - 1) * sin(angle) + 0.5 * (width - 1);
		f2 = 0.5 * (newwidth - 1) * sin(angle) - 0.5 * (newheight - 1) * cos(angle) + 0.5 * (height - 1);

		Mat ImgRevolve;
		ImgRevolve.create(newheight, newwidth, CV_8UC3);
		int x0, y0;
		for (int x = 0; x < newheight; x++)
		{
			for (int y = 0; y < newwidth; y++)
			{
				//该像素在原图像中的坐标
				x0 = -y * sin(angle) + x * cos(angle) + f2 + 0.5;
				y0 = y * cos(angle) + x * sin(angle) + f1 + 0.5;
				if ((y0 >= 0) && (y0 < width) && (x0 >= 0) && (x0 < height))
				{
					for (int k = 0; k < 3; k++)
						ImgRevolve.at<Vec3b>(x, y)[k] = Oringinimag.at<Vec3b>(x0, y0)[k];

				}
				else
				{
					for (int k = 0; k < 3; k++)
						ImgRevolve.at<Vec3b>(x, y)[k] = 0;
				}
			}
		}
		imshow("原图", Oringinimag);
		imshow("旋转", ImgRevolve);
		waitKey();
		imwrite("C:\\Users\\江袅\\Desktop\\旋转图像.bmp", ImgRevolve);
		goto END;

	}


	END:
	return;
}
//二值化
void Binarization()
{
	char imageName[] = "C:\\Users\\江袅\\Desktop\\1.bmp";
	Mat oringinimag = imread(imageName, IMREAD_COLOR);   // 读入图片 

	if (oringinimag.empty())     // 判断文件是否正常打开  
	{
		fprintf(stderr, "Can not load image %s\n", imageName);
		waitKey(6000);  // 等待6000 ms后窗口自动关闭   
		return;
	}
	imshow("oringinimag", oringinimag);  // 显示图片 
	waitKey();

	int height = oringinimag.rows;
	int width = oringinimag.cols;


	//将图片转化为灰度图像
	Mat imgGray;
	imgGray.create(height, width, CV_8UC1);//CV_8UC1 8为无符号整型 单通道
	Vec3b* poringinimag;
	uchar* pimgGray;
	for (int i = 0; i < height; i++) {
		poringinimag = oringinimag.ptr<Vec3b>(i);
		pimgGray = imgGray.ptr<uchar>(i);
		for (int j = 0; j < width; j++) {
			pimgGray[j] = poringinimag[j][0] * 0.114 + poringinimag[j][1] * 0.587 + poringinimag[j][2] * 0.299;//一个心理学公式			
		}

	}
	imshow("灰度图像", imgGray);
	waitKey();



}

int main()
{
   // GrayLinerTransform();
	//HighPassFilter();
	//LowPassFilter();
	//MedianFilter();
	//GeometricProcessing();
    return 0;
}
