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

jbyteArray JniHelper::to_java_byte_array(JNIEnv *env, const ByteVector &in)
{
    jbyteArray java_bytes = env->NewByteArray(in.size());
    if (java_bytes == nullptr)
    {
        throw std::bad_alloc();
    }
    env->SetByteArrayRegion(java_bytes, 0, in.size(),
                            reinterpret_cast<const jbyte *>(in.data()));

    return java_bytes;
}

ByteVector JniHelper::to_native_byte_vector(JNIEnv *env, jbyteArray java_byte_array)
{
    ByteVector result;

    if (java_byte_array)
    {
        jsize array_size = env->GetArrayLength(java_byte_array);

        if (array_size > 0)
        {
            jboolean t       = JNI_FALSE;
            jbyte *raw_bytes = env->GetByteArrayElements(java_byte_array, &t);
            result.assign(raw_bytes, raw_bytes + array_size);
            env->ReleaseByteArrayElements(java_byte_array, raw_bytes, JNI_ABORT);
        }
    }

    return result;
}

JNIEnv *JniHelper::env_from_vm(JavaVM *vm)
{
    JNIEnv *env  = nullptr;
    auto errcode = vm->GetEnv((void **)&env, JNI_VERSION_1_6);
    if (errcode != JNI_OK)
    {
        throw std::runtime_error("Cannot get JNIEnv from JavaVM: " + errcode);
    }

    return env;
}

JavaVM *JniHelper::vm_from_env(JNIEnv *env)
{
    JavaVM *java_vm = nullptr;
    if (env->GetJavaVM(&java_vm) < 0)
    {
        throw std::runtime_error("Cannot get JavaVM from JNIEnv. Something is wrong.");
    }

    return java_vm;
}

jmethodID JniHelper::find_method_on_class(JNIEnv *env, const std::string &class_fqn,
                                          const std::string &method_name,
                                          const std::string &method_sig)
{
    jclass clazz = env->FindClass(class_fqn.c_str());
    JniHelper::CheckException(env);

    if (clazz == nullptr)
    {
        throw std::runtime_error("Cannot find class " + class_fqn);
    }

    jmethodID method_id =
        env->GetMethodID(clazz, method_name.c_str(), method_sig.c_str());
    JniHelper::CheckException(env);

    if (method_id == nullptr)
    {
        throw std::runtime_error("Cannot find method " + method_name + ": " + method_sig);
    }
    return method_id;
}
}