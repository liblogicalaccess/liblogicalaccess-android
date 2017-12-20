#include "MifareAndroidCommands.hpp"
#include "../androidreaderunit.hpp"
#include "../jnihelper.h"

namespace logicalaccess
{
    std::vector<unsigned char> MifareAndroidCommands::readBlock(int sector, int block, std::vector<unsigned char> keyA)
    {
        std::vector<unsigned char> result;
        auto env = AndroidReaderUnit::getEnv();

        jbyteArray array = env->NewByteArray(keyA.size());
        env->SetByteArrayRegion(array, 0, keyA.size(),
                                reinterpret_cast<jbyte *>((unsigned char *)&keyA[0]));

        jclass cls            = env->FindClass("com/islog/liblogicalaccess/AndroidTag");
        jclass jNDKhelper     = (jclass)env->NewGlobalRef(cls);
        jmethodID execCommand = env->GetStaticMethodID(jNDKhelper, "mifareReadBlock", "(II[B)[B");
        jbyteArray resultDataArray =
                (jbyteArray)env->CallStaticObjectMethod(jNDKhelper, execCommand, sector, block, array);

        JniHelper::CheckException(env);

        env->DeleteLocalRef(cls);
        env->DeleteLocalRef(array);

        if (resultDataArray)
        {
            jsize resultSize = env->GetArrayLength(resultDataArray);

            if (resultSize > 0)
            {
                jboolean t     = JNI_FALSE;
                jbyte *jresult = env->GetByteArrayElements(resultDataArray, &t);
                result.assign(jresult, jresult + resultSize);
                env->ReleaseByteArrayElements(resultDataArray, jresult, JNI_ABORT);
            }
        }

        return result;
    }
}