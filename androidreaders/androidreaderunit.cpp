/**
 * \file stidstrreaderunit.cpp
 * \author Maxime C. <maxime-dev@islog.com>
 * \brief STidSTR reader unit.
 */

#include "androidreaderunit.hpp"
#include "androidreaderprovider.hpp"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "logicalaccess/myexception.hpp"
#include "logicalaccess/services/accesscontrol/cardsformatcomposite.hpp"

#include "../../../liblogicalaccess/plugins/pluginscards/desfire/desfirecommands.hpp"
#include "../../../liblogicalaccess/plugins/pluginsreaderproviders/iso7816/commands/desfireiso7816resultchecker.hpp"
#include "../../../liblogicalaccess/plugins/pluginsreaderproviders/iso7816/readercardadapters/iso7816readercardadapter.hpp"
#include "androidreaderunitconfiguration.hpp"
#include "jnihelper.h"
#include "logicalaccess/cards/commands.hpp"
#include "logicalaccess/dynlibrary/librarymanager.hpp"
#include "readercardadapters/androiddatatransport.hpp"
#include "commands/MifareAndroidCommands.hpp"
#include "../../../liblogicalaccess/plugins/pluginsreaderproviders/iso7816/commands/desfireiso7816commands.hpp"
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <logicalaccess/logs.hpp>
#include <thread>
#include <android/log.h>

namespace logicalaccess
{
JNIEnv *AndroidReaderUnit::m_env;

AndroidReaderUnit::AndroidReaderUnit()
    : ISO7816ReaderUnit(READER_ANDROID)
{
    d_readerUnitConfig.reset(new AndroidReaderUnitConfiguration());

    setDataTransport(std::make_shared<AndroidDataTransport>());

    std::shared_ptr<ReaderCardAdapter> rca(new ReaderCardAdapter());
    rca->setDataTransport(getDataTransport());

    //	d_card_type = "DESFire";
    d_card_type = CHIP_UNKNOWN;
}

AndroidReaderUnit::~AndroidReaderUnit()
{
    disconnectFromReader();
}

std::string AndroidReaderUnit::getName() const
{
    return "Android";
}

std::string AndroidReaderUnit::getConnectedName()
{
    return getName();
}

void AndroidReaderUnit::setCardType(std::string cardType)
{
    LOG(LogLevel::INFOS) << "Setting card type {" << cardType << "}";
    d_card_type = cardType;
}

std::shared_ptr<Chip> AndroidReaderUnit::createChip(std::string type)
{
    std::shared_ptr<Chip> chip = ReaderUnit::createChip(type);
    setcryptocontext setcryptocontextfct;

    if (chip)
    {
        LOG(LogLevel::INFOS) << "Chip created successfully !";
        std::shared_ptr<ReaderCardAdapter> rca =
            std::make_shared<ISO7816ReaderCardAdapter>();
        std::shared_ptr<Commands> commands;

        // get Uid from android
        std::vector<unsigned char> result;
        auto env          = AndroidReaderUnit::getEnv();
        jclass cls        = env->FindClass("com/islog/liblogicalaccess/AndroidTag");
        jclass jNDKhelper = (jclass)env->NewGlobalRef(cls);
        jmethodID getUID  = env->GetStaticMethodID(jNDKhelper, "getUID", "()[B");
        jbyteArray resultDataArray =
            (jbyteArray)env->CallStaticObjectMethod(jNDKhelper, getUID);
        env->DeleteLocalRef(cls);

        JniHelper::CheckException(env);

        if (resultDataArray)
        {
            jsize resultSize = env->GetArrayLength(resultDataArray);

            if (resultSize > 0)
            {
                jboolean t     = JNI_FALSE;
                jbyte *jresult = env->GetByteArrayElements(resultDataArray, &t);
                result.assign(jresult, jresult + resultSize);
                env->ReleaseByteArrayElements(resultDataArray, jresult, JNI_ABORT);

                chip->setChipIdentifier(result);
            }
        }

        if (type == "DESFire")
        {
            LOG(LogLevel::INFOS) << "Mifare DESFire EV1 Chip created";
            rca->setResultChecker(std::make_shared<DESFireISO7816ResultChecker>());
            commands = LibraryManager::getInstance()->getCommands("DESFireISO7816");
            *(void **)(&setcryptocontextfct) =
                LibraryManager::getInstance()->getFctFromName(
                    "setCryptoContextDESFireEV1ISO7816Commands",
                    LibraryManager::READERS_TYPE);
            setcryptocontextfct(&commands, &chip);
        }
        else if (type == "DESFireEV1")
        {
            rca->setResultChecker(std::make_shared<DESFireISO7816ResultChecker>());
            commands = LibraryManager::getInstance()->getCommands("DESFireEV1ISO7816");
            *(void **)(&setcryptocontextfct) =
                LibraryManager::getInstance()->getFctFromName(
                    "setCryptoContextDESFireEV1ISO7816Commands",
                    LibraryManager::READERS_TYPE);
            setcryptocontextfct(&commands, &chip);
            std::dynamic_pointer_cast<DESFireISO7816Commands>(commands)->setSAMChip(
                    getSAMChip());
        }
        else if (type == "Mifare1K" || type == "Mifare4K") {
            commands = std::make_shared<MifareAndroidCommands>();
        }
        else if (type == "NFCA")
        {
            // Mifare classic, ultralight & C
            return chip;
        }
        else
            return chip;

        rca->setDataTransport(getDataTransport());
        if (commands)
        {
            commands->setReaderCardAdapter(rca);
            commands->setChip(chip);
            chip->setCommands(commands);
        }

        if (type == "DESFire")
        {
            struct logicalaccess::DESFireCommands::DESFireCardVersion dataVersion;
            connect(); // have to for android :'(
            try
            {
                // Hack smartMX. Required otherwise we receive
                // WRONG CLASS from the card.
                // We aren't sure why this is needed.
                std::dynamic_pointer_cast<logicalaccess::DESFireCommands>(commands)
                    ->selectApplication(0);

                std::dynamic_pointer_cast<logicalaccess::DESFireCommands>(commands)
                    ->getVersion(dataVersion);
                if (dataVersion.hardwareMjVersion >= 1)
                {
                    return createChip("DESFireEV1");
                }
            }
            catch (std::exception &e)
            {
                __android_log_print(ANDROID_LOG_ERROR, "RFIDGUARD", "%s", e.what());
                // Ignore
            }
        }
    }
    return chip;
}

bool AndroidReaderUnit::waitInsertion(unsigned int maxwait)
{
    bool inserted;
    std::string cardType = "";
    std::chrono::steady_clock::time_point const clock_timeout =
        std::chrono::steady_clock::now() + std::chrono::milliseconds(maxwait);
    do
    {
        jclass cls        = m_env->FindClass("com/islog/liblogicalaccess/AndroidTag");
        jclass jNDKhelper = (jclass)m_env->NewGlobalRef(cls);
        jmethodID getCurrentCardType = m_env->GetStaticMethodID(
            jNDKhelper, "getCurrentCardType", "()Ljava/lang/String;");
        jstring jCardType =
            (jstring)m_env->CallStaticObjectMethod(jNDKhelper, getCurrentCardType);
        m_env->DeleteLocalRef(cls);
        const char *s = m_env->GetStringUTFChars(jCardType, NULL);
        cardType      = s;
        m_env->ReleaseStringUTFChars(jCardType, s);

        JniHelper::CheckException(m_env);

        inserted = (cardType != "");
        if (!inserted)
            std::this_thread::sleep_for(std::chrono::milliseconds(maxwait / 10));
    } while (!inserted && std::chrono::steady_clock::now() < clock_timeout);

    if (inserted)
    {
        if (d_card_type != CHIP_UNKNOWN)
            cardType   = d_card_type;
        d_insertedChip = createChip(cardType);
    }

    return inserted;
}

bool AndroidReaderUnit::waitRemoval(unsigned int /*maxwait*/)
{
    jclass cls           = m_env->FindClass("com/islog/liblogicalaccess/AndroidTag");
    jclass jNDKhelper    = (jclass)m_env->NewGlobalRef(cls);
    jmethodID removeCard = m_env->GetStaticMethodID(jNDKhelper, "removeCard", "()V");
    m_env->CallStaticVoidMethod(jNDKhelper, removeCard);
    m_env->DeleteLocalRef(cls);

    JniHelper::CheckException(m_env);
    return true;
}

bool AndroidReaderUnit::connect()
{
    jclass cls            = m_env->FindClass("com/islog/liblogicalaccess/AndroidTag");
    jclass jNDKhelper     = (jclass)m_env->NewGlobalRef(cls);
    jmethodID connectCard = m_env->GetStaticMethodID(jNDKhelper, "connect", "()Z");
    jboolean connected    = m_env->CallStaticBooleanMethod(jNDKhelper, connectCard);
    m_env->DeleteLocalRef(cls);

    JniHelper::CheckException(m_env);
    return (bool)(connected == JNI_TRUE);
}

void AndroidReaderUnit::disconnect()
{
    jclass cls               = m_env->FindClass("com/islog/liblogicalaccess/AndroidTag");
    jclass jNDKhelper        = (jclass)m_env->NewGlobalRef(cls);
    jmethodID disconnectCard = m_env->GetStaticMethodID(jNDKhelper, "disconnect", "()V");
    m_env->CallStaticVoidMethod(jNDKhelper, disconnectCard);
    m_env->DeleteLocalRef(cls);

    JniHelper::CheckException(m_env);
}

bool AndroidReaderUnit::connectToReader()
{
    return ISO7816ReaderUnit::connectToReader();
}

void AndroidReaderUnit::disconnectFromReader()
{
}

std::shared_ptr<Chip> AndroidReaderUnit::getSingleChip()
{
    std::shared_ptr<Chip> chip = d_insertedChip;
    return chip;
}

std::vector<std::shared_ptr<Chip>> AndroidReaderUnit::getChipList()
{
    std::vector<std::shared_ptr<Chip>> chipList;
    std::shared_ptr<Chip> singleChip = getSingleChip();
    if (singleChip)
    {
        chipList.push_back(singleChip);
    }
    return chipList;
}

std::string AndroidReaderUnit::getReaderSerialNumber()
{
    return std::string();
}

bool AndroidReaderUnit::isConnected()
{
    return true;
}

void AndroidReaderUnit::serialize(boost::property_tree::ptree &parentNode)
{
    boost::property_tree::ptree node;
    ReaderUnit::serialize(node);
    parentNode.add_child(getDefaultXmlNodeName(), node);
}

void AndroidReaderUnit::unSerialize(boost::property_tree::ptree &node)
{
    ReaderUnit::unSerialize(node);
}
}
