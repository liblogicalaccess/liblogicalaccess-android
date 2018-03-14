#pragma once

#include <logicalaccess/readerproviders/readerunit.hpp>
#include <jni.h>
#include "morphosamreaderprovider.hpp"
#include "logicalaccess/plugins/readers/iso7816/iso7816readerunit.hpp"

namespace logicalaccess
{
/**
 * A ReaderUnit that is designed for use on a MorphoTablet when
 * we want to interact with the SAM reader on the device.
 *
 * This MorphoSAMReaderUnit is tightly coupled to the RFIDGuard infrastructure
 * and Android as it relies on the global AndroidSupportContext being available.
 */
class MorphoSAMReaderUnit : public ISO7816ReaderUnit
{
  public:
    MorphoSAMReaderUnit();

    std::shared_ptr<Chip> createChip(std::string type) override;

    bool waitInsertion(unsigned int maxwait) override;

    bool waitRemoval(unsigned int maxwait) override;

    bool isConnected() override;

    void setCardType(std::string cardType) override;

    std::shared_ptr<Chip> getSingleChip() override;

    std::vector<std::shared_ptr<Chip>> getChipList() override;

    bool connect() override;

    void disconnect() override;

    bool connectToReader() override;

    void disconnectFromReader() override;

    std::string getName() const override;

    std::string getReaderSerialNumber() override;

    void setEnv(JNIEnv *env)
    {
        m_env = env;
    };

    static JNIEnv *getEnv()
    {
        return m_env;
    };

  private:
    static JNIEnv *m_env;
};
}
