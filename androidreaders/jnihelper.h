#ifndef JNIHELPER_H
#define JNIHELPER_H

#include <jni.h>
#include <string>
#include <logicalaccess/lla_fwd.hpp>

namespace logicalaccess {
    class JniHelper {
    public:
        JniHelper();

        ~JniHelper();

        static void CheckException(JNIEnv *env);

        /**
         * Copy a c++ ByteVector to a Java allocated byte array.
         *
         * The return jbyteArray is a local ref.
         */
        static jbyteArray to_java_byte_array(JNIEnv *env, const ByteVector &in);

        /**
         * Copy a jbyteArray to a C++ byte vector.
         */
        static ByteVector to_native_byte_vector(JNIEnv *env, jbyteArray java_byte_array);

        /**
         * Retrieve a JNIEnv pointer from JavaVM.
         *
         * If this is possible (wrong thread) and exception is thrown.
         */
        static JNIEnv *env_from_vm(JavaVM *vm);

        /**
         * Retrieve the JavaVM pointer from the JNIEnv pointer.
         *
         * Throw on error.
         */
        static JavaVM *vm_from_env(JNIEnv *env);

        /**
         * Attempt to find a Java Method on a given class.
         *
         * Throw on failure.
         */
        static jmethodID find_method_on_class(JNIEnv *env,
                                              const std::string &class_fqn,
                                              const std::string &method_name,
                                              const std::string &method_sig);

    };
}

#endif // JNIHELPER_H
