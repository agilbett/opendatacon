/*	opendatacon
 *
 *	Copyright (c) 2016:
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
 * SNMPSingleton.h
 *
 *  Created on: 30/03/2016
 *      Author: Alan Murray
 */

#ifndef opendatacon_suite_SNMPSingleton_h
#define opendatacon_suite_SNMPSingleton_h

#include "SNMPPort.h"

class SNMPSingleton
{
public:
	SNMPSingleton(SNMPSingleton const&) = delete;
	void operator=(SNMPSingleton const&) = delete;
	
	static SNMPSingleton& GetInstance()
	{
		static SNMPSingleton instance;
		return instance;
	}
	
	static void SnmpCallback(int reason, Snmp_pp::Snmp *snmp, Snmp_pp::Pdu &pdu, Snmp_pp::SnmpTarget &target, void *obj)
	{
		GetInstance().SnmpCallbackImpl(reason, snmp, pdu, target, obj);
	}
	
	void RegisterPort(const std::string& destIP, SNMPPort* target);
	void UnregisterPort(SNMPPort* target);
	std::shared_ptr<Snmp_pp::Snmp> GetSession(uint16_t UdpPort);
	std::shared_ptr<Snmp_pp::Snmp> GetTrapSession(uint16_t UdpPort, const std::string& SourceAddr );
	std::shared_ptr<Snmp_pp::v3MP> Getv3MP();

private:
	void SnmpCallbackImpl(int reason, Snmp_pp::Snmp *snmp, Snmp_pp::Pdu &pdu, Snmp_pp::SnmpTarget &target, void *obj);
	
	std::unordered_map<std::string, SNMPPort*> SourcePortMap;
	std::shared_ptr<Snmp_pp::v3MP> v3mpPtr;
	std::unordered_map<uint16_t, std::weak_ptr<Snmp_pp::Snmp>> SnmpSessions;
	std::unordered_map<uint16_t, std::weak_ptr<Snmp_pp::Snmp>> SnmpTrapSessions;
	std::unordered_set<SNMPPort*> PortSet;
	
	SNMPSingleton()
	{
		Snmp_pp::DefaultLog::log()->set_profile("off");
		Snmp_pp::Snmp::socket_startup();  // Initialize socket subsystem
	}
	
	~SNMPSingleton()
	{
		Snmp_pp::Snmp::socket_cleanup();  // Shut down socket subsystem
	}
};

#endif