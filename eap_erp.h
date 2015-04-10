/*
*	eap_erp.h
*
*	function prototypes for EAP/ERP protocol
*
*	Author: Yi Ding (Aaron)
*
*	date: Sep. 2009
*
*	University of Helsinki
*/

#ifndef __eap_erp_h__
#define __eap_erp_h__


#include "eap_erp_pkt.h"
#include "agent.h"
#include "packet.h"
#include "trace.h"
#include "random.h"
#include "classifier-port.h"
#include "tclcl.h"
#include "address.h"
#include "ip.h"


#define CURRENT_TIME Scheduler::instance().clock()
#define JITTER (Random::uniform()*0.5)


class EAP_ERP_Agent;

class EAP_ERP_PktTimer : public TimerHandler
{
    public:
	EAP_ERP_PktTimer(EAP_ERP_Agent* agent) : TimerHandler()
	{
	    agent_ = agent;
	}

    protected:
	EAP_ERP_Agent* agent_;
	virtual void expire(Event* e);
};

class EAP_ERP_Agent : public Agent
{
    friend class EAP_ERP_PktTimer;

    protected:
	void send_eap_erp_pkt(int);
	void recv_eap_erp_pkt(Packet*);

    public:
	EAP_ERP_Agent();
	int command(int argc, const char*const* argv);
	void recv(Packet*, Handler*);

    private:
	double delay_;
	double jitter_;
};

#endif
