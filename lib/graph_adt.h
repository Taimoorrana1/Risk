#ifndef GRAPHADT_H
#define GRAPHADT_H

#include <string>
#include <unordered_map>
#include <set>
#include <iostream>

#include <cereal/cereal.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/set.hpp>
#include <cereal/archives/xml.hpp>
/**
 * @brief The Edge struct
 */
struct Edge{
	std::string edgename;
	std::string element;
	int weight;

	template <class Archive>
	void serialize(Archive &ar){
		ar(cereal::make_nvp("edgeName", this->edgename), cereal::make_nvp("elementName", this->element), cereal::make_nvp("edgeWeight", this->weight));
	}
};


typedef std::unordered_map<std::string, Edge> node_hashmap;
typedef std::unordered_map<std::string, node_hashmap> graph_hashmap;
typedef std::set<std::string> string_set;
typedef std::unordered_map< std::string, std::set< std::string> > subgraph;


void printSet(const std::set<std::string>&);
void printEdge(const Edge&);
/**
 * @brief The GraphADT class which implements all the standard features of a graph verifying connectivity etc..
 *
 */

class GraphADT {
private:
	int number_of_vertices;
	int number_of_edges;
	graph_hashmap thegraph;
	string_set set_of_vertices;
	string_set set_of_edges;
public:
	GraphADT(): number_of_vertices(0), number_of_edges(0) {} //using initialization list
	GraphADT(GraphADT const&);
	GraphADT copyUtil(const GraphADT&);
	string_set nodes() const;
	string_set edges() const;
	/**
	 * @brief countAllNodes returns all the nodes
	 * @return
	 */
	int countAllNodes() const;
	/**
	 * @brief countAllEdges returns all the edges
	 * @return
	 */
	int countAllEdges() const;
	Edge getEdge(std::string, std::string) const;
	string_set incidentEdges(std::string v) const;
	std::string opposite(std::string, std::string) const;
	string_set endNodes(std::string) const;
	bool areAdjacent(std::string, std::string) const;
	bool insertNode(std::string);
	bool renameNode(std::string, std::string);
	bool removeNode(std::string);
	void insertEdge(std::string, std::string, std::string);
	void removeEdge(std::string, std::string);
	std::string getEdgeElem (std::string) const;
	void replaceEdgeElem(std::string, std::string);
	bool isEqual(const GraphADT&) const;
	bool emptyGraph();
	GraphADT graph_intersection(const GraphADT&) const;
	GraphADT graph_difference(const GraphADT&) const;
	GraphADT graph_union(const GraphADT&) const;
	GraphADT graph_complement() const;
	void printGraph() const;
	void insertEdge(std::string, std::string);
	GraphADT copyGraph() const;

	template<class Archive>
	void serialize(Archive &ar){
		ar(cereal::make_nvp("graphADT",this->thegraph), cereal::make_nvp("setOfEdges",this->set_of_edges), cereal::make_nvp("countries",this->set_of_vertices), cereal::make_nvp("numberOfCountries",this->number_of_vertices), cereal::make_nvp("NumberOfLinks",this->number_of_edges));
	}

};
/**
 * @brief The DirectedGraphADT class implements the directed map extends GraphADT
 */
class DirectedGraphADT : GraphADT {

public:
	bool areAdjacent(std::string, std::string) const;
};

/**
 * @brief The SubGraphADT class
 * This subclasses the GraphADT class in order to obtain subgraphs from the graph.
 * The subgraphs are stored as sets of std strings.
 */
class SubGraphADT : public GraphADT {
	subgraph allSubgraphs;
	std::unordered_map <std::string, std::string> countries_continents;

public:
	/**
	 * @brief insertNode inserts a node
	 * @return
	 */
	bool insertNode(std::string, std::string);
	string_set subgraphContents(const std::string&);
	std::string getSubgraphName(const std::string&);
	bool renameNode(std::string, std::string);
	bool removeNode(std::string vertexname);

	template<class Archive>
	void serialize(Archive &ar){
		ar(cereal::base_class<GraphADT>(this), cereal::make_nvp("subgraphs",this->allSubgraphs), cereal::make_nvp("countriesContinents", this->countries_continents));
	}
};

#endif // GRAPHADT_H
