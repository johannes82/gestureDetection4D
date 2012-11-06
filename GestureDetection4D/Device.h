#ifndef DEVICE_H_
#define DEVICE_H_


// Headers for OpenNI
#include <XnCppWrapper.h>
// Header for NITE
#include "XnVNite.h"

// local header
#include "CyclicBuffer.h"
#include "../FeatureExtractor4D/SimpleFeatureExtractor.h"
#include "../SVM/GestureSVM.h"

#define BUFFER_SIZE 30
#define FEATURE_VECTOR_FREQUENCY 3
#define WINDOW_NAME "GestureDetection4D"
#define SVM_MODEL_FILE "../../SVM_Model.txt"
#define DEBUG_FLAG

#define CHECK_RC(rc, what)											\
	if (rc != XN_STATUS_OK)											\
	{																\
	printf("%s failed: %s\n", what, xnGetStatusString(rc));		\
	return rc;													\
	}

#define CHECK_ERRORS(rc, errors, what)		\
	if (rc == XN_STATUS_NO_NODE_PRESENT)	\
	{										\
	XnChar strError[1024];				\
	errors.ToString(strError, 1024);	\
	printf("%s\n", strError);			\
	return (rc);						\
	}

using namespace xn;

// global cyclic buffer
CyclicBuffer<XnPoint3D> pointBuffer(BUFFER_SIZE);
SimpleFeatureExtractor featureExtractor;
GestureSVM gestureSVM;
int frequencyCounter = FEATURE_VECTOR_FREQUENCY;
int trainingClass = 0;

// OpenNI objects
Context g_Context;
ScriptNode g_ScriptNode;
DepthGenerator g_DepthGenerator;
HandsGenerator g_HandsGenerator;
GestureGenerator g_GestureGenerator;

// NITE objects
XnVSessionManager* g_pSessionManager;
XnVFlowRouter* g_pFlowRouter;

// the drawer
XnVPointDrawer* g_pDrawer;

#define GL_WIN_SIZE_X 640
#define GL_WIN_SIZE_Y 480

// Draw the depth map?
XnBool g_bDrawDepthMap = true;
XnBool g_bPrintFrameID = false;
// Use smoothing?
XnFloat g_fSmoothing = 0.0f;
XnBool g_bPause = false;
XnBool g_bQuit = false;

SessionState g_SessionState = NOT_IN_SESSION;

/// opens an onifile
XnStatus openDeviceFile(const char* csFile)
{
	XnStatus rc;
	rc = g_Context.Init();
	CHECK_RC(rc, "Context.Init()");
	rc = g_Context.OpenFileRecording(csFile);
	CHECK_RC(rc, "OpenOpenFileRecording");
	Player g_Player;
	rc = g_Context.FindExistingNode(XN_NODE_TYPE_PLAYER, g_Player);
	CHECK_RC(rc, "Find Playernode");
	g_Player.SetRepeat(false);

	return XN_STATUS_OK;
}

Point3D convertPoint(XnPoint3D* xnPoint) {
	Point3D p;
	p.X = xnPoint->X;
	p.Y = xnPoint->Y;
	p.Z = xnPoint->Z;
	return p;
}

void extractFeatureFromBuffer() {
	printf("Getting FeatureVector...\n");
	std::vector<Point3D> pVector;
	for (int i = 0; i < BUFFER_SIZE; i++) {
		pVector.push_back(convertPoint(pointBuffer.next()));
	}
	gestureSVM.train(featureExtractor.getFeatureVector(pVector), trainingClass);

#ifdef DEBUG_FLAG
	std::vector<float> fVector = featureExtractor.getFeatureVector(pVector);

	for(std::vector<float>::iterator iter = fVector.begin(); iter != fVector.end();iter+=3) {
		printf("FeatureVector X: %.2f, Y: %.2f, Z: %.2f\n",*iter,*(iter+1),*(iter+2));
	}
#endif

	printf("Training class %d\n", trainingClass);

}

#endif
