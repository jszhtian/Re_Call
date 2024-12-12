#include <windows.h>
#include <string>

#include <fcntl.h>

using namespace std;

struct BINHDR {
  unsigned int entry_count;
  unsigned int filenames_length;
};

struct BINENTRY {
  unsigned int filename_offset;
  unsigned int offset;
  unsigned int length;
};

struct HZC1HDR {
  unsigned char  signature[4]; // "hzc1"
  unsigned int original_length;
  unsigned int header_length;
};

struct NVSGHDR {
  unsigned char  signature[4]; // "NVSG"
  uint16_t unknown1;
  uint16_t type;
  uint16_t width;
  uint16_t height;
  uint16_t offset_x;
  uint16_t offset_y;
  unsigned int unknown2;
  unsigned int entry_count;
  unsigned int unknown3;
  unsigned int unknown4;
};


int main(int argc, char** argv) {
  if (argc != 2) {
    return -1;
  }

  string in_filename = argv[1];

  int fd = open(in_filename.c_str(), O_RDONLY | O_BINARY); 

  BINHDR hdr;
  read(fd, &hdr, sizeof(hdr));

  BINENTRY* entries = new BINENTRY[hdr.entry_count];
  read(fd, entries, sizeof(BINENTRY) * hdr.entry_count);

  unsigned int filenames_len  = hdr.filenames_length;
  unsigned char*    filenames_buff = new unsigned char[filenames_len];
  read(fd, filenames_buff, filenames_len);
  
  FILE *fp = fopen("info.txt", "wb");

  for (unsigned int i = 0; i < hdr.entry_count; i++) {
    string filename = (char*) (filenames_buff + entries[i].filename_offset);
	fprintf(fp, "%s\r\n\r\n", filename.c_str());

  }

  delete [] filenames_buff;
  delete [] entries;

  fclose(fp);
  close(fd);

  return 0;
}
