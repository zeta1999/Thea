//============================================================================
//
// This file is part of the Thea toolkit.
//
// This software is distributed under the BSD license, as detailed in the
// accompanying LICENSE.txt file. Portions are derived from other works:
// their respective licenses and copyright information are reproduced in
// LICENSE.txt and/or in the relevant source files.
//
// Author: Siddhartha Chaudhuri
// First version: 2013
//
//============================================================================

#include "FileSystem.hpp"
#include "StringAlg.hpp"
#include <boost/cstdint.hpp>
#include <boost/filesystem.hpp>
#include <boost/version.hpp>
#include <algorithm>
#include <cstdio>

namespace Thea {

bool
FileSystem::exists(std::string const & path)
{
  return boost::filesystem::exists(path);
}

bool
FileSystem::fileExists(std::string const & path)
{
  return boost::filesystem::is_regular_file(path);
}

bool
FileSystem::directoryExists(std::string const & path)
{
  return boost::filesystem::is_directory(path);
}

int64
FileSystem::fileSize(std::string const & path)
{
  boost::uintmax_t size = boost::filesystem::file_size(path);
  if (size == static_cast<boost::uintmax_t>(-1))
    return -1;
  else
    return static_cast<int64>(size);
}

std::string
FileSystem::resolve(std::string const & path)
{
  return boost::filesystem::system_complete(path).string();
}

bool
FileSystem::createDirectory(std::string const & path)
{
  if (directoryExists(path))
    return true;

  return boost::filesystem::create_directories(path);
}

bool
FileSystem::readWholeFile(std::string const & path, std::string & ret)
{
  if (!fileExists(path))
  {
    THEA_ERROR << "FileSystem: File '" << path << "' not found";
    return false;
  }

  int64 length = fileSize(path);
  if (length <= 0)
  {
    ret.clear();
    return true;
  }

  char * buffer = (char *)std::malloc((size_t)length);
  if (!buffer)
  {
    THEA_ERROR << "FileSystem: Could not allocate buffer to hold " << length << " bytes from file '" << path << '\'';
    return false;
  }

  FILE * f = std::fopen(path.c_str(), "rb");
  if (!f)
  {
    THEA_ERROR << "FileSystem: Couldn't open file '" << path << "' for reading";
    return false;
  }

  size_t num_read = std::fread(buffer, 1, length, f);
  if ((int64)num_read != length)
  {
    THEA_ERROR << "FileSystem: Error reading from file '" << path << '\'';
    return false;
  }

  std::fclose(f);

  ret.assign(buffer, (size_t)length);
  std::free(buffer);

  return true;
}

namespace FileSystemInternal {

bool
objectSatisfiesConstraints(boost::filesystem::directory_entry const & object, int types,
                           Array<std::string> const & patterns, bool ignore_case)
{
  if (types > 0 && types != FileSystem::ObjectType::ALL)
  {
    boost::filesystem::file_status status = object.symlink_status();
    if (!boost::filesystem::is_symlink(status))
      status = object.status();

    bool ok = false;

    if (!ok && (types & FileSystem::ObjectType::FILE) && boost::filesystem::is_regular_file(status))
      ok = true;

    if (!ok && (types & FileSystem::ObjectType::DIRECTORY) && boost::filesystem::is_directory(status))
      ok = true;

    if (!ok && (types & FileSystem::ObjectType::SYMLINK) && boost::filesystem::is_symlink(status))
      ok = true;

    if (!ok)
      return false;
  }

  if (!patterns.empty())
  {
#if BOOST_VERSION / 100000 > 1 || (BOOST_VERSION / 100000 == 1 && BOOST_VERSION / 100 % 1000 > 45)
    std::string name = object.path().filename().string();
#else
    std::string name = object.path().filename();
#endif

    if (ignore_case)
      name = toLower(name);

    bool ok = false;
    for (size_t i = 0; !ok && i < patterns.size(); ++i)
      if (patternMatch(patterns[i], name))
        ok = true;

    if (!ok)
      return false;
  }

  return true;
}

} // namespace FileSystemInternal

intx
FileSystem::getDirectoryContents(std::string const & dir, Array<std::string> & objects, int types, std::string const & patterns,
                                 int flags)
{
  if (!directoryExists(dir))
    return -1;

  bool ignore_case  =  (flags & Flags::CASE_INSENSITIVE);
  bool recursive    =  (flags & Flags::RECURSIVE);
  bool sorted       =  (flags & Flags::SORTED);

  Array<std::string> patlist;
  if (!patterns.empty())
  {
    stringSplit(patterns, ' ', patlist, true);

    if (ignore_case)
    {
      for (size_t i = 0; i < patlist.size(); ++i)
        patlist[i] = toLower(patlist[i]);
    }
  }

  objects.clear();

  if (recursive)
  {
    boost::filesystem::recursive_directory_iterator objects_end;
    for (boost::filesystem::recursive_directory_iterator iter(dir); iter != objects_end; ++iter)
      if (FileSystemInternal::objectSatisfiesConstraints(*iter, types, patlist, ignore_case))
        objects.push_back(iter->path().string());
  }
  else
  {
    boost::filesystem::directory_iterator objects_end;
    for (boost::filesystem::directory_iterator iter(dir); iter != objects_end; ++iter)
      if (FileSystemInternal::objectSatisfiesConstraints(*iter, types, patlist, ignore_case))
        objects.push_back(iter->path().string());
  }

  if (sorted)
    std::sort(objects.begin(), objects.end());

  return (intx)objects.size();
}

bool
FileSystem::remove(std::string const & path, bool recursive)
{
  try
  {
    if (recursive)
      boost::filesystem::remove_all(path);
    else
      boost::filesystem::remove(path);
  }
  catch (...)
  {
    return false;
  }

  return true;
}

bool
FileSystem::copyFile(std::string const & from, std::string const & to)
{
  try
  {
    boost::filesystem::copy_file(from, to);
  }
  catch (...)
  {
    return false;
  }

  return true;
}

} // namespace Thea
