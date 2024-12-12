#pragma		once

#include	"tona3_array.h"
#include	"tona3_string.h"
#include	"tona3_tree.h"

namespace NT3
{

struct Sxml_key
{
	TSTR	key;
	TSTR	value;
};

struct Sxml_tag
{
	TSTR	name;
	ARRAY<Sxml_key>	key;
};

struct Sxml_item
{
	Sxml_tag			tag;
	ARRAY<Sxml_item>	item;
	TSTR				data;
};

struct Sxml_data
{
	Sxml_item	top;
};

class Cxml
{
public:
	bool	load_xml(CTSTR& file_path, Sxml_data* data);
	bool	save_xml(CTSTR& file_path, Sxml_data* data);

private:
	bool	analize_item(TSTR::const_iterator& str_first, TSTR::const_iterator str_last, Sxml_item* item);
	bool	analize_open_tag(TSTR::const_iterator& str_first, TSTR::const_iterator str_last, Sxml_tag* tag);
	bool	analize_close_tag(TSTR::const_iterator& str_first, TSTR::const_iterator str_last, TSTR& tag_name);
	bool	analize_data(TSTR::const_iterator& str_first, TSTR::const_iterator str_last, TSTR& data);
	bool	save_item(Sxml_item* item, TSTR& xml_str, int depth);

	TSTR	xml_encode(CTSTR& str);
};


/*
struct Sxml_data
{
	ARRAY<Sxml_node>	node_list;
};

// ****************************************************************
// xml
// ================================================================
class Cxml
{
public:
	bool	load(CTSTR& file_path, Sxml_data* xml);

private:
	bool	skip_separator(TSTR::const_iterator& itr, TSTR::const_iterator end);
};

// ****************************************************************
// xml writer
// ================================================================
class Cxml_writer
{
public:
	void	open_tag(TSTR& text, CTSTR& tag_name);					// <tag>
	void	set_data(TSTR& text, CTSTR& tag_name, CTSTR& data);		//		<tag>aaa</tag>
	void	close_tag(TSTR& text);									// </tag>

private:
	ARRAY<TSTR>	tag_list;

	TSTR	convert_to_xml_str(CTSTR& str);
};
*/

}
