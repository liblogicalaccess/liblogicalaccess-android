/**
 * \file stidstrreaderunit.cpp
 * \author Maxime C. <maxime-dev@islog.com>
 * \brief STidSTR reader unit.
 */

#include "androidreaderunit.hpp"
#include "androidreaderprovider.hpp"


#include <iostream>
#include <iomanip>
#include <sstream>

#include "logicalaccess/myexception.hpp"
#include "logicalaccess/services/accesscontrol/cardsformatcomposite.hpp"

#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <logicalaccess/logs.hpp>
#include "logicalaccess/cards/commands.hpp"
#include <chrono>
#include <thread>
#include "logicalaccess/dynlibrary/librarymanager.hpp"
#include "../../../liblogicalaccess/plugins/pluginsreaderproviders/iso7816/commands/desfireiso7816resultchecker.hpp"
#include "../../../liblogicalaccess/plugins/pluginsreaderproviders/iso7816/readercardadapters/iso7816readercardadapter.hpp"
#include "readercardadapters/androiddatatransport.hpp"
#include "androidreaderunitconfiguration.hpp"
#include "jnihelper.h"


namespace logicalaccess
{
	JNIEnv *AndroidReaderUnit::m_env;

	AndroidReaderUnit::AndroidReaderUnit()
		: ReaderUnit(READER_ANDROID)
	{
		d_readerUnitConfig.reset(new AndroidReaderUnitConfiguration());

		setDataTransport(std::make_shared<AndroidDataTransport>());

		std::shared_ptr<ReaderCardAdapter> rca(new ReaderCardAdapter());
		rca->setDataTransport(getDataTransport());

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
			std::shared_ptr<ReaderCardAdapter> rca = std::make_shared<ISO7816ReaderCardAdapter>();
			std::shared_ptr<Commands> commands;

			if (type == "DESFire" || type == "DESFireEV1")
			{
				LOG(LogLevel::INFOS) << "Mifare DESFire EV1 Chip created";
				rca->setResultChecker(std::make_shared<DESFireISO7816ResultChecker>());
				commands = LibraryManager::getInstance()->getCommands("DESFireEV1ISO7816");
				*(void**)(&setcryptocontextfct) = LibraryManager::getInstance()->getFctFromName("setCryptoContextDESFireEV1ISO7816Commands", LibraryManager::READERS_TYPE);
				setcryptocontextfct(&commands, &chip);
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
		}
		return chip;
	}

	bool AndroidReaderUnit::waitInsertion(unsigned int maxwait)
	{
		bool inserted = false;
		std::string cardType = "";
		std::chrono::steady_clock::time_point const clock_timeout = std::chrono::steady_clock::now() + std::chrono::milliseconds(maxwait);
		do
		{
			jclass cls = m_env->FindClass("com/islog/liblogicalaccess/IsoDepCommand");
			jclass jNDKhelper = (jclass) m_env->NewGlobalRef(cls);
			jmethodID getMycardType = m_env->GetStaticMethodID(jNDKhelper, "getMycardType", "()Ljava/lang/String;");
			jstring jCardType = (jstring) m_env->CallStaticObjectMethod(jNDKhelper, getMycardType);
			m_env->DeleteLocalRef(cls);
			const char *s = m_env->GetStringUTFChars(jCardType, NULL);
			cardType = s;
			m_env->ReleaseStringUTFChars(jCardType, s);

			JniHelper::CheckException(m_env);

			inserted = (cardType != "");
			if (!inserted)
				std::this_thread::sleep_for(std::chrono::milliseconds(maxwait / 10));
		} while (!inserted && std::chrono::steady_clock::now() < clock_timeout);

		if (inserted)
		{
			if (d_card_type != CHIP_UNKNOWN)
				cardType = d_card_type;
			d_insertedChip = createChip(cardType);
		}

		return inserted;
	}

	bool AndroidReaderUnit::waitRemoval(unsigned int maxwait)
	{
		jclass cls = m_env->FindClass("com/islog/liblogicalaccess/IsoDepCommand");
		jclass jNDKhelper = (jclass) m_env->NewGlobalRef(cls);
		jmethodID removeCard = m_env->GetStaticMethodID(jNDKhelper, "removeCard", "()V");
		m_env->CallStaticVoidMethod(jNDKhelper, removeCard);
		m_env->DeleteLocalRef(cls);

		JniHelper::CheckException(m_env);
		return true;
	}

	bool AndroidReaderUnit::connect()
	{
		//TODO
		return true;
	}

	void AndroidReaderUnit::disconnect()
	{
	}

	bool AndroidReaderUnit::connectToReader()
	{
		return true;
	}

	void AndroidReaderUnit::disconnectFromReader()
	{
	}

	std::shared_ptr<Chip> AndroidReaderUnit::getSingleChip()
	{
		std::shared_ptr<Chip> chip = d_insertedChip;
		return chip;
	}

	std::vector<std::shared_ptr<Chip> > AndroidReaderUnit::getChipList()
	{
		std::vector<std::shared_ptr<Chip> > chipList;
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
	
	void AndroidReaderUnit::serialize(boost::property_tree::ptree& parentNode)
	{
		boost::property_tree::ptree node;
		ReaderUnit::serialize(node);
		parentNode.add_child(getDefaultXmlNodeName(), node);
	}

	void AndroidReaderUnit::unSerialize(boost::property_tree::ptree& node)
	{
		ReaderUnit::unSerialize(node);
	}
}
