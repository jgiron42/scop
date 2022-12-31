#ifndef SCOP_SHADER_HPP
# define SCOP_SHADER_HPP
# include <string>
# include <fstream>
# include <iostream>

class shader {
private:
	unsigned int id;
	static bool ReadFile(const char *name, std::string &dst) {
		std::ifstream stream;
		stream.open(name);
		for (std::string line; std::getline(stream, line); )
			dst += line + "\n";
		return true;
	}

	static unsigned int compile_shader(const std::string &filename, int type)
	{
		std::string content;
		unsigned int id = glCreateShader(type);
		shader::ReadFile(filename.c_str(), content);
		const char * shader_cstr = content.c_str();
		glShaderSource(id, 1, &shader_cstr, NULL);
		glCompileShader(id);

		int  success;
		char infoLog[512];
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		if(!success)
		{
			glGetShaderInfoLog(id, 512, NULL, infoLog);
			std::cout << "ERROR: \"" << filename << "\" COMPILATION_FAILED\n" << infoLog << std::endl;
			throw std::exception();
		}
		return id;
	}

public:
	shader(std::string const &vs, std::string const &fs)
	{
		unsigned int vs_id = shader::compile_shader(vs, GL_VERTEX_SHADER);
		unsigned int fs_id = shader::compile_shader(fs, GL_FRAGMENT_SHADER);
		this->id = glCreateProgram();
		glAttachShader(this->id, vs_id);
		glAttachShader(this->id, fs_id);
		glLinkProgram(this->id);
		glDeleteShader(vs_id);
		glDeleteShader(fs_id);
		int success;
		char infoLog[512];
		glGetProgramiv(this->id, GL_LINK_STATUS, &success);
		if(!success) {
			glGetProgramInfoLog(this->id, 512, NULL, infoLog);
			std::cout << "ERROR: LINKING_FAILED\n" << infoLog << std::endl;
			throw std::exception();
		}
	}

	shader(shader && other) = default;
	shader &operator=(const shader& o) = default;

	void use() const {
		glUseProgram(this->id);
	}

	int get_uniform_location(std::string const &name) const
	{
		return glGetUniformLocation(this->id, name.c_str());
	}

	~shader() {
		glDeleteProgram(this->id);
	}
};

#endif //SCOP_SHADER_HPP
