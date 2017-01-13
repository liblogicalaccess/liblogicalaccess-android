#include "jnihelper.h"
#include <stdexcept>
#include <jni.h>

namespace logicalaccess {

	JniHelper::JniHelper() {

	}

	JniHelper::~JniHelper() {

	}

	void JniHelper::CheckException(JNIEnv *env) {
		if (env->ExceptionCheck()) {
			jthrowable e = env->ExceptionOccurred();
			env->ExceptionClear();

			jmethodID toString = env->GetMethodID(env->FindClass("java/lang/Object"), "toString",
												  "()Ljava/lang/String;");
			jstring estring = (jstring) env->CallObjectMethod(e, toString);

			jboolean isCopy;
			const char *message = env->GetStringUTFChars(estring, &isCopy);
			throw std::runtime_error(message);
		}
	}
}