#include <jni.h>
#include "androiddatatransport.hpp"
#include "../androidreaderunit.hpp"
#include "../jnihelper.h"

namespace logicalaccess
{
    void AndroidDataTransport::send(const std::vector<unsigned char> &cmd)
    {
        std::vector<unsigned char> result;
        //QAndroidJniEnvironment env;

        auto env = AndroidReaderUnit::getEnv();

        jbyteArray array = env->NewByteArray (cmd.size());
        env->SetByteArrayRegion(array, 0, cmd.size(), reinterpret_cast<jbyte*>((char*)&cmd[0]));

      /*  QAndroidJniObject resultArray = QAndroidJniObject::callStaticObjectMethod("com/islog/androidnfc/IsoDepCommand",
                                                                                  "execCommand",
                                                                                  "([B)[B", array);*/

        jclass cls = env->FindClass("com/islog/liblogicalaccess/IsoDepCommand");
        jclass jNDKhelper = (jclass) env->NewGlobalRef(cls);
        jmethodID execCommand = env->GetStaticMethodID(jNDKhelper, "execCommand", "([B)[B");
        jbyteArray resultDataArray = (jbyteArray) env->CallStaticObjectMethod(jNDKhelper, execCommand, array);
        env->DeleteLocalRef(cls);
        env->DeleteLocalRef(array);

        JniHelper::CheckException(env);

        if (resultDataArray)
        {
            jsize resultSize = env->GetArrayLength(resultDataArray);

            if (resultSize > 0)
            {
                jboolean t = false;
                jbyte *jresult = env->GetByteArrayElements(resultDataArray, &t);
                result.assign(jresult, jresult + resultSize);
                env->ReleaseByteArrayElements(resultDataArray, jresult, JNI_ABORT);
            }
        }

        m_result = result;
    }

    std::vector<unsigned char> AndroidDataTransport::receive(long int timeout)
    {
        return m_result;
    }
}