#ifndef SCOP_BITMAP_HPP
# define SCOP_BITMAP_HPP
# include <string>
# include <fstream>
# include <vector>
# include <cstdint>

class Bitmap {
	struct {
		char		magic[2];
		uint32_t	size;
		uint16_t	unused1;
		uint16_t	unused2;
		uint32_t	begin_offset;
	} __attribute__((packed))		file_header;
	uint32_t	header_size;
	struct {
		int32_t width;
		int32_t height;
		uint16_t color_planes;
		uint16_t bpp;
		uint32_t compression;
		uint32_t image_size;
		uint32_t hres;
		uint32_t vres;
		uint32_t npalette;
		uint32_t nimportant;
	} __attribute__((packed))		dib_header;
	std::ifstream		stream;
	std::vector<char>	_data;
	void	parse_header(void);
	void	parse_data(void);
public:
	Bitmap(std::string const &filename);
	const char		*data() const;
	uint32_t		size() const;
	uint32_t		width() const;
	uint32_t		height() const;
};


#endif //SCOP_BITMAP_HPP
