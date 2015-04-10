/*
*	eap_erp_pkt.h
*
*	header for EAP/ERP packet
*	
*	Author: Yi Ding (Aaron)
*
*	date: Sep. 2009
*
*	University of Helsinki
*/

#ifndef	__eap_erp_pkt_h__
#define __eap_erp_pkt_h__

#include "packet.h"

#define HDR_EAP_ERP_PKT(p) hdr_eap_erp_pkt::access(p)

struct hdr_eap_erp_pkt
{
//    nsaddr_t pkt_src_;

/*
    code 1: EAP Request
    code 2: EAP Response
    code 3: Success
    code 4: Failure
    code 5: ERP-Init
    code 6: ERP-Finish
*/
    int Code;

/*
    type 1: ID
    type 2: AKA challenge
    type 3: ERP-start
    type 4: ERP-reauthentication
*/
    int Type;

    int Identifier;
    int ERP_Result;
//    int ERP_Bootstrap;

//    inline nsaddr_t& pkt_src() {return pkt_src_;}
    inline int& pkt_code() {return Code;}
    inline int& pkt_identifier() {return Identifier;}
    inline int& pkt_type() {return Type;}
    inline int& pkt_erp_result() {return ERP_Result;}
//    inline int& pkt_erp_bootstrap() {return ERP_Bootstrap;}

    static int offset_;

    inline static int& offset() 
    { return offset_; }

    inline static hdr_eap_erp_pkt* access(const Packet* p)
    {
	return (hdr_eap_erp_pkt*) p->access(offset_);
    }
};

#endif
