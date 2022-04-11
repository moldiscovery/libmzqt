/*
 Debug.cpp
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

#include <QFileInfo>
#include <QString>
#include <QStringList>
#include <QProcess>

#include "Debug.h"

using namespace mzqt;

namespace mzqt {

  class Debug::Manager {

  public:
    Manager();
    void flush(bool addEndLine);
    bool setFile(const QString &path);
    bool possible(Level requestLevel);
    bool init(Level level);

    Debug::Level level_;
    Debug::Level requestLevel_;
    QString buffer_;
    QTextStream stream_;
    QFile file_;
  };
}

Debug::Manager::Manager() :
  level_(Debug::LEVEL_UNDEFINED), requestLevel_(Debug::LEVEL_UNDEFINED),
      buffer_(), stream_(&buffer_)
{

}

void Debug::Manager::flush(bool addEndLine)
{
  if (possible(requestLevel_)) {
#if 0
    qt_message_output(QtDebugMsg, buffer_.toLocal8Bit().data());
#else
    qDebug(buffer_.toLocal8Bit().data());
#endif

    if (file_.isOpen() == true) {

      if (addEndLine)
        stream_ << endl;

      file_.write(buffer_.toLocal8Bit());
      file_.flush();
    }

    buffer_.clear();
  }
}

bool Debug::Manager::setFile(const QString &path)
{
  bool result = true;
  file_.setFileName(path);
  if (file_.open(QIODevice::WriteOnly | QIODevice::Text) == false) {
    qDebug() << "unable to open debugging file: " << path;
    result = false;
  }

  return result;
}

bool Debug::Manager::possible(Level requestLevel)
{
  return requestLevel <= level_ && requestLevel != LEVEL_UNDEFINED || level_
      == ALWAYS;
}

bool Debug::Manager::init(Level level)
{
  bool result = true;

  if (level_ == LEVEL_UNDEFINED) {
    //first time, setup using environment variables
    level_ = NONE;

    QStringList env = QProcess::systemEnvironment();
    QString componentName;
    foreach(const QString &s, env)
      {
        QStringList var = s.split('=');
        if (var.value(0) == "MZQT_DEBUG_LEVEL") {
          bool ok = false;
          int varLevel = var.value(1).toInt(&ok);
          if (!ok || varLevel < Debug::ALWAYS || varLevel > Debug::VERY_HIGH) {
            qDebug() << "invalid MZQT_DEBUG_LEVEL value:" << var.value(1);
            result = false;
          }
          else {
            level_ = (Debug::Level) varLevel;
          }
        }
        else if (var.value(0) == "MZQT_DEBUG_FILE") {
          file_.setFileName(var.value(1));
          if (file_.open(QIODevice::WriteOnly | QIODevice::Text) == false) {
            qDebug() << "invalid MZQT_DEBUG_FILE value:" << var.value(1);
            result = false;
          }
        }
      }
  }

  if (requestLevel_ != level && level != LEVEL_UNDEFINED)
    requestLevel_ = level; //set a new request level for printing

  return result;
}
///////////////////////////////////////////////////////////////////////////////
Debug::Level Debug::level()
{
  return manager().level_;
}

void Debug::setLevel(Level level)
{
  //unedefined only for first time
  if (level != LEVEL_UNDEFINED)
    manager().level_ = level;
}

Debug::Level Debug::requestLevel()
{
  return manager().requestLevel_;
}

QTextStream &Debug::stream()
{
  return manager().stream_;
}

void Debug::flush(bool addEndLine)
{
  manager().flush(addEndLine);
}

bool Debug::setFile(const QString &path)
{
  return manager().setFile(path);
}

QString Debug::filePath()
{
  return QFileInfo(manager().file_).filePath();
}

QString Debug::dirPath()
{
  return QFileInfo(manager().file_).path();
}

bool Debug::possible(Level requestLevel)
{
  return manager().possible(requestLevel);
}

Debug::Manager &Debug::manager()
{
  static Manager instance;

  return instance;
}

Debug &Debug::instance()
{
  static Debug instance;

  return instance;
}

Debug &Debug::dbg(Level level)
{
  instance().manager().init(level);

  return instance();
}

QDebug Debug::msg()
{
  return qDebug();
}

mzqt::Debug &mzqt::operator<<(Debug &d, Debug::Command c)
{
  if (c == Debug::ENDL) {
    d.flush(true);
  }

  return d;
}
