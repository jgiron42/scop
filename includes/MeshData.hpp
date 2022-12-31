#ifndef SCOP_MESHDATA_HPP
# define SCOP_MESHDATA_HPP
# include <vector>
# include <map>
# include <array>
# include <string>
# include <functional>
# include <optional>
# include "Bitmap.hpp"

class MeshData {
public:
	class InvalidSyntax : public std::exception {};
	typedef struct coords {
		float x;
		float y;
		float z;
		bool operator<(const coords &o)  const;
	}			coords;
	struct texture_coords {
		float u;
		float v;
	};
	typedef struct {
		coords pos;
		coords normals;
		coords color;
		texture_coords textureCoords;
	}			vertex;
	struct	material {
		std::string name;
		coords	ambient;
		coords	diffuse; // unused
		coords	specular; // unused
		float	specular_component; // unused
		float	opacity; // unused
		float	refraction; // unused
		int		illum; // unused
		std::optional<Bitmap> texture;
	};
	typedef struct	s_raw_face_entry{
		int		vertex_index;
		size_t	normal_index;
		int		texture_index;
		int		color_index; // this make no sense in the obj format context but is necessary for the subject
		bool operator<(const s_raw_face_entry &o)  const;
	}				raw_face_entry;
	typedef struct {
		std::vector<raw_face_entry> vertices;
		material					*mat;
		bool						smooth;
	}	raw_face;
private:
	// This represents the parsed obj file
	std::vector<coords> 				raw_vertices;
	std::vector<coords>					normals;
	std::vector<raw_face>				raw_faces;
	std::vector<coords>					colors;
	std::vector<texture_coords>					texture_coordinates;

	// the next fields are computed from the preceding
	std::map<raw_face_entry, size_t>	vertices_index_map;
	std::vector<std::vector<size_t>>	polygons; // can contain polygons beyond triangles

	void parse_obj(std::string const &filename);
	void parse_mtl(std::string const &filename);
	static std::vector<raw_face_entry> parse_raw_faces(std::string const &);
	void	compute_normals(void);
	void	compute_colors(void);
	void	compute_polygons(void);
	void	compute_triangles(void);
	void	center(void);

	size_t	get_vertex_id(raw_face_entry);
public:
	// mtl
	std::map<std::string, material>		materials;
	std::vector<vertex>					vertices; // final VBO
	std::vector<unsigned int>					triangles; // final EBO
	MeshData(std::string const &filename);
};

#endif
