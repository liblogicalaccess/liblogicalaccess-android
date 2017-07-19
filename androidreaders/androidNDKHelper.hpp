#include <jni.h>

#ifndef LOGICALCCESS_ANDROIDNDKHELPER
#define LOGICALCCESS_ANDROIDNDKHELPER

namespace logicalaccess
{
    class AndroidNDKHelper
    {
    public:
        AndroidNDKHelper() {}
	    void Init(JNIEnv *env);
    };
}

#endif /* LOGICALCCESS_ANDROIDNDKHELPER */
