
#include "hgx.h"
#include "zlib.h"
#include <memory>
#include "BmpWriter.h"

#pragma comment(lib ,"zlib.lib")

struct HG3HDR {
	unsigned char signature[4]; // "HG-3"
	unsigned long unknown1;
	unsigned long unknown2;
	unsigned long unknown3;
	unsigned long entry_count;
};

struct HG3TAG {
	char          signature[8];
	unsigned long offset_next;
	unsigned long length;
};

struct HG3STDINFO {
	unsigned long width;
	unsigned long height;
	unsigned long depth;
	unsigned long offset_x;
	unsigned long offset_y;
	unsigned long total_width;
	unsigned long total_height;
	unsigned long unknown1;
	unsigned long unknown2;
	unsigned long unknown3;
};

struct HG3IMG {
	unsigned long unknown;
	unsigned long height;
	unsigned long data_len;
	unsigned long orig_data_len;
	unsigned long cmd_len;
	unsigned long orig_cmd_len;
};

struct HG3IMGAL {
	unsigned long length;
	unsigned long original_length;
};

struct HG2HDR {
	unsigned char signature[4]; // "HG-2"
	unsigned long unknown1;
	unsigned long type;         // 0x25 = 88 bytes (full header), 0x20 = 80 bytes (truncated)
	unsigned long width;
	unsigned long height;
	unsigned long depth;
	unsigned long unknown3;
	unsigned long unknown4;
	unsigned long data_len;
	unsigned long orig_data_len;
	unsigned long cmd_len;
	unsigned long orig_cmd_len;
	unsigned long extra_length; // 40
	unsigned long unknown5;
	unsigned long total_width;
	unsigned long total_height;
	unsigned long offset_x;
	unsigned long offset_y;
	unsigned long unknown6;
	unsigned long unknown7;
	unsigned long unknown8;
	unsigned long unknown9;
};

// This encoding tries to optimize for lots of zeros. I think. :)
unsigned char unpack_val(unsigned char c) {
	unsigned char z = c & 1 ? 0xFF : 0;
	return (c >> 1) ^ z;
}

void undeltafilter(unsigned char* buff,
	unsigned long  len,
	unsigned char* out_buff,
	unsigned long  width,
	unsigned long  height,
	unsigned long  depth_bytes)
{
	unsigned long table1[256] = { 0 };
	unsigned long table2[256] = { 0 };
	unsigned long table3[256] = { 0 };
	unsigned long table4[256] = { 0 };

	for (unsigned long i = 0; i < 256; i++) {
		unsigned long val = i & 0xC0;

		val <<= 6;
		val |= i & 0x30;

		val <<= 6;
		val |= i & 0x0C;

		val <<= 6;
		val |= i & 0x03;

		table4[i] = val;
		table3[i] = val << 2;
		table2[i] = val << 4;
		table1[i] = val << 6;
	}

	unsigned long  sect_len = len / 4;
	unsigned char* sect1 = buff;
	unsigned char* sect2 = sect1 + sect_len;
	unsigned char* sect3 = sect2 + sect_len;
	unsigned char* sect4 = sect3 + sect_len;

	unsigned char* out_p = out_buff;
	unsigned char* out_end = out_buff + len;

	while (out_p < out_end) {
		unsigned long val = table1[*sect1++] | table2[*sect2++] | table3[*sect3++] | table4[*sect4++];

		*out_p++ = unpack_val((unsigned char)(val >> 0));
		*out_p++ = unpack_val((unsigned char)(val >> 8));
		*out_p++ = unpack_val((unsigned char)(val >> 16));
		*out_p++ = unpack_val((unsigned char)(val >> 24));
	}

	unsigned long stride = width * depth_bytes;

	for (unsigned long x = depth_bytes; x < stride; x++) {
		out_buff[x] += out_buff[x - depth_bytes];
	}

	for (unsigned long y = 1; y < height; y++) {
		unsigned char* line = out_buff + y * stride;
		unsigned char* prev = out_buff + (y - 1) * stride;

		for (unsigned long x = 0; x < stride; x++) {
			line[x] += prev[x];
		}
	}
}

class bitbuff_t {
public:
	bitbuff_t(unsigned char* buff, unsigned long len)
		: buff(buff),
		len(len),
		index(0)
	{}

	bool get_bit(void) {
		if (index > 7) {
			buff++;
			len--;
			index = 0;
		}

		return (*buff >> index++) & 1;
	}

	// Didn't expect to see this in the wild...
	unsigned long get_elias_gamma_value(void) {
		unsigned long value = 0;
		unsigned long digits = 0;

		while (!get_bit()) digits++;

		value = 1 << digits;

		while (digits--) {
			if (get_bit()) {
				value |= 1 << digits;
			}
		}

		return value;
	}

private:
	unsigned long  index;
	unsigned char* buff;
	unsigned long  len;
};

void unrle(unsigned char*  buff,
	unsigned long   len,
	unsigned char*  cmd_buff,
	unsigned long   cmd_len,
	unsigned char*& out_buff,
	unsigned long&  out_len)
{
	bitbuff_t cmd_bits(cmd_buff, cmd_len);

	bool copy_flag = cmd_bits.get_bit();

	out_len = cmd_bits.get_elias_gamma_value();
	out_buff = new unsigned char[out_len];

	unsigned long n = 0;
	for (unsigned long i = 0; i < out_len; i += n) {
		n = cmd_bits.get_elias_gamma_value();

		if (copy_flag) {
			memcpy(out_buff + i, buff, n);
			buff += n;
		}
		else {
			memset(out_buff + i, 0, n);
		}

		copy_flag = !copy_flag;
	}
}

void process_image(unsigned long iPos,
	unsigned char* InputBuffer,
	const string& filename,
	unsigned long width,
	unsigned long height,
	unsigned long depth_bytes,
	unsigned long total_width,
	unsigned long total_height,
	unsigned long offset_x,
	unsigned long offset_y,
	unsigned long len,
	unsigned long orig_len,
	unsigned long cmd_len,
	unsigned long orig_cmd_len)
{
	unsigned char* buff = new unsigned char[orig_len];
	{
		unsigned char* temp = new unsigned char[len];
		memcpy(temp, InputBuffer + iPos, len);
		iPos += len;

		uncompress(buff, &orig_len, temp, len);
		delete[] temp;
	}

	unsigned char* cmd_buff = new unsigned char[orig_cmd_len];
	{
		unsigned char* temp = new unsigned char[cmd_len];
		memcpy(temp, InputBuffer + iPos, cmd_len);
		iPos += cmd_len;

		uncompress(cmd_buff, &orig_cmd_len, temp, cmd_len);
	}

	unsigned long  out_len = 0;
	unsigned char* out_buff = new unsigned char[out_len];
	unrle(buff, orig_len, cmd_buff, orig_cmd_len, out_buff, out_len);

	unsigned long  rgba_len = out_len;
	unsigned char* rgba_buff = new unsigned char[rgba_len];
	undeltafilter(out_buff, out_len, rgba_buff, width, height, depth_bytes);

#ifdef EXPAND_FRAGMENTS
	{
		unsigned long  offset_x_bytes = offset_x * depth_bytes;
		unsigned long  offset_y_vert = total_height - offset_y - height;

		unsigned long  stride = width * depth_bytes;
		unsigned long  exp_stride = total_width * depth_bytes;
		unsigned long  exp_len = total_height * exp_stride;
		unsigned char* exp_buff = new unsigned char[exp_len];
		memset(exp_buff, 0, exp_len);

		for (unsigned long y = 0; y < height; y++) {
			unsigned char* src = rgba_buff + y * stride;
			unsigned char* dst = exp_buff + (y + offset_y_vert) * exp_stride;

			for (unsigned long x = 0; x < stride; x++) {
				dst[offset_x_bytes + x] = src[x];
			}
		}

		delete[] rgba_buff;

		width = total_width;
		height = total_height;
		rgba_len = exp_len;
		rgba_buff = exp_buff;
	}
#endif

	write_bmp(filename + ".bmp",
		rgba_buff,
		rgba_len,
		width,
		height,
		(unsigned short)depth_bytes, 0);

	delete[] rgba_buff;
	delete[] out_buff;
	delete[] cmd_buff;
	delete[] buff;
}


string stringf(const char* fmt, unsigned long f1)
{
	char temp[1024] = { 0 };
	sprintf(temp, fmt, f1);
	return string(temp);
}

string stringf(const char* fmt, unsigned long f1, unsigned long f2)
{
	char temp[1024] = { 0 };
	sprintf(temp, fmt, f1, f2);
	return string(temp);
}

string make_filename(const string& prefix,
	bool          use_index,
	unsigned long index,
	unsigned long subindex,
	unsigned long width,
	unsigned long height,
	unsigned long offset_x,
	unsigned long offset_y)
{
	string filename = prefix;

	if (use_index) {
		filename += stringf("+%03d", index);
	}

	if (use_index || subindex) {
		filename += stringf("+%03d", subindex);
	}

#ifndef EXPAND_FRAGMENTS
	if (offset_x || offset_y) {
		filename += stringf("+x%dy%d",
			offset_x,
			offset_y);
	}
#endif

	return filename;
}

void write_file_internal(const string& out_filename, void* buff, unsigned long len)
{
	FILE* fout = fopen(out_filename.c_str(), "wb");
	fwrite(buff, 1, len, fout);
	fclose(fout);
}


string get_file_prefix(string in_filename)
{
	size_t pos = in_filename.find_first_of(".");
	return in_filename.substr(0, pos);
}

bool UnpackImage(const string& filename, void* Buffer, unsigned long FileSize) 
{
	string in_filename(filename);
	string prefix(get_file_prefix(in_filename));
	string out_filename;

	unsigned iPos = 0;
	char sigcheck[4] = { 0 };
	memcpy(sigcheck, ((unsigned char*)Buffer + iPos), sizeof(sigcheck));

	if (!memcmp(sigcheck, "HG-3", 4)) {
		HG3HDR hdr;
		memcpy(&hdr, ((unsigned char*)Buffer + iPos), sizeof(hdr));
		iPos += sizeof(hdr);

		// Seems entry_count is sometimes inaccurate
		// for (unsigned long i = 0; i <= hdr.entry_count; i++) {
		for (unsigned long i = 0; true; i++) 
		{
			HG3TAG tag = { 0 };
			memcpy(&tag, ((unsigned char*)Buffer + iPos), sizeof(tag));
			iPos += sizeof(tag);

			if (memcmp(tag.signature, "stdinfo", 7)) {
				break;
			}

			HG3STDINFO stdinfo;
			memcpy(&stdinfo, ((unsigned char*)Buffer + iPos), sizeof(stdinfo));
			iPos += sizeof(stdinfo);

			unsigned long img_index = 0;

			while (tag.offset_next) 
			{
				memcpy(&tag, ((unsigned char*)Buffer + iPos), sizeof(tag));
				iPos += sizeof(tag);

				out_filename = make_filename(prefix,
					i > 0 || hdr.entry_count > 0,
					i,
					img_index++,
					stdinfo.total_width,
					stdinfo.total_height,
					stdinfo.offset_x,
					stdinfo.offset_y);

				if (!memcmp(tag.signature, "img_al", 5)) {
					HG3IMGAL imghdr;
					memcpy(&imghdr, ((unsigned char*)Buffer + iPos), sizeof(imghdr));
					iPos += sizeof(imghdr);

					unsigned long  len = imghdr.length;
					unsigned char* buff = new unsigned char[len];
					memcpy(buff, ((unsigned char*)Buffer + iPos),len);
					iPos += len;

					unsigned long  out_len = imghdr.original_length;
					unsigned char* out_buff = new unsigned char[out_len];
					uncompress(out_buff, &out_len, buff, len);

					write_bmp(out_filename + ".bmp",
						out_buff,
						out_len,
						stdinfo.width,
						stdinfo.height,
						1,
						WRITE_BMP_ALIGN | WRITE_BMP_FLIP);

					delete[] out_buff;
					delete[] buff;
				}
				else if (!memcmp(tag.signature, "img", 3) && memcmp(tag.signature, "img_jpg", 6)) {
					HG3IMG imghdr;
					memcpy(&imghdr, ((unsigned char*)Buffer + iPos), sizeof(imghdr));
					iPos += sizeof(imghdr);

					process_image(iPos,
						(unsigned char*)Buffer,
						out_filename,
						stdinfo.width,
						stdinfo.height,
						stdinfo.depth / 8,
						stdinfo.total_width,
						stdinfo.total_height,
						stdinfo.offset_x,
						stdinfo.offset_y,
						imghdr.data_len,
						imghdr.orig_data_len,
						imghdr.cmd_len,
						imghdr.orig_cmd_len);
				}
				else {
					unsigned long  len = tag.length;
					unsigned char* buff = new unsigned char[len];
					memcpy(buff, ((unsigned char*)Buffer + iPos), len);
					iPos += len;

					write_file_internal(out_filename, buff, len);

					delete[] buff;
				}
			}

			// Some extra stuff before the next tag.
			iPos += 8;
		}
	}
	else if (!memcmp(sigcheck, "HG-2", 4))
	{
		HG2HDR hdr;
		memcpy(&hdr, ((unsigned char*)Buffer + iPos), sizeof(hdr));
		iPos += sizeof(hdr);

		// Hack :)
		if (hdr.type == 0x20) {
			//lseek(fd, -8, SEEK_CUR);
			iPos -= 8;
		}

		if (out_filename.empty()) {
			out_filename = make_filename(prefix,
				false,
				0,
				0,
				hdr.total_width,
				hdr.total_height,
				hdr.offset_x,
				hdr.offset_y);
		}

		process_image(iPos,
			(unsigned char*)Buffer,
			out_filename,
			hdr.width,
			hdr.height,
			hdr.depth / 8,
			hdr.total_width,
			hdr.total_height,
			hdr.offset_x,
			hdr.offset_y,
			hdr.data_len,
			hdr.orig_data_len,
			hdr.cmd_len,
			hdr.orig_cmd_len);
	}
	else {
		//fprintf(stderr, "%s: what's this?\n", in_filename.c_str());
		return false;
	}

	return true;
}