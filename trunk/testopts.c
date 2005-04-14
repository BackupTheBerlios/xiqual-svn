#include "xiopts.h"


struct xiopts opt[] = {
	{"h", LOT_FLAG, "Indicates you want help on options"},
	{"help", LOT_FLAG, "Like -h"},
	{"val", LOT_VALUE, "A signed integer is required with this option"},
	{"f", LOT_FLOAT, "A floating point number follows this"},
	{"key", LOT_KEYWORD, "Follow this with a string (surrounded by \" if containing spaces)"},
	{0, 0, 0}
};


int main(int argc, char *argv[])
{
	xiargs *myargs;
	OptNode *o;
	Node *n;

	myargs = xi_getopts(argc, argv, "LameOpts showcase - options don't do much.",
		"[options]... [filename]...", opt);

	if(!myargs) {
		fprintf(stderr, "Please specify options.\n");
		return 2;
	}

	o = (OptNode *)myargs->opts.head;
	while(o) {
		switch(o->type) {
		case LOT_FLAG:
			printf("%s is TRUE\n", o->name);
			break;
		case LOT_KEYWORD:
			printf("%s = %s\n", o->name, o->arg);
			break;
		case LOT_VALUE:
			printf("%s = %ld\n", o->name, o->val);
			break;
		case LOT_FLOAT:
			printf("%s = %f\n", o->name, o->fval);
		}
		o = o->next;
	}
	n = myargs->leftovers.head;
	if(n) printf("Leftover args:\n");
	while(n) {
		printf("%s\n", (char *)n->data);
		n = n->next;
	}
	xi_freeopts(myargs);
	return 0;
}
