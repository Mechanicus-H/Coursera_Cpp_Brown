#pragma once

#include <istream>
#include <map>
#include <string>
#include <variant>
#include <vector>
#include <iostream>

namespace Json {

	

class Node : std::variant<std::vector<Node>, std::map<std::string, Node>, double, std::string, bool>
{
public:
	enum Type
	{ Invalid, Vector, Map, Double, String, Bool };
private:
	static int tab_for_print;
public:
	using variant::variant;
	Type GetType() const { return static_cast<Type>( index() ); }
	const auto& AsArray() const	{ return std::get<std::vector<Node>>(*this); }
	const auto& AsMap() const {	return std::get<std::map<std::string, Node>>(*this); }
	double AsDouble() const { return std::get<double>(*this); }
	const auto& AsString() const { return std::get<std::string>(*this);	}
	bool AsBool() const { return std::get<bool>(*this); }
	std::string ToString() const;
};

class Document 
{
private:
	Node root;
public:
	Document() {}
	explicit Document(Node root);
	const Node& GetRoot() const;
	void SetRoot(Node new_root) { root = new_root; }
};

int Node::tab_for_print = 0;;

Document Load(std::istream& input);

} // namespace Json

std::ostream& operator << (std::ostream & out, const Json::Document & doc);