//gsoap ns service name:	face
//gsoap ns service style:	rpc
//gsoap ns service encoding:	encoded

#include<iostream>

struct SingleFaceInfo {
	float x1;
	float y1;
	float x2;
	float y2; 
	float score;
	float landmarks[10];
};

struct FaceRes {
	int face_count;
	struct SingleFaceInfo single_face_info[50];
};

int ns__FaceDetectByImgpath(std::string img_path,  struct FaceRes *face_res);
int ns__FaceDetectByBase64 (std::string img_base64,struct FaceRes *face_res);