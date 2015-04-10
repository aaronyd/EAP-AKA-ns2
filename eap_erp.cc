/*
*	eap_erp.cc
*
*	EAP/ERP protocol implementation
*
*	Author: Yi Ding (Aaron)
*
*	date: Sep. 2009
*
*	University of Helsinki
*/


#if 1

#include "eap_erp.h"

int hdr_eap_erp_pkt::offset_;

static class EapErpHeaderClass : public PacketHeaderClass
{
    public:

	EapErpHeaderClass() : PacketHeaderClass("PacketHeader/EAP-ERP", sizeof (hdr_eap_erp_pkt))
	{
	    bind_offset(&hdr_eap_erp_pkt::offset_);
	}

} class_eaperp_hdr;

static class EapErpClass : public TclClass
{
    public:

	EapErpClass() : TclClass("Agent/EAP-ERP") {}

	TclObject* create (int argc, const char* const* argv)
	{
	    return (new EAP_ERP_Agent());
	}
}class_eaperp_agent;

/*
void EAP_ERP_PktTimer::expire(Event* e)
{
    agent_->send_eap_erp_pkt();
}
*/

EAP_ERP_Agent::EAP_ERP_Agent() : Agent(PT_EAP_ERP)
{
    bind("packetSize_",&size_);
    bind("cryptDelay_",&delay_);
    bind("cryptJitter_",&jitter_);
}

int EAP_ERP_Agent::command(int argc, const char*const* argv)
{
    int i;

    if (argc==2)
    {
    	if (strcmp(argv[1],"start")==0)
	    {
	        i=0;
	        send_eap_erp_pkt(i);
	        return (TCL_OK);
	    }

	    if (strcmp(argv[1],"start-ERP")==0)
	    {
	        i=1;
	        send_eap_erp_pkt(i);
	        return (TCL_OK);
	    }
    }

    return (Agent::command(argc, argv));
}

void EAP_ERP_Agent::recv(Packet* p, Handler* h)
{
    struct hdr_cmn* chdr = HDR_CMN(p);
//    struct hdr_ip* iphdr = HDR_IP(p);

    if (chdr->ptype()==PT_EAP_ERP)
    	recv_eap_erp_pkt(p);

    return;
}

void EAP_ERP_Agent::send_eap_erp_pkt(int i)
{

//  for EAP-AKA

    if(i==0)
    {
        Packet* p = allocpkt();
        struct hdr_cmn* chdr = HDR_CMN(p);

        struct hdr_eap_erp_pkt* ehdr = HDR_EAP_ERP_PKT(p);

        ehdr->Code = 1;
        ehdr->Identifier = 1;
        ehdr->Type = 1;
        ehdr->ERP_Result = 0;

        chdr->ptype() = PT_EAP_ERP;
	    Scheduler::instance().schedule(target_, p, 0.0);
        return;
    }


//  for ERP

    if(i==1)
    {
        Packet* p = allocpkt();
        struct hdr_cmn* chdr = HDR_CMN(p);

        struct hdr_eap_erp_pkt* ehdr = HDR_EAP_ERP_PKT(p);

        ehdr->Code = 5;
        ehdr->Identifier = 1;
        ehdr->Type = 3;
        ehdr->ERP_Result = 0;

        chdr->ptype() = PT_EAP_ERP;
    	Scheduler::instance().schedule(target_, p, 0.0);
        return;
    }
}

void EAP_ERP_Agent::recv_eap_erp_pkt(Packet* p)
{

    struct hdr_eap_erp_pkt* ehdr = HDR_EAP_ERP_PKT(p);

//  for EAP Request

    if(ehdr->Code==1)
    {
    	if(ehdr->Type==1)
	    {
	        Packet::free(p);
	        Packet* pkt = allocpkt();
            struct hdr_cmn* chdr = HDR_CMN(pkt);
	        struct hdr_eap_erp_pkt* ehdr2 = HDR_EAP_ERP_PKT(pkt);
	        ehdr2->Code = 2;
	        ehdr2->Identifier = 1;
	        ehdr2->Type = 1;
	        ehdr2->ERP_Result = 0;

            chdr->ptype() = PT_EAP_ERP;

    	    Scheduler::instance().schedule(target_, pkt, 0.0);
    	    return;
    	}

    	if(ehdr->Type==2)
    	{
    	    Packet::free(p);
	        Packet* pkt = allocpkt();
            struct hdr_cmn* chdr = HDR_CMN(pkt);
            struct hdr_eap_erp_pkt* ehdr2 = HDR_EAP_ERP_PKT(pkt);
    	    ehdr2->Code = 2;
	        ehdr2->Identifier = 1;
	        ehdr2->Type = 2;
	        ehdr2->ERP_Result = 0;

            chdr->ptype() = PT_EAP_ERP;

    	    Scheduler::instance().schedule(target_, pkt, (delay_ + jitter_));

	        return;
	    }
    }

//  for EAP response

    if(ehdr->Code==2)
    {
    	if(ehdr->Type==1)
    	{
    	    Packet::free(p);
    	    Packet* pkt = allocpkt();
            struct hdr_cmn* chdr = HDR_CMN(pkt);
    	    struct hdr_eap_erp_pkt* ehdr2 = HDR_EAP_ERP_PKT(pkt);
    	    ehdr2->Code = 1;
    	    ehdr2->Identifier = 1;
    	    ehdr2->Type = 2;
    	    ehdr2->ERP_Result = 0;

            chdr->ptype() = PT_EAP_ERP;

    	    Scheduler::instance().schedule(target_, pkt, (delay_ + jitter_));
	        return;
	    }

	    if(ehdr->Type==2)
	    {
	        Packet::free(p);
	        Packet* pkt = allocpkt();
            struct hdr_cmn* chdr = HDR_CMN(pkt);
	        struct hdr_eap_erp_pkt* ehdr2 = HDR_EAP_ERP_PKT(pkt);
	        ehdr2->Code = 3;
	        ehdr2->Identifier = 1;
	        ehdr2->Type = 1;
	        ehdr2->ERP_Result = 0;

            chdr->ptype() = PT_EAP_ERP;

    	    Scheduler::instance().schedule(target_, pkt, (delay_ + jitter_));
	        return;
	    }
    }

//  for EAP Success

    if(ehdr->Code==3)
    {
    }

//  for ERP

    if(ehdr->Code==5)
    {
	    if(ehdr->Type==3)
	    {
            Packet::free(p);
       	    Packet* pkt = allocpkt();
            struct hdr_cmn* chdr = HDR_CMN(pkt);
            struct hdr_eap_erp_pkt* ehdr2 = HDR_EAP_ERP_PKT(pkt);
            ehdr2->Code = 5;
            ehdr2->Identifier = 1;
            ehdr2->Type = 4;
            ehdr2->ERP_Result = 0;

            chdr->ptype() = PT_EAP_ERP;

       	    Scheduler::instance().schedule(target_, pkt, (delay_ + jitter_));
            return;
        }

        if(ehdr->Type==4)
        {
            Packet::free(p);
   	        Packet* pkt = allocpkt();
            struct hdr_cmn* chdr = HDR_CMN(pkt);
   	        struct hdr_eap_erp_pkt* ehdr2 = HDR_EAP_ERP_PKT(pkt);
            ehdr2->Code = 6;
            ehdr2->Identifier = 1;
            ehdr2->Type = 4;
            ehdr2->ERP_Result = 1;

            chdr->ptype() = PT_EAP_ERP;
                  
   	        Scheduler::instance().schedule(target_, pkt, (delay_ + jitter_));
            return;
        }
    }

//  for ERP reply

    if(ehdr->Code==6)
    {
    }
}

#endif
