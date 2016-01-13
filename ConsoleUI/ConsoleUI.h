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
//
//  WebUI.h
//  opendatacon
//
//  Created by Alan Murray on 06/09/2014.
//
//

#ifndef __opendatacon__WebUI__
#define __opendatacon__WebUI__

#include <opendatacon/IUI.h>

#include <asio.hpp>
#include <vector>
#include <map>
#include <sstream>
#include <functional>
#include <opendatacon/ILoggable.h>
#include "tinycon.h"

class ConsoleUI: public ODC::IUI, tinyConsole
{
public:
	ConsoleUI(const std::string& aName, ODC::Context& aParent, const std::string& aConfFilename, const Json::Value& aConfOverrides);
	virtual ~ConsoleUI(void);

	void AddHelp(std::string help);

	/* tinyConsole functions */
	int trigger (std::string s) override;
	int hotkeys(char c) override;

	/* Implement IUI interface */
	void AddCommand(const std::string name, std::function<void (std::stringstream&)> callback, const std::string desc = "No description available\n");
	void AddResponder(const std::string name, const ODC::IUIResponder& pResponder);

	/* Implement Plugin interface */
	void BuildOrRebuild() {};
	void Enable();
	void Disable();

	/* Implement ConfigParser interface */
	void ProcessElements(const Json::Value& JSONRoot) {};

	/* Implement Log Destination interface */
	virtual inline void Log(const ODC::LogEntry& arEntry)
	{
		std::ostringstream oss;

		std::string time_str = platformtime::time_string();

		oss <<time_str<<" - "<< arEntry.GetFilters().toString()<<" - "<<arEntry.GetAlias();
		if(!arEntry.GetLocation())
			oss << " - " << arEntry.GetLocation();
		oss << " - " << arEntry.GetMessage();
		if(arEntry.GetErrorCode() != -1)
			oss << " - " << arEntry.GetErrorCode();

		std::string partial_cmd;
		partial_cmd.assign(buffer.begin(), buffer.end());

		std::unique_lock<std::mutex> lock(mutex);
		std::cout << '\r' << oss.str() << std::endl << _prompt << partial_cmd << std::flush;
	};

private:
	/* */
	std::string context;
	std::unique_ptr<asio::thread> uithread;
	static std::mutex mutex;

	/* tinyConsole functions */
	std::map<std::string,std::function<void (std::stringstream&)> > mCmds;
	std::map<std::string,std::string> mDescriptions;
	std::string help_intro;

	/* UI response handlers */
	std::unordered_map<std::string, const ODC::IUIResponder*> Responders;

	/* Internal functions */
	void ExecuteCommand(const ODC::IUIResponder* pResponder, const std::string& command, std::stringstream& args);
};

#endif /* defined(__opendatacon__WebUI__) */
