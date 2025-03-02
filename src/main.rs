mod render_gl;
use crate::render_gl::*;

fn main() {
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

    use std::ffi::{CStr, CString};

    let vert_shader = render_gl::Shader::from_vert_source(
        &CString::new(include_str!("../assets/triangle.vert")).unwrap(),
    )
    .unwrap();

    let frag_shader = render_gl::Shader::from_frag_source(
        &CString::new(include_str!("../assets/triangle.frag")).unwrap(),
    )
    .unwrap();

    let shader_program = render_gl::Program::from_shaders(&[vert_shader, frag_shader]).unwrap();
    shader_program.set_used();
    println!("using shader program:{}", shader_program.id());

    let err = unsafe { gl::GetError() };
    println!("OpenGL Error: {}", err);

    let vertices: Vec<f32> = vec![
        // positions
        0.5, 0.5, 0.0, // top right
        0.5, -0.5, 0.0, // bottom right
        -0.5, 0.5, 0.0, // top left
        // second triangle
        0.5, -0.5, 0.0, // bottom right
        -0.5, -0.5, 0.0, // bottom let
    ];

    let indices: Vec<u32> = vec![
        0, 1, 3, //first
        1, 2, 3, //second
    ];

    let vbo: Vbo = Vbo::init();
    vbo.set(&vertices);

    let vao: Vao = Vao::init();
    vao.set();
    println!("vao id:{}", vao.id);

    let ebo: Ebo = Ebo::init();
    ebo.set(&indices);

    let pos_attr = unsafe {
        gl::GetAttribLocation(shader_program.id(), CString::new("aPos").unwrap().as_ptr())
    };
    println!("Position attribute location: {}", pos_attr);
    let mut size = 0;
    unsafe {
        gl::GetBufferParameteriv(gl::ARRAY_BUFFER, gl::BUFFER_SIZE, &mut size);
    }
    println!("VBO Size: {}", size);
    let mut status = 0;
    unsafe {
        gl::GetProgramiv(shader_program.id(), gl::LINK_STATUS, &mut status);
    }
    println!("Shader Program Link Status: {}", status);

    let mut vao_bound = 0;
    unsafe {
        gl::GetIntegerv(gl::VERTEX_ARRAY_BINDING, &mut vao_bound);
    }
    println!("VAO Bound: {}", vao_bound);

    // println!("idk what this is: {:?}",ebo.);

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
