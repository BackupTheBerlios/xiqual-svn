#include "stdarg.h"
#include "stdlib.h"

#include "xinet.h"


void net_handle(SelectServer *server, Node *node)
{
	if(server->onread) server->onread(server, node);
}
