#include "ParseRDFTest.h"
#include "parseRDF.h"
#include <algorithm>

using namespace std;

#define COMPARE_BY(edge1, edge2, field)    \
	if (edge1.field != edge2.field)        \
		return edge1.field < edge2.field

bool compare(const Edge &e1, const Edge &e2) {
	COMPARE_BY(e1, e2, from_label);
	COMPARE_BY(e1, e2, to_label);
	COMPARE_BY(e1, e2, edge_label);
	return false;
}

bool operator ==(vector <Edge> v1, vector <Edge> v2) {
	if (v1.size() != v2.size())
		return false;
	sort(v1.begin(), v1.end(), compare);
	sort(v2.begin(), v2.end(), compare);
	
	for (int i = 0; i < (int)v1.size(); i++)
		if (v1[i].from_label != v2[i].from_label ||
			v1[i].to_label != v2[i].to_label ||
			v1[i].edge_label != v2[i].edge_label)
			return false;
	return true;
}

void ParseRDFTest::test_empty_file() {
	vector <Edge> got = parseRDF("test/files/test_empty.nt");
	vector <Edge> expected = {};
	DO_CHECK(got == expected);
}

void ParseRDFTest::test_single_edge() {
	vector <Edge> got = parseRDF("test/files/test_single_edge.nt");
	vector <Edge> expected = {Edge(
		"http://example.org/boka",
		"memos,kekos,abrikos",
		"http://example.org/joka")};
	DO_CHECK(got == expected);
}

void ParseRDFTest::test_general() {
	vector <Edge> got = parseRDF("test/files/test_general.nt");
	string beg = "http://";
	auto gen = [beg](string suf) {
		return Edge(beg + "a" + suf, beg + "c" + suf, beg + "b" + suf); };
	vector <Edge> expected = {
		gen("1"), gen("2"), gen("3"), gen("4"), gen("5")};
	DO_CHECK(got == expected);
}

void ParseRDFTest::run_all_tests() {
	printf("ParseRDF testing...\n");
	test_empty_file();
	test_single_edge();
	test_general();
}

