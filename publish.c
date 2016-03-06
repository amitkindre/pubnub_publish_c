#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <json.h>

#include "pubnub.h"
#include "pubnub-sync.h"


int  publish_data(char *msg_string);

int main(int argc, char* argv[])
{
	char str[100];
	if(argc == 2)
		publish_data(argv[1]);
	else
	{
		gets(str);
		publish_data(str);
	}
	return 0;
}


int  publish_data(char *msg_string)
{
	struct pubnub_sync *sync = pubnub_sync_init();

	struct pubnub *p = pubnub_init(
			/* publish_key */ "demo",
			/* subscribe_key */ "demo",
			/* pubnub_callbacks */ &pubnub_sync_callbacks,
			/* pubnub_callbacks data */ sync);
	
	json_object *msg;
	msg = json_object_new_object();
	json_object_object_add(msg, "num", json_object_new_int(42));
	json_object_object_add(msg, "str", json_object_new_string(msg_string));
	/* msg = { "num": 42, "str": "\"Hello, world!\" she said." } */

	pubnub_publish(
			/* struct pubnub */ p,
			/* channel */ "my_channel",
			/* message */ msg,
			/* default timeout */ 20,
			/* callback; sync needs NULL! */ NULL,
			/* callback data */ NULL);
			
	json_object_put(msg);


	if (pubnub_sync_last_result(sync) != PNR_OK)
		return EXIT_FAILURE;

	msg = pubnub_sync_last_response(sync);
	printf("pubnub publish ok: %s\n", json_object_get_string(msg));
	json_object_put(msg);


	pubnub_done(p);
	return EXIT_SUCCESS;

}
