#include "NativeDemo.h"
#include <stdio.h>
#include <string>

using namespace std; 

#include "FaceDetection/include/common.h"
#include "FaceDetection/include/face_detection.h"
#include "FaceDetection/include/face_detection.h"
#include "FaceAlignment/include/face_alignment.h"
#include "FaceIdentification/include/Face_identification.h"

using namespace seeta;

#ifdef __cplusplus
extern "C" {
#endif

	//人脸特征float数组长度
#define	SEETAFACE_FEATURE_NUM 2048

	//sdk是否初始化成功
	static bool sb_sdk_init_ok = false;


	//重要：需要java端调用init()来初始化模型路径
	static seeta::FaceDetection detector(NULL);
	static seeta::FaceAlignment point_detector(NULL);
	static seeta::FaceIdentification face_recognizer(NULL);
	static	jmethodID m_mid;

	//人脸框
	static	jfieldID tLeftField = NULL, tRightField = NULL, tTopField = NULL, tBottomField = NULL;

	//特征点数组,眼睛鼻子嘴巴
	static	jfieldID tLandmarkField = NULL;

	//特征数组
	static	jfieldID tFeaturesField = NULL;
	
	//旋转角
	static jfieldID tRollField = NULL;

	//偏航角
	static jfieldID tYawField = NULL;

	//俯仰角
	static jfieldID tPitchField = NULL;


	
	/**
	* 初始化seetaface人脸识别库
	* 参数：
	* vFaceModelDir:人脸识别模型文件所在目录，如/sdcard/
	*/
	JNIEXPORT jboolean JNICALL Java_utryface_UtryFace_initModelPath(JNIEnv *env, jobject obj, jstring vFaceModelDir)
	{
		//如果已初始化则直接返回
		if (sb_sdk_init_ok){
			return true;
		}

		jboolean tRet = false;
		if (NULL == vFaceModelDir) {
			return tRet;
		}

		//获取正面人脸检测模型的绝对路径的目录（不是/aaa/bbb.bin这样的路径，是/aaa/)
		const char* tDir = env->GetStringUTFChars(vFaceModelDir, 0);
		if (NULL == tDir) {
			return tRet;
		}
		LOGD("init, tDir=%s\n", tDir);

		string tFaceModelDir = tDir;
		string tLastChar = tFaceModelDir.substr(tFaceModelDir.length() - 1, 1);
		LOGD("init, tFaceModelDir last =%s\n", tLastChar.c_str());

		if ("\\" == tLastChar){
			tFaceModelDir = tFaceModelDir.substr(0, tFaceModelDir.length() - 1) + "/";
		}
		else if (tLastChar != "/"){
			//目录补齐/
			tFaceModelDir += "/";
		}

		LOGD("init, tFaceModelDir=%s\n", tFaceModelDir.c_str());

		//检测模型路径
		string tDetectModelPath = tFaceModelDir + "d_v1.0.bin";
		//LOGD("init,  tDetectModelPath=%s", tDetectModelPath.c_str());

		//对齐模型路径
		string tAlignModelPath = tFaceModelDir + "a_v1.1.bin";
		//LOGD("init,  tAlignModelPath=%s", tAlignModelPath.c_str());

		//识别模型路径
		string tRecoModelPath = tFaceModelDir + "r_v1.0.bin";
		//LOGD("init,  tRecoModelPath=%s", tRecoModelPath.c_str());

		//3个模型文件都要存在，否则初始化失败
		FILE* fp; // = fopen(tDetectModelPath.c_str(), "r");
		errno_t tErrOpenFile;
		tErrOpenFile = fopen_s(&fp, tDetectModelPath.c_str(), "r");
		if (0 != tErrOpenFile){
			LOGE("init, detect model not exist, %s\n", tDetectModelPath.c_str());
			return tRet;
		}
		fclose(fp);

		tErrOpenFile = fopen_s(&fp, tAlignModelPath.c_str(), "r");
		if (0 != tErrOpenFile){
			LOGE("init, align model not exist, %s\n", tAlignModelPath.c_str());
			return tRet;
		}

		fclose(fp);

		tErrOpenFile = fopen_s(&fp, tRecoModelPath.c_str(), "r");
		if (0 != tErrOpenFile){
			LOGE("init, face recog model not exist, %s\n", tRecoModelPath.c_str());
			return tRet;
		}
		fclose(fp);

		LOGD("init, all face model exist\n");

		//初始化人脸检测对齐识别器
		//初始化人脸检测对齐识别器
		detector.initWithModel(tDetectModelPath.c_str());
		point_detector.initWithModel(tAlignModelPath.c_str());
		face_recognizer.initWithModel(tRecoModelPath.c_str());

		sb_sdk_init_ok = true;
		tRet = true;
		return tRet;
	}


	/*
	*计算2个人脸的相似度
	*/
	JNIEXPORT jfloat JNICALL Java_utryface_UtryFace_CalcSimilarity(JNIEnv *env, jobject obj, jfloatArray vFeat1, jfloatArray vFeat2)
	{
		//LOGD("CalcSimilarity");
		//如果没有初始化则返回
		if (!sb_sdk_init_ok){
			return -1;
		}

		//long t0 = getMillisec();

		//LOGD("CalcSimilarity,1");
		if (NULL == vFeat1 || NULL == vFeat2){
			return -1;
		}
		float tSim = -1;

		float *tFeat1, *tFeat2;


		tFeat1 = env->GetFloatArrayElements(vFeat1, 0);
		if (0 == tFeat1) {
			return 0;
		}

		tFeat2 = env->GetFloatArrayElements(vFeat2, 0);
		if (0 == tFeat1) {
			return 0;
		}

		//LOGD("CalcSimilarity tFeat2 ok\n");

		// Caculate similarity of two faces
		tSim = face_recognizer.CalcSimilarity(tFeat1, tFeat2);

		//保留2位小数
		tSim = ((int)(100 * tSim)) / 100.0;
		//LOGD("CalcFaceSim, tSim=%.2f",tSim);
		env->ReleaseFloatArrayElements(vFeat1, tFeat1, 0);
		env->ReleaseFloatArrayElements(vFeat2, tFeat2, 0);

		//long t = getMillisec() - t0;
		//LOGD("CalcSimilarity, tSim=%.4f, time=%ld", tSim, t);

		return tSim;
	}

	///**
	// * 检测人脸
	// * jbyteArray vImgByte,图像数据byte格式
	// * jint vWidth, 图像宽度
	// * jint vHeight,图像高度
	JNIEXPORT jobjectArray JNICALL Java_utryface_UtryFace_DetectFacesBytes(JNIEnv *env, jobject obj,
		jbyteArray vImgByte, jint vWidth, jint vHeight)
	{
		printf("DetectFacesByte, 1 vWidth=%d, vHeight=%d \n", vWidth, vHeight);
		if (vWidth < 1 || vHeight < 1){
			return NULL;
		}

		jbyte *tImgData = env->GetByteArrayElements(vImgByte, 0);
		if (NULL == tImgData){
			return NULL;
		}

		int tLen = env->GetArrayLength(vImgByte);

		int tCh = tLen / vWidth / vHeight;
		printf("DetectFacesByte, tImgData leng=%d , tCh = %d\n", tLen, tCh);

		if (3!=tCh && 4!=tCh){
			return NULL;
		}

		//获取人脸对象
		jclass m_cls = env->FindClass("utryface/CMUtryFace");

		if (m_mid == NULL){
			m_mid = env->GetMethodID(m_cls, "<init>", "()V");
			printf("DetectFacesByte, utryface/CMUtryFace 2\n");

			//人脸范围字段
			tLeftField = env->GetFieldID(m_cls, "left", "I");
			tRightField = env->GetFieldID(m_cls, "right", "I");
			tTopField = env->GetFieldID(m_cls, "top", "I");
			tBottomField = env->GetFieldID(m_cls, "bottom", "I");

			//特征点数组,眼睛鼻子嘴巴
			tLandmarkField = env->GetFieldID(m_cls, "landmarks", "[I");

			//特征数组
			tFeaturesField = env->GetFieldID(m_cls, "features", "[F");
	
		}
			//旋转角
			tRollField = env->GetFieldID(m_cls, "roll_angle", "F");

			//偏航角
			tYawField = env->GetFieldID(m_cls, "yaw_angle", "F");

			//俯仰角
			tPitchField = env->GetFieldID(m_cls, "pitch_angle", "F");

			printf("DetectFacesByte, utryface/CMUtryFace 3\n");
	

			//4通道转3通道
			LOGD("DetectFacesByte, rgb_bmp3 new ok\n");
			unsigned char *rgb_bmp3 = new unsigned char[vWidth*vHeight * 3];
			if (4 == tCh){
				//unsigned char *rgb_bmp3 = new unsigned char[vWidth*vHeight * 3];
				CMImgProc::RGBA2RGB((unsigned char*)tImgData, rgb_bmp3, vWidth, vHeight);
				LOGD("DetectFacesByte, channel 4 to 3 ok\n");
			}
			else{
				memcpy(rgb_bmp3, tImgData, vWidth*vHeight * 3);

				//下面的方式会导致崩溃
				//rgb_bmp3 = (unsigned char*) tImgData;
			}

			unsigned char *gray = new unsigned char[vWidth*vHeight];

			CMImgProc::RGBA2GRAY((unsigned char*)tImgData, gray, vWidth, vHeight, tCh);
			LOGD("DetectFacesByte, gray ok===\n");
			//LOGD("tImg, rows=%d, cols=%d",   tImg.rows, tImg.cols);

			ImageData img_color;
			img_color.data = rgb_bmp3;
			img_color.width = vWidth;
			img_color.height = vHeight;
			img_color.num_channels = 3; //因为recognizer要求的是3通道

			//人脸检测要求的是灰度图
			ImageData img_gray;
			img_gray.data = gray;
			img_gray.width = vWidth;
			img_gray.height = vHeight;
			img_gray.num_channels = 1;

			//time_t t0;
			//long tTime0 = getMillisec();
			//tTime0 = time(&t0);

			//LOGD("DetectFacesByte, 当前时间戳:%d", tTime0);
			LOGD("DetectFacesByte, detector.Detect\n");
			std::vector<seeta::FaceInfo> faces = detector.Detect(img_gray);

			//time_t t1;
			//long tTime1 = getMillisec(); //time(&t1);
			//LOGD("DetectFacesByte, 检测耗时:%d秒", tTime1 - tTime0);

			int32_t num_face = static_cast<int32_t>(faces.size());
			if (0 == num_face){
				//没有人脸
				env->ReleaseByteArrayElements(vImgByte, tImgData, 0);
				LOGD("DetectFacesByte, no face \n");
				delete[] rgb_bmp3;
				delete[] gray;
				return NULL;
			}
			else{
				LOGD("DetectFacesByte, face num:%d\n", num_face);
			}

			//先建空的数组
			jobjectArray tRetFaces = env->NewObjectArray(num_face, m_cls, 0);
			for (int i = 0; i < num_face; i++) {
				//组装脸部矩形数据
				seeta::FaceInfo tFace = faces[i];

				//新建一个对象
				jobject m_obj = env->NewObject(m_cls, m_mid);

				//整型值
				env->SetIntField(m_obj, tLeftField, tFace.bbox.x);
				env->SetIntField(m_obj, tRightField, tFace.bbox.x + tFace.bbox.width);
				env->SetIntField(m_obj, tTopField, tFace.bbox.y);
				env->SetIntField(m_obj, tBottomField, tFace.bbox.y + tFace.bbox.width);

				seeta::FacialLandmark face_points[5];
				point_detector.PointDetectLandmarks(img_gray, faces[i], face_points);

				for (int i = 0; i < 5; i++){
					printf("PointDetectLandmarks %d:(%d,%d)\n", i, (int)face_points[i].x, (int)face_points[i].y);
				}

				jint* tLandmarks = new jint[10];
				for (int i = 0; i < 5; i++){
					tLandmarks[i * 2] = (int)face_points[i].x;
					tLandmarks[i * 2 + 1] = (int)face_points[i].y;
				}

				//计算roll_angle旋转角
				int dx = face_points[1].x - face_points[0].x;
				int dy = face_points[1].y - face_points[0].y;
				float tRollRad = -atan2(dy, dx);
				float tRollAngle = -atan2(dy, dx) * 180 / 3.1415;
				env->SetFloatField(m_obj, tRollField, tRollAngle);
				LOGI("roll_angle=%.2f\n", tRollAngle);

				//平视角度的5点
				seeta::FacialLandmark rot_face_points[5];
				float tCosRoll = cos(tRollRad);
				float tSinRoll = sin(tRollRad);
				//		LOGI("tCosRoll,tSinRoll=(%f,%f)",tCosRoll,tSinRoll);

				//先旋转5点到平视角度
				for (int i = 0; i < 5; i++){
					rot_face_points[i].x = tCosRoll * face_points[i].x - tSinRoll * face_points[i].y;
					rot_face_points[i].y = tSinRoll * face_points[i].x + tCosRoll * face_points[i].y;
					//			LOGI("旋转后点[%d]:(%f,%f)", i, rot_face_points[i].x, rot_face_points[i].y);
				}

				//计算两眼中心点
				float tEyeMidX = 0.5*(rot_face_points[0].x + rot_face_points[1].x);
				//计算鼻尖点与左眼的水平距离
				float tNoseLeftEyeDist = abs(rot_face_points[2].x - rot_face_points[0].x);
				//计算鼻尖点与右眼的水平距离
				float tNoseRightEyeDist = abs(rot_face_points[1].x - rot_face_points[2].x);

				//取鼻尖到2眼的水平距离的最大值
				float tMaxNoseEyeDist = max(tNoseLeftEyeDist, tNoseRightEyeDist);
				//计算偏航角
				float tYawAngle = atan2(tNoseRightEyeDist - tNoseLeftEyeDist, tMaxNoseEyeDist) * 180 / 3.1415;
				LOGI("yaw_angle=%.2f\n", tYawAngle);
				env->SetFloatField(m_obj, tYawField, tYawAngle);

				//计算俯仰角
				//计算两眼中心点
				float tMouthMidY = 0.5*(rot_face_points[3].y + rot_face_points[4].y);
				//计算鼻尖点与眼睛的竖直距离,这时2个眼睛的y坐标几乎是相同的，所以任取一个眼睛y坐标即可
				float tNoseEyeVertDist = abs(rot_face_points[2].y - rot_face_points[0].y);
				//计算嘴唇与眼的竖直距离
				float tMouthEyeVertDist = abs(tMouthMidY - rot_face_points[0].y);

				//		LOGI("tNoseEyeVertDist=%f", tNoseEyeVertDist);
				//		LOGI("tMouthEyeVertDist=%f", tMouthEyeVertDist);

				//鼻尖与眼睛距离占眼睛与嘴唇竖直距离的比值
				float tVertDistRatio = tNoseEyeVertDist / (tMouthEyeVertDist + 0.001);
				//		LOGI("tVertDistRatio=%f", tVertDistRatio);

				//准备计算俯仰角
				float tVertDistRatioErr = 0;
				if (tVertDistRatio < 0.55){
					tVertDistRatioErr = tVertDistRatio - 0.55;
				}
				else if (tVertDistRatio > 0.62){
					tVertDistRatioErr = tVertDistRatio - 0.62;
				}

				float tPitchAngle = 2.8 * tVertDistRatioErr * 180 / 3.1415;
				LOGI("pitch_angle=%.2f\n", tPitchAngle);
				env->SetFloatField(m_obj, tPitchField, tPitchAngle);

				//特征点
				jintArray jiArray = env->NewIntArray(10);
				env->SetIntArrayRegion(jiArray, 0, 10, tLandmarks);
				env->SetObjectField(m_obj, tLandmarkField, jiArray);

				float feat1[SEETAFACE_FEATURE_NUM];
				//long t1 = getMillisec(); //time(&t1);

				face_recognizer.ExtractFeatureWithCrop(img_color, face_points, feat1);
				//LOGD("DetectFacesByte, ExtractFeatureWithCrop耗时:%d秒",  getMillisec() - t1);

				jfloatArray jnArray = env->NewFloatArray(SEETAFACE_FEATURE_NUM);
				env->SetFloatArrayRegion(jnArray, 0, SEETAFACE_FEATURE_NUM, feat1);
				env->SetObjectField(m_obj, tFeaturesField, jnArray);

				for (int i = 0; i < 5; i++){
					printf("feat%d:%.2f\n", i, feat1[i * 10]);
				}

				/*jfloat nArr[8] = {8,7,6,5,4,3,2,1};
					jfloatArray jnArray = env->NewFloatArray( 8);
					env->SetFloatArrayRegion(jnArray,0,8,nArr);
					env->SetObjectField(m_obj, tFeaturesField,jnArray);
					*/

				//添加到对象数组
				env->SetObjectArrayElement(tRetFaces, i, m_obj);
			}
			env->ReleaseByteArrayElements(vImgByte, tImgData, 0);
			if (NULL != rgb_bmp3){
				delete[] rgb_bmp3;
			}

			if (NULL != gray){
				delete[] gray;
			}

			return tRetFaces;
		}

#ifdef __cplusplus
	}
#endif
