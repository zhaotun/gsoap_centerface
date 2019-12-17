# gsoap_centerface
a webservice demo with gsoap.

本项目通过 gsoap 编写 webservice 服务端、客户端程序，在服务端利用 opencv_4.1.2 的DNN模块加载 onnx 格式的 centerface 人脸检测模型，共提供两个接口函数，可分别接收 图片路径或base64格式的图片，人脸检测结果以结构体的形式返回。详细内容可参考 __[gsoap 创建服务端、客户端进行 centerface 人脸检测.pdf](https://github.com/zhaotun/gsoap_centerface/blob/master/gsoap%20创建服务端、客户端进行%20centerface%20人脸检测.pdf)__
