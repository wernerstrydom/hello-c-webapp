#include "mongoose.h"
#include <signal.h>
#include <stdio.h>

static int s_signo;
static void signal_handler(int signo) {
	printf("Signal %d received, exiting...\n", signo);
	s_signo = signo;
}

static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
	if (ev == MG_EV_HTTP_MSG) {
		struct mg_http_message *hm = (struct mg_http_message *) ev_data;
		if (mg_http_match_uri(hm, "/")) {
			mg_http_reply(c, 200, "", "Hello World\n");
		} else {
			mg_http_reply(c, 404, "", "Not Found\n");
		}
	}
}

int main()
{
	signal(SIGINT, signal_handler);
	signal(SIGTERM, signal_handler);

	struct mg_mgr mgr;
	mg_mgr_init(&mgr);
	mg_http_listen(&mgr, "http://0.0.0.0:8000", fn, &mgr);
	while (s_signo == 0)  mg_mgr_poll(&mgr, 1000);
	mg_mgr_free(&mgr);
	return 0;
}
