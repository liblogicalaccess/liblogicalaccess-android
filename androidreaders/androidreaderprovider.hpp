/**
 * \file androidreaderprovider.hpp
 * \author Adrien J. <adrien-dev@islog.com>
 * \brief Android card reader provider.
 */

#ifndef LOGICALACCESS_ANDROIDREADERPROVIDER_HPP
#define LOGICALACCESS_ANDROIDREADERPROVIDER_HPP

#include "androidreaderunit.hpp"
#include "logicalaccess/readerproviders/readerprovider.hpp"

namespace logicalaccess
{
#define READER_ANDROID "Android"

/**
 * \brief Android Reader Provider class.
 */
class  AndroidReaderProvider : public ReaderProvider
{
  public:
    /**
     * \brief Get the AndroidReaderProvider instance.
     */
    static std::shared_ptr<AndroidReaderProvider> getSingletonInstance();

    /**
     * \brief Destructor.
     */
    ~AndroidReaderProvider();

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
        return READER_ANDROID;
    };

    /**
     * \brief Get the reader provider name.
     * \return The reader provider name.
     */
    virtual std::string getRPName() const
    {
        return READER_ANDROID;
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
    AndroidReaderProvider();

    /**
     * \brief The reader list.
     */
    ReaderList d_readers;
};
}

#endif /* LOGICALACCESS_ANDROIDREADERPROVIDER_HPP */
