#include "serd.h"
#include "parseRDF.h"

using namespace std;

Edge::Edge(string from_label, string to_label, string edge_label):
	from_label(from_label), to_label(to_label), edge_label(edge_label) {}

struct Graph {
	vector <Edge> *v;
};

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
	(void)flags;
	(void)object_datatype;
	(void)object_lang;
	
	Graph *g = (Graph*)handle;
	g->v->push_back(Edge(
		string((char*)subject->buf),
		string((char*)object->buf),
		string((char*)predicate->buf)));

	return SERD_SUCCESS;
}

vector <Edge> parseRDF(const char* path) {
	Graph *g = new Graph();
	vector <Edge> edges;
	g->v = &edges;
	SerdReader *reader = serd_reader_new(
		SERD_NTRIPLES, g, free,
		NULL, NULL, handle_triple, NULL);

	serd_reader_read_file(reader, (const uint8_t*)(path));

	serd_reader_free(reader);
	
	return edges;
}
