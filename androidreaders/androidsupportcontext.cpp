//
// Created by xaqq on 3/13/18.
//

#include <stdexcept>
#include "androidsupportcontext.hpp"
#include "jnihelper.h"

namespace logicalaccess
{
AndroidSupportContext *gl_android_support_context = nullptr;

AndroidSupportContext::AndroidSupportContext(JNIEnv *env, jobject java_support_context)
{

    load_java_attributes(env);
    java_support_context_ = env->NewGlobalRef(java_support_context);
    if (java_support_context == nullptr)
        throw std::bad_alloc();
}

AndroidSupportContext::~AndroidSupportContext()
{
    JNIEnv *env = JniHelper::env_from_vm(java_vm_);
    env->DeleteGlobalRef(java_support_context_);
}

jobject AndroidSupportContext::get_sam_data_transport()
{
    JNIEnv *env = JniHelper::env_from_vm(java_vm_);
    return env->CallObjectMethod(java_support_context_, method_getSamDataTransport_);
}

jobject AndroidSupportContext::get_android_reader_data_transport()
{

    JNIEnv *env = JniHelper::env_from_vm(java_vm_);
    return env->CallObjectMethod(java_support_context_,
                                 method_getAndroidRFIDDataTransport_);
}

void AndroidSupportContext::load_java_attributes(JNIEnv *env)
{
    java_vm_ = JniHelper::vm_from_env(env);

    method_getSamDataTransport_ = JniHelper::find_method_on_class(
        env, "com/islog/rfidguard/llasupport/CardAnalysisSupportContext",
        "getSamDataTransport", "()Lcom/islog/rfidguard/llasupport/IDataTransport;");

    method_getAndroidRFIDDataTransport_ = JniHelper::find_method_on_class(
        env, "com/islog/rfidguard/llasupport/CardAnalysisSupportContext",
        "getAndroidRFIDDataTransport",
        "()Lcom/islog/rfidguard/llasupport/IDataTransport;");
}

JNIEnv *AndroidSupportContext::get_jni_env()
{
    return JniHelper::env_from_vm(java_vm_);
}
}
