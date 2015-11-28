/*****************************************************************************************
 *                                                                                       *
 * GHOUL                                                                                 *
 * General Helpful Open Utility Library                                                  *
 *                                                                                       *
 * Copyright (c) 2012-2014                                                               *
 *                                                                                       *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this  *
 * software and associated documentation files (the "Software"), to deal in the Software *
 * without restriction, including without limitation the rights to use, copy, modify,    *
 * merge, publish, distribute, sublicense, and/or sell copies of the Software, and to    *
 * permit persons to whom the Software is furnished to do so, subject to the following   *
 * conditions:                                                                           *
 *                                                                                       *
 * The above copyright notice and this permission notice shall be included in all copies *
 * or substantial portions of the Software.                                              *
 *                                                                                       *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,   *
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A         *
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT    *
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF  *
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE  *
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                                         *
 ****************************************************************************************/

/// Logs the 'message' with the 'category' at a level of 'logLevel'
#define LOGC(__loglevel__, __category__, __message__) \
    do { \
        std::ostringstream __tmp__; __tmp__ << __message__; \
        if (mocca::LogManager::isInitialized()) \
            LogMgr.logMessage( \
                (__loglevel__), \
                (__category__), \
                __tmp__.str() \
            ); \
        else { \
            std::cout << (__category__) << " (" << \
            mocca::LogManager::stringFromLevel(__loglevel__) << \
            ") : " << __tmp__.str() << std::endl; \
        } \
    } while (false)

#define LOGNC(__loglevel__, __message__) \
    do { \
        std::ostringstream __tmp__; __tmp__ << __message__; \
        if (mocca::LogManager::isInitialized()) \
            LogMgr.logMessage( \
                (__loglevel__), \
                __tmp__.str() \
            ); \
                else { \
            std::cout << "(" << \
            mocca::LogManager::stringFromLevel(__loglevel__) << \
            ") : " << __tmp__.str() << std::endl; \
                } \
        } while (false)

/// Logs the 'message' with the 'category' at a level of LogManager::LogLevel::Debug
#define LDEBUGC(__category__, __message__) \
    LOGC(mocca::LogManager::LogLevel::Debug, __category__, __message__)

/// Logs the 'message' with the 'category' at a level of LogManager::LogLevel::Info
#define LINFOC(__category__, __message__) \
    LOGC(mocca::LogManager::LogLevel::Info, __category__, __message__)

/// Logs the 'message' with the 'category' at a level of LogManager::LogLevel::Warning
#define LWARNINGC(__category__, __message__) \
    LOGC(mocca::LogManager::LogLevel::Warning, __category__, __message__)

/// Logs the 'message' with the 'category' at a level of LogManager::LogLevelError
#define LERRORC(__category__, __message__) \
    LOGC(mocca::LogManager::LogLevel::Error, __category__, __message__)

/// Logs the 'message' with the 'category' at a level of LogManager::LogLevelFatal
#define LFATALC(__category__, __message__) \
    LOGC(mocca::LogManager::LogLevel::Fatal, __category__, __message__)

/**
 * Logs the 'message' with a level of LogManager::LogLevel::Debug.
 */
#define LDEBUG(__message__) \
    LOGNC(mocca::LogManager::LogLevel::Debug, __message__)

/**
 * Logs the 'message' with a level of LogManager::LogLevel::Info.
 */
#define LINFO(__message__) \
    LOGNC(mocca::LogManager::LogLevel::Info, __message__)

/**
 * Logs the 'message' with a level of LogManager::LogLevel::Warning.
 */
#define LWARNING(__message__) \
    LOGNC(mocca::LogManager::LogLevel::Warning, __message__)

/**
 * Logs the 'message' with a level of LogManager::LogLevel::Error.
 */
#define LERROR(__message__) \
    LOGNC(mocca::LogManager::LogLevel::Error, __message__)

/**
 * Logs the 'message' with a level of LogManager::LogLevel::Fatal.
 */
#define LFATAL(__message__) \
    LOGNC(mocca::LogManager::LogLevel::Fatal, __message__)
