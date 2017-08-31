/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
#include "CMImgProc.h" 
/* Header for class NativeDemo */

#ifndef _Included_NativeDemo
#define _Included_NativeDemo
#ifdef __cplusplus
extern "C" {
#endif


/**
* 初始化seetaface人脸识别库
* 参数：
* vFaceModelDir:人脸识别模型文件所在目录，如/sdcard/
*/
	JNIEXPORT jboolean JNICALL Java_utryface_UtryFace_initModelPath(
	JNIEnv *env, jobject obj, jstring vFaceModelDir); 
 
/*
 * Class:     NativeDemo
 * Method:    test
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_utryface_UtryFace_test
  (JNIEnv *, jobject);

/*
*计算2个人脸的相似度
*/
JNIEXPORT jfloat JNICALL Java_utryface_UtryFace_CalcSimilarity
(JNIEnv *env, jobject obj, jfloatArray vFeat1, jfloatArray vFeat2);

///**
// * 检测人脸
// * jbyteArray v_img_data,图像数据
// * jint cols, 图像宽度
// *  jint rows,图像高度
// *  jobject vFaceBmp返回的一个人脸抠图
//jbyteArray v_img_data,
JNIEXPORT jobjectArray JNICALL Java_utryface_UtryFace_DetectFacesBytes(JNIEnv *env, jobject obj,
	jbyteArray vImgByte, jint vWidth, jint vHeight);

#ifdef __cplusplus
}
#endif
#endif
