#include <jni.h>
#include <iostream>
#include "Test.h"

using namespace std;

JNIEXPORT jstring JNICALL
Java_Test_sup(JNIEnv *env, jobject obj) {
	cout << "sup" << endl;
	return env->NewStringUTF("hello");
}