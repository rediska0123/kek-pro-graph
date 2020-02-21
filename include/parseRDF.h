#ifndef PARSE_RDF_H
#define PARSE_RDF_H

#include <string>
#include <vector>

struct Edge {
	Edge(std::string from_label, std::string to_label, std::string edge_label);
	
	std::string from_label;
	std::string to_label;
	std::string edge_label;
};

// parseRDF parses the NTriples RDF format file to list of edges.
std::vector <Edge> parseRDF(const char *filename);

#endif
