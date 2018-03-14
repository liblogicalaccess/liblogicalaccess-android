/**
 * \file MifareAndroidCommands.hpp
 * \author Adrien J. <adrien.jund@islog.com>
 * \brief Android Mifare commands.
 */

#ifndef MIFAREANDROIDCOMMANDS_HPP
#define MIFAREANDROIDCOMMANDS_HPP

#include "logicalaccess/cards/commands.hpp"

#include <string>
#include <vector>

namespace logicalaccess
{
class LIBLOGICALACCESS_API MifareAndroidCommands : public Commands
{
  public:
    MifareAndroidCommands();
    std::vector<unsigned char> readBlock(int sector, int block,
                                         std::vector<unsigned char> keyA);
};
}

#endif /* MIFAREANDROIDCOMMANDS_HPP */