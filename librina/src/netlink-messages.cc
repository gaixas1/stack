//
// Netlink messages
//
//    Eduard Grasa          <eduard.grasa@i2cat.net>
//    Leonardo Bergesio     <leonardo.bergesio@i2cat.net>
//    Francesco Salvestrini <f.salvestrini@nextworks.it>
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
// MA  02110-1301  USA
//

#include <sstream>
#include <unistd.h>

#define RINA_PREFIX "librina.nl-messages"

#include "librina/logs.h"
#include "netlink-messages.h"

#include "librina/application.h"
#include "librina/ipc-process.h"
#include "librina/ipc-manager.h"

namespace rina {

/* CLASS BASE NETLINK MESSAGE */
BaseNetlinkMessage::BaseNetlinkMessage(
		RINANetlinkOperationCode operationCode) {
	this->operationCode = operationCode;
	sourcePortId = 0;
	destPortId = 0;
	sourceIPCProcessId = 0;
	destIPCProcessId = 0;
	sequenceNumber = 0;
	family = -1;
	responseMessage = false;
	requestMessage = false;
	notificationMessage = false;
	port_id = 0;
	result = 0;
}

BaseNetlinkMessage::~BaseNetlinkMessage() {
}

unsigned int BaseNetlinkMessage::getDestPortId() const {
	return destPortId;
}

void BaseNetlinkMessage::setDestPortId(unsigned int destPortId) {
	this->destPortId = destPortId;
}

unsigned int BaseNetlinkMessage::getSequenceNumber() const {
	return sequenceNumber;
}

void BaseNetlinkMessage::setSequenceNumber(unsigned int sequenceNumber) {
	this->sequenceNumber = sequenceNumber;
}

unsigned int BaseNetlinkMessage::getSourcePortId() const {
	return sourcePortId;
}

void BaseNetlinkMessage::setSourcePortId(unsigned int sourcePortId) {
	this->sourcePortId = sourcePortId;
}

unsigned short BaseNetlinkMessage::getDestIpcProcessId() const {
	return destIPCProcessId;
}

void BaseNetlinkMessage::setDestIpcProcessId(unsigned short destIpcProcessId) {
	destIPCProcessId = destIpcProcessId;
}

unsigned short BaseNetlinkMessage::getSourceIpcProcessId() const {
	return sourceIPCProcessId;
}

void BaseNetlinkMessage::setSourceIpcProcessId(
		unsigned short sourceIpcProcessId) {
	sourceIPCProcessId = sourceIpcProcessId;
}

RINANetlinkOperationCode BaseNetlinkMessage::getOperationCode() const {
	return operationCode;
}

int BaseNetlinkMessage::getFamily() const {
	return family;
}

void BaseNetlinkMessage::setFamily(int family) {
	this->family = family;
}

bool BaseNetlinkMessage::isNotificationMessage() const {
	return notificationMessage;
}

void BaseNetlinkMessage::setNotificationMessage(bool notificationMessage) {
	this->notificationMessage = notificationMessage;
}

void BaseNetlinkMessage::setOperationCode(
		RINANetlinkOperationCode operationCode) {
	this->operationCode = operationCode;
}

bool BaseNetlinkMessage::isRequestMessage() const {
	return requestMessage;
}

void BaseNetlinkMessage::setRequestMessage(bool requestMessage) {
	this->requestMessage = requestMessage;
}

bool BaseNetlinkMessage::isResponseMessage() const {
	return responseMessage;
}

void BaseNetlinkMessage::setResponseMessage(bool responseMessage) {
	this->responseMessage = responseMessage;
}

const std::string BaseNetlinkMessage::toString(){
	std::stringstream ss;
	std::string type;
	if (requestMessage) {
		type = "Request";
	} else if (responseMessage) {
		type = "Response";
	} else if (notificationMessage) {
		type = "Notification";
	} else {
		type = "Unknown";
	}

	ss << "Fam: " << family << "; Opcode: "
			<< BaseNetlinkMessage::operationCodeToString(operationCode)
			<< "; Sport: " << sourcePortId
			<< "; Dport: " << destPortId
			<< "; Seqnum: " << sequenceNumber
			<< "; " << type
			<< "; SIPCP: " << sourceIPCProcessId
			<< "; DIPCP: " << destIPCProcessId;
	return ss.str();
}

const std::string BaseNetlinkMessage::operationCodeToString(RINANetlinkOperationCode operationCode) {
	std::string result;

	switch(operationCode) {
	case RINA_C_UNSPEC:
		result = "0_Unespecified operation";
		break;
	case RINA_C_IPCM_ASSIGN_TO_DIF_REQUEST:
		result = "1_ASSIGN_TO_DIF_REQ";
		break;
	case RINA_C_IPCM_ASSIGN_TO_DIF_RESPONSE:
		result = "2_ASSIGN_TO_DIF_RESP";
		break;
	case RINA_C_IPCM_UPDATE_DIF_CONFIG_REQUEST:
		result = "3_UPDATE_DIF_CONF_REQ";
		break;
	case RINA_C_IPCM_UPDATE_DIF_CONFIG_RESPONSE:
		result = "4_UPDATE_DIF_CONFIG_RESP";
		break;
	case RINA_C_IPCM_IPC_PROCESS_DIF_REGISTRATION_NOTIFICATION:
		result = "5_DIF_REG_NOT";
		break;
	case RINA_C_IPCM_IPC_PROCESS_DIF_UNREGISTRATION_NOTIFICATION:
		result = "6_DIF_UNREG_NOT";
		break;
	case RINA_C_IPCM_ENROLL_TO_DIF_REQUEST:
		result = "7_ENROLL_TO_DIF_REQ";
		break;
	case RINA_C_IPCM_ENROLL_TO_DIF_RESPONSE:
		result = "8_ENROLL_TO_DIF_RESP";
		break;
	case RINA_C_IPCM_DISCONNECT_FROM_NEIGHBOR_REQUEST:
		result = "9_DISC_FROM_NEIGH_REQ";
		break;
	case RINA_C_IPCM_DISCONNECT_FROM_NEIGHBOR_RESPONSE:
		result = "10_DISC_FROM_NEIGH_RESP";
		break;
	case RINA_C_IPCM_ALLOCATE_FLOW_REQUEST:
		result = "11_IPCM_ALLOC_FLOW_REQ";
		break;
	case RINA_C_IPCM_ALLOCATE_FLOW_REQUEST_ARRIVED:
		result = "12_IPCM_ALLOC_FLOW_RESP_ARR";
		break;
	case RINA_C_IPCM_ALLOCATE_FLOW_REQUEST_RESULT:
		result = "13_IPCM_ALLOC_FLOW_REQ_RES";
		break;
	case RINA_C_IPCM_ALLOCATE_FLOW_RESPONSE:
		result = "14_IPCM_ALLOC_FLOW_RESP";
		break;
	case RINA_C_IPCM_DEALLOCATE_FLOW_REQUEST:
		result = "15_IPCM_DEALLOC_FLOW_REQ";
		break;
	case RINA_C_IPCM_DEALLOCATE_FLOW_RESPONSE:
		result = "16_ICPM_DEALLOC_FLOW_RESP";
		break;
	case RINA_C_IPCM_FLOW_DEALLOCATED_NOTIFICATION:
		result = "17_IPCM_FLOW_DEALLOC_NOT";
		break;
	case RINA_C_IPCM_REGISTER_APPLICATION_REQUEST:
		result = "18_IPCM_REG_APP_REQ";
		break;
	case RINA_C_IPCM_REGISTER_APPLICATION_RESPONSE:
		result = "19_ICPM_REG_APP_RESP";
		break;
	case RINA_C_IPCM_UNREGISTER_APPLICATION_REQUEST:
		result = "20_IPCM_UNREG_APP_REQ";
		break;
	case RINA_C_IPCM_UNREGISTER_APPLICATION_RESPONSE:
		result = "21_IPCM_UNREG_APP_RESP";
		break;
	case RINA_C_IPCM_QUERY_RIB_REQUEST:
		result = "22_QUERY_RIB_REQ";
		break;
	case RINA_C_IPCM_QUERY_RIB_RESPONSE:
		result = "23_QUERY_RIB_RESP";
		break;
	case RINA_C_RMT_MODIFY_FTE_REQUEST:
		result = "24_MODIFY_FT_REQ";
		break;
	case RINA_C_RMT_DUMP_FT_REQUEST:
		result = "25_DUMP_FT_REQ";
		break;
	case RINA_C_RMT_DUMP_FT_REPLY:
		result = "26_DUMP_FT_RESP";
		break;
	case RINA_C_IPCM_SOCKET_CLOSED_NOTIFICATION:
		result = "27_SOCK_CLOSED_NOT";
		break;
	case RINA_C_IPCM_IPC_MANAGER_PRESENT:
		result = "28_IPCM_PRESENT";
		break;
	case RINA_C_IPCP_CONN_CREATE_REQUEST:
		result = "29_CREATE_EFCP_CONN_REQ";
		break;
	case RINA_C_IPCP_CONN_CREATE_RESPONSE:
		result = "30_CREATE_EFCP_CONN_RESP";
		break;
	case RINA_C_IPCP_CONN_CREATE_ARRIVED:
		result = "31_CREATE_EFCP_CONN_ARR";
		break;
	case RINA_C_IPCP_CONN_CREATE_RESULT:
		result = "32_CREATE_EFCP_CONN_RES";
		break;
	case RINA_C_IPCP_CONN_UPDATE_REQUEST:
		result = "33_UPDATE_EFCP_CONN_REQ";
		break;
	case RINA_C_IPCP_CONN_UPDATE_RESULT:
		result = "34_UPDATE_EFCP_CONN_RES";
		break;
	case RINA_C_IPCP_CONN_DESTROY_REQUEST:
		result = "35_DESTROY_EFCP_CONN_REQ";
		break;
	case RINA_C_IPCP_CONN_DESTROY_RESULT:
		result = "36_DESTROY_EFCP_CONN_RES";
		break;
	case RINA_C_IPCM_SET_POLICY_SET_PARAM_REQUEST:
		result = "37_SET_POL_SET_PARAM_REQ";
		break;
	case RINA_C_IPCM_SET_POLICY_SET_PARAM_RESPONSE:
		result = "38_SET_POL_SET_PARAM_RESP";
		break;
	case RINA_C_IPCM_SELECT_POLICY_SET_REQUEST:
		result = "39_SET_POL_SET_REQ";
		break;
	case RINA_C_IPCM_SELECT_POLICY_SET_RESPONSE:
		result = "40_SET_POL_SET_RESP";
		break;
	case RINA_C_IPCP_UPDATE_CRYPTO_STATE_REQUEST:
		result = "41_UPDATE_CRYPTO_STATE_REQ";
		break;
	case RINA_C_IPCP_UPDATE_CRYPTO_STATE_RESPONSE:
		result = "42_UPDATE_CRYPTO_STATE_RESP";
		break;
	case RINA_C_IPCP_ADDRESS_CHANGE_REQUEST:
		result = "43_ADDRESS_CHANGE_REQ";
		break;
	case RINA_C_IPCP_ALLOCATE_PORT_REQUEST:
		result = "44_ALLOCATE_PORT_REQUEST";
		break;
	case RINA_C_IPCP_ALLOCATE_PORT_RESPONSE:
		result = "45_ALLOCATE_PORT_RESPONSE";
		break;
	case RINA_C_IPCP_DEALLOCATE_PORT_REQUEST:
		result = "46_DEALLOCATE_PORT_REQUEST";
		break;
	case RINA_C_IPCP_DEALLOCATE_PORT_RESPONSE:
		result = "47_DEALLOCATE_PORT_RESPONSE";
		break;
	case RINA_C_IPCP_MANAGEMENT_SDU_WRITE_REQUEST:
		result = "48_MANAGEMENT_SDU_WRITE_REQUEST";
		break;
	case RINA_C_IPCP_MANAGEMENT_SDU_WRITE_RESPONSE:
		result = "49_MANAGEMENT_SDU_WRITE_RESPONSE";
		break;
	case RINA_C_IPCP_MANAGEMENT_SDU_READ_NOTIF:
		result = "50_MANAGEMENT_SDU_READ_NOTIF";
		break;
	case RINA_C_IPCM_CREATE_IPCP_REQUEST:
		result = "51_CREATE_IPCP_REQUEST";
		break;
	case RINA_C_IPCM_CREATE_IPCP_RESPONSE:
		result = "52_CREATE_IPCP_RESPONSE";
		break;
	case RINA_C_IPCM_DESTROY_IPCP_REQUEST:
		result = "53_DESTROY_IPCP_REQUEST";
		break;
	case RINA_C_IPCM_DESTROY_IPCP_RESPONSE:
		result = "54_DESTROY_IPCP_RESPONSE";
		break;
	case RINA_C_IPCM_IPC_PROCESS_INITIALIZED:
		result = "55_IPC_PROCESS_INITIALIZED";
		break;
	case RINA_C_APP_ALLOCATE_FLOW_REQUEST:
		result = "56_APP_ALLOC_FLOW_REQ";
		break;
	case RINA_C_APP_ALLOCATE_FLOW_REQUEST_RESULT:
		result = "57_APP_ALLOC_FLOW_REQ_RES";
		break;
	case RINA_C_APP_ALLOCATE_FLOW_REQUEST_ARRIVED:
		result = "58_APP_ALLOC_FLOW_REQ_ARR";
		break;
	case RINA_C_APP_ALLOCATE_FLOW_RESPONSE:
		result = "59_APP_ALLOC_FLOW_RESP";
		break;
	case RINA_C_APP_DEALLOCATE_FLOW_REQUEST:
		result = "60_APP_DEALLOC_FLOW_REQ";
		break;
	case RINA_C_APP_DEALLOCATE_FLOW_RESPONSE:
		result = "61_APP_DEALLOC_FLOW_RESP";
		break;
	case RINA_C_APP_FLOW_DEALLOCATED_NOTIFICATION:
		result = "62_APP_FLOW_DEALLOC_NOT";
		break;
	case RINA_C_APP_REGISTER_APPLICATION_REQUEST:
		result = "63_APP_REG_REQ";
		break;
	case RINA_C_APP_REGISTER_APPLICATION_RESPONSE:
		result = "64_APP_REG_RESP";
		break;
	case RINA_C_APP_UNREGISTER_APPLICATION_REQUEST:
		result = "65_APP_UNREG_REQ";
		break;
	case RINA_C_APP_UNREGISTER_APPLICATION_RESPONSE:
		result = "66_APP_UNREG_RESP";
		break;
	case RINA_C_APP_APPLICATION_REGISTRATION_CANCELED_NOTIFICATION:
		result = "67_APP_REG_CANC_NOT";
		break;
	case RINA_C_APP_GET_DIF_PROPERTIES_REQUEST:
		result = "68_GET_DIF_PROPS_REQ";
		break;
	case RINA_C_APP_GET_DIF_PROPERTIES_RESPONSE:
		result = "69_GET_DIF_PROPS_RESP";
		break;
	case RINA_C_IPCM_PLUGIN_LOAD_REQUEST:
		result = "70_PLUGIN_LOAD_REQ";
		break;
	case RINA_C_IPCM_PLUGIN_LOAD_RESPONSE:
		result = "71_PLUGIN_LOAD_RESP";
		break;
	case RINA_C_IPCM_FWD_CDAP_MSG_REQUEST:
		result = "72_FWD_CDAP_MSG_REQ";
		break;
	case RINA_C_IPCM_FWD_CDAP_MSG_RESPONSE:
		result = "73_FWD_CDAP_MSG_RESP";
	 	break;
	case RINA_C_IPCM_MEDIA_REPORT:
		result = "74_MEDIA_REPORT";
	 	break;
	default:
		result = "Unknown operation";
	}

	return result;
}

IPCEvent* BaseNetlinkMessage::toIPCEvent()
{
	IPCEvent * event = 0;

	switch(operationCode) {
	case RINA_C_IPCM_DEALLOCATE_FLOW_RESPONSE: {
		event = new IpcmDeallocateFlowResponseEvent(result, sequenceNumber);
		break;
	}
	case RINA_C_IPCM_REGISTER_APPLICATION_RESPONSE: {
		event = new IpcmRegisterApplicationResponseEvent(result, sequenceNumber);
		break;
	}
	case RINA_C_IPCM_UNREGISTER_APPLICATION_RESPONSE: {
		event = new IpcmUnregisterApplicationResponseEvent(result, sequenceNumber);
		break;
	}
	case RINA_C_IPCM_ASSIGN_TO_DIF_RESPONSE: {
		event = new AssignToDIFResponseEvent(result, sequenceNumber);
		break;
	}
	case RINA_C_IPCM_UPDATE_DIF_CONFIG_RESPONSE: {
		event = new UpdateDIFConfigurationResponseEvent(result, sequenceNumber);
		break;
	}
	case RINA_C_IPCM_DISCONNECT_FROM_NEIGHBOR_RESPONSE: {
		event = new DisconnectNeighborResponseEvent(result, sequenceNumber);
		break;
	}
	case RINA_C_IPCM_SET_POLICY_SET_PARAM_RESPONSE: {
		event = new SetPolicySetParamResponseEvent(result, sequenceNumber);
		break;
	}
	case RINA_C_IPCM_SELECT_POLICY_SET_RESPONSE: {
		new SelectPolicySetResponseEvent(result, sequenceNumber);
		break;
	}
	case RINA_C_IPCP_MANAGEMENT_SDU_WRITE_RESPONSE: {
		event = new WriteMgmtSDUResponseEvent(result, sequenceNumber);
		break;
	}
	case RINA_C_IPCM_CREATE_IPCP_RESPONSE: {
		event = new CreateIPCPResponseEvent(result, sequenceNumber);
		break;
	}
	case RINA_C_IPCM_DESTROY_IPCP_RESPONSE: {
		event = new DestroyIPCPResponseEvent(result, sequenceNumber);
		break;
	}
	case RINA_C_IPCP_DEALLOCATE_PORT_RESPONSE: {
		event = new DeallocatePortResponseEvent(result, port_id,
							sequenceNumber);
		break;
	}
	case RINA_C_IPCP_ALLOCATE_PORT_RESPONSE: {
		event = new AllocatePortResponseEvent(result, port_id,
						      sequenceNumber);
		break;
	}
	case RINA_C_IPCP_UPDATE_CRYPTO_STATE_RESPONSE: {
		event = new UpdateCryptoStateResponseEvent(result, port_id,
							   sequenceNumber);
		break;
	}
	case RINA_C_IPCP_CONN_DESTROY_RESULT: {
		event = new DestroyConnectionResultEvent(result, port_id,
							 sequenceNumber);
		break;
	}
	case RINA_C_IPCP_CONN_UPDATE_RESULT: {
		event = new UpdateConnectionResponseEvent(port_id, result,
							  sequenceNumber);
		break;
	}
	case RINA_C_IPCM_ALLOCATE_FLOW_REQUEST_RESULT: {
		event = new IpcmAllocateFlowRequestResultEvent(result, port_id,
							       sequenceNumber);
		break;
	}
	case RINA_C_IPCP_CONN_CREATE_RESPONSE: {
		event = new CreateConnectionResponseEvent(port_id, cep_id,
		                        		  sequenceNumber);
		break;
	}
	case RINA_C_IPCM_PLUGIN_LOAD_RESPONSE: {
		event = new PluginLoadResponseEvent(result, sequenceNumber);
		break;
	}
	case RINA_C_IPCM_FLOW_DEALLOCATED_NOTIFICATION: {
		event = new FlowDeallocatedEvent(port_id, result);
		break;
	}
	case RINA_C_IPCM_DEALLOCATE_FLOW_REQUEST: {
		event = new FlowDeallocateRequestEvent(port_id, sequenceNumber);
		break;
	}
	default: {
	}
	}

	return event;
}

/* CLASS RINA APP ALLOCATE FLOW MESSAGE */
AppAllocateFlowRequestMessage::AppAllocateFlowRequestMessage() :
                BaseNetlinkMessage(RINA_C_APP_ALLOCATE_FLOW_REQUEST) {
}

const ApplicationProcessNamingInformation&
AppAllocateFlowRequestMessage::getDestAppName() const {
	return destAppName;
}

void AppAllocateFlowRequestMessage::setDestAppName(
		const ApplicationProcessNamingInformation& destAppName) {
	this->destAppName = destAppName;
}

const FlowSpecification&
AppAllocateFlowRequestMessage::getFlowSpecification() const {
	return flowSpecification;
}

void AppAllocateFlowRequestMessage::setFlowSpecification(
		const FlowSpecification& flowSpecification) {
	this->flowSpecification = flowSpecification;
}

const ApplicationProcessNamingInformation&
AppAllocateFlowRequestMessage::getSourceAppName() const {
	return sourceAppName;
}

void AppAllocateFlowRequestMessage::setSourceAppName(
		const ApplicationProcessNamingInformation& sourceAppName) {
	this->sourceAppName = sourceAppName;
}

const ApplicationProcessNamingInformation&
AppAllocateFlowRequestMessage::getDifName() const {
        return difName;
}

void AppAllocateFlowRequestMessage::setDifName(
                const ApplicationProcessNamingInformation& difName) {
        this->difName = difName;
}

IPCEvent* AppAllocateFlowRequestMessage::toIPCEvent(){
	FlowRequestEvent * event =
			new FlowRequestEvent( flowSpecification,
					true, sourceAppName, destAppName,
					getSourceIpcProcessId(),
					getSequenceNumber());
	event->DIFName = difName;
	return event;
}

/* CLASS APP ALLOCATE FLOW REQUEST RESULT MESSAGE */
AppAllocateFlowRequestResultMessage::AppAllocateFlowRequestResultMessage() :
				BaseNetlinkMessage(RINA_C_APP_ALLOCATE_FLOW_REQUEST_RESULT) {
	this->portId = 0;
}

const std::string&
AppAllocateFlowRequestResultMessage::getErrorDescription() const {
	return errorDescription;
}

void AppAllocateFlowRequestResultMessage::setErrorDescription(
		const std::string& errorDescription) {
	this->errorDescription = errorDescription;
}

const ApplicationProcessNamingInformation&
AppAllocateFlowRequestResultMessage::getDifName() const {
	return difName;
}

void AppAllocateFlowRequestResultMessage::setDifName(
		const ApplicationProcessNamingInformation& difName) {
	this->difName = difName;
}

const ApplicationProcessNamingInformation&
AppAllocateFlowRequestResultMessage::getSourceAppName() const {
	return sourceAppName;
}

void AppAllocateFlowRequestResultMessage::setSourceAppName(
		const ApplicationProcessNamingInformation& sourceAppName) {
	this->sourceAppName = sourceAppName;
}

int AppAllocateFlowRequestResultMessage::getPortId() const {
	return portId;
}

void AppAllocateFlowRequestResultMessage::setPortId(int portId) {
	this->portId = portId;
}

IPCEvent* AppAllocateFlowRequestResultMessage::toIPCEvent(){
        AllocateFlowRequestResultEvent * event =
                        new AllocateFlowRequestResultEvent(
                                        sourceAppName,
                                        difName,
                                        portId,
                                        this->getSequenceNumber());
        return event;
}

/* CLASS APP ALLOCATE FLOW REQUEST ARRIVED MESSAGE */
AppAllocateFlowRequestArrivedMessage::AppAllocateFlowRequestArrivedMessage() :
                BaseNetlinkMessage(
                                RINA_C_APP_ALLOCATE_FLOW_REQUEST_ARRIVED) {
	this->portId = 0;
}

const ApplicationProcessNamingInformation&
AppAllocateFlowRequestArrivedMessage::getDestAppName() const {
	return destAppName;
}

void AppAllocateFlowRequestArrivedMessage::setDestAppName(
		const ApplicationProcessNamingInformation& destAppName) {
	this->destAppName = destAppName;
}

const FlowSpecification&
AppAllocateFlowRequestArrivedMessage::getFlowSpecification() const {
	return flowSpecification;
}

void AppAllocateFlowRequestArrivedMessage::setFlowSpecification(
		const FlowSpecification& flowSpecification) {
	this->flowSpecification = flowSpecification;
}

const ApplicationProcessNamingInformation&
AppAllocateFlowRequestArrivedMessage::getSourceAppName() const {
	return sourceAppName;
}

void AppAllocateFlowRequestArrivedMessage::setSourceAppName(
		const ApplicationProcessNamingInformation& sourceAppName) {
	this->sourceAppName = sourceAppName;
}

int AppAllocateFlowRequestArrivedMessage::getPortId() const {
	return portId;
}

void AppAllocateFlowRequestArrivedMessage::setPortId(int portId) {
	this->portId = portId;
}

const ApplicationProcessNamingInformation&
AppAllocateFlowRequestArrivedMessage::getDifName() const {
	return difName;
}

void AppAllocateFlowRequestArrivedMessage::setDifName(
		const ApplicationProcessNamingInformation& difName) {
	this->difName = difName;
}

IPCEvent* AppAllocateFlowRequestArrivedMessage::toIPCEvent(){
	FlowRequestEvent * event =
			new FlowRequestEvent(
				portId, flowSpecification, false,
				destAppName, sourceAppName, difName,
				getSourceIpcProcessId(), getSequenceNumber());
	return event;
}

/* CLASS APP ALLOCATE FLOW RESPONSE MESSAGE */
AppAllocateFlowResponseMessage::AppAllocateFlowResponseMessage() :
                BaseNetlinkMessage(RINA_C_APP_ALLOCATE_FLOW_RESPONSE) {
	this->notifySource = true;
}

bool AppAllocateFlowResponseMessage::isNotifySource() const {
	return notifySource;
}

void AppAllocateFlowResponseMessage::setNotifySource(bool notifySource) {
	this->notifySource = notifySource;
}


IPCEvent* AppAllocateFlowResponseMessage::toIPCEvent(){
        AllocateFlowResponseEvent * event =
                        new AllocateFlowResponseEvent(
                                        result,
                                        notifySource,
                                        getSourceIpcProcessId(),
                                        getSequenceNumber());
        return event;
}

/* CLASS APP DEALLOCATE FLOW REQUEST MESSAGE */
AppDeallocateFlowRequestMessage::AppDeallocateFlowRequestMessage() :
                BaseNetlinkMessage(RINA_C_APP_DEALLOCATE_FLOW_REQUEST) {
}

const ApplicationProcessNamingInformation&
AppDeallocateFlowRequestMessage::getApplicationName() const {
	return applicationName;
}

void AppDeallocateFlowRequestMessage::setApplicationName(
		const ApplicationProcessNamingInformation& applicationName) {
	this->applicationName = applicationName;
}

IPCEvent* AppDeallocateFlowRequestMessage::toIPCEvent(){
	FlowDeallocateRequestEvent * event = new FlowDeallocateRequestEvent(
			port_id, applicationName, getSequenceNumber());
	return event;
}

/* CLASS APP DEALLOCATE FLOW RESPONSE MESSAGE */
AppDeallocateFlowResponseMessage::AppDeallocateFlowResponseMessage() :
		BaseNetlinkMessage(RINA_C_APP_DEALLOCATE_FLOW_RESPONSE) {
}

const ApplicationProcessNamingInformation&
AppDeallocateFlowResponseMessage::getApplicationName() const {
	return applicationName;
}

void AppDeallocateFlowResponseMessage::setApplicationName(
		const ApplicationProcessNamingInformation& applicationName) {
	this->applicationName = applicationName;
}

IPCEvent* AppDeallocateFlowResponseMessage::toIPCEvent(){
        DeallocateFlowResponseEvent * event = new DeallocateFlowResponseEvent(
                        applicationName, port_id, result,
                        getSequenceNumber());
        return event;
}

/* CLASS APP FLOW DEALLOCATED NOTIFICATION MESSAGE */
AppFlowDeallocatedNotificationMessage::AppFlowDeallocatedNotificationMessage() :
		BaseNetlinkMessage(RINA_C_APP_FLOW_DEALLOCATED_NOTIFICATION) {
}

const ApplicationProcessNamingInformation&
AppFlowDeallocatedNotificationMessage::getApplicationName() const {
	return applicationName;
}

void AppFlowDeallocatedNotificationMessage::setApplicationName(
		const ApplicationProcessNamingInformation& applicationName) {
	this->applicationName = applicationName;
}

IPCEvent* AppFlowDeallocatedNotificationMessage::toIPCEvent(){
	FlowDeallocatedEvent * event = new FlowDeallocatedEvent(port_id, result);
	return event;
}

/* CLASS APP REGISTER APPLICATION REQUEST MESSAGE */
AppRegisterApplicationRequestMessage::AppRegisterApplicationRequestMessage() :
                BaseNetlinkMessage(RINA_C_APP_REGISTER_APPLICATION_REQUEST) {
}

const ApplicationRegistrationInformation&
AppRegisterApplicationRequestMessage::getApplicationRegistrationInformation()
const {
	return applicationRegistrationInformation;
}

void
AppRegisterApplicationRequestMessage::setApplicationRegistrationInformation(
		const ApplicationRegistrationInformation& appRegistrationInfo) {
	this->applicationRegistrationInformation = appRegistrationInfo;
}

IPCEvent* AppRegisterApplicationRequestMessage::toIPCEvent(){
	ApplicationRegistrationRequestEvent * event =
			new ApplicationRegistrationRequestEvent(
					applicationRegistrationInformation,
					getSequenceNumber());

	return event;
}

/* CLASS APP REGISTER APPLICATION RESPONSE MESSAGE */
AppRegisterApplicationResponseMessage::AppRegisterApplicationResponseMessage() :
		BaseNetlinkMessage(RINA_C_APP_REGISTER_APPLICATION_RESPONSE) {
}

const ApplicationProcessNamingInformation&
AppRegisterApplicationResponseMessage::getApplicationName() const {
	return applicationName;
}

void AppRegisterApplicationResponseMessage::setApplicationName(
		const ApplicationProcessNamingInformation& applicationName) {
	this->applicationName = applicationName;
}

const ApplicationProcessNamingInformation&
AppRegisterApplicationResponseMessage::getDifName() const {
	return difName;
}

void AppRegisterApplicationResponseMessage::setDifName(
		const ApplicationProcessNamingInformation& difName) {
	this->difName = difName;
}

IPCEvent* AppRegisterApplicationResponseMessage::toIPCEvent(){
        RegisterApplicationResponseEvent * event =
                        new RegisterApplicationResponseEvent(
                                        applicationName,
                                        difName,
                                        result,
                                        getSequenceNumber());

        return event;
}

/* CLASS APP UNREGISTER APPLICATION REQUEST MESSAGE */
AppUnregisterApplicationRequestMessage::AppUnregisterApplicationRequestMessage() :
                BaseNetlinkMessage(RINA_C_APP_UNREGISTER_APPLICATION_REQUEST) {
}

const ApplicationProcessNamingInformation&
AppUnregisterApplicationRequestMessage::getApplicationName() const {
	return applicationName;
}

void AppUnregisterApplicationRequestMessage::setApplicationName(
		const ApplicationProcessNamingInformation& applicationName) {
	this->applicationName = applicationName;
}

const ApplicationProcessNamingInformation&
AppUnregisterApplicationRequestMessage::getDifName() const {
	return difName;
}

void AppUnregisterApplicationRequestMessage::setDifName(
		const ApplicationProcessNamingInformation& difName) {
	this->difName = difName;
}

IPCEvent* AppUnregisterApplicationRequestMessage::toIPCEvent(){
	ApplicationUnregistrationRequestEvent * event =
			new ApplicationUnregistrationRequestEvent(
					applicationName,
					difName,
					getSequenceNumber());

	return event;
}


/* CLASS APP UNREGISTER APPLICATION RESPONSE MESSAGE */
AppUnregisterApplicationResponseMessage::AppUnregisterApplicationResponseMessage() :
		BaseNetlinkMessage(RINA_C_APP_UNREGISTER_APPLICATION_RESPONSE) {
}

const ApplicationProcessNamingInformation&
AppUnregisterApplicationResponseMessage::getApplicationName() const {
	return applicationName;
}

void AppUnregisterApplicationResponseMessage::setApplicationName(
		const ApplicationProcessNamingInformation& applicationName) {
	this->applicationName = applicationName;
}

IPCEvent* AppUnregisterApplicationResponseMessage::toIPCEvent(){
        UnregisterApplicationResponseEvent * event =
                        new UnregisterApplicationResponseEvent(
                                        applicationName,
                                        result,
                                        getSequenceNumber());

        return event;
}


/* CLASS APP REGISTRATION CANCELLED NOTIFICATION MESSAGE */
AppRegistrationCanceledNotificationMessage::AppRegistrationCanceledNotificationMessage() :
		BaseNetlinkMessage(RINA_C_APP_APPLICATION_REGISTRATION_CANCELED_NOTIFICATION) {
	this->code = 0;
}

int AppRegistrationCanceledNotificationMessage::getCode() const {
	return code;
}

void AppRegistrationCanceledNotificationMessage::setCode(int code) {
	this->code = code;
}

const std::string& AppRegistrationCanceledNotificationMessage::getReason() const {
	return reason;
}

void AppRegistrationCanceledNotificationMessage::setReason(
		const std::string& reason) {
	this->reason = reason;
}

const ApplicationProcessNamingInformation&
AppRegistrationCanceledNotificationMessage::getDifName() const {
	return difName;
}

void AppRegistrationCanceledNotificationMessage::setDifName(
		const ApplicationProcessNamingInformation& difName) {
	this->difName = difName;
}

const ApplicationProcessNamingInformation&
AppRegistrationCanceledNotificationMessage::getApplicationName() const {
	return applicationName;
}

void AppRegistrationCanceledNotificationMessage::setApplicationName(
		const ApplicationProcessNamingInformation& applicationName) {
	this->applicationName = applicationName;
}

IPCEvent* AppRegistrationCanceledNotificationMessage::toIPCEvent(){
	AppRegistrationCanceledEvent * event = new AppRegistrationCanceledEvent(
			code, reason, difName, getSequenceNumber());
	return event;
}

/*CLASS APP GET DIF PROPERTIES REQUEST MESSAGE */
AppGetDIFPropertiesRequestMessage::AppGetDIFPropertiesRequestMessage():
                BaseNetlinkMessage(
				RINA_C_APP_GET_DIF_PROPERTIES_REQUEST){
}

const ApplicationProcessNamingInformation&
AppGetDIFPropertiesRequestMessage::getApplicationName() const {
	return applicationName;
}

void AppGetDIFPropertiesRequestMessage::setApplicationName(
		const ApplicationProcessNamingInformation& applicationName) {
	this->applicationName = applicationName;
}

const ApplicationProcessNamingInformation&
AppGetDIFPropertiesRequestMessage::getDifName() const {
	return difName;
}

void AppGetDIFPropertiesRequestMessage::setDifName(
		const ApplicationProcessNamingInformation& difName) {
	this->difName = difName;
}

IPCEvent* AppGetDIFPropertiesRequestMessage::toIPCEvent(){
	GetDIFPropertiesRequestEvent * event =
			new GetDIFPropertiesRequestEvent(
					applicationName,
					difName,
					getSequenceNumber());

	return event;
}

/* CLASS APP GET DIF PROPERTIES RESPONSE MESSAGE */
AppGetDIFPropertiesResponseMessage::AppGetDIFPropertiesResponseMessage()
		:BaseNetlinkMessage(RINA_C_APP_GET_DIF_PROPERTIES_RESPONSE){
}

const ApplicationProcessNamingInformation&
AppGetDIFPropertiesResponseMessage::getApplicationName() const {
	return applicationName;
}

void AppGetDIFPropertiesResponseMessage::setApplicationName(
		const ApplicationProcessNamingInformation& applicationName) {
	this->applicationName = applicationName;
}

const std::list<DIFProperties>&
	AppGetDIFPropertiesResponseMessage::getDIFProperties() const{
	return difProperties;
}

void AppGetDIFPropertiesResponseMessage::setDIFProperties(
		const std::list<DIFProperties>& difProperties){
	this->difProperties = difProperties;
}

void AppGetDIFPropertiesResponseMessage::addDIFProperty(
		const DIFProperties& difProperties){
	this->difProperties.push_back(difProperties);
}

IPCEvent* AppGetDIFPropertiesResponseMessage::toIPCEvent(){
        GetDIFPropertiesResponseEvent * event =
                        new GetDIFPropertiesResponseEvent(
                                        applicationName,
                                        difProperties,
                                        result,
                                        getSequenceNumber());

        return event;
}

/* CLASS IPCM REGISTER APPLICATION REQUEST MESSAGE */
IpcmRegisterApplicationRequestMessage::IpcmRegisterApplicationRequestMessage():
                BaseNetlinkMessage(
			RINA_C_IPCM_REGISTER_APPLICATION_REQUEST) {
        regIpcProcessId = 0;
}

const ApplicationProcessNamingInformation&
IpcmRegisterApplicationRequestMessage::getApplicationName() const{
	return applicationName;
}

void IpcmRegisterApplicationRequestMessage::setApplicationName(
		const ApplicationProcessNamingInformation& applicationName){
	this->applicationName = applicationName;
}

const ApplicationProcessNamingInformation&
IpcmRegisterApplicationRequestMessage::getDifName() const{
	return difName;
}

void IpcmRegisterApplicationRequestMessage::setDifName(
		const ApplicationProcessNamingInformation& difName){
	this->difName = difName;
}

unsigned short IpcmRegisterApplicationRequestMessage::
getRegIpcProcessId() const {
        return regIpcProcessId;
}

void IpcmRegisterApplicationRequestMessage::
setRegIpcProcessId(unsigned short regIpcProcessId) {
        this->regIpcProcessId = regIpcProcessId;
}

IPCEvent* IpcmRegisterApplicationRequestMessage::toIPCEvent(){
	ApplicationRegistrationInformation information =
		ApplicationRegistrationInformation(APPLICATION_REGISTRATION_SINGLE_DIF);
	information.difName = difName;
	information.appName = applicationName;
	information.dafName = dafName;
	information.ipcProcessId = regIpcProcessId;
	ApplicationRegistrationRequestEvent * event =
			new ApplicationRegistrationRequestEvent(
					information,
					getSequenceNumber());

	return event;
}

/* CLASS IPCM UNREGISTER APPLICATION REQUEST MESSAGE */
IpcmUnregisterApplicationRequestMessage::IpcmUnregisterApplicationRequestMessage() :
                BaseNetlinkMessage(
			RINA_C_IPCM_UNREGISTER_APPLICATION_REQUEST) {
}

const ApplicationProcessNamingInformation&
IpcmUnregisterApplicationRequestMessage::getApplicationName() const {
	return applicationName;
}

void IpcmUnregisterApplicationRequestMessage::setApplicationName(
		const ApplicationProcessNamingInformation& applicationName) {
	this->applicationName = applicationName;
}

const ApplicationProcessNamingInformation&
IpcmUnregisterApplicationRequestMessage::getDifName() const {
	return difName;
}

void IpcmUnregisterApplicationRequestMessage::setDifName(
		const ApplicationProcessNamingInformation& difName) {
	this->difName = difName;
}

IPCEvent* IpcmUnregisterApplicationRequestMessage::toIPCEvent(){
	ApplicationUnregistrationRequestEvent * event =
			new ApplicationUnregistrationRequestEvent(
					applicationName,
					difName,
					getSequenceNumber());
	return event;
}

/* CLASS IPCM ASSIGN TO DIF REQUEST MESSAGE */
IpcmAssignToDIFRequestMessage::IpcmAssignToDIFRequestMessage():
                BaseNetlinkMessage(
				RINA_C_IPCM_ASSIGN_TO_DIF_REQUEST) {
}

const DIFInformation&
IpcmAssignToDIFRequestMessage::getDIFInformation() const{
	return difInformation;
}

void IpcmAssignToDIFRequestMessage::setDIFInformation(
		const DIFInformation& difInformation){
	this->difInformation = difInformation;
}

IPCEvent* IpcmAssignToDIFRequestMessage::toIPCEvent(){
	AssignToDIFRequestEvent * event =
			new AssignToDIFRequestEvent(
			                getDIFInformation(),
					getSequenceNumber());
	return event;
}

/* CLASS IPCM UPDATE DIF CONFIGURATION REQUEST MESSAGE */
IpcmUpdateDIFConfigurationRequestMessage::
IpcmUpdateDIFConfigurationRequestMessage():
BaseNetlinkMessage(RINA_C_IPCM_UPDATE_DIF_CONFIG_REQUEST) {
}

const DIFConfiguration&
IpcmUpdateDIFConfigurationRequestMessage::getDIFConfiguration() const{
        return difConfiguration;
}

void IpcmUpdateDIFConfigurationRequestMessage::setDIFConfiguration(
                const DIFConfiguration& difConfiguration)
{
        this->difConfiguration = difConfiguration;
}

IPCEvent* IpcmUpdateDIFConfigurationRequestMessage::toIPCEvent()
{
        UpdateDIFConfigurationRequestEvent * event =
                        new UpdateDIFConfigurationRequestEvent(
                                        getDIFConfiguration(),
                                        getSequenceNumber());
        return event;
}

/* CLASS IPCM ENROLL TO DIF REQUEST MESSAGE */
IpcmEnrollToDIFRequestMessage:: IpcmEnrollToDIFRequestMessage():
        BaseNetlinkMessage(RINA_C_IPCM_ENROLL_TO_DIF_REQUEST) {
	prepare_for_handover = false;
}

const ApplicationProcessNamingInformation&
IpcmEnrollToDIFRequestMessage::getDifName() const {
        return difName;
}

void IpcmEnrollToDIFRequestMessage::setDifName(
                const ApplicationProcessNamingInformation& difName) {
        this->difName = difName;
}

const ApplicationProcessNamingInformation&
IpcmEnrollToDIFRequestMessage::getNeighborName() const {
        return neighborName;
}

void IpcmEnrollToDIFRequestMessage::setNeighborName(
                const ApplicationProcessNamingInformation& neighborName) {
        this->neighborName = neighborName;
}

const ApplicationProcessNamingInformation&
IpcmEnrollToDIFRequestMessage::getSupportingDifName() const {
        return supportingDIFName;
}

void IpcmEnrollToDIFRequestMessage::setSupportingDifName(
                const ApplicationProcessNamingInformation& supportingDifName) {
        supportingDIFName = supportingDifName;
}

IPCEvent* IpcmEnrollToDIFRequestMessage::toIPCEvent(){
        EnrollToDAFRequestEvent * event =
                        new EnrollToDAFRequestEvent(
                                        difName, supportingDIFName,
                                        neighborName, prepare_for_handover,
					disc_neigh_name, getSequenceNumber());
        return event;
}

/* CLASS ENROLL TO DIF RESPONSE MESSAGE */
IpcmEnrollToDIFResponseMessage::IpcmEnrollToDIFResponseMessage():
		BaseNetlinkMessage(RINA_C_IPCM_ENROLL_TO_DIF_RESPONSE) {
}

const std::list<Neighbor>&
IpcmEnrollToDIFResponseMessage::getNeighbors() const {
        return neighbors;
}

void IpcmEnrollToDIFResponseMessage::setNeighbors(
                const std::list<Neighbor>& neighbors) {
        this->neighbors = neighbors;
}

void IpcmEnrollToDIFResponseMessage::addNeighbor(
                const Neighbor& neighbor) {
        neighbors.push_back(neighbor);
}

void IpcmEnrollToDIFResponseMessage::setDIFInformation(
                const DIFInformation& difInformation) {
        this->difInformation = difInformation;
}

const DIFInformation&
IpcmEnrollToDIFResponseMessage::getDIFInformation() const {
        return difInformation;
}

IPCEvent* IpcmEnrollToDIFResponseMessage::toIPCEvent(){
        EnrollToDIFResponseEvent * event =
                        new EnrollToDIFResponseEvent(neighbors, difInformation,
                                        result, getSequenceNumber());
        return event;
}

/* CLASS IPCM DISCONNECT NEIGHBOR REQUEST MESSAGE */
IpcmDisconnectNeighborRequestMessage:: IpcmDisconnectNeighborRequestMessage():
        BaseNetlinkMessage(RINA_C_IPCM_DISCONNECT_FROM_NEIGHBOR_REQUEST)
{
}

const ApplicationProcessNamingInformation&
IpcmDisconnectNeighborRequestMessage::getNeighborName() const
{
        return neighborName;
}

void IpcmDisconnectNeighborRequestMessage::setNeighborName(
                const ApplicationProcessNamingInformation& neighborName)
{
        this->neighborName = neighborName;
}

IPCEvent* IpcmDisconnectNeighborRequestMessage::toIPCEvent()
{
	DisconnectNeighborRequestEvent * event =
                        new DisconnectNeighborRequestEvent(neighborName,
                        				   getSequenceNumber());
        return event;
}

/* CLASS IPCM ALLOCATE FLOW REQUEST MESSAGE */
IpcmAllocateFlowRequestMessage::IpcmAllocateFlowRequestMessage():
                BaseNetlinkMessage(RINA_C_IPCM_ALLOCATE_FLOW_REQUEST) {
}

const ApplicationProcessNamingInformation&
IpcmAllocateFlowRequestMessage::getDestAppName() const {
	return destAppName;
}

void IpcmAllocateFlowRequestMessage::setDestAppName(
		const ApplicationProcessNamingInformation& destAppName) {
	this->destAppName = destAppName;
}

const FlowSpecification&
IpcmAllocateFlowRequestMessage::getFlowSpec() const {
	return flowSpec;
}

void IpcmAllocateFlowRequestMessage::setFlowSpec(
		const FlowSpecification& flowSpec) {
	this->flowSpec = flowSpec;
}

const ApplicationProcessNamingInformation&
IpcmAllocateFlowRequestMessage::getSourceAppName() const {
	return sourceAppName;
}

void IpcmAllocateFlowRequestMessage::setSourceAppName(
		const ApplicationProcessNamingInformation& sourceAppName) {
	this->sourceAppName = sourceAppName;
}

const ApplicationProcessNamingInformation&
IpcmAllocateFlowRequestMessage::getDifName() const {
	return difName;
}

void IpcmAllocateFlowRequestMessage::setDifName(
		const ApplicationProcessNamingInformation& difName) {
	this->difName = difName;
}

IPCEvent* IpcmAllocateFlowRequestMessage::toIPCEvent(){
	FlowRequestEvent * event =
			new FlowRequestEvent(-1, flowSpec, true,
			                sourceAppName, destAppName,
					difName, getSourceIpcProcessId(),
					getSequenceNumber());
	return event;
}

/* CLASS IPCM ALLOCATE FLOW REQUEST ARRIVED MESSAGE */
IpcmAllocateFlowRequestArrivedMessage::IpcmAllocateFlowRequestArrivedMessage()
	: BaseNetlinkMessage(
			RINA_C_IPCM_ALLOCATE_FLOW_REQUEST_ARRIVED) {
        portId = 0;
}

const ApplicationProcessNamingInformation&
IpcmAllocateFlowRequestArrivedMessage::getDestAppName() const {
	return destAppName;
}

void IpcmAllocateFlowRequestArrivedMessage::setDestAppName(
		const ApplicationProcessNamingInformation& destAppName) {
	this->destAppName = destAppName;
}

const FlowSpecification&
IpcmAllocateFlowRequestArrivedMessage::getFlowSpecification() const {
	return flowSpecification;
}

void IpcmAllocateFlowRequestArrivedMessage::setFlowSpecification(
		const FlowSpecification& flowSpecification) {
	this->flowSpecification = flowSpecification;
}

const ApplicationProcessNamingInformation&
IpcmAllocateFlowRequestArrivedMessage::getSourceAppName() const {
	return sourceAppName;
}

void IpcmAllocateFlowRequestArrivedMessage::setSourceAppName(
		const ApplicationProcessNamingInformation& sourceAppName) {
	this->sourceAppName = sourceAppName;
}

const ApplicationProcessNamingInformation&
IpcmAllocateFlowRequestArrivedMessage::getDifName() const {
	return difName;
}

void IpcmAllocateFlowRequestArrivedMessage::setDifName(
		const ApplicationProcessNamingInformation& difName) {
	this->difName = difName;
}

int IpcmAllocateFlowRequestArrivedMessage::getPortId() const {
        return portId;
}

void IpcmAllocateFlowRequestArrivedMessage::setPortId(int portId){
        this->portId = portId;
}

IPCEvent* IpcmAllocateFlowRequestArrivedMessage::toIPCEvent(){
	FlowRequestEvent * event =
			new FlowRequestEvent(portId, flowSpecification,
					false, destAppName, sourceAppName,
					difName, getSourceIpcProcessId(),
					getSequenceNumber());
	return event;
}

/* CLASS IPCM ALLOCATE FLOW RESPONSE MESSAGE */
IpcmAllocateFlowResponseMessage::IpcmAllocateFlowResponseMessage() :
				BaseNetlinkMessage(RINA_C_IPCM_ALLOCATE_FLOW_RESPONSE) {
	this->notifySource = false;
}

bool IpcmAllocateFlowResponseMessage::isNotifySource() const
{
	return notifySource;
}

void IpcmAllocateFlowResponseMessage::setNotifySource(bool notifySource)
{
	this->notifySource = notifySource;
}

IPCEvent* IpcmAllocateFlowResponseMessage::toIPCEvent(){
        AllocateFlowResponseEvent * event =
                        new AllocateFlowResponseEvent(
                                        result,
                                        notifySource,
                                        getSourceIpcProcessId(),
                                        getSequenceNumber());
        return event;
}

/* CLASS IPCM IPC PROCESS REGISTERED TO DIF NOTIFICATION MESSAGE */
IpcmDIFRegistrationNotification::
IpcmDIFRegistrationNotification():BaseNetlinkMessage(RINA_C_IPCM_IPC_PROCESS_DIF_REGISTRATION_NOTIFICATION){
	registered = false;
}

const ApplicationProcessNamingInformation&
IpcmDIFRegistrationNotification::getDifName() const {
	return difName;
}

void IpcmDIFRegistrationNotification::setDifName(
		const ApplicationProcessNamingInformation& difName) {
	this->difName = difName;
}

const ApplicationProcessNamingInformation&
IpcmDIFRegistrationNotification::getIpcProcessName() const {
	return ipcProcessName;
}

void IpcmDIFRegistrationNotification::setIpcProcessName(
		const ApplicationProcessNamingInformation& ipcProcessName) {
	this->ipcProcessName = ipcProcessName;
}

void IpcmDIFRegistrationNotification::setRegistered(bool registered){
	this->registered = registered;
}

bool IpcmDIFRegistrationNotification::isRegistered() const{
	return registered;
}

IPCEvent* IpcmDIFRegistrationNotification::toIPCEvent(){
	IPCEvent * event =
	        new IPCProcessDIFRegistrationEvent(ipcProcessName,
	                                difName, registered,
	                                getSequenceNumber());
	return event;
}


/* CLASS QUERY RIB REQUEST MESSAGE */
IpcmDIFQueryRIBRequestMessage::IpcmDIFQueryRIBRequestMessage():
                BaseNetlinkMessage(RINA_C_IPCM_QUERY_RIB_REQUEST) {
	objectInstance = -1;
	scope = 0;
}

const std::string& IpcmDIFQueryRIBRequestMessage::getFilter() const {
	return filter;
}

void IpcmDIFQueryRIBRequestMessage::setFilter(const std::string& filter) {
	this->filter = filter;
}

const std::string& IpcmDIFQueryRIBRequestMessage::getObjectClass() const {
	return objectClass;
}

void IpcmDIFQueryRIBRequestMessage::setObjectClass(
		const std::string& objectClass) {
	this->objectClass = objectClass;
}

unsigned long IpcmDIFQueryRIBRequestMessage::getObjectInstance() const {
	return objectInstance;
}

void IpcmDIFQueryRIBRequestMessage::setObjectInstance(
		unsigned long objectInstance) {
	this->objectInstance = objectInstance;
}

const std::string&
IpcmDIFQueryRIBRequestMessage::getObjectName() const {
	return objectName;
}

void IpcmDIFQueryRIBRequestMessage::setObjectName(
		const std::string& objectName) {
	this->objectName = objectName;
}

unsigned int IpcmDIFQueryRIBRequestMessage::getScope() const {
	return scope;
}

void IpcmDIFQueryRIBRequestMessage::setScope(unsigned int scope) {
	this->scope = scope;
}

IPCEvent* IpcmDIFQueryRIBRequestMessage::toIPCEvent(){
	IPCEvent * event = new QueryRIBRequestEvent(objectClass, objectName,
			objectInstance, scope, filter, getSequenceNumber());
	return event;
}

/* CLASS QUERY RIB RESPONSE MESSAGE */
IpcmDIFQueryRIBResponseMessage::IpcmDIFQueryRIBResponseMessage()
	:BaseNetlinkMessage(RINA_C_IPCM_QUERY_RIB_RESPONSE){
}

const std::list<rib::RIBObjectData>&
	IpcmDIFQueryRIBResponseMessage::getRIBObjects() const{
	return ribObjects;
}

void IpcmDIFQueryRIBResponseMessage::setRIBObjects(
		const std::list<rib::RIBObjectData>& ribObjects){
	this->ribObjects = ribObjects;
}

void IpcmDIFQueryRIBResponseMessage::addRIBObject(const rib::RIBObjectData& ribObject){
	ribObjects.push_back(ribObject);
}

IPCEvent* IpcmDIFQueryRIBResponseMessage::toIPCEvent(){
        IPCEvent * event = new QueryRIBResponseEvent(
                        ribObjects, result,
                        getSequenceNumber());
        return event;
}

/* CLASS SET POLICY SET PARAM REQUEST MESSAGE */
IpcmSetPolicySetParamRequestMessage::IpcmSetPolicySetParamRequestMessage():
                BaseNetlinkMessage(RINA_C_IPCM_SET_POLICY_SET_PARAM_REQUEST) {
}

IPCEvent* IpcmSetPolicySetParamRequestMessage::toIPCEvent(){
	IPCEvent * event = new SetPolicySetParamRequestEvent(path, name,
			                        value, getSequenceNumber());
	return event;
}

/* CLASS SELECT POLICY SET REQUEST MESSAGE */
IpcmSelectPolicySetRequestMessage::IpcmSelectPolicySetRequestMessage():
                BaseNetlinkMessage(RINA_C_IPCM_SELECT_POLICY_SET_REQUEST) {
}

IPCEvent* IpcmSelectPolicySetRequestMessage::toIPCEvent(){
	IPCEvent * event = new SelectPolicySetRequestEvent(path, name,
			                        getSequenceNumber());
	return event;
}

/* CLASS PLUGIN LOAD REQUEST MESSAGE */
IpcmPluginLoadRequestMessage::IpcmPluginLoadRequestMessage():
                BaseNetlinkMessage(RINA_C_IPCM_PLUGIN_LOAD_REQUEST) {
}

IPCEvent* IpcmPluginLoadRequestMessage::toIPCEvent(){
	IPCEvent * event = new PluginLoadRequestEvent(name, load,
			                              getSequenceNumber());
	return event;
}

/* CLASS FWD CDAP MSG REQUEST MESSAGE */
IpcmFwdCDAPRequestMessage::IpcmFwdCDAPRequestMessage():
                BaseNetlinkMessage(RINA_C_IPCM_FWD_CDAP_MSG_REQUEST) {
}

IPCEvent* IpcmFwdCDAPRequestMessage::toIPCEvent(){
	IPCEvent * event = new FwdCDAPMsgRequestEvent(sermsg, result,
			                       getSequenceNumber());
	return event;
}

/* CLASS FWD CDAP MSG RESPONSE MESSAGE */
IpcmFwdCDAPResponseMessage::IpcmFwdCDAPResponseMessage():
                BaseNetlinkMessage(RINA_C_IPCM_FWD_CDAP_MSG_RESPONSE) {
}

IPCEvent* IpcmFwdCDAPResponseMessage::toIPCEvent(){
        IPCEvent * event = new FwdCDAPMsgResponseEvent(sermsg, result,
                                               getSequenceNumber());
        return event;
}

/* CLAS IPCM IPC PROCESS INITIALIZED MESSAGE */
IpcmIPCProcessInitializedMessage::IpcmIPCProcessInitializedMessage() :
BaseNetlinkMessage(RINA_C_IPCM_IPC_PROCESS_INITIALIZED){
}

IpcmIPCProcessInitializedMessage::IpcmIPCProcessInitializedMessage(
                const ApplicationProcessNamingInformation& name) :
BaseNetlinkMessage(RINA_C_IPCM_IPC_PROCESS_INITIALIZED){
        this->name = name;
}

const ApplicationProcessNamingInformation&
IpcmIPCProcessInitializedMessage::getName() const {
        return name;
}

void IpcmIPCProcessInitializedMessage::setName(
                const ApplicationProcessNamingInformation& name) {
        this->name = name;
}

IPCEvent* IpcmIPCProcessInitializedMessage::toIPCEvent(){
        IPCEvent * event = new IPCProcessDaemonInitializedEvent(
                        getSourceIpcProcessId(), name, getSequenceNumber());
        return event;
}

/* CLASS IPCM CONNECTION CREATE REQUEST MESSAGE */
IpcpConnectionCreateRequestMessage::IpcpConnectionCreateRequestMessage():
                BaseNetlinkMessage(RINA_C_IPCP_CONN_CREATE_REQUEST) {
}

IPCEvent* IpcpConnectionCreateRequestMessage::toIPCEvent() {
        return 0;
}

/* CLASS IPCM CONNECTION UPDATE REQUEST MESSAGE */
IpcpConnectionUpdateRequestMessage::IpcpConnectionUpdateRequestMessage() :
         BaseNetlinkMessage(RINA_C_IPCP_CONN_UPDATE_REQUEST){
        portId = 0;
        sourceCepId = 0;
        destinationCepId = 0;
}

IPCEvent* IpcpConnectionUpdateRequestMessage::toIPCEvent() {
        return 0;
}

/* CLASS IPCM CONNECTION CREATE ARRIVED MESSAGE */
IpcpConnectionCreateArrivedMessage::IpcpConnectionCreateArrivedMessage():
                BaseNetlinkMessage(RINA_C_IPCP_CONN_CREATE_ARRIVED) {
}

IPCEvent* IpcpConnectionCreateArrivedMessage::toIPCEvent() {
        return 0;
}

/* CLASS IPCM CONNECTION CREATE RESULT MESSAGE */
IpcpConnectionCreateResultMessage::IpcpConnectionCreateResultMessage() :
         BaseNetlinkMessage(RINA_C_IPCP_CONN_CREATE_RESULT){
        portId = 0;
        sourceCepId = 0;
        destCepId = 0;
}

int IpcpConnectionCreateResultMessage::getSourceCepId() const {
        return sourceCepId;
}

void IpcpConnectionCreateResultMessage::setSourceCepId(int sourceCepId) {
        this->sourceCepId = sourceCepId;
}

int IpcpConnectionCreateResultMessage::getDestCepId() const {
        return destCepId;
}

void IpcpConnectionCreateResultMessage::setDestCepId(int destCepId) {
        this->destCepId = destCepId;
}

int IpcpConnectionCreateResultMessage::getPortId() const {
        return portId;
}

void IpcpConnectionCreateResultMessage::setPortId(int portId) {
        this->portId = portId;
}

IPCEvent* IpcpConnectionCreateResultMessage::toIPCEvent() {
        IPCEvent * event = new CreateConnectionResultEvent(portId, sourceCepId,
                        destCepId, getSequenceNumber());
        return event;
}

/* CLASS RmtAddForwardingTableEntriesRequestMessage */
RmtModifyPDUFTEntriesRequestMessage::
RmtModifyPDUFTEntriesRequestMessage():
BaseNetlinkMessage(RINA_C_RMT_MODIFY_FTE_REQUEST){
        mode = 0;
}

const std::list<PDUForwardingTableEntry *>&
RmtModifyPDUFTEntriesRequestMessage::getEntries() const {
        return entries;
}

void RmtModifyPDUFTEntriesRequestMessage::
setEntries(const std::list<PDUForwardingTableEntry *>& entries) {
        this->entries = entries;
}

void RmtModifyPDUFTEntriesRequestMessage::
addEntry(PDUForwardingTableEntry * entry) {
        entries.push_back(entry);
}

int RmtModifyPDUFTEntriesRequestMessage::getMode() const {
        return mode;
}

void RmtModifyPDUFTEntriesRequestMessage::setMode(int mode) {
        this->mode = mode;
}

IPCEvent* RmtModifyPDUFTEntriesRequestMessage::toIPCEvent() {
        return 0;
}

/* CLASS RmtDumpPDUFTEntriesResponseMessage */
RmtDumpPDUFTEntriesResponseMessage::
RmtDumpPDUFTEntriesResponseMessage():
BaseNetlinkMessage(RINA_C_RMT_DUMP_FT_REPLY){
}

const std::list<PDUForwardingTableEntry>&
RmtDumpPDUFTEntriesResponseMessage::getEntries() const {
        return entries;
}

void RmtDumpPDUFTEntriesResponseMessage::
setEntries(const std::list<PDUForwardingTableEntry>& entries) {
        this->entries = entries;
}

void RmtDumpPDUFTEntriesResponseMessage::
addEntry(const PDUForwardingTableEntry& entry) {
        entries.push_back(entry);
}

IPCEvent* RmtDumpPDUFTEntriesResponseMessage::toIPCEvent() {
        IPCEvent * event = new DumpFTResponseEvent(entries,
                        result, getSequenceNumber());
        return event;
}

/// CLASS IPCPUpdateCryptoStateRequestMessage
IPCPUpdateCryptoStateRequestMessage::IPCPUpdateCryptoStateRequestMessage()
	: BaseNetlinkMessage(RINA_C_IPCP_UPDATE_CRYPTO_STATE_REQUEST)
{
}

IPCEvent* IPCPUpdateCryptoStateRequestMessage::toIPCEvent() {
        return 0;
}

/// CLASS IPCPAddressChangeRequestMessage
IPCPAddressChangeRequestMessage::IPCPAddressChangeRequestMessage()
	: BaseNetlinkMessage(RINA_C_IPCP_ADDRESS_CHANGE_REQUEST)
{
	new_address = 0;
	old_address = 0;
	use_new_timeout = 0;
	deprecate_old_timeout = 0;
}

IPCEvent* IPCPAddressChangeRequestMessage::toIPCEvent() {
        return 0;
}

/// CLASS IpcmMediaReportMessage
IpcmMediaReportMessage::IpcmMediaReportMessage()
	: BaseNetlinkMessage(RINA_C_IPCM_MEDIA_REPORT)
{
}

IPCEvent* IpcmMediaReportMessage::toIPCEvent()
{
        IPCEvent * event = new MediaReportEvent(report,
        					getSequenceNumber());
        return event;
}

/// CLASS IPCPAllocatePortRequestMessage
IPCPAllocatePortRequestMessage::IPCPAllocatePortRequestMessage()
	: BaseNetlinkMessage(RINA_C_IPCP_ALLOCATE_PORT_REQUEST)
{
}

IPCEvent* IPCPAllocatePortRequestMessage::toIPCEvent() {
        return 0;
}

/// Class IPCPWriteMgmtSDURequestMessage
IPCPWriteMgmtSDURequestMessage::IPCPWriteMgmtSDURequestMessage()
	: BaseNetlinkMessage(RINA_C_IPCP_MANAGEMENT_SDU_WRITE_REQUEST)
{
	sdu = 0;
	size = 0;
	port_id = 0;
	address = 0;
}

IPCEvent* IPCPWriteMgmtSDURequestMessage::toIPCEvent()
{
	return 0;
}

///IPCPReadMgmtSDUNotificationMessage
IPCPReadMgmtSDUNotificationMessage::IPCPReadMgmtSDUNotificationMessage()
	: BaseNetlinkMessage(RINA_C_IPCP_MANAGEMENT_SDU_READ_NOTIF)
{
	sdu = 0;
	size = 0;
	port_id = 0;
}

IPCEvent* IPCPReadMgmtSDUNotificationMessage::toIPCEvent()
{
        IPCEvent * event = new ReadMgmtSDUResponseEvent(result,
        						sdu,
							size,
							port_id,
							getSequenceNumber());
        return event;
}

///IpcmCreateIPCPRequestMessage
IpcmCreateIPCPRequestMessage::IpcmCreateIPCPRequestMessage()
	: BaseNetlinkMessage(RINA_C_IPCM_CREATE_IPCP_REQUEST)
{
	ipcp_id = 0;
	nl_port_id = 0;
}

IPCEvent* IpcmCreateIPCPRequestMessage::toIPCEvent()
{
	return 0;
}

///IpcmDestroyIPCPRequestMessage
IpcmDestroyIPCPRequestMessage::IpcmDestroyIPCPRequestMessage()
	: BaseNetlinkMessage(RINA_C_IPCM_DESTROY_IPCP_REQUEST)
{
	ipcp_id = 0;
}

IPCEvent* IpcmDestroyIPCPRequestMessage::toIPCEvent()
{
	return 0;
}

}
