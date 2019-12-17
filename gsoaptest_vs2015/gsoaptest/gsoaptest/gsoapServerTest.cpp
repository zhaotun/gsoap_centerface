#include "stdio.h"

//#include "soapH.h"
#include "face.nsmap"
#include "soapfaceService.h"

// multi-threaded
//#include "plugin/threads.h" // multi-threaded, need to add threads.h threads.cpp
                            // plugin is in gsoap-2.8/gsoap/pligin

#include <iostream>
#include <opencv2/opencv.hpp>
#include "cv_dnn_centerface.h"
#include "base64.h"

using namespace cv;
using namespace std;

//Centerface centerface("models/onnx/centerface.onnx");

int port = 8080;
string model = "models/onnx/centerface_bnmerged.onnx";

// multi-threaded
//void *process_request(void *arg)
//{
//	faceService *service = (faceService*)arg;
//
//	THREAD_DETACH(THREAD_ID);
//	if (service)
//	{
//		service->serve();
//		service->destroy(); /* clean up */
//		delete service;
//	}
//	return NULL;
//}

int main(int argc, char **argv)
{
	// multi-threaded
	//faceService service(SOAP_IO_KEEPALIVE);                      /* enable HTTP kee-alive */
	//service.soap->send_timeout = service.soap->recv_timeout = 5; /* 5 sec socket idle timeout */
	//service.soap->transfer_timeout = 30;                         /* 30 sec message transfer timeout */
	////faceService::bind(const char *host, int port, int backlog)
	//SOAP_SOCKET m = service.bind(NULL, port, 100);              /* master socket */
	//if (soap_valid_socket(m))
	//{
	//	while (soap_valid_socket(service.accept()))
	//	{
	//		THREAD_TYPE tid;
	//		void *arg = (void*)service.copy();
	//		/* use updated THREAD_CREATE from plugin/threads.h https://www.genivia.com/files/threads.zip */
	//		if (arg)
	//			while (THREAD_CREATE(&tid, (void*(*)(void*))process_request, arg))
	//				Sleep(1);
	//	}
	//}
	//service.soap_stream_fault(std::cerr);
	//service.destroy(); /* clean up */
	//return 0;
	
	faceService service;
	service.soap->send_timeout = service.soap->recv_timeout = 5; // 5 sec socket idle timeout
	service.soap->transfer_timeout = 30;                         // 30 sec message transfer timeout
	while (service.run(port))
		service.soap_stream_fault(std::cerr);
	service.destroy();
	return 0;
		
	//int nPort = 8080;
	//struct soap fun_soap;
	//soap_init(&fun_soap);
	//int nMaster = (int)soap_bind(&fun_soap, NULL, nPort, 100);
	//if (nMaster < 0)
	//{
	//	soap_print_fault(&fun_soap, stderr);
	//	exit(-1);
	//}
	//fprintf(stderr, "Socket connection successful : master socket = %d\n", nMaster);
	//while (true)
	//{
	//	int nSlave = (int)soap_accept(&fun_soap);
	//	if (nSlave < 0)
	//	{
	//		soap_print_fault(&fun_soap, stderr);
	//		exit(-1);
	//	}
	//	fprintf(stderr, "Socket connection successful : slave socket = %d\n", nSlave);
	//	soap_serve(&fun_soap);
	//	soap_end(&fun_soap);
	//}
	//return 0;
}

/*通过Base64进行人脸检测，返回结构体*/
//int ns__FaceDetectByBase64(struct soap *soap, std::string img_base64, struct FaceRes *face_res)
int faceService::FaceDetectByBase64(const std::string& img_base64, struct FaceRes *face_res)
{
	//if (NULL == faceCount)
	//{
	//	printf("Error:The third argument should not be NULL!\n");
	//	return SOAP_ERR;
	//}
	//else
	//{

	Centerface centerface(model);

	// base64 to mat
	std::string img_decode = base64_decode(img_base64);
	std::vector<uchar> data(img_decode.begin(), img_decode.end());
	cv::Mat img_mat = cv::imdecode(cv::Mat(data), 1);

	//cv::Mat image = cv::imread(img_base64);
	std::vector<FaceInfo> face_info;
	centerface.detect(img_mat, face_info, cv::Size(img_mat.cols, img_mat.rows));

	// 人脸数目
	//face_res->face_count = face_info.size();
	face_res->__size = face_info.size();
	face_res->__ptr  = (struct ns__SingleFaceInfo**)malloc( (face_res->__size + 1) * sizeof(struct ns__SingleFaceInfo) );	

	// 保存每一个人脸信息
	for (int i = 0; i < face_info.size(); i++) {
		// 人脸框

		face_res->__ptr[i] = (struct ns__SingleFaceInfo*)soap_malloc(soap, sizeof(struct ns__SingleFaceInfo));

		face_res->__ptr[i]->x1 = face_info[i].x1;
		face_res->__ptr[i]->y1 = face_info[i].y1;
		face_res->__ptr[i]->x2 = face_info[i].x2;
		face_res->__ptr[i]->y2 = face_info[i].y2;
		// 人脸置信度
		face_res->__ptr[i]->score = face_info[i].score;
		// 人脸关键点
		for (int j = 0; j < 10; j++)
			face_res->__ptr[i]->landmarks[j] = face_info[i].landmarks[j];
	}
	return SOAP_OK;
	//}
	//return SOAP_OK;
}

/*通过图片路径进行人脸检测，返回结构体*/
//int ns__FaceDetectByImgpath(struct soap *soap, std::string imgpath, struct FaceRes *face_res)
int faceService::FaceDetectByImgpath(const std::string& imgpath, struct FaceRes *face_res)
{
	//if (NULL == faceCount)
	//{
	//	printf("Error:The third argument should not be NULL!\n");
	//	return SOAP_ERR;
	//}
	//else
	//{

	//Centerface centerface(modelpath);
	Centerface centerface(model);
	cv::Mat image = cv::imread(imgpath);
	std::vector<FaceInfo> face_info;
	centerface.detect(image, face_info, cv::Size(image.cols, image.rows));

	// 人脸数目
	//face_res->face_count = face_info.size();
	face_res->__size = face_info.size();
	face_res->__ptr  = (struct ns__SingleFaceInfo**)malloc((face_res->__size + 1) * sizeof(struct ns__SingleFaceInfo));

	// 保存每一个人脸信息
	for (int i = 0; i < face_info.size(); i++) {
		// 人脸框
		face_res->__ptr[i] = (struct ns__SingleFaceInfo*)soap_malloc(soap, sizeof(struct ns__SingleFaceInfo));

		face_res->__ptr[i]->x1 = face_info[i].x1;
		face_res->__ptr[i]->y1 = face_info[i].y1;
		face_res->__ptr[i]->x2 = face_info[i].x2;
		face_res->__ptr[i]->y2 = face_info[i].y2;
		// 人脸置信度
		face_res->__ptr[i]->score = face_info[i].score;
		// 人脸关键点
		for (int j = 0; j < 10; j++)
			face_res->__ptr[i]->landmarks[j] = face_info[i].landmarks[j];
	}
	return SOAP_OK;
	//}
	//return SOAP_OK;
}

