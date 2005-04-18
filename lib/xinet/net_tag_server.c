#include "stdarg.h"
#include "stdlib.h"

#include "xinet.h"


static CmdNode *net_newfunc(char *name, void *func);

void net_tag_serverA(SelectServer *server, tagitem *tags)
{
	tagitem	*ti;
	struct linger li;
	struct cmdarray *cmd;
	struct CmdNode *func;

	ti = tag_next(&tags);
	while(ti) {
		switch(ti->tag) {
		case XN_NUMCONNECTIONS:
			net_addnodes(server, ti->data);
			break;
		/* Not specifying timeout makes net_process() poll and return immediately */
		case XN_TIMEOUT_S:
			server->timeout_s = ti->data;
			break;
		case XN_TIMEOUT_MS:
			server->timeout_ms = ti->data;
			break;
		case XN_PORT:
			server->address.sin_port = _SwapBE16(ti->data);
			break;
		case XN_QUEUE:
			server->queue = ti->data;
			break;
		case XN_LINGER:
			li.l_onoff = 1;
			li.l_linger = ti->data;
			setsockopt(server->sock, SOL_SOCKET, SO_LINGER,
				(char *)&li, sizeof(li));
			break;
		/* Called right after making a successful new connection.
		 * Put your welcome messages here :)
		 */
		case XN_CONNECT_FUNC:
			server->onconnect = (void *)ti->data;
			break;
		/* Breaking away is hard to do - this is where you do it */
		case XN_CLOSE_FUNC:
			server->onclose = (void *)ti->data;
			break;
		/* This function is what reads network packages.
		 * I'd say it's necessary (for now).
		 */
		case XN_READ_FUNC:
			server->onread = (void *)ti->data;
			break;
		case XN_DISCONNECT_FUNC:
			server->ondisconnect = (void *)ti->data;
			break;
		case XN_SERVERFULL_FUNC:
			server->onserverfull = (void *)ti->data;
			break;
		case XN_COMMANDS:
			cmd = (cmdarray *)ti->data;
			if(!cmd) break;
			while(cmd->text) {
				func = net_newfunc(cmd->text, cmd->command);
				list_add(&server->commands, (Node *)func);
				(cmd)++;
			}
			break;
		case XN_MODE:
			switch(ti->data) {
			/* Accept only text commands if this is specified */
			case NET_MODE_TEXT:
				server->mode = NET_MODE_TEXT;
				break;
			/* Default is 0 (NET_MODE_LONG) */
			}
			break;
		}
		ti = tag_next(&tags);
	}
}

static CmdNode *net_newfunc(char *name, void *func)
{
	CmdNode	*n;

	n = calloc(1, sizeof(CmdNode));
	if(n) {
		if(name) n->name = strdup(name);
		if(!n->name) {
			free(n);
			return NULL;
		}
		n->func = func;
	}
	return n;
}

void net_tag_server(SelectServer *server, ulong tag1, ... )
{
	XI_VOIDARG(net_tag_serverA(server, (tagitem *)&tag1))
}
