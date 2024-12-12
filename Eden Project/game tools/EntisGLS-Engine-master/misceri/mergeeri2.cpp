// mergeeri2.cpp, v1.0 2013/10/13
// coded by asmodean

// contact: 
//   web:   http://asmodean.reverse.net
//   email: asmodean [at] hush.com
//   irc:   asmodean on efnet (irc.efnet.net)

// This tool merges Entis Rasterized Image (*.eri) deltas from XML
// specification.

// Convert to PNG first using erisacvt or whatever.
// Dump description with eri2txt.

#include "as-util.h"
#include "as-xml.h"

as::image_t* get_image(const string& eri_filename) {
  int    offset_x         = 0;
  int    offset_y         = 0;
  string description_file = as::get_file_prefix(eri_filename) + "+description.txt";
  FILE*  fh               = as::open_or_die_file(description_file);

  while (!feof(fh)) {
    if (as::read_line(fh) == "#hot-spot") {
      string              hot_spot = as::read_line(fh);
      as::split_strings_t fields   = as::splitstr(hot_spot, ",");

      offset_x = atol(fields[0].c_str());
      offset_y = atol(fields[1].c_str());
      break;
    }
  }

  fclose(fh);

  offset_x *= -1;
  offset_y *= -1;

  auto part = new as::image_t(as::get_file_prefix(eri_filename) + ".png");
  part->set_offset(offset_x, offset_y);

  // This is actually a slightly lossy operation but premultiplied results are
  // lame anyway, soooo...
  part->unpremultiply();

  return part;
}

int main(int argc, char** argv) {
  if (argc != 3) {
    fprintf(stderr, "mergeeri2 v1.0, coded by asmodean\n\n");
    fprintf(stderr, "usage: %s <bs_define.xml> <bs_layer_files.xml>\n", argv[0]);
    return -1;
  }

  string define_filename      = argv[1];
  string layer_files_filename = argv[2];

  as::xml_t define_xml(define_filename);
  as::xml_t layer_files_xml(layer_files_filename);

  struct layerset_t {
    typedef map<string, uint32_t> file_to_z_t;
    file_to_z_t file_to_z;
  };

  typedef list<layerset_t> layersets_t;
  layersets_t layersets;

  {
    auto layerset_nodes = layer_files_xml.find("layerset");

    for (auto& layerset : layerset_nodes) {
      layerset_t stored_layerset;

      auto layer_nodes = layerset.find("layer");

      for (auto& layer : layer_nodes) {
        string   src = layer.attribute("src");
        uint32_t z   = layer.numeric_attribute("z");

        stored_layerset.file_to_z[src] = z;
      }

      layersets.push_back(stored_layerset);
    }
  }

  as::make_path("merged/");

  auto person_nodes = define_xml.find("meta[@class='人物']");

  for (auto& person : person_nodes) {
    string person_id  = person.attribute("id");
    auto   size_nodes = person.find("meta[@class='サイズ']");

    for (auto& size : size_nodes) {
      string size_id    = size.attribute("id");
      auto   pose_nodes = size.find("meta[@class='ポーズ']");

      for (auto& pose : pose_nodes) {
        string pose_id = pose.attribute("id");

        typedef list<string> layers_t;
        layers_t layers;

        auto default_nodes = pose.find("meta[@default]/meta[@id=../@default]");

        for (auto& default : default_nodes) {
          string src = default.attribute("src");

          if (!src.empty()) {
            layers.push_back(src);
          }
        }

        auto garments_nodes = pose.find("meta[@class='服装']");

        for (auto& garments : garments_nodes) {
          string garments_id = garments.attribute("id");
          auto   hair_nodes  = garments.find("meta[@class='髪型']");

          for (auto& hair : hair_nodes) {
            string hair_id         = hair.attribute("id");
            auto   arm_group_nodes = hair.find("meta");

            for (auto& arm_group : arm_group_nodes) {
              string arm_group_class = arm_group.attribute("class");
              auto   arm_nodes       = arm_group.find("meta");

              for (auto& arm : arm_nodes) {
                string arm_id = arm.attribute("id");
                string src    = arm.attribute("src");

                string out_filename = as::stringf("merged/%s+%s+%s+%s+%s+%s+%s.bmp",
                                                  person_id.c_str(),
                                                  size_id.c_str(),
                                                  pose_id.c_str(),
                                                  garments_id.c_str(),
                                                  hair_id.c_str(),
                                                  arm_group_class.c_str(),
                                                  arm_id.c_str());

                typedef std::multimap<uint32_t, string> z_to_file_t;
                z_to_file_t z_to_file;

                for (auto& layerset : layersets) {
                  if (layerset.file_to_z.find(src) != layerset.file_to_z.end()) {
                    for (auto& layer : layers) {
                      z_to_file.insert(std::make_pair(layerset.file_to_z[layer], layer));
                    }
                    break;
                  }
                }

                if (z_to_file.empty()) {
                  fprintf(stderr, "%s: warning: layer set not found, layer order may be wrong.\n", out_filename.c_str());
                  for (auto& layer : layers) {
                    z_to_file.insert(std::make_pair(0, layer));
                  }
                }

                as::image_t* image = get_image(src);

                for (auto& file : z_to_file) {
                  as::image_t* part = get_image(file.second);
                  image->blend(*part);
                  delete part;
                }

                image->write(out_filename);

                delete image;
              }
            }
          }
        }
      }
    }
  }

  return 0;
}
