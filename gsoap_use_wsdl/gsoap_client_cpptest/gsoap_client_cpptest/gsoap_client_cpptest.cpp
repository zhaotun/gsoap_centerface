#include "stdio.h"
#include<iostream>

#include "face.nsmap"
//#include "soapH.h"
#include "soapfaceProxy.h" 

#include "base64/base64.h"

#include <opencv2/opencv.hpp>

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

	printf("The Client is runing...\n");
	
	//struct soap *CalculateSoap = soap_new();
	////soap_init(CalculateSoap);
	//char * server_addr = "http://localhost:8080";


	/*************** FaceDetectByImgpath ***************/
	ns2__FaceDetectByImgpathResponse faceRes;

	clock_t startTime, endTime;
	startTime = clock();//计时开始
	//int iRet = soap_call_ns2__FaceDetectByImgpath(CalculateSoap, server_addr, "", image_file, faceRes);
	int iRet = face.FaceDetectByImgpath(image_file, faceRes);
	endTime = clock();//计时结束
	cout << "FaceDetectByImgpath time is: " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;

	// 显示结果
	if (iRet == SOAP_ERR)
	{
		printf("Error while calling the soap_call_ns__sub");
	}
	else
	{
		cout << "_______FaceDetectByImgpath face_res.face_count = " << faceRes.face_res->item.size() << endl;
		
		int faceNum = faceRes.face_res->item.size();	
		// 开始绘制检测结果
		Mat image = imread(image_file);
		for (int i = 0; i <  faceNum; i++)
		{
			//float a = faceRes.face_res->item[i]->x1;
			// 在原图画人脸框
			cv::rectangle(image, cv::Point(faceRes.face_res->item[i]->x1, faceRes.face_res->item[i]->y1), cv::Point(faceRes.face_res->item[i]->x2, faceRes.face_res->item[i]->y2), cv::Scalar(0, 255, 0), 2);
			// 在原图画关键点
			for (int j = 0; j < 5; j++)
			{
				//float t = faceRes.face_res->item[i]->landmarks->__ptritem[2 * j];
				cv::circle(image, cv::Point(faceRes.face_res->item[i]->landmarks->__ptritem[2 * j], faceRes.face_res->item[i]->landmarks->__ptritem[2 * j + 1]), 2, cv::Scalar(255, 255, 0), 2);
			}
		}
		cv::imwrite(image_file + "_path.jpg", image);
	}

	/*************** FaceDetectByBase64 ***************/
	// bytes to base64
	Mat img = imread(image_file);
	std::vector<uchar> buf;
	cv::imencode(".jpg", img, buf);// Mat to bytes 
	uchar *enc_msg = reinterpret_cast<unsigned char*>(buf.data());
	std::string img_base64 = base64_encode(enc_msg, buf.size());// bytes to base64

	ns2__FaceDetectByBase64Response faceResBase64;
	startTime = clock();//计时开始
	//iRet = soap_call_ns2__FaceDetectByBase64(CalculateSoap, server_addr, "", img_base64, faceResBase64);
	iRet = face.FaceDetectByBase64(img_base64, faceResBase64);
	endTime = clock();//计时结束
	cout << "FaceDetectByBase64 time is: " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;

	// 显示结果
	if (iRet == SOAP_ERR)
	{
		printf("Error while calling the soap_call_ns__sub");
	}
	else
	{
		cout << "_______FaceDetectByBase64 face_res.face_count = " << faceResBase64.face_res->item.size() << endl;

		int faceNum = faceResBase64.face_res->item.size();
		// 开始绘制检测结果
		Mat image = imread(image_file);
		for (int i = 0; i < faceNum; i++)
		{
			//float a = faceRes.face_res->item[i]->x1;
			// 在原图画人脸框
			cv::rectangle(image, cv::Point(faceResBase64.face_res->item[i]->x1, faceResBase64.face_res->item[i]->y1), cv::Point(faceResBase64.face_res->item[i]->x2, faceResBase64.face_res->item[i]->y2), cv::Scalar(0, 255, 0), 2);
			// 在原图画关键点
			for (int j = 0; j < 5; j++)
			{
				//float t = faceRes.face_res->item[i]->landmarks->__ptritem[2 * j];
				cv::circle(image, cv::Point(faceResBase64.face_res->item[i]->landmarks->__ptritem[2 * j], faceResBase64.face_res->item[i]->landmarks->__ptritem[2 * j + 1]), 2, cv::Scalar(255, 255, 0), 2);
			}
		}
		cv::imwrite(image_file + "_base64.jpg", image);
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