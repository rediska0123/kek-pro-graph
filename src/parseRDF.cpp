#include <stdio.h>
#include "parseRDF.h"
#include <cstdlib>

typedef struct {
	int a;
} Graph;

SerdStatus handle_triple(
	void*              handle,
	SerdStatementFlags flags,
	const SerdNode*    graph,
	const SerdNode*    subject,
	const SerdNode*    predicate,
	const SerdNode*    object,
	const SerdNode*    object_datatype,
	const SerdNode*    object_lang)
{
	(void)handle;
	(void)flags;
	(void)object_datatype;
	(void)object_lang;

	printf("	%s %s %s\n", subject->buf, object->buf, predicate->buf);
	return SERD_SUCCESS;
}

// TODO: write comments.
void parseRDF(const char* path) {
	Graph *rt = (Graph*)calloc(1, sizeof(Graph));
	SerdReader *reader = serd_reader_new(
		SERD_NTRIPLES, rt, free,
		NULL, NULL, handle_triple, NULL);

	serd_reader_read_file(reader, (const uint8_t*)(path));

	serd_reader_free(reader);
}
