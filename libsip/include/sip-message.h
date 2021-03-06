#ifndef _sip_message_h_
#define _sip_message_h_

#include "sip-header.h"
#include "sip-dialog.h"
#include "http-parser.h"
#include <stdint.h>

// 8.1.1.7 Via (p39)
#define SIP_BRANCH_PREFIX		"z9hG4bK"
#define SIP_MAX_FORWARDS		70

#define SIP_METHOD_INVITE		"INVITE"
#define SIP_METHOD_CANCEL		"CANCEL"
#define SIP_METHOD_BYE			"BYE"
#define SIP_METHOD_ACK			"ACK"
#define SIP_METHOD_OPTIONS		"OPTIIONS"
#define SIP_METHOD_REGISTER		"REGISTER"

#define SIP_HEADER_FROM			"From"
#define SIP_HEADER_TO			"To"
#define SIP_HEADER_CALLID		"Call-ID"
#define SIP_HEADER_CSEQ			"CSeq"
#define SIP_HEADER_MAX_FORWARDS	"Max-Forwards"
#define SIP_HEADER_VIA			"Via"
#define SIP_HEADER_CONTACT		"Contact"
#define SIP_HEADER_ROUTE		"Route"
#define SIP_HEADER_RECORD_ROUTE "Record-Route"

#define SIP_HEADER_ABBR_FROM				'f'
#define SIP_HEADER_ABBR_TO					't'
#define SIP_HEADER_ABBR_CALLID				'i'
#define SIP_HEADER_ABBR_VIA					'v'
#define SIP_HEADER_ABBR_CONTACT				'm'
#define SIP_HEADER_ABBR_SUPPORTED			'k'
#define SIP_HEADER_ABBR_SUBJECT				's'
#define SIP_HEADER_ABBR_CONTENT_TYPE		'c'
#define SIP_HEADER_ABBR_CONTENT_LENGTH		'l'
#define SIP_HEADER_ABBR_CONTENT_ENCODING	'e'

struct sip_message_t
{
	// request line
	union
	{
		struct sip_requestline_t c;
		struct sip_statusline_t s;
	} u;
	
	// 6-headers
	struct sip_contact_t to;
	struct sip_contact_t from;
	struct sip_vias_t vias;
	struct cstring_t callid;
	struct sip_cseq_t cseq;
	int maxforwards;

	// contacts/routers
	struct sip_contacts_t contacts;
	struct sip_uris_t routers;
	struct sip_uris_t record_routers; // Record-Route

	// other headers
	struct sip_params_t headers;

	const void *payload;
	int size; // payload size in byte

	// internal use only
	struct
	{
		char* ptr;
		char* end;
	} ptr;
};

struct sip_message_t* sip_message_create();
int sip_message_destroy(struct sip_message_t* msg);
int sip_message_init(struct sip_message_t* msg, const char* method, const char* from, const char* to);
int sip_message_init2(struct sip_message_t* msg, const char* method, const struct sip_dialog_t* dialog);

int sip_message_load(struct sip_message_t* msg, const struct http_parser_t* parser);
int sip_message_write(const struct sip_message_t* msg, uint8_t* data, int bytes);

/// @return 1-invite, 0-noninvite
int sip_message_isinvite(const struct sip_message_t* msg);

#endif /* !_sip_message_h_ */
