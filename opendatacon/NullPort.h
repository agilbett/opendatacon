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
/*
 * NullPort.h
 *
 *  Created on: 04/08/2014
 *      Author: Neil Stephens <dearknarl@gmail.com>
 */

#ifndef NULLPORT_H_
#define NULLPORT_H_

#include <opendatacon/DataPort.h>

/* The equivalent of /dev/null as a DataPort */
class NullPort: public DataPort
{
private:
	typedef asio::basic_waitable_timer<std::chrono::steady_clock> Timer_t;
	std::unique_ptr<Timer_t> pTimer;
public:
	NullPort(std::string aName, std::string aConfFilename, const Json::Value aConfOverrides):
		DataPort(aName, aConfFilename, aConfOverrides)
	{}
	void Enable() override
	{
		pTimer.reset(new Timer_t(*pIOS, std::chrono::seconds(3)));
		pTimer->async_wait(
		      [this](asio::error_code err_code)
		      {
		            if (err_code != asio::error::operation_aborted)
				{
					PublishEvent(ConnectState::PORT_UP, 0);
					PublishEvent(ConnectState::CONNECTED, 0);
				}
			});
		return;
	}
	void Disable() override
	{
		pTimer->cancel();
		pTimer.reset();
		PublishEvent(ConnectState::PORT_DOWN, 0);
		PublishEvent(ConnectState::DISCONNECTED, 0);
	}
	void BuildOrRebuild(IOManager& IOMgr, openpal::LogFilters& LOG_LEVEL) override{}
	void ProcessElements(const Json::Value& JSONRoot) override{}

	//so the compiler won't warn we're hiding the base class overload we still want to use
	using DataPort::Event;

	std::future<CommandStatus> Event(const Binary& meas, uint16_t index, const std::string& SenderName) override { return IOHandler::CommandFutureSuccess(); }
	std::future<CommandStatus> Event(const DoubleBitBinary& meas, uint16_t index, const std::string& SenderName) override { return IOHandler::CommandFutureSuccess(); }
	std::future<CommandStatus> Event(const Analog& meas, uint16_t index, const std::string& SenderName) override { return IOHandler::CommandFutureSuccess(); }
	std::future<CommandStatus> Event(const Counter& meas, uint16_t index, const std::string& SenderName) override { return IOHandler::CommandFutureSuccess(); }
	std::future<CommandStatus> Event(const FrozenCounter& meas, uint16_t index, const std::string& SenderName) override { return IOHandler::CommandFutureSuccess(); }
	std::future<CommandStatus> Event(const BinaryOutputStatus& meas, uint16_t index, const std::string& SenderName) override { return IOHandler::CommandFutureSuccess(); }
	std::future<CommandStatus> Event(const AnalogOutputStatus& meas, uint16_t index, const std::string& SenderName) override { return IOHandler::CommandFutureSuccess(); }

	std::future<CommandStatus> Event(const ControlRelayOutputBlock& arCommand, uint16_t index, const std::string& SenderName) override { return IOHandler::CommandFutureSuccess(); }
	std::future<CommandStatus> Event(const AnalogOutputInt16& arCommand, uint16_t index, const std::string& SenderName) override { return IOHandler::CommandFutureSuccess(); }
	std::future<CommandStatus> Event(const AnalogOutputInt32& arCommand, uint16_t index, const std::string& SenderName) override { return IOHandler::CommandFutureSuccess(); }
	std::future<CommandStatus> Event(const AnalogOutputFloat32& arCommand, uint16_t index, const std::string& SenderName) override { return IOHandler::CommandFutureSuccess(); }
	std::future<CommandStatus> Event(const AnalogOutputDouble64& arCommand, uint16_t index, const std::string& SenderName) override { return IOHandler::CommandFutureSuccess(); }

	std::future<CommandStatus> Event(const BinaryQuality qual, uint16_t index, const std::string& SenderName) override { return IOHandler::CommandFutureSuccess(); }
	std::future<CommandStatus> Event(const DoubleBitBinaryQuality qual, uint16_t index, const std::string& SenderName) override { return IOHandler::CommandFutureSuccess(); }
	std::future<CommandStatus> Event(const AnalogQuality qual, uint16_t index, const std::string& SenderName) override { return IOHandler::CommandFutureSuccess(); }
	std::future<CommandStatus> Event(const CounterQuality qual, uint16_t index, const std::string& SenderName) override { return IOHandler::CommandFutureSuccess(); }
	std::future<CommandStatus> Event(const FrozenCounterQuality qual, uint16_t index, const std::string& SenderName) override { return IOHandler::CommandFutureSuccess(); }
	std::future<CommandStatus> Event(const BinaryOutputStatusQuality qual, uint16_t index, const std::string& SenderName) override { return IOHandler::CommandFutureSuccess(); }
	std::future<CommandStatus> Event(const AnalogOutputStatusQuality qual, uint16_t index, const std::string& SenderName) override { return IOHandler::CommandFutureSuccess(); }

	std::future<CommandStatus> ConnectionEvent(ConnectState state, const std::string& SenderName) override { return IOHandler::CommandFutureSuccess(); }
};

#endif /* NULLPORT_H_ */
