/**  */
INTERNAL Mesh gl_create_mesh_array(float* vertices, 
								   uint32* indices,
								   uint32 num_of_vertices,
								   uint32 num_of_indices)
{
	Mesh mesh;
	// Need to store to index_count because we need the count of indices when we are drawing in Mesh::render_mesh
	mesh.index_count = num_of_indices;

	glGenVertexArrays(1, &mesh.id_vao); // Defining some space in the GPU for a vertex array and giving you the vao ID
	glBindVertexArray(mesh.id_vao); // Binding a VAO means we are currently operating on that VAO
		// Indentation is to indicate that we are now working within the bound VAO
		glGenBuffers(1, &mesh.id_vbo); // Creating a buffer object inside the bound VAO and returning the ID
		glBindBuffer(GL_ARRAY_BUFFER, mesh.id_vbo); // Bind VBO to operate on that VBO
			/* Connect the vertices data to the actual gl array buffer for this VBO. We need to pass in the size of the data we are passing as well.
			GL_STATIC_DRAW (as opposed to GL_DYNAMIC_DRAW) means we won't be changing these data values in the array. 
			The vertices array does not need to exist anymore after this call because that data will now be stored in the VAO on the GPU. */
			glBufferData(GL_ARRAY_BUFFER, 4 /*bytes cuz uint32*/ * num_of_vertices, vertices, GL_STATIC_DRAW); // NOTE: 12 instead of 9 now because 12 elements in vertices
			/* Index is location in VAO of the attribute we are creating this pointer for.
			Size is number of values we are passing in (e.g. size is 3 if x y z).
			Normalized is normalizing the values.
			Stride is the number of values to skip after getting the values we need.
				for example, you could have vertices and colors in the same array
				[ Ax, Ay, Az,  Ar, Ag, Ab,  Bx, By, Bz,  Br, Bg, Bb ]
					use          stride        use          stride
				In this case, the stride would be 3 because we need to skip 3 values (the color values) to reach the next vertex data.
			Apparently the last parameter is the offset? */
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0); // vertex pointer
			glEnableVertexAttribArray(0); // Enabling location in VAO for the attribute
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)( sizeof(float) * 3)); // uv coord pointer
			glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind the VBO

		// Index Buffer Object
		glGenBuffers(1, &mesh.id_ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.id_ibo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 /*bytes cuz uint32*/ * num_of_indices, indices, GL_STATIC_DRAW); // 4 bytes (for uint32) * 12 elements in indices
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0); // Unbind the VAO;

	return mesh;
}

/**  */
INTERNAL void gl_render_mesh(Mesh& mesh)
{
	if (mesh.index_count == 0) // Early out if index_count == 0, nothing to draw
	{
		// TODO log a warning
		return;
	}

	// Bind VAO, bind VBO, draw elements(indexed draw)
	glBindVertexArray(mesh.id_vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.id_ibo);
			glDrawElements(GL_TRIANGLES, mesh.index_count, GL_UNSIGNED_INT, nullptr); // Last param could be pointer to indices but no need cuz IBO is already bound
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

/** Clearing GPU memory: glDeleteBuffers and glDeleteVertexArrays deletes the buffer
	object and vertex array object off the GPU memory. 
*/
INTERNAL void gl_delete_mesh(Mesh& mesh)
{
	if (mesh.id_ibo != 0)
	{
		glDeleteBuffers(1, &mesh.id_ibo);
		mesh.id_ibo = 0;
	}
	if (mesh.id_vbo != 0)
	{
		glDeleteBuffers(1, &mesh.id_vbo);
		mesh.id_vbo = 0;
	}
	if (mesh.id_vao != 0)
	{
		glDeleteVertexArrays(1, &mesh.id_vao);
		mesh.id_vao = 0;
	}

	mesh.index_count = 0;
}