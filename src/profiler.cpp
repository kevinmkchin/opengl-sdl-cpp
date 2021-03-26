GLOBAL_VAR int      perf_profiler_level = 0;
GLOBAL_VAR real32   perf_gameloop_elapsed_secs = 0.f;

uint8   PERF_TEXT_SIZE = 14;
uint16  PERF_DRAW_X = 4;
uint16  PERF_DRAW_Y = PERF_TEXT_SIZE + 3;

// Font
TTAFont*    perf_font_handle;
Texture     perf_font_atlas;

// Meshes
Mesh        perf_frametime_vao;

INTERNAL void cmd_profiler_set_level(int level)
{
    perf_profiler_level = level;
}

INTERNAL void profiler_initialize(TTAFont* in_perf_font_handle, Texture in_perf_font_atlas)
{
    perf_font_handle = in_perf_font_handle;
    perf_font_atlas = in_perf_font_atlas;
    perf_frametime_vao = gl_create_mesh_array(0, 0, 0, 0, 2, 2, GL_DYNAMIC_DRAW);
}

INTERNAL void profiler_render(ShaderProgram ui_shader, ShaderProgram text_shader)
{
    if(!perf_profiler_level)
    {
        return;
    }

    if(1 <= perf_profiler_level)
    {
        std::string frametime_temp = std::to_string(1000.f*perf_gameloop_elapsed_secs);
        std::string perf_frametime_string = "LAST FRAME TIME: " 
            + frametime_temp.substr(0, frametime_temp.find(".")+3)
            + "ms   FPS: "
            + std::to_string((int16)(1.f/perf_gameloop_elapsed_secs))
            + "hz";
        kctta_clear_buffer();
        kctta_move_cursor(PERF_DRAW_X, PERF_DRAW_Y);
        kctta_append_line(perf_frametime_string.c_str(), perf_font_handle, PERF_TEXT_SIZE);
        TTAVertexBuffer vb = kctta_grab_buffer();
        gl_rebind_buffers(perf_frametime_vao, 
            vb.vertex_buffer, vb.index_buffer, 
            vb.vertices_array_count, vb.indices_array_count);

        glm::mat4 perf_frametime_transform = glm::mat4(1.f);

        glUseProgram(text_shader.id_shader_program);
            glUniformMatrix4fv(text_shader.id_uniform_projection, 1, GL_FALSE, glm::value_ptr(g_matrix_projection_ortho));
            gl_use_texture(perf_font_atlas);
            glUniform3f(glGetUniformLocation(text_shader.id_shader_program, "text_colour"), 1.f, 1.f, 1.f);
            glUniformMatrix4fv(text_shader.id_uniform_model, 1, GL_FALSE, glm::value_ptr(perf_frametime_transform));
            if(perf_frametime_vao.index_count > 0)
            {
                gl_render_mesh(perf_frametime_vao);
            }
        glUseProgram(0);
    }
}