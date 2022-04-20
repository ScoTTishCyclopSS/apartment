#include "object.h"

void Object::initModel(Shader shader) {

		// texture mapping
		mesh.texture = pgr::createTexture(tex_path);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh.texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		CHECK_GL_ERROR();

		// VBO
		glGenBuffers(1, &mesh.vbo);
		glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * countVertices * countAttribsPerVertex, vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		CHECK_GL_ERROR();

		// EBO
		glGenBuffers(1, &mesh.ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * countTriangles * 3, triangles, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		CHECK_GL_ERROR();

		// VAO
		glGenVertexArrays(1, &mesh.vao);
		glBindVertexArray(mesh.vao);
		CHECK_GL_ERROR();

		glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
		CHECK_GL_ERROR();

		glEnableVertexAttribArray(shader.posLoc);
		glVertexAttribPointer(shader.posLoc, 3, GL_FLOAT, GL_FALSE, countAttribsPerVertex * sizeof(float), (void*)(0)); // (x, y) ... ...
		CHECK_GL_ERROR();

		glEnableVertexAttribArray(shader.nrmLoc);
		glVertexAttribPointer(shader.nrmLoc, 3, GL_FLOAT, GL_FALSE, countAttribsPerVertex * sizeof(float), (void*)(3 * sizeof(float))); // ... (nx, ny, nz) ...
		CHECK_GL_ERROR();

		glEnableVertexAttribArray(shader.texLoc);
		glVertexAttribPointer(shader.texLoc, 2, GL_FLOAT, GL_FALSE, countAttribsPerVertex * sizeof(float), (void*)(6 * sizeof(float))); // ... ... (u, v)

		glBindVertexArray(0);
		CHECK_GL_ERROR();

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glCullFace(GL_BACK);
	//glEnable(GL_CULL_FACE);
	return;
}

void Object::drawObject(Shader shader, const glm::mat4& viewMat, const glm::mat4& projectMat, const glm::mat4& modelMat) {

	// send mat to shaders
	shader.setTransformUniforms(modelMat, viewMat, projectMat);
	CHECK_GL_ERROR();

	// send material info
	//shader.setMaterialUniforms(mesh.ambient, mesh.diffuse, mesh.specular, mesh.shiness, mesh.texture);
	CHECK_GL_ERROR();

	glUniformMatrix4fv(shader.PVMLoc, 1, GL_FALSE, value_ptr(projectMat * viewMat * modelMat));
	CHECK_GL_ERROR();

	// draw
	glBindVertexArray(mesh.vao);
	glDrawElements(GL_TRIANGLES, countTriangles * 3, GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);
	CHECK_GL_ERROR();

	return;
}
