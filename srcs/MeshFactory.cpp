#include "../includes/MeshFactory.hpp"

MeshFactory::MeshFactory(const MeshData &data) : ObjectFactory("srcs/shaders/mesh.vsh", "srcs/shaders/mesh.fsh") {

	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->EBO);

	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshData::vertex), nullptr);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MeshData::vertex), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(MeshData::vertex), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(MeshData::vertex), (void*)(9 * sizeof(float)));
	glEnableVertexAttribArray(3);

	if (!data.materials.empty())
	{
		const MeshData::material &m = data.materials.begin()->second; // we consider only one texture for the moment
		if (m.texture)
		{
			const Bitmap &bitmap(m.texture.value());
			glGenTextures(1, &this->tex);
			glBindTexture(GL_TEXTURE_2D, this->tex);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmap.width(), bitmap.height(), 0, GL_BGR, GL_UNSIGNED_BYTE, bitmap.data());
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	this->EBO_size = (GLsizei)(data.triangles.size());


	glBufferData(GL_ARRAY_BUFFER, (GLsizei)(data.vertices.size() * sizeof(MeshData::vertex)), data.vertices.data(), GL_STATIC_DRAW);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizei)(data.triangles.size() * sizeof (unsigned int)), data.triangles.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	glBindVertexArray(0);

}

MeshFactory::~MeshFactory() {
	glDeleteVertexArrays(1, &this->VAO);
	glDeleteBuffers(1, &this->VBO);
	glDeleteBuffers(1, &this->EBO);
}

void MeshFactory::build() {

}