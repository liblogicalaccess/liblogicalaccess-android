#ifndef JNIHELPER_H
#define JNIHELPER_H

#include <jni.h>
namespace logicalaccess {
    class JniHelper {
    public:
        JniHelper();

        ~JniHelper();

        void static CheckException(JNIEnv *env);
    };
}

#endif // JNIHELPER_H
