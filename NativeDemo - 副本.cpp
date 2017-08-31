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

	//��������float���鳤��
#define	SEETAFACE_FEATURE_NUM 2048

	//sdk�Ƿ��ʼ���ɹ�
	static bool sb_sdk_init_ok = false;


	//��Ҫ����Ҫjava�˵���init()����ʼ��ģ��·��
	static seeta::FaceDetection detector(NULL);
	static seeta::FaceAlignment point_detector(NULL);
	static seeta::FaceIdentification face_recognizer(NULL);
	static	jmethodID m_mid;

	//������
	static	jfieldID tLeftField = NULL, tRightField = NULL, tTopField = NULL, tBottomField = NULL;

	//����������,�۾��������
	static	jfieldID tLandmarkField = NULL;

	//��������
	static	jfieldID tFeaturesField = NULL;
	
	//��ת��
	static jfieldID tRollField = NULL;

	//ƫ����
	static jfieldID tYawField = NULL;

	//������
	static jfieldID tPitchField = NULL;


	
	/**
	* ��ʼ��seetaface����ʶ���
	* ������
	* vFaceModelDir:����ʶ��ģ���ļ�����Ŀ¼����/sdcard/
	*/
	JNIEXPORT jboolean JNICALL Java_utryface_UtryFace_initModelPath(JNIEnv *env, jobject obj, jstring vFaceModelDir)
	{
		//����ѳ�ʼ����ֱ�ӷ���
		if (sb_sdk_init_ok){
			return true;
		}

		jboolean tRet = false;
		if (NULL == vFaceModelDir) {
			return tRet;
		}

		//��ȡ�����������ģ�͵ľ���·����Ŀ¼������/aaa/bbb.bin������·������/aaa/)
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
			//Ŀ¼����/
			tFaceModelDir += "/";
		}

		LOGD("init, tFaceModelDir=%s\n", tFaceModelDir.c_str());

		//���ģ��·��
		string tDetectModelPath = tFaceModelDir + "d_v1.0.bin";
		//LOGD("init,  tDetectModelPath=%s", tDetectModelPath.c_str());

		//����ģ��·��
		string tAlignModelPath = tFaceModelDir + "a_v1.1.bin";
		//LOGD("init,  tAlignModelPath=%s", tAlignModelPath.c_str());

		//ʶ��ģ��·��
		string tRecoModelPath = tFaceModelDir + "r_v1.0.bin";
		//LOGD("init,  tRecoModelPath=%s", tRecoModelPath.c_str());

		//3��ģ���ļ���Ҫ���ڣ������ʼ��ʧ��
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

		//��ʼ������������ʶ����
		//��ʼ������������ʶ����
		detector.initWithModel(tDetectModelPath.c_str());
		point_detector.initWithModel(tAlignModelPath.c_str());
		face_recognizer.initWithModel(tRecoModelPath.c_str());

		sb_sdk_init_ok = true;
		tRet = true;
		return tRet;
	}


	/*
	*����2�����������ƶ�
	*/
	JNIEXPORT jfloat JNICALL Java_utryface_UtryFace_CalcSimilarity(JNIEnv *env, jobject obj, jfloatArray vFeat1, jfloatArray vFeat2)
	{
		//LOGD("CalcSimilarity");
		//���û�г�ʼ���򷵻�
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

		//����2λС��
		tSim = ((int)(100 * tSim)) / 100.0;
		//LOGD("CalcFaceSim, tSim=%.2f",tSim);
		env->ReleaseFloatArrayElements(vFeat1, tFeat1, 0);
		env->ReleaseFloatArrayElements(vFeat2, tFeat2, 0);

		//long t = getMillisec() - t0;
		//LOGD("CalcSimilarity, tSim=%.4f, time=%ld", tSim, t);

		return tSim;
	}

	///**
	// * �������
	// * jbyteArray vImgByte,ͼ������byte��ʽ
	// * jint vWidth, ͼ����
	// * jint vHeight,ͼ��߶�
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

		//��ȡ��������
		jclass m_cls = env->FindClass("utryface/CMUtryFace");

		if (m_mid == NULL){
			m_mid = env->GetMethodID(m_cls, "<init>", "()V");
			printf("DetectFacesByte, utryface/CMUtryFace 2\n");

			//������Χ�ֶ�
			tLeftField = env->GetFieldID(m_cls, "left", "I");
			tRightField = env->GetFieldID(m_cls, "right", "I");
			tTopField = env->GetFieldID(m_cls, "top", "I");
			tBottomField = env->GetFieldID(m_cls, "bottom", "I");

			//����������,�۾��������
			tLandmarkField = env->GetFieldID(m_cls, "landmarks", "[I");

			//��������
			tFeaturesField = env->GetFieldID(m_cls, "features", "[F");
	
		}
			//��ת��
			tRollField = env->GetFieldID(m_cls, "roll_angle", "F");

			//ƫ����
			tYawField = env->GetFieldID(m_cls, "yaw_angle", "F");

			//������
			tPitchField = env->GetFieldID(m_cls, "pitch_angle", "F");

			printf("DetectFacesByte, utryface/CMUtryFace 3\n");
	

			//4ͨ��ת3ͨ��
			LOGD("DetectFacesByte, rgb_bmp3 new ok\n");
			unsigned char *rgb_bmp3 = new unsigned char[vWidth*vHeight * 3];
			if (4 == tCh){
				//unsigned char *rgb_bmp3 = new unsigned char[vWidth*vHeight * 3];
				CMImgProc::RGBA2RGB((unsigned char*)tImgData, rgb_bmp3, vWidth, vHeight);
				LOGD("DetectFacesByte, channel 4 to 3 ok\n");
			}
			else{
				memcpy(rgb_bmp3, tImgData, vWidth*vHeight * 3);

				//����ķ�ʽ�ᵼ�±���
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
			img_color.num_channels = 3; //��ΪrecognizerҪ�����3ͨ��

			//�������Ҫ����ǻҶ�ͼ
			ImageData img_gray;
			img_gray.data = gray;
			img_gray.width = vWidth;
			img_gray.height = vHeight;
			img_gray.num_channels = 1;

			//time_t t0;
			//long tTime0 = getMillisec();
			//tTime0 = time(&t0);

			//LOGD("DetectFacesByte, ��ǰʱ���:%d", tTime0);
			LOGD("DetectFacesByte, detector.Detect\n");
			std::vector<seeta::FaceInfo> faces = detector.Detect(img_gray);

			//time_t t1;
			//long tTime1 = getMillisec(); //time(&t1);
			//LOGD("DetectFacesByte, ����ʱ:%d��", tTime1 - tTime0);

			int32_t num_face = static_cast<int32_t>(faces.size());
			if (0 == num_face){
				//û������
				env->ReleaseByteArrayElements(vImgByte, tImgData, 0);
				LOGD("DetectFacesByte, no face \n");
				delete[] rgb_bmp3;
				delete[] gray;
				return NULL;
			}
			else{
				LOGD("DetectFacesByte, face num:%d\n", num_face);
			}

			//�Ƚ��յ�����
			jobjectArray tRetFaces = env->NewObjectArray(num_face, m_cls, 0);
			for (int i = 0; i < num_face; i++) {
				//��װ������������
				seeta::FaceInfo tFace = faces[i];

				//�½�һ������
				jobject m_obj = env->NewObject(m_cls, m_mid);

				//����ֵ
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

				//����roll_angle��ת��
				int dx = face_points[1].x - face_points[0].x;
				int dy = face_points[1].y - face_points[0].y;
				float tRollRad = -atan2(dy, dx);
				float tRollAngle = -atan2(dy, dx) * 180 / 3.1415;
				env->SetFloatField(m_obj, tRollField, tRollAngle);
				LOGI("roll_angle=%.2f\n", tRollAngle);

				//ƽ�ӽǶȵ�5��
				seeta::FacialLandmark rot_face_points[5];
				float tCosRoll = cos(tRollRad);
				float tSinRoll = sin(tRollRad);
				//		LOGI("tCosRoll,tSinRoll=(%f,%f)",tCosRoll,tSinRoll);

				//����ת5�㵽ƽ�ӽǶ�
				for (int i = 0; i < 5; i++){
					rot_face_points[i].x = tCosRoll * face_points[i].x - tSinRoll * face_points[i].y;
					rot_face_points[i].y = tSinRoll * face_points[i].x + tCosRoll * face_points[i].y;
					//			LOGI("��ת���[%d]:(%f,%f)", i, rot_face_points[i].x, rot_face_points[i].y);
				}

				//�����������ĵ�
				float tEyeMidX = 0.5*(rot_face_points[0].x + rot_face_points[1].x);
				//����Ǽ�������۵�ˮƽ����
				float tNoseLeftEyeDist = abs(rot_face_points[2].x - rot_face_points[0].x);
				//����Ǽ�������۵�ˮƽ����
				float tNoseRightEyeDist = abs(rot_face_points[1].x - rot_face_points[2].x);

				//ȡ�Ǽ⵽2�۵�ˮƽ��������ֵ
				float tMaxNoseEyeDist = max(tNoseLeftEyeDist, tNoseRightEyeDist);
				//����ƫ����
				float tYawAngle = atan2(tNoseRightEyeDist - tNoseLeftEyeDist, tMaxNoseEyeDist) * 180 / 3.1415;
				LOGI("yaw_angle=%.2f\n", tYawAngle);
				env->SetFloatField(m_obj, tYawField, tYawAngle);

				//���㸩����
				//�����������ĵ�
				float tMouthMidY = 0.5*(rot_face_points[3].y + rot_face_points[4].y);
				//����Ǽ�����۾�����ֱ����,��ʱ2���۾���y���꼸������ͬ�ģ�������ȡһ���۾�y���꼴��
				float tNoseEyeVertDist = abs(rot_face_points[2].y - rot_face_points[0].y);
				//�����촽���۵���ֱ����
				float tMouthEyeVertDist = abs(tMouthMidY - rot_face_points[0].y);

				//		LOGI("tNoseEyeVertDist=%f", tNoseEyeVertDist);
				//		LOGI("tMouthEyeVertDist=%f", tMouthEyeVertDist);

				//�Ǽ����۾�����ռ�۾����촽��ֱ����ı�ֵ
				float tVertDistRatio = tNoseEyeVertDist / (tMouthEyeVertDist + 0.001);
				//		LOGI("tVertDistRatio=%f", tVertDistRatio);

				//׼�����㸩����
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

				//������
				jintArray jiArray = env->NewIntArray(10);
				env->SetIntArrayRegion(jiArray, 0, 10, tLandmarks);
				env->SetObjectField(m_obj, tLandmarkField, jiArray);

				float feat1[SEETAFACE_FEATURE_NUM];
				//long t1 = getMillisec(); //time(&t1);

				face_recognizer.ExtractFeatureWithCrop(img_color, face_points, feat1);
				//LOGD("DetectFacesByte, ExtractFeatureWithCrop��ʱ:%d��",  getMillisec() - t1);

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

				//��ӵ���������
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
