// gClient.cpp : 定义控制台应用程序的入口点。
#include "stdio.h"

#include "soapfaceProxy.h"
#include "face.nsmap"
//#include "soapH.h"

#include "base64.h"
#include<ctime>

#include <opencv2/opencv.hpp>//用于绘制结果

using namespace std;
using namespace cv; 

const char server[] = "http://localhost:8080";

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cout << "usage: .exe image_path" << std::endl;
		return -1;
	}

	faceProxy face(server);

	std::string image_file = argv[1];
	int faceCount = 0;

	printf("The Client is runing...\n");

	//struct soap *CalculateSoap = soap_new();
	////soap_init(CalculateSoap);
	//char * server_addr = "http://localhost:8080";


	/*************** FaceDetectByImgpath ***************/
	
	clock_t startTime, endTime;
	startTime = clock();//计时开始

	FaceRes face_res;
	//int iRet = soap_call_ns__FaceDetectByImgpath(CalculateSoap, server_addr, "", image_file, &face_res);
	int iRet = face.FaceDetectByImgpath(image_file, &face_res);

	endTime = clock();//计时结束
	cout << "FaceDetectByImgpath time is: " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	
	// 显示结果
	if (iRet == SOAP_ERR)
	{
		printf("Error while calling the soap_call_ns__sub");
	}
	else
	{
		cout << "_______FaceDetectByImgpath face_res.face_count = " << face_res.__size << endl;
		// 开始绘制检测结果
		Mat image = imread(image_file);
		for (int i = 0; i < face_res.__size; i++)
		{
			// 在原图画人脸框
			cv::rectangle(image, cv::Point(face_res.__ptr[i]->x1, face_res.__ptr[i]->y1), cv::Point(face_res.__ptr[i]->x2, face_res.__ptr[i]->y2), cv::Scalar(0, 255, 0), 2);
			// 在原图画关键点
			for (int j = 0; j < 5; j++)
			{
				cv::circle(image, cv::Point(face_res.__ptr[i]->landmarks[2 * j], face_res.__ptr[i]->landmarks[2 * j + 1]), 2, cv::Scalar(255, 255, 0), 2);
			}
		}
		cv::imwrite(image_file + "_path.jpg", image);
	}

	/*************** FaceDetectByBase64 ***************/
	Mat img = imread(image_file);
	std::vector<uchar> buf;
	cv::imencode(".jpg", img, buf);// Mat to bytes 
	uchar *enc_msg = reinterpret_cast<unsigned char*>(buf.data());	
	std::string img_base64 = base64_encode(enc_msg, buf.size());// bytes to base64

	startTime = clock();//计时开始
	//iRet = soap_call_ns__FaceDetectByBase64(CalculateSoap, server_addr, "", img_base64, &face_res);
	iRet = face.FaceDetectByBase64(img_base64, &face_res);
	
	
	endTime = clock();//计时结束
	cout << "FaceDetectByBase64 time is: " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;

	// 显示结果
	if (iRet == SOAP_ERR)
	{
		printf("Error while calling the soap_call_ns__sub");
	}
	else
	{
		cout << "_______FaceDetectByBase64 face_res.face_count = " << face_res.__size << endl;
		//// 开始绘制检测结果
		Mat image2 = imread(image_file);
		for (int i = 0; i <  face_res.__size ; i++)
		{
			// 在原图画人脸框
			cv::rectangle(image2, cv::Point(face_res.__ptr[i]->x1, face_res.__ptr[i]->y1), cv::Point(face_res.__ptr[i]->x2, face_res.__ptr[i]->y2), cv::Scalar(0, 255, 0), 2);
			// 在原图画关键点
			for (int j = 0; j < 5; j++)
			{
				cv::circle(image2, cv::Point(face_res.__ptr[i]->landmarks[2 * j], face_res.__ptr[i]->landmarks[2 * j + 1]), 2, cv::Scalar(255, 255, 0), 2);
			}
		}
		cv::imwrite(image_file + "_base64.jpg", image2);
	}

	//soap_end(CalculateSoap); 
	//soap_done(CalculateSoap);
	//free(CalculateSoap);

	if (face.soap->error)
		face.soap_stream_fault(std::cerr);
	//else
		//std::cout << "result = " << result << std::endl;
	face.destroy(); /* clean up */

	return 0;
}
