#include "json.h"
#include <algorithm>

using namespace std;

namespace Json {

string Vector_ToString(const Node &node);
string Map_ToString(const Node &node);
string Double_ToString(const Node &node);
string String_ToString(const Node &node);
string Bool_ToString(const Node &node);
//===============================================

//===============================================
string Node::ToString() const
{
	switch (GetType())
	{
	case Vector: return Vector_ToString(*this); break;
	case Map: return Map_ToString(*this); break;
	case Double: return Double_ToString(*this); break;
	case String: return String_ToString(*this); break;
	case Bool: return Bool_ToString(*this); break;
	default:		return ""; break;
	}
}
//===============================================
Document::Document(Node root) : root(move(root)) 
{}
//-----------------------------------------------
const Node& Document::GetRoot() const 
{
	return root;
}
//-----------------------------------------------
Node LoadNode(istream& input);

Node LoadArray(istream& input) 
{
	vector<Node> result;

	for (char c; input >> c && c != ']'; )
	{
		if (c != ',') 
			input.putback(c);
		result.push_back(LoadNode(input));
	}
	return Node(move(result));
}
//-----------------------------------------------
Node LoadNumber(istream& input) 
{
	string result;
	char ch = '\0';
	while (isdigit(input.peek()) || input.peek() == '.') 
	{
		ch = input.get();
		result.push_back(ch);
	}
	return Node(stod(result));
}
//-----------------------------------------------
Node LoadString(istream& input)
{
	string line;
	getline(input, line, '"');
	return Node(move(line));
}
//-----------------------------------------------
Node LoadDict(istream& input)
{
	map<string, Node> result;

	for (char c; input >> c && c != '}'; ) 
	{
		if (c == ',')
			input >> c;

		string key = LoadString(input).AsString();
		input >> c; // ':'
		result.emplace(move(key), LoadNode(input));
	}
	return Node(move(result));
}
//-----------------------------------------------
Node LoadBool(istream& input)
{
	static const string s_true = "true";
	bool result = false;
	string buff;
	getline(input, buff, '\n');
	auto it = search(buff.begin(), buff.end(), s_true.begin(), s_true.end());
	if (it != buff.end())
		result = true;
	return Node(result);
}
//-----------------------------------------------
Node LoadNode(istream& input) 
{
	char c;
	input >> c;

	if (c == '[')
		return LoadArray(input);
	else if (c == '{')
		return LoadDict(input);
	else if (c == '"')
		return LoadString(input);
	else if (c == 't' || c == 'f') // true/false
	{
		input.putback(c);
		return LoadBool(input);
	}
	else 
	{
		input.putback(c);
		return LoadNumber(input);
	}
}
//-----------------------------------------------
Document Load(istream& input)
{
	return Document{ LoadNode(input) };
}

} // namespace Json

//-----------------------------------------------
ostream& operator<<(ostream & out, const Json::Document & doc)
{
	out << doc.GetRoot().ToString();
	return out;
}