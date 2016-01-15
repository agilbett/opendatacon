/*	opendatacon
 *
 *	Copyright (c) 2014:
 *
 *		DCrip3fJguWgVCLrZFfA7sIGgvx1Ou3fHfCxnrz4svAi
 *		yxeOtDhDCXf1Z4ApgXvX5ahqQmzRfJ2DoX8S05SqHA==
 *
 *	Licensed under the Apache License, Version 2.0 (the "License");
 *	you may not use this file except in compliance with the License.
 *	You may obtain a copy of the License at
 *
 *		http://www.apache.org/licenses/LICENSE-2.0
 *
 *	Unless required by applicable law or agreed to in writing, software
 *	distributed under the License is distributed on an "AS IS" BASIS,
 *	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *	See the License for the specific language governing permissions and
 *	limitations under the License.
 */
#pragma once

#include <chrono>
#include <mutex>
#include <fstream>
#include <opendatacon/ILoggable.h>


#include <opendatacon/IUI.h>

#include <asio.hpp>
#include <vector>
#include <map>
#include <sstream>
#include <functional>
#include <opendatacon/ILoggable.h>

class FileLog: public ODC::IUI
{
public:
    FileLog(const std::string& aName, ODC::Context& aParent, const std::string& aConfFilename, const Json::Value& aConfOverrides) :
    IUI(aName,aParent,aConfFilename,aConfOverrides),
    mPrintLocation(false),
    mLogName(aName),
    mNumFiles(5),
    mFileSizekB(5*1024),
    mFileIndex(0)
    {};
    
    /* Implement IUI interface */
    void AddCommand(const std::string name, std::function<void (std::stringstream&)> callback, const std::string desc = "No description available\n") {};
    void AddResponder(const std::string name, const ODC::IUIResponder& pResponder) {};
    
    /* Implement Plugin interface */
    void BuildOrRebuild() {};
    void Enable() {};
    void Disable() {};
    
    /* Implement ConfigParser interface */
    void ProcessElements(const Json::Value& JSONRoot) {};
    
    /* Implement Log Destination interface */
    virtual void Log(const ODC::LogEntry& arEntry);
    
private:
    bool mPrintLocation;
    std::string mLogName;
    size_t mNumFiles;
    size_t mFileSizekB;
    size_t mFileIndex;
    std::ofstream mLogFile;
    std::mutex mMutex;
};

/*
class LogToFile
{

public:
        LogToFile(std::string log_name):
                mPrintLocation(false),
                mLogName(log_name),
                mNumFiles(5),
                mFileSizekB(5*1024),
                mFileIndex(0)
        {}
        LogToFile(std::string log_name,size_t file_size_kb, size_t num_files):
                mPrintLocation(false),
                mLogName(log_name),
                mNumFiles(num_files),
                mFileSizekB(file_size_kb),
                mFileIndex(0)
        {}

        void Log( const ODC::LogEntry& arEntry );
        void SetPrintLocation(bool print_loc);

        void SetLogFileSizekB(size_t kB);
        void SetNumLogFiles(size_t num);
        void SetLogName(std::string name);
        size_t GetLogFileSizekB();
        size_t GetNumLogFiles();
        std::string GetLogName();

private:
        bool mPrintLocation;
        std::string mLogName;
        size_t mNumFiles;
        size_t mFileSizekB;
        size_t mFileIndex;
        std::ofstream mLogFile;
        std::mutex mMutex;
};
*/