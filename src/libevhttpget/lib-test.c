#include <evhttpget.h>
#include <parseurl.h>

#include <assert.h>
#include <event.h>
#include <stdio.h>
#include <stdlib.h>


struct event_base *main_event_base = NULL;


// this function is called when we have data from the webserver.   We can specify the receive chunks of the file, or all at once.
static download(int result, char *data, int length, web_request_t *web, void *arg)
{

	assert(web != NULL);
	if (result == 0) {
		assert(data == NULL);
		assert(length == 0);
	}
	else {
		assert(result > 0);
		assert(data != NULL);
		assert(length > 0);
	}


	printf("download: result=%d, length=%d\n", result, length);

	
	
}


int main(int argc, char **argv) 
{
	web_request_t *web;
	char *url = "http://rhokz.com/files/file.html";

	main_event_base = event_init();
	assert(main_event_base != NULL);
	
	if (evdns_init() != 0) {
		printf("Failed to initialise network objects.\n");
		exit(EXIT_FAILURE);
	}
	
 	web = web_alloc(main_event_base);
 	web_set_url(web, url);
 	web_set_callback(web, download);
 	web_set_sizes(web, 0, 0);
 	web_set_method(web, GET_FULL);
 	web_download(web);

 	event_base_loop(main_event_base, 0);

	printf("download complete.\n");

	evdns_shutdown(0);

	return(0);
}
