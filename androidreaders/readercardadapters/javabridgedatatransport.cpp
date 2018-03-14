//
// Created by xaqq on 3/13/18.
//

#include "javabridgedatatransport.hpp"
#include "../jnihelper.h"

namespace logicalaccess
{

JavaBridgeDataTransport::JavaBridgeDataTransport(jobject data_transport, JNIEnv *jni_env)
    : jni_env_(jni_env)
    , ctor_thread_id_(std::this_thread::get_id())
{
    data_transport_ = jni_env->NewGlobalRef(data_transport);
    enforce_correct_data_transport_jobject();
}

JavaBridgeDataTransport::~JavaBridgeDataTransport()
{
    jni_env_->DeleteGlobalRef(data_transport_);
}

std::string JavaBridgeDataTransport::getTransportType() const
{
    return "JavaBridgeDataTransport";
}

bool JavaBridgeDataTransport::connect()
{
    return true;
}

void JavaBridgeDataTransport::disconnect()
{
}

bool JavaBridgeDataTransport::isConnected()
{
    return true;
}

std::string JavaBridgeDataTransport::getName() const
{
    return "JavaBridgeDataTransport";
}

void JavaBridgeDataTransport::serialize(boost::property_tree::ptree &parentNode)
{
    throw std::runtime_error("Not Implemented");
}

void JavaBridgeDataTransport::unSerialize(boost::property_tree::ptree &node)
{
    throw std::runtime_error("Not Implemented");
}

std::string JavaBridgeDataTransport::getDefaultXmlNodeName() const
{
    throw std::runtime_error("Not Implemented");
}

void JavaBridgeDataTransport::send(const std::vector<unsigned char> &data)
{
    enforce_correct_thread();

    jbyteArray java_bytes = JniHelper::to_java_byte_array(jni_env_, data);

    jbyteArray result_byte_array = (jbyteArray)jni_env_->CallObjectMethod(
        data_transport_, transmit_method_id_, java_bytes);
    JniHelper::CheckException(jni_env_);

    pending_result_ = JniHelper::to_native_byte_vector(jni_env_, result_byte_array);
}

std::vector<unsigned char> JavaBridgeDataTransport::receive(long int timeout)
{
    enforce_correct_thread();
    auto result     = std::move(pending_result_);
    pending_result_ = {};

    return result;
}

void JavaBridgeDataTransport::enforce_correct_thread()
{
    if (std::this_thread::get_id() != ctor_thread_id_)
    {
        throw std::runtime_error("JavaBridgeDataTransport called from incorrect thread");
    }
}

void JavaBridgeDataTransport::enforce_correct_data_transport_jobject()
{
    // Validate that the java object implement our interface.
    jclass expected_interface = jni_env_->FindClass(java_class_name);
    if (expected_interface == nullptr)
    {
        throw std::runtime_error("Cannot find class " + std::string(java_class_name));
    }
    JniHelper::CheckException(jni_env_);

    transmit_method_id_ =
        JniHelper::find_method_on_class(jni_env_, java_class_name, "transmit", "([B)[B");
    if (data_transport_ == nullptr)
    {
        throw std::runtime_error("jobject data transport is null");
    }

    if (jni_env_->IsInstanceOf(data_transport_, expected_interface) == JNI_FALSE)
    {
        throw std::runtime_error("jobject data transport does not implement " +
                                 std::string(java_class_name));
    }
}
}
