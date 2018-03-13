#pragma once

#include <jni.h>

namespace logicalaccess
{

class AndroidSupportContext;
// The global current AndroidSupportContext for use by logicalaccess for
// the current processing.
extern AndroidSupportContext *gl_android_support_context;

/**
 * C++ side of CardAnalysisSupportContext
 *
 * This object wraps a Java CardAnalysisSupportContext and expose
 * some of its method.
 *
 * When analysing a card, a `gl_android_support_context` pointer is set
 * that point to the current support context. This allows various part of
 * LLA to access this context, despite being a bit ugly, this is useful.
 */
class AndroidSupportContext
{
  public:
    /**
     * Create a AndroidSupportContext that wraps the Java
     * CardAnalysisSupportContext.
     */
    AndroidSupportContext(JNIEnv *env, jobject java_support_context);

    ~AndroidSupportContext();

    /**
     * Retrieve the java IDataTransport that was set on the java
     * CardAnalysisSupportContext.
     * Might be null.
     */
    jobject get_sam_data_transport();

    /**
     * This is a help function that attempts to retrieve the JNIEnv for
     * the current thread. If this is not possible, this function will throw.
     */
    JNIEnv *get_jni_env();

  private:
    // A GlobalRef to our support context.
    jobject java_support_context_;

    JavaVM *java_vm_;

    jclass class_CardAnalysisSupportContext_;
    jmethodID method_id_getSamDataTransport_;

    void load_java_attributes(JNIEnv *env);
};
}
