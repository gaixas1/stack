//
// IPC Process
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

#include <ostream>
#include <sstream>
#include <errno.h>

#define RINA_PREFIX "librina.ipc-process"

#include "librina/logs.h"
#include "core.h"
#include "utils.h"

namespace rina {

/* CLASS ASSIGN TO DIF REQUEST EVENT */
AssignToDIFRequestEvent::AssignToDIFRequestEvent(
		const DIFInformation& difInformation,
			unsigned int sequenceNumber):
		IPCEvent(ASSIGN_TO_DIF_REQUEST_EVENT, sequenceNumber)
{
	this->difInformation = difInformation;
}

        const DIFInformation&
        AssignToDIFRequestEvent::getDIFInformation() const
{ return difInformation; }

/* CLASS UPDATE DIF CONFIGURATION REQUEST EVENT */
const DIFConfiguration&
UpdateDIFConfigurationRequestEvent::getDIFConfiguration() const
{
        return difConfiguration;
}

UpdateDIFConfigurationRequestEvent::UpdateDIFConfigurationRequestEvent(
                const DIFConfiguration& difConfiguration,
                        unsigned int sequenceNumber):
                IPCEvent(UPDATE_DIF_CONFIG_REQUEST_EVENT, sequenceNumber)
{
        this->difConfiguration = difConfiguration;
}

/* CLASS IPC PROCESS DIF REGISTRATION EVENT */
IPCProcessDIFRegistrationEvent::IPCProcessDIFRegistrationEvent(
		const ApplicationProcessNamingInformation& ipcProcessName,
		const ApplicationProcessNamingInformation& difName,
		bool registered,
		unsigned int sequenceNumber): IPCEvent(
		                IPC_PROCESS_DIF_REGISTRATION_NOTIFICATION,
		                sequenceNumber)
{
        this->ipcProcessName = ipcProcessName;
        this->difName = difName;
        this->registered = registered;
}

const ApplicationProcessNamingInformation&
IPCProcessDIFRegistrationEvent::getIPCProcessName() const{
	return ipcProcessName;
}

const ApplicationProcessNamingInformation&
IPCProcessDIFRegistrationEvent::getDIFName() const{
	return difName;
}

bool IPCProcessDIFRegistrationEvent::isRegistered() const {
        return registered;
}

/* CLASS QUERY RIB REQUEST EVENT */
QueryRIBRequestEvent::QueryRIBRequestEvent(const std::string& objectClass,
		const std::string& objectName, long objectInstance,
		int scope, const std::string& filter,
		unsigned int sequenceNumber):
				IPCEvent(IPC_PROCESS_QUERY_RIB,
                                         sequenceNumber)
{
	this->objectClass = objectClass;
	this->objectName = objectName;
	this->objectInstance = objectInstance;
	this->scope = scope;
	this->filter = filter;
}

const std::string& QueryRIBRequestEvent::getObjectClass() const{
	return objectClass;
}

const std::string& QueryRIBRequestEvent::getObjectName() const{
	return objectName;
}

long QueryRIBRequestEvent::getObjectInstance() const{
	return objectInstance;
}

int QueryRIBRequestEvent::getScope() const{
	return scope;
}

const std::string& QueryRIBRequestEvent::getFilter() const{
	return filter;
}

/* CLASS SET POLICY SET PARAM REQUEST EVENT */
SetPolicySetParamRequestEvent::SetPolicySetParamRequestEvent(
                const std::string& path, const std::string& name,
                const std::string& value, unsigned int sequenceNumber) :
				IPCEvent(IPC_PROCESS_SET_POLICY_SET_PARAM,
                                         sequenceNumber)
{
        this->path = path;
        this->name = name;
        this->value = value;
}

/* CLASS SELECT POLICY SET REQUEST EVENT */
SelectPolicySetRequestEvent::SelectPolicySetRequestEvent(
                const std::string& path, const std::string& name,
                                        unsigned int sequenceNumber) :
				IPCEvent(IPC_PROCESS_SELECT_POLICY_SET,
                                         sequenceNumber)
{
        this->path = path;
        this->name = name;
}

/* CLASS PLUGIN LOAD REQUEST EVENT */
PluginLoadRequestEvent::PluginLoadRequestEvent(const std::string& name,
                                bool load, unsigned int sequenceNumber) :
				IPCEvent(IPC_PROCESS_PLUGIN_LOAD,
                                         sequenceNumber)
{
        this->name = name;
        this->load = load;
}

/* CLASS CREATE CONNECTION RESPONSE EVENT */
CreateConnectionResponseEvent::CreateConnectionResponseEvent(int portId,
        int cepId, unsigned int sequenceNumber):
                IPCEvent(IPC_PROCESS_CREATE_CONNECTION_RESPONSE,
                                sequenceNumber) {
        this->cepId = cepId;
        this->portId = portId;
}

int CreateConnectionResponseEvent::getCepId() const {
        return cepId;
}

int CreateConnectionResponseEvent::getPortId() const {
        return portId;
}

/* CLASS UPDATE CONNECTION RESPONSE EVENT */
UpdateConnectionResponseEvent::UpdateConnectionResponseEvent(int portId,
        int result, unsigned int sequenceNumber):
                IPCEvent(IPC_PROCESS_UPDATE_CONNECTION_RESPONSE,
                                sequenceNumber) {
        this->result = result;
        this->portId = portId;
}

int UpdateConnectionResponseEvent::getResult() const {
        return result;
}

int UpdateConnectionResponseEvent::getPortId() const {
        return portId;
}

/* CLASS CREATE CONNECTION RESULT EVENT */
CreateConnectionResultEvent::CreateConnectionResultEvent(int portId,
        int sourceCepId, int destCepId, unsigned int sequenceNumber):
                IPCEvent(IPC_PROCESS_CREATE_CONNECTION_RESULT,
                                sequenceNumber) {
        this->sourceCepId = sourceCepId;
        this->destCepId = destCepId;
        this->portId = portId;
}

int CreateConnectionResultEvent::getSourceCepId() const {
        return sourceCepId;
}

int CreateConnectionResultEvent::getDestCepId() const {
        return destCepId;
}

int CreateConnectionResultEvent::getPortId() const {
        return portId;
}

/* CLASS DESTROY CONNECTION RESULT EVENT */
DestroyConnectionResultEvent::DestroyConnectionResultEvent(int portId,
        int result, unsigned int sequenceNumber):
                IPCEvent(IPC_PROCESS_DESTROY_CONNECTION_RESULT,
                                sequenceNumber) {
        this->result = result;
        this->portId = portId;
}

int DestroyConnectionResultEvent::getResult() const {
        return result;
}

int DestroyConnectionResultEvent::getPortId() const {
        return portId;
}

/* CLASS DUMP PDU FT RESULT EVENT*/
DumpFTResponseEvent::DumpFTResponseEvent(const std::list<PDUForwardingTableEntry>& entries,
                int result, unsigned int sequenceNumber):
                IPCEvent(IPC_PROCESS_DUMP_FT_RESPONSE,
                                sequenceNumber) {
        this->entries = entries;
        this->result = result;
}

const std::list<PDUForwardingTableEntry>&
DumpFTResponseEvent::getEntries() const {
        return entries;
}

int DumpFTResponseEvent::getResult() const {
        return result;
}

// Class enable encryption response event
UpdateCryptoStateResponseEvent::UpdateCryptoStateResponseEvent(int res,
                int port, unsigned int sequenceNumber) :
                		IPCEvent(IPC_PROCESS_UPDATE_CRYPTO_STATE_RESPONSE,
                				sequenceNumber)
{
	port_id = port;
	result = res;
}

AllocatePortResponseEvent::AllocatePortResponseEvent(int res,
						     int port,
						     unsigned int sequenceNumber):
		IPCEvent(IPC_PROCESS_ALLOCATE_PORT_RESPONSE,
			 sequenceNumber)
{
	port_id = port;
	result = res;
}

DeallocatePortResponseEvent::DeallocatePortResponseEvent(int res,
						         int port,
							 unsigned int sequenceNumber):
		IPCEvent(IPC_PROCESS_DEALLOCATE_PORT_RESPONSE,
			 sequenceNumber)
{
	port_id = port;
	result = res;
}

WriteMgmtSDUResponseEvent::WriteMgmtSDUResponseEvent(int res,
			  	  	  	     unsigned int sequenceNumber):
		IPCEvent(IPC_PROCESS_WRITE_MGMT_SDU_RESPONSE,
			 sequenceNumber)
{
	result = res;
}

ReadMgmtSDUResponseEvent::ReadMgmtSDUResponseEvent(int res,
			 	 	 	   void * data,
						   int s,
						   unsigned int pid,
						   unsigned int sequenceNumber):
		IPCEvent(IPC_PROCESS_READ_MGMT_SDU_NOTIF,
			 sequenceNumber)
{
	result = res;
	sdu = data;
	size = s;
	port_id = pid;
}

/* CLASS EXTENDED IPC MANAGER */
const std::string ExtendedIPCManager::error_allocate_flow =
		"Error allocating flow";

ExtendedIPCManager::ExtendedIPCManager() {
        ipcManagerPort = 0;
        ipcProcessId = 0;
        ipcProcessInitialized = false;
}

ExtendedIPCManager::~ExtendedIPCManager() throw()
{ }

const DIFInformation& ExtendedIPCManager::getCurrentDIFInformation() const{
	return currentDIFInformation;
}

void ExtendedIPCManager::setCurrentDIFInformation(
		const DIFInformation& currentDIFInformation)
{
	this->currentDIFInformation = currentDIFInformation;
}

unsigned short ExtendedIPCManager::getIpcProcessId() const
{ return ipcProcessId; }

void ExtendedIPCManager::setIpcProcessId(unsigned short ipcProcessId)
{ this->ipcProcessId = ipcProcessId; }

void ExtendedIPCManager::setIPCManagerPort(
                unsigned int ipcManagerPort) {
        this->ipcManagerPort = ipcManagerPort;
}

void ExtendedIPCManager::notifyIPCProcessInitialized(
                const ApplicationProcessNamingInformation& name) {
	ScopedLock slock(lock);

        if (ipcProcessInitialized) {
                throw IPCException("IPC Process already initialized");
        }

#if STUB_API
        // Do nothing
#else
        IpcmIPCProcessInitializedMessage message;
        message.setName(name);
        message.setSourceIpcProcessId(ipcProcessId);
        message.setDestPortId(ipcManagerPort);
        message.setNotificationMessage(true);

        try {
                rinaManager->sendMessage(&message, false);
        } catch(NetlinkException &e) {
                throw IPCException(e.what());
        }
#endif
        ipcProcessInitialized = true;
}

bool ExtendedIPCManager::isIPCProcessInitialized() const
{
        return ipcProcessInitialized;
}

ApplicationRegistration * ExtendedIPCManager::appRegistered(
                        const ApplicationProcessNamingInformation& appName,
                        const ApplicationProcessNamingInformation& DIFName) {
        ApplicationRegistration * applicationRegistration;

        ScopedLock slock(lock);

        applicationRegistration = getApplicationRegistration(
                        appName);

        if (!applicationRegistration) {
                applicationRegistration = new ApplicationRegistration(
                                appName);
                putApplicationRegistration(appName,
                                applicationRegistration);
        }

        applicationRegistration->addDIFName(DIFName);

        return applicationRegistration;
}

void ExtendedIPCManager::appUnregistered(
                const ApplicationProcessNamingInformation& appName,
                const ApplicationProcessNamingInformation& DIFName) {
	ScopedLock slock(lock);

        ApplicationRegistration * applicationRegistration =
                        getApplicationRegistration(appName);
        if (!applicationRegistration) {
                throw ApplicationUnregistrationException(
                                IPCManager::application_not_registered_error.c_str());
        }

        std::list<ApplicationProcessNamingInformation>::const_iterator iterator;
        for (iterator = applicationRegistration->DIFNames.begin();
                        iterator != applicationRegistration->DIFNames.end();
                        ++iterator) {
                if (*iterator == DIFName) {
                        applicationRegistration->removeDIFName(DIFName);
                        if (applicationRegistration->DIFNames.size() == 0) {
                                removeApplicationRegistration(appName);
                        }

                        break;
                }
        }
}

void ExtendedIPCManager::assignToDIFResponse(
		const AssignToDIFRequestEvent& event, int result) {
	if (result == 0) {
		this->currentDIFInformation = event.getDIFInformation();
	}
#if STUB_API
	//Do nothing
#else
	BaseNetlinkMessage responseMessage(RINA_C_IPCM_ASSIGN_TO_DIF_RESPONSE);
	responseMessage.result = result;
	responseMessage.setSequenceNumber(event.sequenceNumber);
	responseMessage.setSourceIpcProcessId(ipcProcessId);
        responseMessage.setDestPortId(ipcManagerPort);
	responseMessage.setResponseMessage(true);
	try {
                rinaManager->sendMessage(&responseMessage, false);
	} catch (NetlinkException &e) {
		throw AssignToDIFResponseException(e.what());
	}
#endif
}

void ExtendedIPCManager::enrollToDIFResponse(const EnrollToDAFRequestEvent& event,
                        int result, const std::list<Neighbor> & newNeighbors,
                        const DIFInformation& difInformation) {
#if STUB_API
        // Do nothing
#else
        IpcmEnrollToDIFResponseMessage responseMessage;
        responseMessage.result = result;
        responseMessage.setNeighbors(newNeighbors);
        responseMessage.setDIFInformation(difInformation);
        responseMessage.setSourceIpcProcessId(ipcProcessId);
        responseMessage.setDestPortId(ipcManagerPort);
        responseMessage.setSequenceNumber(event.sequenceNumber);
        responseMessage.setResponseMessage(true);
        try {
        	//FIXME, compute maximum message size dynamically
        	rinaManager->sendMessageOfMaxSize(&responseMessage,
        					  5 * get_page_size(),
        					  false);
        } catch (NetlinkException &e) {
                throw EnrollException(e.what());
        }
#endif
}

void ExtendedIPCManager::disconnectNeighborResponse(const DisconnectNeighborRequestEvent& event,
			        		    int result)
{
#if STUB_API
        // Do nothing
#else
	BaseNetlinkMessage responseMessage(RINA_C_IPCM_DISCONNECT_FROM_NEIGHBOR_RESPONSE);
        responseMessage.result = result;
        responseMessage.setSourceIpcProcessId(ipcProcessId);
        responseMessage.setDestPortId(ipcManagerPort);
        responseMessage.setSequenceNumber(event.sequenceNumber);
        responseMessage.setResponseMessage(true);
        try {
        	//FIXME, compute maximum message size dynamically
        	rinaManager->sendMessage(&responseMessage, false);
        } catch (NetlinkException &e) {
                throw IPCException(e.what());
        }
#endif
}

void ExtendedIPCManager::registerApplicationResponse(
		const ApplicationRegistrationRequestEvent& event, int result) {
#if STUB_API
	//Do nothing
#else
	BaseNetlinkMessage responseMessage(RINA_C_IPCM_REGISTER_APPLICATION_RESPONSE);
	responseMessage.result = result;
	responseMessage.setSequenceNumber(event.sequenceNumber);
	responseMessage.setSourceIpcProcessId(ipcProcessId);
	responseMessage.setDestPortId(ipcManagerPort);
	responseMessage.setResponseMessage(true);
	try {
		rinaManager->sendMessage(&responseMessage, false);
	} catch(NetlinkException &e) {
		throw RegisterApplicationResponseException(e.what());
	}
#endif
}

void ExtendedIPCManager::unregisterApplicationResponse(
		const ApplicationUnregistrationRequestEvent& event, int result) {
#if STUB_API
	// Do nothing
#else
	BaseNetlinkMessage responseMessage(RINA_C_IPCM_UNREGISTER_APPLICATION_RESPONSE);
	responseMessage.result = result;
	responseMessage.setSequenceNumber(event.sequenceNumber);
	responseMessage.setSourceIpcProcessId(ipcProcessId);
	responseMessage.setDestPortId(ipcManagerPort);
	responseMessage.setResponseMessage(true);
	try {
		rinaManager->sendMessage(&responseMessage, false);
	} catch (NetlinkException &e) {
		throw UnregisterApplicationResponseException(e.what());
	}
#endif
}

void ExtendedIPCManager::allocateFlowRequestResult(
		const FlowRequestEvent& event, int result) {
#if STUB_API
	// Do nothing
#else
	BaseNetlinkMessage responseMessage(RINA_C_IPCM_ALLOCATE_FLOW_REQUEST_RESULT);
	responseMessage.result = result;
	responseMessage.port_id = event.portId;
	responseMessage.setSequenceNumber(event.sequenceNumber);
	responseMessage.setSourceIpcProcessId(ipcProcessId);
	responseMessage.setDestPortId(ipcManagerPort);
	responseMessage.setResponseMessage(true);

	try {
		rinaManager->sendMessage(&responseMessage, false);
	} catch(NetlinkException &e) {
		throw AllocateFlowResponseException(e.what());
	}
#endif
}

unsigned int ExtendedIPCManager::allocateFlowRequestArrived(
			const ApplicationProcessNamingInformation& localAppName,
			const ApplicationProcessNamingInformation& remoteAppName,
			const FlowSpecification& flowSpecification,
			int portId) {
#if STUP_API
	return 0;
#else
	IpcmAllocateFlowRequestArrivedMessage message;
	message.setSourceAppName(remoteAppName);
	message.setDestAppName(localAppName);
	message.setFlowSpecification(flowSpecification);
	message.setDifName(currentDIFInformation.get_dif_name());
	message.setPortId(portId);
	message.setSourceIpcProcessId(ipcProcessId);
	message.setDestPortId(ipcManagerPort);
	message.setRequestMessage(true);

	try {
	        rinaManager->sendMessage(&message, true);
	} catch (NetlinkException &e) {
	        throw AllocateFlowRequestArrivedException(e.what());
	}

	return message.getSequenceNumber();
#endif
}

unsigned int ExtendedIPCManager::requestFlowAllocation(
                const ApplicationProcessNamingInformation& localAppName,
                const ApplicationProcessNamingInformation& remoteAppName,
                const FlowSpecification& flowSpec)
{
        return internalRequestFlowAllocation(
                        localAppName, remoteAppName, flowSpec, ipcProcessId);
}

unsigned int ExtendedIPCManager::requestFlowAllocationInDIF(
                const ApplicationProcessNamingInformation& localAppName,
                const ApplicationProcessNamingInformation& remoteAppName,
                const ApplicationProcessNamingInformation& difName,
                const FlowSpecification& flowSpec)
{
        return internalRequestFlowAllocationInDIF(localAppName,
                        remoteAppName, difName, ipcProcessId, flowSpec);
}

FlowInformation ExtendedIPCManager::allocateFlowResponse(const FlowRequestEvent& flowRequestEvent,
							 int result,
							 bool notifySource)
{
        return internalAllocateFlowResponse(flowRequestEvent,
                                            result,
                                            notifySource,
                                            ipcProcessId);
}

void ExtendedIPCManager::notifyflowDeallocated(
		const FlowDeallocateRequestEvent flowDeallocateEvent,
		int result)
{
#if STUB_API
	// Do nothing
#else
	BaseNetlinkMessage responseMessage(RINA_C_IPCM_DEALLOCATE_FLOW_RESPONSE);
	responseMessage.result = result;
	responseMessage.setSourceIpcProcessId(ipcProcessId);
	responseMessage.setSequenceNumber(flowDeallocateEvent.sequenceNumber);
	responseMessage.setDestPortId(ipcManagerPort);
	responseMessage.setResponseMessage(true);
	try {
		rinaManager->sendMessage(&responseMessage, false);
	} catch (NetlinkException &e) {
		throw DeallocateFlowResponseException(e.what());
	}
#endif
}

void ExtendedIPCManager::flowDeallocatedRemotely(
		int portId, int code) {
#if STUB_API
	// Do nothing
#else
	BaseNetlinkMessage message(RINA_C_IPCM_FLOW_DEALLOCATED_NOTIFICATION);
	message.port_id = portId;
	message.result = code;
	message.setSourceIpcProcessId(ipcProcessId);
	message.setDestPortId(ipcManagerPort);
	message.setNotificationMessage(true);
	try {
		rinaManager->sendMessage(&message, false);
	} catch (NetlinkException &e) {
		throw DeallocateFlowResponseException(e.what());
	}
#endif
}

void ExtendedIPCManager::queryRIBResponse(
		const QueryRIBRequestEvent& event, int result,
		const std::list<rib::RIBObjectData>& ribObjects) {
#if STUB_API
	//Do nothing
#else
	IpcmDIFQueryRIBResponseMessage responseMessage;
	responseMessage.result = result;
	responseMessage.setRIBObjects(ribObjects);
	responseMessage.setSequenceNumber(event.sequenceNumber);
	responseMessage.setSourceIpcProcessId(ipcProcessId);
	responseMessage.setDestPortId(ipcManagerPort);
	responseMessage.setResponseMessage(true);
	try {
	        //FIXME, compute maximum message size dynamically
		rinaManager->sendMessageOfMaxSize(&responseMessage,
                                          5 * get_page_size(),
                                          false);
	} catch (NetlinkException &e) {
		throw QueryRIBResponseException(e.what());
	}
#endif
}

unsigned int ExtendedIPCManager::allocatePortId(const ApplicationProcessNamingInformation& appName)
{
	unsigned int result = 0;

#if STUB_API
#else
        IPCPAllocatePortRequestMessage message;
        message.app_name = appName;
        message.setSourceIpcProcessId(ipcProcessId);
        message.setDestIpcProcessId(ipcProcessId);
        message.setDestPortId(0);
        message.setRequestMessage(true);

        try{
                rinaManager->sendMessage(&message, true);
        }catch(NetlinkException &e){
                throw IPCException(e.what());
        }

        result = message.getSequenceNumber();
#endif

        return result;
}

unsigned int ExtendedIPCManager::deallocatePortId(int portId)
{
	unsigned int result = 0;

#if STUB_API
#else
	BaseNetlinkMessage message(RINA_C_IPCP_DEALLOCATE_PORT_REQUEST);
        message.port_id = portId;
        message.setSourceIpcProcessId(ipcProcessId);
        message.setDestIpcProcessId(ipcProcessId);
        message.setDestPortId(0);
        message.setRequestMessage(true);

        try{
                rinaManager->sendMessage(&message, true);
        }catch(NetlinkException &e){
                throw IPCException(e.what());
        }

        result = message.getSequenceNumber();
#endif

        return result;
}

void ExtendedIPCManager::setPolicySetParamResponse(
		const SetPolicySetParamRequestEvent& event, int result) {
#if STUB_API
	//Do nothing
#else
	BaseNetlinkMessage responseMessage(RINA_C_IPCM_SET_POLICY_SET_PARAM_RESPONSE);
	responseMessage.result = result;
	responseMessage.setSequenceNumber(event.sequenceNumber);
	responseMessage.setSourceIpcProcessId(ipcProcessId);
        responseMessage.setDestPortId(ipcManagerPort);
	responseMessage.setResponseMessage(true);
	try {
		rinaManager->sendMessage(&responseMessage, false);
	} catch (NetlinkException &e) {
		throw SetPolicySetParamException(e.what());
	}
#endif
}

void ExtendedIPCManager::selectPolicySetResponse(
		const SelectPolicySetRequestEvent& event, int result) {
#if STUB_API
	//Do nothing
#else
	BaseNetlinkMessage responseMessage(RINA_C_IPCM_SELECT_POLICY_SET_RESPONSE);
	responseMessage.result = result;
	responseMessage.setSequenceNumber(event.sequenceNumber);
	responseMessage.setSourceIpcProcessId(ipcProcessId);
        responseMessage.setDestPortId(ipcManagerPort);
	responseMessage.setResponseMessage(true);
	try {
		rinaManager->sendMessage(&responseMessage, false);
	} catch (NetlinkException &e) {
		throw SelectPolicySetException(e.what());
	}
#endif
}

void ExtendedIPCManager::pluginLoadResponse(const PluginLoadRequestEvent& event,
					    int result) {
#if STUB_API
	//Do nothing
#else
	BaseNetlinkMessage responseMessage(RINA_C_IPCM_PLUGIN_LOAD_RESPONSE);
	responseMessage.result = result;
	responseMessage.setSequenceNumber(event.sequenceNumber);
	responseMessage.setSourceIpcProcessId(ipcProcessId);
        responseMessage.setDestPortId(ipcManagerPort);
	responseMessage.setResponseMessage(true);
	try {
		rinaManager->sendMessage(&responseMessage, false);
	} catch (NetlinkException &e) {
		throw PluginLoadException(e.what());
	}
#endif
}

void ExtendedIPCManager::forwardCDAPRequest(unsigned int sequenceNumber,
                                             const ser_obj_t& sermsg,
                                             int result)
{
#if STUB_API
        //Do nothing
#else
        IpcmFwdCDAPRequestMessage requestMessage;

        requestMessage.sermsg = sermsg;
        requestMessage.result = result;
        requestMessage.setSequenceNumber(sequenceNumber);
        requestMessage.setSourceIpcProcessId(ipcProcessId);
        requestMessage.setDestPortId(ipcManagerPort);
        requestMessage.setResponseMessage(true);
        int size = get_page_size() + sermsg.size_;
        try {
                rinaManager->sendMessageOfMaxSize(&requestMessage,
                                                  size,
                                                  false);
        } catch (NetlinkException &e) {
                throw FwdCDAPMsgException(e.what());
        }
#endif
}

void ExtendedIPCManager::forwardCDAPResponse(unsigned int sequenceNumber,
					     const ser_obj_t& sermsg,
					     int result)
{
#if STUB_API
	//Do nothing
#else
	IpcmFwdCDAPResponseMessage responseMessage;

	responseMessage.sermsg = sermsg;
	responseMessage.result = result;
	responseMessage.setSequenceNumber(sequenceNumber);
	responseMessage.setSourceIpcProcessId(ipcProcessId);
        responseMessage.setDestPortId(ipcManagerPort);
	responseMessage.setResponseMessage(true);
	int size = get_page_size() + sermsg.size_;
	try {
		rinaManager->sendMessageOfMaxSize(&responseMessage,
		                                  2 * size,
                                                  false);
	} catch (NetlinkException &e) {
		throw FwdCDAPMsgException(e.what());
	}
#endif
}

void ExtendedIPCManager::sendMediaReport(const MediaReport& report)
{
#if STUB_API
	//Do nothing
#else
	IpcmMediaReportMessage message;

	message.report = report;
	message.setSourceIpcProcessId(ipcProcessId);
	message.setDestPortId(ipcManagerPort);
	message.setNotificationMessage(true);
	try {
		rinaManager->sendMessage(&message, false);
	} catch (NetlinkException &e) {
		throw IPCException(e.what());
	}
#endif
}

int ExtendedIPCManager::internal_flow_allocated(const rina::FlowInformation& flow_info)
{
	FlowInformation * flow;
	WriteScopedLock writeLock(flows_rw_lock);

	flow = new FlowInformation();
	flow->portId = flow_info.portId;
	flow->difName = flow_info.difName;
	flow->flowSpecification = flow_info.flowSpecification;
	flow->localAppName = flow_info.localAppName;
	flow->remoteAppName = flow_info.remoteAppName;
	flow->state = FlowInformation::FLOW_ALLOCATED;

	initIodev(flow, flow->portId);

	allocatedFlows[flow->portId] = flow;

	return flow->fd;
}

void ExtendedIPCManager::internal_flow_deallocated(int port_id)
{
        FlowInformation * flow;

        WriteScopedLock writeLock(flows_rw_lock);

        flow = getAllocatedFlow(port_id);
        if (flow == 0) {
                throw FlowDeallocationException(
                                IPCManager::unknown_flow_error);
        }

        close(flow->fd);
        allocatedFlows.erase(port_id);
        delete flow;
}

Singleton<ExtendedIPCManager> extendedIPCManager;

/* CLASS CONNECTION */
Connection::Connection() {
        portId = 0;
        sourceAddress = 0;
        destAddress = 0;
        qosId = 0;
        sourceCepId = 0;
        destCepId = 0;
        flowUserIpcProcessId = 0;
}

unsigned int Connection::getDestAddress() const {
        return destAddress;
}

void Connection::setDestAddress(unsigned int destAddress) {
        this->destAddress = destAddress;
}

int Connection::getPortId() const {
        return portId;
}

void Connection::setPortId(int portId) {
        this->portId = portId;
}

unsigned int Connection::getQosId() const {
        return qosId;
}

void Connection::setQosId(unsigned int qosId) {
        this->qosId = qosId;
}

unsigned int Connection::getSourceAddress() const {
        return sourceAddress;
}

void Connection::setSourceAddress(unsigned int sourceAddress) {
        this->sourceAddress = sourceAddress;
}

int Connection::getDestCepId() const {
        return destCepId;
}

void Connection::setDestCepId(int destCepId) {
        this->destCepId = destCepId;
}

unsigned short Connection::getFlowUserIpcProcessId() const {
        return flowUserIpcProcessId;
}

void Connection::setFlowUserIpcProcessId(unsigned short flowUserIpcProcessId) {
        this->flowUserIpcProcessId = flowUserIpcProcessId;
}

int Connection::getSourceCepId() const {
        return sourceCepId;
}

void Connection::setSourceCepId(int sourceCepId) {
        this->sourceCepId = sourceCepId;
}

const DTPConfig& Connection::getDTPConfig() const {
        return dtpConfig;
}

void Connection::setDTPConfig(const DTPConfig& dtpConfig) {
        this->dtpConfig = dtpConfig;
}

const DTCPConfig& Connection::getDTCPConfig() const {
        return dtcpConfig;
}

void Connection::setDTCPConfig(const DTCPConfig& dtcpConfig) {
        this->dtcpConfig = dtcpConfig;
}

const std::string Connection::toString() {
        std::stringstream ss;
        ss<<"Source address: "<<sourceAddress;
        ss<<"; Source cep-id: "<<sourceCepId;
        ss<<"; Dest address: "<<destAddress;
        ss<<"; Dest cep-id: "<<destCepId<<std::endl;
        ss<<"Por-id: "<<portId<<"; QoS-id: "<<qosId;
        ss<<"; Flow user IPC Process id: "<<flowUserIpcProcessId<<std::endl;
        ss<<"DTP Configuration: "<<dtpConfig.toString();
        ss<<"DTCP Configuration: "<<dtcpConfig.toString();
        return ss.str();
}

DTPInformation::DTPInformation()
{
	src_cep_id = 0;
	dest_cep_id = 0;
	src_address = 0;
	dest_address = 0;
	qos_id = 0;
	port_id = 0;
}

DTPInformation::DTPInformation(Connection * connection)
{
	src_cep_id = connection->sourceCepId;
	dest_cep_id = connection->destCepId;
	src_address = connection->sourceAddress;
	dest_address = connection->destAddress;
	qos_id = connection->qosId;
	port_id = connection->portId;
	dtp_config = connection->dtpConfig;
	stats = connection->stats;
}

const std::string DTPInformation::toString() const
{
	std::stringstream ss;
	ss << "CEP-ids: src = " << src_cep_id << ", dest = " << dest_cep_id
	   << "; Addresses: src = " << src_address << ", dest = " << dest_address
	   << "; Qos-id: " << qos_id << "; Port-id: " << port_id << std::endl;
	ss << " DTP config: " << dtp_config.toString();
	ss << " Tx: pdus = " << stats.tx_pdus << ", Bytes = " << stats.tx_bytes
	   << " RX: pdus = " << stats.rx_pdus << ", Bytes = " << stats.rx_bytes << std::endl;

	return ss.str();
}


std::string IPCPNameAddresses::get_addresses_as_string() const
{
	std::stringstream ss;

	for (std::list<unsigned int>::const_iterator it = addresses.begin();
			it != addresses.end(); ++it) {
		ss << *it << "; ";
	}

	return ss.str();
}

/* CLASS ROUTING TABLE ENTRE */
RoutingTableEntry::RoutingTableEntry(){
	cost = 1;
	qosId = 0;
}

const std::string RoutingTableEntry::getKey() const
{
	std::stringstream ss;
	ss << destination.name << "-" << qosId << "-" << nextHopNames.front().alts.front().name;

	return ss.str();
}

PortIdAltlist::PortIdAltlist()
{
}

PortIdAltlist::PortIdAltlist(unsigned int nh)
{
	add_alt(nh);
}

void PortIdAltlist::add_alt(unsigned int nh)
{
	alts.push_back(nh);
}

/* CLASS PDU FORWARDING TABLE ENTRY */
PDUForwardingTableEntry::PDUForwardingTableEntry() {
        address = 0;
        qosId = 0;
        cost = 0;
}

bool PDUForwardingTableEntry::operator==(
                const PDUForwardingTableEntry &other) const {
        if (address != other.getAddress())
                return false;

        if (qosId != other.getQosId())
                return false;

        if (cost != other.cost)
        	return false;

        return true;
}

bool PDUForwardingTableEntry::operator!=(
                const PDUForwardingTableEntry &other) const {
        return !(*this == other);
}

unsigned int PDUForwardingTableEntry::getAddress() const {
        return address;
}

void PDUForwardingTableEntry::setAddress(unsigned int address) {
        this->address = address;
}

const std::list<PortIdAltlist> PDUForwardingTableEntry::getPortIdAltlists() const {
        return portIdAltlists;
}

void PDUForwardingTableEntry::
setPortIdAltlists(const std::list<PortIdAltlist>& portIdAltlists) {
        this->portIdAltlists = portIdAltlists;
}

unsigned int PDUForwardingTableEntry::getQosId() const {
        return qosId;
}

void PDUForwardingTableEntry::setQosId(unsigned int qosId) {
        this->qosId = qosId;
}

const std::string PDUForwardingTableEntry::toString() {
        std::stringstream ss;

        ss<<"Address: "<<address<<" QoS-id: "<<qosId<< " Cost: "<<cost;
        ss<<"List of N-1 port-ids: ";
        for (std::list<PortIdAltlist>::iterator it = portIdAltlists.begin();
                        it != portIdAltlists.end(); it++) {
		for (std::list<unsigned int>::iterator jt = it->alts.begin();
				jt != it->alts.end(); jt++) {
			ss<< *jt << ",";
		}
		ss << ";";
        }
        ss<<std::endl;

        return ss.str();
}

const std::string PDUForwardingTableEntry::getKey() const
{
	std::stringstream ss;
	ss << address << "-" << qosId << "-" << portIdAltlists.front().alts.front();

	return ss.str();
}


/* CLASS READ MANAGEMENT SDU RESULT */
ReadManagementSDUResult::ReadManagementSDUResult() {
        bytesRead = 0;
        portId = 0;
}

int ReadManagementSDUResult::getBytesRead() const {
        return bytesRead;
}

void ReadManagementSDUResult::setBytesRead(int bytesRead) {
        this->bytesRead = bytesRead;
}

int ReadManagementSDUResult::getPortId() const {
        return portId;
}

void ReadManagementSDUResult::setPortId(int portId) {
        this->portId = portId;
}

/* CLASS KERNEL IPC PROCESS */
void KernelIPCProcess::setIPCProcessId(unsigned short ipcProcessId) {
        this->ipcProcessId = ipcProcessId;
}

unsigned short KernelIPCProcess::getIPCProcessId() const {
        return ipcProcessId;
}

unsigned int KernelIPCProcess::assignToDIF(
                const DIFInformation& difInformation) {
        unsigned int seqNum = 0;

#if STUB_API
        // Do nothing
#else
        IpcmAssignToDIFRequestMessage message;
        message.setDIFInformation(difInformation);
        message.setSourceIpcProcessId(ipcProcessId);
        message.setDestIpcProcessId(ipcProcessId);
        message.setDestPortId(0);
        message.setRequestMessage(true);

        try {
        	//FIXME, compute maximum message size dynamically
		rinaManager->sendMessageOfMaxSize(&message,
                                          	  5 * get_page_size(),
                                          	  true);
        } catch (NetlinkException &e) {
                throw AssignToDIFException(e.what());
        }

        seqNum = message.getSequenceNumber();
#endif
        return seqNum;
}

unsigned int KernelIPCProcess::updateDIFConfiguration(
                const DIFConfiguration& difConfiguration) {
        unsigned int seqNum=0;

#if STUB_API
        // Do nothing
#else
        IpcmUpdateDIFConfigurationRequestMessage message;
        message.setDIFConfiguration(difConfiguration);
        message.setSourceIpcProcessId(ipcProcessId);
        message.setDestIpcProcessId(ipcProcessId);
        message.setDestPortId(0);
        message.setRequestMessage(true);

        try {
                rinaManager->sendMessage(&message, true);
        } catch (NetlinkException &e) {
                throw UpdateDIFConfigurationException(e.what());
        }

        seqNum = message.getSequenceNumber();

#endif
        return seqNum;
}

unsigned int KernelIPCProcess::createConnection(const Connection& connection) {
        unsigned int seqNum=0;

#if STUB_API
        // Do nothing
#else
        IpcpConnectionCreateRequestMessage message;
        message.connection = connection;
        message.setSourceIpcProcessId(ipcProcessId);
        message.setDestIpcProcessId(ipcProcessId);
        message.setDestPortId(0);
        message.setRequestMessage(true);

        try {
                rinaManager->sendMessage(&message, true);
        } catch (NetlinkException &e) {
                throw CreateConnectionException(e.what());
        }

        seqNum = message.getSequenceNumber();

#endif
        return seqNum;
}

unsigned int KernelIPCProcess::updateConnection(const Connection& connection) {
        unsigned int seqNum=0;

#if STUB_API
        // Do nothing
#else
        IpcpConnectionUpdateRequestMessage message;
        message.portId = connection.portId;
        message.sourceCepId = connection.sourceCepId;
        message.destinationCepId = connection.destCepId;
        message.setSourceIpcProcessId(ipcProcessId);
        message.setDestIpcProcessId(ipcProcessId);
        message.setDestPortId(0);
        message.setRequestMessage(true);

        try {
                rinaManager->sendMessage(&message, true);
        } catch (NetlinkException &e) {
                throw UpdateConnectionException(e.what());
        }

        seqNum = message.getSequenceNumber();

#endif
        return seqNum;
}

unsigned int KernelIPCProcess::
createConnectionArrived(const Connection& connection) {
        unsigned int seqNum=0;

#if STUB_API
        // Do nothing
#else
        IpcpConnectionCreateArrivedMessage message;
        message.connection = connection;
        message.setSourceIpcProcessId(ipcProcessId);
        message.setDestIpcProcessId(ipcProcessId);
        message.setDestPortId(0);
        message.setRequestMessage(true);

        try {
                rinaManager->sendMessage(&message, true);
        } catch (NetlinkException &e) {
                throw CreateConnectionException(e.what());
        }

        seqNum = message.getSequenceNumber();

#endif
        return seqNum;
}

unsigned int KernelIPCProcess::
destroyConnection(const Connection& connection) {
        unsigned int seqNum = 0;

#if STUB_API
        //Do nothing
#else
        BaseNetlinkMessage message(RINA_C_IPCP_CONN_DESTROY_REQUEST);
        message.port_id = connection.getPortId();
        message.cep_id = connection.getSourceCepId();
        message.setSourceIpcProcessId(ipcProcessId);
        message.setDestIpcProcessId(ipcProcessId);
        message.setDestPortId(0);
        message.setRequestMessage(true);

        try {
                rinaManager->sendMessage(&message, true);
        } catch (NetlinkException &e) {
                throw DestroyConnectionException(e.what());
        }

        seqNum = message.getSequenceNumber();

#endif
        return seqNum;
}

void KernelIPCProcess::
modifyPDUForwardingTableEntries(const std::list<PDUForwardingTableEntry *>& entries,
                        int mode) {
#if STUB_API
        //Do nothing
#else
        RmtModifyPDUFTEntriesRequestMessage message;
        message.setEntries(entries);
        message.setMode(mode);
        message.setSourceIpcProcessId(ipcProcessId);
        message.setDestIpcProcessId(ipcProcessId);
        message.setDestPortId(0);
        message.setRequestMessage(true);

        try {
                rinaManager->sendMessage(&message, true);
        } catch (NetlinkException &e) {
                throw PDUForwardingTableException(e.what());
        }
#endif
}

unsigned int KernelIPCProcess::dumptPDUFT() {
        unsigned int seqNum=0;

#if STUB_API
        //Do nothing
#else
        BaseNetlinkMessage message(RINA_C_RMT_DUMP_FT_REQUEST);
        message.setSourceIpcProcessId(ipcProcessId);
        message.setDestIpcProcessId(ipcProcessId);
        message.setDestPortId(0);
        message.setRequestMessage(true);

        try {
                rinaManager->sendMessage(&message, true);
        } catch (NetlinkException &e) {
                throw PDUForwardingTableException(e.what());
        }

        seqNum = message.getSequenceNumber();
#endif

        return seqNum;
}

unsigned int KernelIPCProcess::updateCryptoState(const CryptoState& state)
{
        unsigned int seqNum=0;

#if STUB_API
        //Do nothing
#else
        IPCPUpdateCryptoStateRequestMessage message;
        message.setSourceIpcProcessId(ipcProcessId);
        message.setDestIpcProcessId(ipcProcessId);
        message.state = state;
        message.setDestPortId(0);
        message.setRequestMessage(true);

        try {
                rinaManager->sendMessage(&message, true);
        } catch (NetlinkException &e) {
                throw Exception(e.what());
        }

        seqNum = message.getSequenceNumber();
#endif

        return seqNum;
}

unsigned int KernelIPCProcess::changeAddress(unsigned int new_address,
					     unsigned int old_address,
				             unsigned int use_new_t,
				             unsigned int deprecate_old_t)
{
	unsigned int seqNum=0;

#if STUB_API
	//Do nothing
#else
	IPCPAddressChangeRequestMessage message;
	message.setSourceIpcProcessId(ipcProcessId);
	message.setDestIpcProcessId(ipcProcessId);
	message.new_address = new_address;
	message.old_address = old_address;
	message.use_new_timeout = use_new_t;
	message.deprecate_old_timeout = deprecate_old_t;
	message.setDestPortId(0);
	message.setRequestMessage(true);

	try {
		rinaManager->sendMessage(&message, true);
	} catch (NetlinkException &e) {
		throw Exception(e.what());
	}

	seqNum = message.getSequenceNumber();
#endif

	return seqNum;
}

unsigned int KernelIPCProcess::setPolicySetParam(
                                const std::string& path,
                                const std::string& name,
                                const std::string& value)
{
        unsigned int seqNum=0;

#if STUB_API
        //Do nothing
#else
        IpcmSetPolicySetParamRequestMessage message;
        message.path = path;
        message.name = name;
        message.value = value;
        message.setSourceIpcProcessId(ipcProcessId);
        message.setDestIpcProcessId(ipcProcessId);
        message.setDestPortId(0);
        message.setRequestMessage(true);

        try {
                rinaManager->sendMessage(&message, true);
        } catch (NetlinkException &e) {
                throw SetPolicySetParamException(e.what());
        }

        seqNum = message.getSequenceNumber();
#endif

        return seqNum;
}

unsigned int KernelIPCProcess::selectPolicySet(
                                const std::string& path,
                                const std::string& name)
{
        unsigned int seqNum=0;

#if STUB_API
        //Do nothing
#else
        IpcmSelectPolicySetRequestMessage message;
        message.path = path;
        message.name = name;
        message.setSourceIpcProcessId(ipcProcessId);
        message.setDestIpcProcessId(ipcProcessId);
        message.setDestPortId(0);
        message.setRequestMessage(true);

        try {
                rinaManager->sendMessage(&message, true);
        } catch (NetlinkException &e) {
                throw SelectPolicySetException(e.what());
        }

        seqNum = message.getSequenceNumber();
#endif

        return seqNum;
}

unsigned int KernelIPCProcess::writeMgmgtSDUToPortId(void * sdu,
						     int size,
						     unsigned int portId)
{
	unsigned int seqNum=0;

#if STUB_API
	//Do nothing
#else
	IPCPWriteMgmtSDURequestMessage message;
	message.setSourceIpcProcessId(ipcProcessId);
	message.setDestIpcProcessId(ipcProcessId);
	message.sdu = sdu;
	message.size = size;
	message.port_id = portId;
	message.setDestPortId(0);
	message.setRequestMessage(true);

	try {
		rinaManager->sendMessage(&message, true);
	} catch (NetlinkException &e) {
		throw Exception(e.what());
	}

	seqNum = message.getSequenceNumber();
#endif

	return seqNum;
}

unsigned int KernelIPCProcess::sendMgmgtSDUToAddress(void * sdu,
						     int size,
						     unsigned int address)
{
	unsigned int seqNum=0;

#if STUB_API
	//Do nothing
#else
	IPCPWriteMgmtSDURequestMessage message;
	message.setSourceIpcProcessId(ipcProcessId);
	message.setDestIpcProcessId(ipcProcessId);
	message.sdu = sdu;
	message.size = size;
	message.address = address;
	message.setDestPortId(0);
	message.setRequestMessage(true);

	try {
		rinaManager->sendMessage(&message, true);
	} catch (NetlinkException &e) {
		throw Exception(e.what());
	}

	seqNum = message.getSequenceNumber();
#endif

	return seqNum;
}
Singleton<KernelIPCProcess> kernelIPCProcess;

// CLASS DirectoryForwardingTableEntry
DirectoryForwardingTableEntry::DirectoryForwardingTableEntry() {
	address_ = 0;
	seqnum_ = 0;
}

ApplicationProcessNamingInformation DirectoryForwardingTableEntry::get_ap_naming_info() const {
	return ap_naming_info_;
}

void DirectoryForwardingTableEntry::set_ap_naming_info(
		const ApplicationProcessNamingInformation &ap_naming_info) {
	ap_naming_info_ = ap_naming_info;
}

unsigned int DirectoryForwardingTableEntry::get_address() const {
	return address_;
}

void DirectoryForwardingTableEntry::set_address(unsigned int address) {
	address_ = address;
}

long DirectoryForwardingTableEntry::get_seqnum() const {
	return seqnum_;
}

void DirectoryForwardingTableEntry::set_seqnum(unsigned int seqnum) {
	seqnum_ = seqnum;
}

const std::string DirectoryForwardingTableEntry::getKey() const{
	return ap_naming_info_.getEncodedString();
}

bool DirectoryForwardingTableEntry::operator==(const DirectoryForwardingTableEntry &object) {
	if (object.get_ap_naming_info() != ap_naming_info_) {
		return false;
	}

	if (object.get_address() != address_) {
		return false;
	}
	return true;
}

std::string DirectoryForwardingTableEntry::toString() {
    std::stringstream ss;
    ss << this->get_ap_naming_info().toString() << std::endl;
	ss << "IPC Process address: " << address_ << std::endl;
	ss << "Sequence number: " << seqnum_ << std::endl;

	return ss.str();
}

//	CLASS WhatevercastName
bool WhatevercastName::operator==(const WhatevercastName &other) {
	if (name_ == other.name_) {
		return true;
	}
	return false;
}

std::string WhatevercastName::toString() {
	std::string result = "Name: " + name_ + "\n";
	result = result + "Rule: " + rule_;
	return result;
}

}
