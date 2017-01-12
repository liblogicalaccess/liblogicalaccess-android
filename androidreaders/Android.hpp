#include <jni.h>

#ifndef LOGICALCCESS_ANDROID
#define LOGICALCCESS_ANDROID

namespace logicalaccess
{
    class Android
    {
    public:
	Android() {}
	void Init(JNIEnv *env);
    };
}

#endif /* LOGICALCCESS_ANDROID */
