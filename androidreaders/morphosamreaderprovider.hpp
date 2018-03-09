//
// Created by xaqq on 3/9/18.
//

#pragma once

#include <memory>
#include "logicalaccess/readerproviders/readerprovider.hpp"

namespace logicalaccess
{
#define READER_MORPHO_SAM "MORPHO_SAM"

/**
 * \brief MorphoSAMReaderProvider Reader Provider class.
 *
 * This is for the SAM slot on Morpho/Safran/Sagem tablet.
 */
class LIBLOGICALACCESS_API MorphoSAMReaderProvider : public ReaderProvider
{
  public:
    static std::shared_ptr<MorphoSAMReaderProvider> getSingletonInstance();

    /**
     * \brief Destructor.
     */
    ~MorphoSAMReaderProvider() = default;

    /**
     * \brief Release the provider resources.
     */
    virtual void release();

    /**
     * \brief Get the reader provider type.
     * \return The reader provider type.
     */
    virtual std::string getRPType() const
    {
        return READER_MORPHO_SAM;
    };

    /**
     * \brief Get the reader provider name.
     * \return The reader provider name.
     */
    virtual std::string getRPName() const
    {
        return READER_MORPHO_SAM;
    };

    /**
     * \brief List all readers of the system.
     * \return True if the list was updated, false otherwise.
     */
    virtual bool refreshReaderList();

    /**
     * \brief Get reader list for this reader provider.
     * \return The reader list.
     */
    virtual const ReaderList &getReaderList()
    {
        return d_readers;
    };

    /**
     * \brief Create a new reader unit for the reader provider.
     * \return A reader unit.
     */
    virtual std::shared_ptr<ReaderUnit> createReaderUnit();

  protected:
    /**
     * \brief Constructor.
     */
    MorphoSAMReaderProvider();

    /**
     * \brief The reader list.
     */
    ReaderList d_readers;
};
}

