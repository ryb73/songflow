#include <jni.h>
#include <stdio.h>
#include "blah_Test.h"

JNIEXPORT jstring JNICALL
Java_blah_Test_sup(JNIEnv *env, jobject obj) {
	printf("sup.\n");
	return env->NewStringUTF("hello");
}