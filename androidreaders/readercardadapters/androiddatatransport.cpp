#include <jni.h>
#include "androiddatatransport.hpp"
#include "../androidreaderunit.hpp"
#include "../jnihelper.h"

namespace logicalaccess
{
    void AndroidDataTransport::send(const std::vector<unsigned char> &cmd)
    {
        std::vector<unsigned char> result;
        auto env = AndroidReaderUnit::getEnv();

        jbyteArray array = env->NewByteArray (cmd.size());
        env->SetByteArrayRegion(array, 0, cmd.size(), reinterpret_cast<jbyte*>((char*)&cmd[0]));

        jclass cls = env->FindClass("com/islog/liblogicalaccess/AndroidTag");
        jclass jNDKhelper = (jclass) env->NewGlobalRef(cls);
        jmethodID execCommand = env->GetStaticMethodID(jNDKhelper, "execCommand", "([B)[B");
        jbyteArray resultDataArray = (jbyteArray) env->CallStaticObjectMethod(jNDKhelper, execCommand, array);

        JniHelper::CheckException(env);

        env->DeleteLocalRef(cls);
        env->DeleteLocalRef(array);

        if (resultDataArray)
        {
            jsize resultSize = env->GetArrayLength(resultDataArray);

            if (resultSize > 0)
            {
                jboolean t = JNI_FALSE;
                jbyte *jresult = env->GetByteArrayElements(resultDataArray, &t);
                result.assign(jresult, jresult + resultSize);
                env->ReleaseByteArrayElements(resultDataArray, jresult, JNI_ABORT);
            }
        }

        m_result = result;
    }

    std::vector<unsigned char> AndroidDataTransport::receive(long int /*timeout*/)
    {
        return m_result;
    }
}