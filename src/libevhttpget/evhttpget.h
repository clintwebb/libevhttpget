/*

	libevhttpget
	(c) Copyright Hyper-Active Systems, Australia

	Contact:
		Clinton Webb
		webb.clint@gmail.com

	This is a small and simple library used with libevent to download a file from an HTTP server.

	Requires:
		libparseurl - http://hyper-active.com.au/libparseurl
		libevent    - http://
		

	It is released under GPL v2 or later license.  Details are included in the LICENSE file.

*/

#ifndef __EVHTTPGET_H
#define __EVHTTPGET_H

#include <event.h>
#include <expbuf.h>

#define GET_HEAD	1
#define GET_FULL	2
#define GET_PART	3


// The maximum number of IP addresses for a DNS hostname that we will use.
#define WEB_MAX_DNS_ENTRIES    5


typedef struct {

	// socket and event handling
	int handle;
	struct sockaddr_in sin;
	struct event event;
	struct event_base *base;

	// download parameters supplied by the parent
	char *url;
	int method;
	int state;
	int start, size;

	// incoming data buffer.  This will be used to temporarily hold data before passing it on to the callback routine.  outgoing data buffer.  Data placed in here will be transmitted to the remote socket as soon as the WRITE event fires.
	expbuf_t in, out;

	// headers received from the remote webserver.
	char *headers;
	unsigned int headers_len;

	// pointers to callback 
	void *callback;
	void *arg;
	
	// internal data
	char *host;
	int port;
		
	uint32_t 
} web_request_t;


web_request_t * web_alloc(struct event_base *base);

void web_set_url(web_request_t *web, char *url);
void web_set_callback(web_request_t *web, void *func);
void web_set_arg(web_request_t *web, void *arg);
void web_set_sizes(web_request_t *web, int start, int size);
void web_set_method(web_request_t *web, int method);


int web_download(web_request_t *web);
void web_clear(web_request_t *web);


#endif
