//
// Created by xaqq on 3/9/18.
//

#include <logicalaccess/myexception.hpp>
#include <logicalaccess/cards/commands.hpp>
#include <logicalaccess/dynlibrary/librarymanager.hpp>
#include "morphosamreaderunit.hpp"
#include "readercardadapters/morphosamdatatransport.hpp"

namespace logicalaccess {

    JNIEnv *MorphoSAMReaderUnit::m_env = nullptr;

    bool MorphoSAMReaderUnit::waitInsertion(unsigned int maxwait) {
        d_insertedChip = createChip("SAM_AV2");
        return true;
    }

    bool MorphoSAMReaderUnit::waitRemoval(unsigned int maxwait) {
        return true;
    }

    bool MorphoSAMReaderUnit::isConnected() {
        return true;
    }

    void MorphoSAMReaderUnit::setCardType(std::string cardType) {

    }

    std::shared_ptr<Chip>
    MorphoSAMReaderUnit::getSingleChip() {
        return nullptr;
    }

    std::vector<std::shared_ptr<Chip>>
    MorphoSAMReaderUnit::getChipList() {
        return {};
    }

    bool MorphoSAMReaderUnit::connect() {
        return true;
    }

    void MorphoSAMReaderUnit::disconnect() {

    }

    bool MorphoSAMReaderUnit::connectToReader() {
        return true;
    }

    void MorphoSAMReaderUnit::disconnectFromReader() {

    }

    std::string MorphoSAMReaderUnit::getName() const {
        return "NO_IMPLEMENTED";
    }

    std::string MorphoSAMReaderUnit::getReaderSerialNumber() {
        return "NOT_IMPLEMENTED";
    }

    MorphoSAMReaderUnit::MorphoSAMReaderUnit()
            : ReaderUnit(READER_MORPHO_SAM) {}

    std::shared_ptr<Chip> MorphoSAMReaderUnit::createChip(std::string type) {
        if (type != "SAM_AV2")
        {
            throw LibLogicalAccessException("Only handle SAM_AV2, sorry");
        }

        std::shared_ptr<DataTransport> data_transport = std::make_shared<MorphoSAMDataTransport>();
        std::shared_ptr<Commands> commands;
        commands = LibraryManager::getInstance()->getCommands("SAM_AV2");
        commands->getReaderCardAdapter()->setDataTransport(data_transport);

        ChipPtr  chip = ReaderUnit::createChip(type);
        chip->setCommands(commands);

        return chip;
    }
}
