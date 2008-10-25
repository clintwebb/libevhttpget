/*

	libevhttpget
	(c) Copyright Hyper-Active Systems, Australia

	Contact:
		Clinton Webb
		webb.clint@gmail.com

*/



#include "evhttpget.h"

#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>


#include <event.h>
#include <evdns.h>
#include <parseurl.h>

#ifndef INVALID_HANDLE
#define INVALID_HANDLE	-1
#endif


#define MAX_HEADERS    4096


web_request_t * web_alloc(struct event_base *base)
{
	web_request_t *web;

	assert(base != NULL);

	web = (web_request_t *) malloc(sizeof(web_request_t));
	if (web != NULL) {
		web->handle = INVALID_HANDLE;
		memset(&web->event, 0, sizeof(struct event));
		web->base = base;
		web->url = NULL;
		web->method = GET_FULL;
		web->state = 0;
		web->start = 0;
		web->size = 0;
		web->callback = NULL;
		web->arg = NULL;
		expbuf_init(web->in);
		expbuf_init(web->out);
		web->host = NULL;
		web->port= 80;
	}
	
	return(web);
}




void web_set_url(web_request_t *web, char *url)
{
	assert(web != NULL);
	assert(url != NULL);

	assert(web->url == NULL);
	web->url = url;
}

void web_set_callback(web_request_t *web, void *func)
{
	assert(web != NULL);
	assert(func != NULL);
	assert(web->callback == NULL);
	web->callback = func;
}

void web_set_arg(web_request_t *web, void *arg)
{
	assert(web != NULL);
	assert(arg != NULL);
	assert(web->arg == NULL);
	web->arg = arg;
}

void web_set_sizes(web_request_t *web, int start, int size)
{
	assert(web != NULL);
	assert(start >= 0);
	assert(size >= 0);
	assert(web->start == 0);
	assert(web->size == 0);
	web->start = start;
	web->size = size;
}

void web_set_method(web_request_t *web, int method)
{
	assert(web != NULL);
	assert(method == GET_FULL || method == GET_HEAD || method == GET_PART);
	assert(web->method == GET_FULL);
	web->method = method;
}


static int http_connect(web_request_t *web) {


	

// 	// connect to the remote host address/port
//        
// 	// Create the socket
// 	assert(web->handle == INVALID_HANDLE);
// 	
// 	web->handle = socket(AF_INET, SOCK_STREAM, 0);
// 	if (web->handle >= 0) {
// 
// 		printf("dns_callback: created socket handle.  handle=%d\n", web->handle);
// 	
// 		// CJW: Connect to the server
// 
// 		// socket is not yet non-blocking... meaning that it will block until the connection is made.
// 		// this is not how we want it, but for initial testing it is acceptable.   Basically, we will
// 		// want the socket in non-blocking mode before the connect is made, and then we wait for the
// 		// connect (write) event on the socket handle.
// 		
// 		if (connect(web->handle, (struct sockaddr*)&in, sizeof(struct sockaddr)) >= 0) {
// 
// 			printf("dns_callback: connect successful.  handle=%d\n", web->handle);
// 		
// 			// set the socket in non-blocking mode.
// 			assert(web->handle > 0);
// 			sockopts = fcntl(web->handle, F_GETFL);
// 			if (sockopts >= 0) {
// 				sockopts = (sockopts | O_NONBLOCK);
// 				fcntl(web->handle, F_SETFL, sockopts);
// 
// 				printf("dns_callback: Set socket handle to non-blocking.  handle=%d\n", web->handle);
// 			}
// 		}
// 		else {
// 			printf("dns_callback: connect failed.  handle=%d\n", web->handle);
// 			close(web->handle);
// 			web->handle = 0;
// 		}
// 	}

	return(0);
	
}



// this function is called once the results of the DNS lookup have been obtained.
static void dns_callback(int result, char type, int count, int ttl, void *addresses, void *arg)
{
	char headers[MAX_HEADERS];
//  struct sockaddr_in *pin;
  web_request_t *web;
	int sockopts;
	int i;
	uint32_t *packed;

	assert(result == DNS_ERR_NONE);
	assert(type == DNS_IPv4_A);
	assert(addresses != NULL);
	assert(arg != NULL);

	web = (web_request_t *) arg;

	printf("dns_callback: result=%d, count=%d, ttl=%d\n", result, count, ttl);

	// address needs to be cast according to type... but what is the type in this case?
	for (i = 0; i < count; ++i) {
			int oct[4];
			packed = (uint32_t*)addresses;
			oct[0] = (packed[i] & 0x000000ff);
			oct[1] = (packed[i] & 0x0000ff00)>>8;
			oct[2] = (packed[i] & 0x00ff0000)>>16;
			oct[3] = (packed[i] & 0xff000000)>>24;
			printf("\t%i.%i.%i.%i\n", oct[0], oct[1], oct[2], oct[3]);
	}


	// should we only handle one IP, or should we cater for all of them?

	
	// build the HTTP headers
	// add headers to outgoing buffer.
	// initialise incoming buffer.
	// setup events to listen for data, and to write out what is in the buffer.

//	assert(0);

}


static void web_resolve(web_request_t *web)
{
	assert(web != NULL);


    unsigned long ulAddress;
    struct hostent *hp;

    assert(szAddr != NULL && szAddr[0] != '\0' && iPort > 0);
    assert(pSin != NULL);

    // First, assign the family and port.
    pSin->sin_family = AF_INET;
    pSin->sin_port = htons(iPort);

    // Look up by standard notation (xxx.xxx.xxx.xxx) first.
    ulAddress = inet_addr(szAddr);
    if ( ulAddress != (unsigned long)(-1) )  {
        // Success. Assign, and we're done.  Since it was an actual IP address, then we dont doany DNS lookup for that, so we cant do any checking for any other address type (such as MX).
        pSin->sin_addr.s_addr = ulAddress;
        return 0;
    }

	
}


int web_download(web_request_t *web)
{
	char *host;
	int status = 0;

	assert(web != NULL);
	
	assert(web->url != NULL);
	assert(web->method == GET_HEAD || web->method == GET_FULL || web->method == GET_PART);
	assert(web->base != NULL);
	assert(web->callback != NULL);
	assert((web->method == GET_HEAD && web->start == 0 && web->size == 0) || (web->method == GET_FULL && web->start == 0 && web->size >= 0) || (web->method == GET_PART && web->start >= 0 && web->size > 0));

	host = url_gethost(web->url);
	if (host != NULL) {

		// first we attempt to resolve if it is an IP address.
		// if that fails, then we need to do a DNS lookup.
		// if that fails, then we exit.
		// connect to the IP and continue the download.

	
	// now that we have a host, we need to start the DNS lookup.
 		evdns_resolv_conf_parse(DNS_OPTIONS_ALL, "/etc/resolv.conf");
 		evdns_resolve_ipv4((const char *) host, 0, dns_callback, web);

 		web->state = 1;
	}
	else {
		status = 1;
	}

	return(status);
}




void web_clear(web_request_t *web)
{
	assert(web != NULL);
	assert(web->handle == INVALID_HANDLE);
}



