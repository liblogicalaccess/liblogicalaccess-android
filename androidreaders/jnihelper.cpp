#include "jnihelper.h"
#include <cassert>
#include <stdexcept>

namespace logicalaccess
{

JniHelper::JniHelper()
{
}

JniHelper::~JniHelper()
{
}

void JniHelper::CheckException(JNIEnv *env)
{
    if (env->ExceptionCheck())
    {
        jthrowable e = env->ExceptionOccurred();
        env->ExceptionClear();
        assert(!env->ExceptionCheck());

        jmethodID toString = env->GetMethodID(env->FindClass("java/lang/Object"),
                                              "toString", "()Ljava/lang/String;");
        jstring estring = (jstring)env->CallObjectMethod(e, toString);

        const char *message = env->GetStringUTFChars(estring, NULL);
        std::string error   = std::string(message);
        env->ReleaseStringUTFChars(estring, message);

        throw std::runtime_error(error);
    }
}
}