#pragma once

#include <jni.h>
#include <logicalaccess/readerproviders/datatransport.hpp>
#include <thread>

namespace logicalaccess
{

/**
 * A DataTransport that relies on a Java object implementing our
 * Java IDataTransport interface to exchange data with the underlying device.
 *
 * This object simply forwards call to the Java object, which is
 * responsible for the underlying physical transport.
 *
 * @note The JavaBridgeDataTransport, due to its reliance on
 * the JNI interface, is pinned to the thread that created it.
 * This is because the JNIEnv pointer is thread-specific, and we
 * cannot / don't want to retrieve a JNIEnv pointer for each call to the
 * DataTransport.
 *
 * @note throws std::runtime_error when invoked from an other thread
 * than the one that create the object.
 *
 * @note The Java interface has a single transmit(byte[]) -> byte[] method.
 *       Therefore you shouldn't call send() multiple time w/o calling receive()
 *       otherwise you will lose data.
 */
class JavaBridgeDataTransport : public DataTransport
{
  public:
    JavaBridgeDataTransport(jobject data_transport, JNIEnv *jni_env);

    ~JavaBridgeDataTransport();

    std::string getTransportType() const override;

    bool connect() override;

    void disconnect() override;

    bool isConnected() override;

    std::string getName() const override;

    void serialize(boost::property_tree::ptree &parentNode) override;

    void unSerialize(boost::property_tree::ptree &node) override;

    std::string getDefaultXmlNodeName() const override;

  protected:
    void send(const std::vector<unsigned char> &data) override;

    std::vector<unsigned char> receive(long int timeout) override;

  private:
    // Make sure we are called from the thread that created us,
    // for JNI thread safety concern.
    void enforce_correct_thread();

    // Perform some sanitfy check wrt the data_transport jobject.
    void enforce_correct_data_transport_jobject();

    // Interface that must be implemented by the java object we use.
    static constexpr const char *const java_class_name =
        "com/islog/rfidguard/llasupport/IDataTransport";

    // The real java based data transport.
    // It must implement IDataTransport.
    // This is a GlobalRef.
    jobject data_transport_;

    JNIEnv *const jni_env_;

    jmethodID transmit_method_id_;

    std::thread::id ctor_thread_id_;

    // Data block received from the last send() call.
    // Will be returned the next time receive() is called.
    ByteVector pending_result_;
};
}
