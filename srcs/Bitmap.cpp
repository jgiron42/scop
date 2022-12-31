#include "../includes/Bitmap.hpp"
#include <exception>
#include <iostream>
#include <strings.h>

Bitmap::Bitmap(const std::string &filename) : file_header({}), dib_header({}) {
	this->stream.open(filename);
	if (!this->stream.is_open())
		throw std::runtime_error("Can't open " + filename);
	this->parse_header();
	this->parse_data();
}

void Bitmap::parse_header() {
	this->stream.read((char *) &(this->file_header), sizeof(this->file_header));
	this->stream.read((char *) &(this->header_size), sizeof(this->header_size));
	if (this->header_size == 12) {
		struct {
			uint16_t width;
			uint16_t height;
			uint16_t color_planes;
			uint16_t bpp;
		} __attribute__((packed)) bitmap_core_header = {};
		this->stream.read((char *) &(bitmap_core_header), this->header_size - (int)sizeof(this->header_size));
		this->dib_header.width = bitmap_core_header.width;
		this->dib_header.height = bitmap_core_header.height;
		this->dib_header.color_planes = bitmap_core_header.color_planes;
		this->dib_header.bpp = bitmap_core_header.bpp;
	} else
		this->stream.read((char *) &(this->dib_header), std::min<int>((int)sizeof(this->dib_header), this->header_size));
}

void Bitmap::parse_data() {
	this->stream.seekg(this->file_header.begin_offset);
	this->_data.resize(this->file_header.size);
	this->stream.read(this->_data.data(), this->dib_header.width * this->dib_header.height * this->dib_header.bpp);
}

uint32_t Bitmap::size() const {return this->file_header.size;}

uint32_t Bitmap::width() const {return this->dib_header.width;}

uint32_t Bitmap::height() const {return this->dib_header.height;}

const char *Bitmap::data() const {return this->_data.data();}
