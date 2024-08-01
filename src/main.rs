use render_gl::{BufferType, VertexArray, VertexBuffer};

mod render_gl;

fn main() {
    let sdl = sdl2::init().unwrap();
    let video_subsystem = sdl.video().unwrap();

    let gl_attr = video_subsystem.gl_attr();
    gl_attr.set_context_profile(sdl2::video::GLProfile::Core);
    gl_attr.set_context_version(4, 1);

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

    use std::ffi::CString;

    let vert_shader = render_gl::Shader::from_vert_source(
        &CString::new(include_str!("../assets/triangle.vert")).unwrap(),
    )
    .unwrap();

    let frag_shader = render_gl::Shader::from_frag_source(
        &CString::new(include_str!("../assets/triangle.frag")).unwrap(),
    )
    .unwrap();

    let shader_program = render_gl::Program::from_shaders(&[vert_shader, frag_shader]).unwrap();

    let mut vertices: Vec<f32> = vec![
        // positions        //colors
        -0.5, -0.5, 0.0, 1.0, 0.0, 0.0, // bottom right1
        0.5, -0.5, 0.0, 0.0, 1.0, 0.0, // bottom left
        0.5, 0.5, 0.0, 0.0, 0.0, 1.0, // top
        -0.5, 0.5, 0.0, 1.0, 1.0, 1.0, // top
    ];

    let mut vertices1: Vec<f32> = vec![
        // positions        //colors
        -0.5, -0.5, 0.0, 1.0, 0.0, 0.0, // bottom right1
        0.5, -0.5, 0.0, 0.0, 1.0, 0.0, // bottom left
        0.0, 0.5, 0.0, 0.0, 0.0, 1.0, // top
    ];

    // 3 4 5, 9 10 11, 15 16 17
    // 0 1 2, 6 7 8, 12 13 14
    // 0 1 2, 0 1 2, 0 1 2

    let mut vbo = VertexBuffer::new().unwrap();
    let target = BufferType::Array;
    vbo.bind(target);
    let sliced = unsafe {
        std::slice::from_raw_parts(vertices.as_ptr() as *const u8, vertices.len() * std::mem::size_of::<f32>())
    };
    VertexBuffer::buffer_data(target, sliced, gl::STATIC_DRAW);
    // unsafe {
    //     gl::BufferData(
    //         gl::ARRAY_BUFFER,
    //         (vertices.len() * std::mem::size_of::<f32>()) as gl::types::GLsizeiptr,
    //         vertices.as_ptr() as *const gl::types::GLvoid,
    //         gl::STATIC_DRAW,
    //     );
    // }


    let mut vao = VertexArray::new().unwrap();
    vao.bind();
    unsafe {
        gl::EnableVertexAttribArray(0);
        gl::VertexAttribPointer(
            0,
            3,
            gl::FLOAT,
            gl::FALSE,
            (6 * std::mem::size_of::<f32>()) as gl::types::GLint,
            std::ptr::null(),
        );

        gl::EnableVertexAttribArray(1);
        gl::VertexAttribPointer(
            1,
            3,
            gl::FLOAT,
            gl::FALSE,
            (6 * std::mem::size_of::<f32>()) as gl::types::GLint,
            (3 * std::mem::size_of::<f32>()) as *const gl::types::GLvoid,
        );

        gl::BindBuffer(gl::ARRAY_BUFFER, 0);
        gl::BindVertexArray(0);
    }
    let mut counter = 0;
    'main: loop {
        for event in event_pump.poll_iter() {
            match event {
                sdl2::event::Event::Quit { .. } => break 'main,
                _ => {}
            }
        }
        ////////RENDER HERE////////
        unsafe {
            gl::Clear(gl::COLOR_BUFFER_BIT);
        }
        shader_program.set_used();
        unsafe {
            vao.bind();
            gl::DrawArrays(gl::TRIANGLE_FAN, 0, 4);
        }
        window.gl_swap_window();
        ///////////////////////////
    }
}