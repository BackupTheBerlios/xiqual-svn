#include "lists.h"
#include "xistr.h"

int main()
{
	strtoken	*t;
	Node	*n;

	t = str_tokenise("variable = something very long\nin two lines", "= \n");
	if(!t) {
		fprintf(stderr, "Memory allocation failed\n");
		return 2;
	}
	printf("String: \"%s\"\nNumber of tokens: %ld\n",
		t->s, t->tokens.size);
	printf("Original sentence split into words, dropping spaces and =\n");
	printf("The tokens are:\n");
	n = t->tokens.head;
	while(n) {
		printf("%s (strlen=%d)\n", (char *)n->data, strlen(n->data));
		n = n->next;
	}
	str_freetoken(t);
	return 0;
}
