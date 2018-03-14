//
// Created by xaqq on 3/9/18.
//

#include <logicalaccess/myexception.hpp>
#include <logicalaccess/cards/commands.hpp>
#include <logicalaccess/dynlibrary/librarymanager.hpp>
#include "morphosamreaderunit.hpp"
#include "../../../liblogicalaccess/plugins/pluginsreaderproviders/iso7816/commands/samav2iso7816commands.hpp"
#include "../../../liblogicalaccess/plugins/pluginsreaderproviders/iso7816/commands/samiso7816resultchecker.hpp"
#include "readercardadapters/javabridgedatatransport.hpp"
#include "androidsupportcontext.hpp"

namespace logicalaccess
{

JNIEnv *MorphoSAMReaderUnit::m_env = nullptr;

bool MorphoSAMReaderUnit::waitInsertion(unsigned int maxwait)
{
    d_insertedChip = createChip("SAM_AV2");
    return true;
}

bool MorphoSAMReaderUnit::waitRemoval(unsigned int maxwait)
{
    return true;
}

bool MorphoSAMReaderUnit::isConnected()
{
    return true;
}

void MorphoSAMReaderUnit::setCardType(std::string cardType)
{
}

std::shared_ptr<Chip> MorphoSAMReaderUnit::getSingleChip()
{
    return d_insertedChip;
}

std::vector<std::shared_ptr<Chip>> MorphoSAMReaderUnit::getChipList()
{
    return {d_insertedChip};
}

bool MorphoSAMReaderUnit::connect()
{
    return true;
}

void MorphoSAMReaderUnit::disconnect()
{
}

bool MorphoSAMReaderUnit::connectToReader()
{
    return true;
}

void MorphoSAMReaderUnit::disconnectFromReader()
{
}

std::string MorphoSAMReaderUnit::getName() const
{
    return "ADRIEN_SAM_MORPHO_READER";
}

std::string MorphoSAMReaderUnit::getReaderSerialNumber()
{
    return "NOT_IMPLEMENTED";
}

MorphoSAMReaderUnit::MorphoSAMReaderUnit()
    : ISO7816ReaderUnit(READER_MORPHO_SAM)
{
}

std::shared_ptr<Chip> MorphoSAMReaderUnit::createChip(std::string type)
{
    if (type != "SAM_AV2")
    {
        throw LibLogicalAccessException("Only handle SAM_AV2, sorry");
    }

    // We create a JavaBridgeDataTransport that use the Java data transport
    // specified for the SAM in the AndroidSupportContext.
    std::shared_ptr<DataTransport> data_transport =
        std::make_shared<JavaBridgeDataTransport>(
            gl_android_support_context->get_sam_data_transport(),
            gl_android_support_context->get_jni_env());

    std::shared_ptr<Commands> commands;
    commands.reset(new SAMAV2ISO7816Commands());
    std::shared_ptr<SAMDESfireCrypto> samcrypto(new SAMDESfireCrypto());
    std::dynamic_pointer_cast<SAMAV2ISO7816Commands>(commands)->setCrypto(samcrypto);
    std::shared_ptr<ReaderCardAdapter> rca = std::make_shared<ISO7816ReaderCardAdapter>();
    commands->setReaderCardAdapter(rca);
    commands->getReaderCardAdapter()->setResultChecker(
        std::make_shared<SAMISO7816ResultChecker>());
    commands->getReaderCardAdapter()->setDataTransport(data_transport);

    ChipPtr chip = ReaderUnit::createChip(type);
    chip->setCommands(commands);

    return chip;
}
}
