#include "TetraedreFactory.hpp"

TetraedreFactory::TetraedreFactory() : ObjectFactory("srcs/shaders/shader.vsh", "srcs/shaders/shader.fsh") {
	vertex vertices[] = {
			{-0.5f	, -0.433f	,  0.45f	, 255	, 0		, 0		}, // A
			{0.0f	, 0.433f	,  0.45f	, 0		, 0		, 255	}, // C
			{0.0f	, 0.0f		,  -0.45f	, 255	, 255	, 255	}, // D

			{-0.5f	, -0.433f	,  0.45f	, 255	, 0		, 0		}, // A
			{0.5f	, -0.433f	,  0.45f	, 0		, 255	, 0		}, // B
			{0.0f	, 0.0f		,  -0.45f	, 255	, 255	, 255	}, // D

			{-0.5f	, -0.433f	,  0.45f	, 255	, 0		, 0		}, // A
			{0.5f	, -0.433f	,  0.45f	, 0		, 255	, 0		}, // B
			{0.0f	, 0.433f	,  0.45f	, 0		, 0		, 255	}, // C

			{0.5f	, -0.433f	,  0.45f	, 0		, 255	, 0		}, // B
			{0.0f	, 0.433f	,  0.45f	, 0		, 0		, 255	}, // C
			{0.0f	, 0.0f		,  -0.45f	, 255	, 255	, 255	}, // D
	};
	unsigned int indices[] = {
			0, 2, 3,
			0, 1, 3,
			0, 1, 2,
			1, 2, 3
	};

	this->EBO_size = 4 * 3;

	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->EBO);

	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	glBindVertexArray(0);
}

void TetraedreFactory::build() {
	vertex vertices[] = {
			{-0.5f	, -0.5f	,  0.5f	, 255	, 0		, 0		}, // A
			{0.0f	, 0.5f	,  0.5f	, 0		, 0		, 255	}, // C
			{0.0f	, 0.0f	,  -0.37f, 255	, 255	, 255	}, // D

			{-0.5f	, -0.5f	,  0.5f	, 255	, 0		, 0		}, // A
			{0.5f	, -0.5f	,  0.5f	, 0		, 255	, 0		}, // B
			{0.0f	, 0.0f	,  -0.37f, 255	, 255	, 255	}, // D

			{-0.5f	, -0.5f	,  0.5f	, 255	, 0		, 0		}, // A
			{0.5f	, -0.5f	,  0.5f	, 0		, 255	, 0		}, // B
			{0.0f	, 0.5f	,  0.5f	, 0		, 0		, 255	}, // C

			{0.5f	, -0.5f	,  0.5f	, 0		, 255	, 0		}, // B
			{0.0f	, 0.5f	,  0.5f	, 0		, 0		, 255	}, // C
			{0.0f	, 0.0f	,  -0.37f, 255	, 255	, 255	}, // D
	};
	unsigned int indices[] = {
			0, 2, 3,
			0, 1, 3,
			0, 1, 2,
			1, 2, 3
	};

	this->EBO_size = 4 * 3;

	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->EBO);

	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	glBindVertexArray(0);
}