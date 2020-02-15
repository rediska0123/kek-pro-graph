#include <stdio.h>
#include "parseRDF.h"
#include <cstdlib>

typedef struct {
	int n;
	const SerdNode* graph;
} Graph;

SerdStatus test_sink(
	void*              handle,
	SerdStatementFlags flags,
	const SerdNode*    graph,
	const SerdNode*    subject,
	const SerdNode*    predicate,
	const SerdNode*    object,
	const SerdNode*    object_datatype,
	const SerdNode*    object_lang)
{
	(void)flags;
	(void)subject;
	(void)predicate;
	(void)object;
	(void)object_datatype;
	(void)object_lang;

	Graph* rt = (Graph*)handle;
	++rt->n;
	rt->graph = object;
	printf("%d	", rt->n);
	printf("	%s %d %d %d %d\n", rt->graph->buf, (int)rt->graph->n_bytes, (int)rt->graph->n_chars, (int)rt->graph->flags, (int)rt->graph->type);
	return SERD_SUCCESS;
}

void parseRDF(const char* path) {
	Graph *rt = (Graph*)calloc(1, sizeof(Graph));
	SerdReader *reader = serd_reader_new(
		SERD_TURTLE, rt, free,
		NULL, NULL, test_sink, NULL);

	SerdNode g = serd_node_from_string(SERD_URI, (const uint8_t*)("me"));
	serd_reader_set_default_graph(reader, &g);

	serd_reader_read_file(reader, (const uint8_t*)(path));
	printf("%s", (const char*)rt->graph->buf);

	serd_reader_free(reader);
}
