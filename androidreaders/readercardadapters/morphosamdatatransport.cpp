//
// Created by xaqq on 3/9/18.
//

#include "morphosamdatatransport.hpp"
#include "../morphosamreaderunit.hpp"
#include "../jnihelper.h"

namespace logicalaccess
{

    void MorphoSAMDataTransport::send(const std::vector<unsigned char> &cmd) {
        std::vector<unsigned char> result;
        auto env = MorphoSAMReaderUnit::getEnv();

        jbyteArray array = env->NewByteArray(cmd.size());
        env->SetByteArrayRegion(array, 0, cmd.size(),
                                reinterpret_cast<jbyte *>((char *)&cmd[0]));

        jclass cls            = env->FindClass("com/islog/rfidguard/SAM/MorphoSAMHelper");
        jclass jNDKhelper     = (jclass)env->NewGlobalRef(cls);
        jmethodID transceive = env->GetStaticMethodID(jNDKhelper, "transceive", "([B)[B");
        jbyteArray resultDataArray =
                (jbyteArray)env->CallStaticObjectMethod(jNDKhelper, transceive, array);

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

        result_ = result;
    }

    std::vector<unsigned char> MorphoSAMDataTransport::receive(long int timeout) {
        return result_;
    }
}
