#include "../includes/MeshData.hpp"
#include <istream>
#include <fstream>
#include <regex>
#include <iostream>
#include <string>
#include "Vector.hpp"
#include <filesystem>

MeshData::MeshData(const std::string &filename) {
	this->parse_obj(filename);
	std::cout << "number of vertices: " << this->raw_vertices.size() << std::endl;
	this->compute_colors();
	this->compute_normals();
	this->compute_polygons();
	this->raw_vertices.clear();
	this->raw_faces.clear();
	this->normals.clear();
	this->vertices_index_map.clear();
	this->compute_triangles();
	this->polygons.clear();
	this->center();
}

void MeshData::parse_obj(const std::string &filename) {
	std::ifstream stream;
	stream.open(filename);
	if (!stream.is_open())
		throw std::runtime_error("Can't open " + filename);

	static std::regex v (R"(v +(-?[0-9]+\.?[0-9]*) +(-?[0-9]+\.?[0-9]*) +(-?[0-9]+\.?[0-9]*)( +(-?[0-9]+\.?[0-9]*))?)", std::regex_constants::extended);
	static std::regex vn(R"(vn +(-?[0-9]+\.?[0-9]*) +(-?[0-9]+\.?[0-9]*) +(-?[0-9]+\.?[0-9]*))", std::regex_constants::extended);
	static std::regex vp(R"(vp +(-?[0-9]+\.?[0-9]*)( +(-?[0-9]+\.?[0-9]*))?( +(-?[0-9]+\.?[0-9]*))?)", std::regex_constants::extended);
	static std::regex vt(R"(vt +(0(\.[0-9]*)?|1(\.0*)?)( +(0(\.[0-9]*)?|1(\.0*)?)( +(0(\.[0-9]*)?|1(\.0*)?))?)?)", std::regex_constants::extended);
	static std::regex f (R"(f(( +([0-9]+)((/([0-9]*))(/([0-9]+))?)?){3,}))", std::regex_constants::extended);
	static std::regex o (R"(o +[[:print:]]+)", std::regex_constants::extended);
	static std::regex mtllib(R"(mtllib +([[:print:]]+.mtl))", std::regex_constants::extended);
	static std::regex usemtl(R"(usemtl +([a-zA-Z]+))", std::regex_constants::extended);
	static std::regex smooth(R"(s +((1|0)|(on|off)))", std::regex_constants::extended);
	static std::regex comment("^#.*", std::regex_constants::extended);
	static std::regex empty_line("^[[:space:]]*$", std::regex_constants::extended);

	material *current_material = nullptr;
	bool	smooth_value = false;
	while (!stream.eof())
	{
		std::string line;
		std::getline(stream, line);
		std::match_results< std::string ::const_iterator> results;

		if (std::regex_match(line, results, v))
			this->raw_vertices.push_back({std::stof(results[1]), std::stof(results[2]), std::stof(results[3])});
		else if (std::regex_match(line, results, vn))
			this->normals.push_back({std::stof(results[1]), std::stof(results[2]), std::stof(results[3])});
		else if (std::regex_match(line, results, vp)) {
			// unused
		}
		else if (std::regex_match(line, results, o)) {
			// unused
		}
		else if (std::regex_match(line, results, vt)) {
			this->texture_coordinates.push_back({std::stof(results[1]), results[5].matched ? std::stof(results[5]) : 0});
		}
		else if (std::regex_match(line, results, f))
			this->raw_faces.push_back((raw_face){parse_raw_faces(results[1]), current_material, smooth_value});
		else if (std::regex_match(line, results, mtllib))
		{
			std::filesystem::path base(filename);
			this->parse_mtl(base.remove_filename() / std::filesystem::path(results[1]));
		}
		else if (std::regex_match(line, results, usemtl))
			current_material = &this->materials[results[1]];
		else if (std::regex_match(line, results, smooth))
			smooth_value = results[2].matched ? std::stoi(results[2]) : (results[3] == "on");
		else if (!std::regex_match(line, results, comment) &&
				 !std::regex_match(line, results, empty_line))
		{
			std::cerr << filename <<  ": Invalid syntax: \"" << line << '"' << std::endl;
			throw InvalidSyntax();
		}
	}
	this->raw_vertices.shrink_to_fit();
	this->normals.shrink_to_fit();
	this->raw_faces.shrink_to_fit();
}

void MeshData::parse_mtl(const std::string &filename) {
	std::ifstream stream;
	stream.open(filename);
	if (!stream.is_open())
		throw std::runtime_error("Can't open " + filename);

	static std::regex newmtl(R"(newmtl +([a-zA-Z]+))", std::regex_constants::extended);
	static std::regex Ka(R"(Ka +(0(\.[0-9]*)|1(\.0*)?) +(0(\.[0-9]*)|1(\.0*)?) +(0(\.[0-9]*)|1(\.0*)?))", std::regex_constants::extended);
	static std::regex Kd(R"(Kd +(0(\.[0-9]*)|1(\.0*)?) +(0(\.[0-9]*)|1(\.0*)?) +(0(\.[0-9]*)|1(\.0*)?))", std::regex_constants::extended);
	static std::regex Ks(R"(Ks +(0(\.[0-9]*)|1(\.0*)?) +(0(\.[0-9]*)|1(\.0*)?) +(0(\.[0-9]*)|1(\.0*)?))", std::regex_constants::extended);
	static std::regex Ns(R"(Ns +([0-9]{1,3}(\.[0-9]*)?|1000(\.0*)?))", std::regex_constants::extended);
	static std::regex Ni(R"(Ni +(0\.[0-9][0-9][1-9]|0\.[1-9][0-9][0-9]|0\.[0-9][1-9][0-9]|[1-9](\.[0-9]*)|10(\.0*)?))", std::regex_constants::extended);
	static std::regex d(R"(d +(0(\.[0-9]*)|1(\.0*)?))", std::regex_constants::extended);
	static std::regex texture(R"((map_Ka|map_Kd|map_Ns|map_d|map_bump|bump|disp|decal) +([[:print:]]+.bmp))", std::regex_constants::extended);
	static std::regex illum(R"(illum +(10|[0-9]))", std::regex_constants::extended);
	static std::regex comment("^#.*", std::regex_constants::extended);
	static std::regex empty_line("^[[:space:]]*$", std::regex_constants::extended);

	material *current_material = nullptr;
	while (!stream.eof())
	{
		std::string line;
		std::getline(stream, line);
		std::match_results< std::string ::const_iterator> results;

		if (std::regex_match(line, results, newmtl))
			current_material = &this->materials[results[1]];
		else if (!current_material &&
				!std::regex_match(line, results, comment) &&
				!std::regex_match(line, results, empty_line))
		{
			std::cerr << filename << ": Invalid syntax: \"" << line << "\": no material specified" << std::endl;
			throw InvalidSyntax();
		}
		else if (std::regex_match(line, results, Ka))
			current_material->ambient = {std::stof(results[1]), std::stof(results[4]), std::stof(results[7])};
		else if (std::regex_match(line, results, Kd))
			current_material->diffuse = {std::stof(results[1]), std::stof(results[4]), std::stof(results[7])};
		else if (std::regex_match(line, results, Ks))
			current_material->specular = {std::stof(results[1]), std::stof(results[4]), std::stof(results[7])};
		else if (std::regex_match(line, results, Ns))
			current_material->specular_component = std::stof(results[1]);
		else if (std::regex_match(line, results, Ni))
			current_material->refraction = std::stof(results[1]);
		else if (std::regex_match(line, results, d))
			current_material->opacity = std::stof(results[1]);
		else if (std::regex_match(line, results, illum))
			current_material->illum = std::stoi(results[1]);
		else if (std::regex_match(line, results, texture))
		{
			if (results[1] == "map_Ka")
			{
				std::filesystem::path base(filename);
				current_material->texture.emplace(base.remove_filename() / std::filesystem::path(results[2]));
			}
			// we only do the ambient map
		}
		else if (!std::regex_match(line, results, comment) &&
				 !std::regex_match(line, results, empty_line))
		{
			std::cerr << filename << ": Invalid syntax: \"" << line << '"' << std::endl;
			throw InvalidSyntax();
		}
	}
}

std::vector<MeshData::raw_face_entry> MeshData::parse_raw_faces(const std::string &s) {
	static std::regex entry_regex("([0-9]+)(/([0-9]+)?(/([0-9]+))?)?", std::regex_constants::extended);
	std::match_results<std::string::const_iterator> res;
	std::vector<raw_face_entry> ret;

	for (auto it = std::sregex_iterator(s.begin(), s.end(), entry_regex); it != std::sregex_iterator(); it++)
	{
		raw_face_entry entry;
		entry.vertex_index = std::stoi((*it)[1]);
		entry.texture_index = ((*it)[3].matched) ? std::stoi((*it)[3]) : 0;
		entry.normal_index = ((*it)[5].matched) ? std::stoi((*it)[5]) : 0;
		ret.push_back(entry);
	}
	return ret;
}

void MeshData::compute_polygons() {
	this->normals.resize(1);
	for (auto &rf : this->raw_faces)
	{
		std::vector<size_t> current;
		for (size_t i = 0; i < rf.vertices.size(); i++)
			current.push_back(this->get_vertex_id(rf.vertices[i]));
		current.shrink_to_fit();
		this->polygons.push_back(current);
	}
	this->polygons.shrink_to_fit();
}

void MeshData::compute_normals() {
	std::map<coords, size_t> m;

	for (auto &rf : this->raw_faces)
		for (size_t i = 0; i < rf.vertices.size(); i++)
		{
			if (rf.vertices[i].normal_index)
				continue ;
			coords b = this->raw_vertices[rf.vertices[i].vertex_index - 1];
			coords a = this->raw_vertices[rf.vertices[(((ssize_t)i - 1) < 0 ? rf.vertices.size() - 1 : i - 1)].vertex_index - 1];
			coords c = this->raw_vertices[rf.vertices[(i + 1) % rf.vertices.size()].vertex_index - 1];
			Vector<float> va{a.x, a.y, a.z};
			Vector<float> vb{b.x, b.y, b.z};
			Vector<float> vc{c.x, c.y, c.z};
			va -= vb;
			vc -= vb;
			auto vn = cross_product(vc, va);
			vn = vn / vn.norm();
			coords res = {vn.data[0], vn.data[1], vn.data[2]};
			auto it = m.find(res);
			if (it == m.end())
			{
				this->normals.push_back(res);
				it = m.insert(std::make_pair(res, this->normals.size())).first;
			}
			rf.vertices[i].normal_index = it->second;
		}
}

void MeshData::compute_colors() {
	std::map<coords, size_t> m;

	this->colors.push_back({255, 0, 0});
	this->colors.push_back({0, 255, 0});
	this->colors.push_back({0, 0, 255});
	this->colors.push_back({0, 255, 255});
	this->colors.push_back({255, 0, 255});
	this->colors.push_back({255, 255, 0});
	this->colors.push_back({255, 255, 255});

	for (size_t i = 0; i < this->raw_faces.size(); i++)
		for (size_t j = 0; j < this->raw_faces[i].vertices.size(); j++)
			this->raw_faces[i].vertices[j].color_index = (int)(i % this->colors.size()) + 1;
}

void MeshData::compute_triangles() {
	for (const auto &f : this->polygons)
		for (size_t i = 0; i + 1 < f.size(); i += 2)
		{
			this->triangles.push_back(f[i]);
			this->triangles.push_back(f[i + 1]);
			this->triangles.push_back(f[(i + 2) % f.size()]);
		}
}

size_t MeshData::get_vertex_id(MeshData::raw_face_entry entry) {
	auto it = this->vertices_index_map.find(entry);
	if (it != this->vertices_index_map.end())
		return it->second;
	vertex v = {
			.pos = this->raw_vertices[entry.vertex_index - 1],
			.normals = {0, 0, 0},
			.color = {0, 0, 0,},
			.textureCoords = {0, 0}
	};
	if (entry.normal_index != 0)
		v.normals = this->normals[entry.normal_index - 1];
	if (entry.color_index != 0)
		v.color = this->colors[entry.color_index - 1];
	if (entry.texture_index != 0)
		v.textureCoords = this->texture_coordinates[entry.texture_index - 1];
	this->vertices.push_back(v);
	this->vertices_index_map[entry] = this->vertices.size() - 1;
	return this->vertices.size() - 1;
}

void MeshData::center() {
	coords max = {-INFINITY, -INFINITY, -INFINITY};
	coords min = {INFINITY, INFINITY, INFINITY};
	for (const auto &v : this->vertices)
	{
		if (v.pos.x < min.x)
			min.x = v.pos.x;
		if (v.pos.y < min.y)
			min.y = v.pos.y;
		if (v.pos.z < min.z)
			min.z = v.pos.z;
		if (v.pos.x > max.x)
			max.x = v.pos.x;
		if (v.pos.y > max.y)
			max.y = v.pos.y;
		if (v.pos.z > max.z)
			max.z = v.pos.z;
	}
	coords average =
			{
					(max.x + min.x) / 2,
					(max.y + min.y) / 2,
					(max.z + min.z) / 2,
			};
	for (auto &v : this->vertices)
	{
		v.pos.x -= average.x;
		v.pos.y -= average.y;
		v.pos.z -= average.z;
	}
}

bool MeshData::s_raw_face_entry::operator<(const MeshData::s_raw_face_entry &o) const {
	return (
			this->vertex_index < o.vertex_index ||
			this->normal_index < o.normal_index ||
			this->color_index < o.color_index ||
			this->texture_index < o.texture_index
			);
}

bool MeshData::coords::operator<(const MeshData::coords &o) const {
	return (
			this->x < o.x ||
			this->y < o.y ||
			this->z < o.z
			);
}