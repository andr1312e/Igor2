/******************************************************************************
 *
 * package:
 * file:        hierarchy.h
 * created:     September 2007
 * author:      Martin Heinrich
 *
 *
 * Copyright 2007 Martin Heinrich
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/

#ifndef LOG4QT_HIERARCHY_H
#define LOG4QT_HIERARCHY_H

#include "loggerrepository.h"

#include <QHash>
#include <QReadWriteLock>

namespace Log4Qt
{

/*!
 * \brief The class Hierarchy implements a logger repository.
 *
 * \note All the functions declared in this class are thread-safe.
 */
class  LOG4QT_EXPORT Hierarchy : public LoggerRepository
{
public:
    Hierarchy();
    ~Hierarchy() Q_DECL_OVERRIDE;

public:
    bool exists(const QString &name) const Q_DECL_OVERRIDE;
    Logger *logger(const QString &name) Q_DECL_OVERRIDE;
    QList<Logger *> loggers() const Q_DECL_OVERRIDE;
    Logger *rootLogger() const Q_DECL_OVERRIDE;
    Level threshold() const Q_DECL_OVERRIDE;
    void setThreshold(Level level) Q_DECL_OVERRIDE;
    void setThreshold(const QString &threshold) Q_DECL_OVERRIDE;

    bool isDisabled(Level level) Q_DECL_OVERRIDE;
    void resetConfiguration() Q_DECL_OVERRIDE;
    void shutdown() Q_DECL_OVERRIDE;

private:
    Logger *createLogger(const QString &name);
    void resetLogger(Logger *logger, Level level) const;

private:
    mutable QReadWriteLock mObjectGuard;
    QHash<QString, Logger *> mLoggers;
    Level mThreshold;
    Logger *mRootLogger;
};

inline Logger *Hierarchy::rootLogger() const
{
    return mRootLogger;
}

inline Level Hierarchy::threshold() const
{
    return mThreshold;
}

inline void Hierarchy::setThreshold(Level level)
{
    mThreshold = level;
}

inline bool Hierarchy::isDisabled(Level level)
{
    return level < mThreshold;
}

} // namespace Log4Qt

#endif // LOG4QT_HIERARCHY_H
