mod render_gl;
mod shader_gl;
mod texture_gl;
use texture_gl::Texture;

use crate::render_gl::*;
use std::time::Instant;

fn main() {
    let now = Instant::now();
    let sdl = sdl2::init().unwrap();
    let video_subsystem = sdl.video().unwrap();

    let gl_attr = video_subsystem.gl_attr();
    gl_attr.set_context_profile(sdl2::video::GLProfile::Core);
    gl_attr.set_context_version(3, 3);

    let window = video_subsystem
        .window("please work", 800, 600)
        .opengl()
        .resizable()
        .build()
        .unwrap();

    let _gl_context = window.gl_create_context().unwrap();
    let _gl =
        gl::load_with(|s| video_subsystem.gl_get_proc_address(s) as *const std::os::raw::c_void);

    let mut event_pump = sdl.event_pump().unwrap();

    unsafe {
        gl::Viewport(0, 0, 800, 600);
        gl::ClearColor(0.3, 0.3, 0.6, 1.0);
    }

    /* SHADER PART START */
    use std::ffi::CString;

    let vert_shader = shader_gl::Shader::from_vert_source(
        &CString::new(include_str!("../assets/triangle.vert")).unwrap(),
    )
    .unwrap();

    let frag_shader = shader_gl::Shader::from_frag_source(
        &CString::new(include_str!("../assets/triangle.frag")).unwrap(),
    )
    .unwrap();

    let texture_vert = shader_gl::Shader::from_vert_source(
        &CString::new(include_str!("../assets/texture.vs")).unwrap(),
    )
    .unwrap();
    let texture_frag = shader_gl::Shader::from_vert_source(
        &CString::new(include_str!("../assets/texture.fs")).unwrap(),
    )
    .unwrap();

    // let shader_program = shader_gl::Program::from_shaders(&[vert_shader, frag_shader]).unwrap();

    let texture_shader_program =
        shader_gl::Program::from_shaders(&[texture_vert, texture_frag]).unwrap();

    // shader_program.set_used();
    // println!("using shader program:{}", shader_program.id());

    let err = unsafe { gl::GetError() };
    println!("OpenGL Error: {}", err);

    /* SHADER PART END */

    let texture_coordinates: Vec<f32> = vec![
        0.0, 0.0, //lower-left
        1.0, 0.0, //lower right
        0.5, 1.0, //top center
    ];

    // let vertices: Vec<f32> = vec![
    // positions
    //     0.5, 0.5, 0.0, // top right
    //     0.5, -0.5, 0.0, // bottom right
    //     -0.5, 0.5, 0.0, // top left
    // second triangle
    // 0.5, -0.5, 0.0, // bottom right
    //     -0.5, -0.5, 0.0, // bottom let
    // ];

    //Texture example
    let vertices: Vec<f32> = vec![
        // positions          // colors           // texture coords
        0.5, 0.5, 0.0, 1.0, 0.0, 0.0, 1.0, 1.0, // top right
        0.5, -0.5, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, // bottom right
        -0.5, -0.5, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, // bottom left
        -0.5, 0.5, 0.0, 1.0, 1.0, 0.0, 0.0, 1.0, // top left
    ];

    let indices: Vec<u32> = vec![
        0, 1, 3, //first
        1, 2, 3, //second
    ];

    let vbo: Vbo = Vbo::init();
    vbo.set(&vertices);

    let vao: Vao = Vao::init();
    vao.set2();
    println!("vao id:{}", vao.id);

    let ebo: Ebo = Ebo::init();
    ebo.set(&indices);

    let txt: texture_gl::Texture = texture_gl::Texture::init();
    txt.set_defaults("assets/wall.png");

    /* DEBUG SECTION START */

    let pos_attr = unsafe {
        gl::GetAttribLocation(
            texture_shader_program.id(),
            CString::new("aPos").unwrap().as_ptr(),
        )
    };
    println!("Position attribute location: {}", pos_attr);
    let mut size = 0;
    unsafe {
        gl::GetBufferParameteriv(gl::ARRAY_BUFFER, gl::BUFFER_SIZE, &mut size);
    }
    println!("VBO Size: {}", size);
    let mut status = 0;
    unsafe {
        gl::GetProgramiv(texture_shader_program.id(), gl::LINK_STATUS, &mut status);
    }
    println!("Shader Program Link Status: {}", status);

    let mut vao_bound = 0;
    unsafe {
        gl::GetIntegerv(gl::VERTEX_ARRAY_BINDING, &mut vao_bound);
    }
    println!("VAO Bound: {}", vao_bound);

    let mut ebo_bound = 0;
    unsafe {
        gl::GetIntegerv(gl::ELEMENT_ARRAY_BUFFER_BINDING, &mut ebo_bound);
    }
    println!("EBO Bound: {}", ebo_bound);

    /* DEBUG SECTION END */

    // unsafe { gl::PolygonMode(gl::FRONT_AND_BACK, gl::LINE) }

    // println!("idk what this is: {:?}",ebo.);
    texture_shader_program.set_used();

    'main: loop {
        for event in event_pump.poll_iter() {
            match event {
                sdl2::event::Event::Quit { .. } => break 'main,
                _ => {}
            }
        }
        ////////RENDER HERE////////
        unsafe {
            gl::ClearColor(1.0, 0.0, 0.0, 1.0);
            gl::Clear(gl::COLOR_BUFFER_BIT);
            // gl::PointSize(10.0);
            // gl::DrawArrays(gl::POINTS, 0, 1);
            // let time_value: f32 = now.elapsed().as_secs_f32();
            // let g_val = time_value.sin();
            //
            // let vertex_color_location = gl::GetUniformLocation(
            //     shader_program.id(),
            //     CString::new("vertexColor").unwrap().as_ptr(),
            // );
            // gl::Uniform1f(vertex_color_location, g_val);
            gl::ActiveTexture(gl::TEXTURE0);
            gl::BindTexture(gl::TEXTURE_2D, txt.id());

            gl::DrawElements(
                gl::TRIANGLES,
                indices.len() as i32,
                gl::UNSIGNED_INT,
                0 as *const _,
            );
            // gl::DrawArrays(gl::TRIANGLES, 0, 3);
        }
        window.gl_swap_window();
        ///////////////////////////
    }
}
