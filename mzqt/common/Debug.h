/*
 Debug.h
 Created on: 22/10/2010
 Author: Fabien Fontaine
 Copyright (C) 2010 Lead Molecular Design Sl
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef MZQT_DEBUG_H_
#define MZQT_DEBUG_H_

#include <iostream>
#include <QDebug>
#include <QFile>

namespace mzqt {

  /*! Singleton debugger class activated by environment variable
   * MZQT_DEBUG_LEVEL and MZQT_DEBUG_FILE
   */

  class Debug {

  public:

    enum Command {
      ENDL
    };

    enum Level {
      LEVEL_UNDEFINED = -2,
      ALWAYS = -1,
      NONE = 0,
      LOW = 1,
      MEDIUM = 2,
      HIGH = 3,
      VERY_HIGH = 4
    };

    static void flush(bool addEndLine = false);
    static void setLevel(Level level);
    static Level level();
    static Level requestLevel();
    static QTextStream &stream();
    static bool setFile(const QString &path);
    static QString filePath();
    static QString dirPath();
    static bool possible(Level requestLevel);

    template<typename T>
    Debug &operator<<(const T &t);

    static Debug &dbg(Level level = LEVEL_UNDEFINED);
    static QDebug msg();

  private:

    class Manager;

    Debug()
    {
    }
    ~Debug()
    {
    }
    Debug(const Debug &); // intentionally undefined
    Debug & operator=(const Debug &); // intentionally undefined

    static Manager &manager();
    static Debug & instance();

  };

  //!< Manipulator like method
  Debug &operator<<(Debug &d, Debug::Command c);
}

template<typename T>
mzqt::Debug &mzqt::Debug::operator<<(const T &t)
{
  if (possible(requestLevel()) == true) {
    stream() << t;
  }

  return dbg(requestLevel());
}

#endif /* MZQT_DEBUG_H_ */
