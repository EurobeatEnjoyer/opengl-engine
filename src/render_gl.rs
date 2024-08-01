    use gl;
    use std;
    use std::ffi::{c_void, CStr, CString};
    use gl::types::{self, GLuint, GLenum,GLchar, GLint};

    pub struct VertexArray {
        vao: GLuint,
    }

    impl VertexArray {
        pub fn new() -> Option<Self> {
            let mut vao: gl::types::GLuint = 0;
            unsafe {
                gl::GenVertexArrays(1, &mut vao);
            }
           if vao !=0 {
            Some(Self{vao:vao})
           } else {
            None
           }
        }

        pub fn bind(&self) {
            unsafe {gl::BindVertexArray(self.vao)}
        }

        pub fn clear_bind(&self) {
            unsafe {gl::BindVertexArray(0)}
        }
    }
    #[derive(Clone,Copy)]
    pub enum BufferType {
        Array = gl::ARRAY_BUFFER as isize,
        ElementArray = gl::ELEMENT_ARRAY_BUFFER as isize,
    }
    pub struct VertexBuffer {
        vbo: gl::types::GLuint,
    }
    
    impl VertexBuffer {
        pub fn new() ->Option<Self> {
            let mut vbo: gl::types::GLuint = 0;

            unsafe {gl::GenBuffers(1, &mut vbo);}

            if vbo != 0 {
                Some(Self{vbo:vbo}) 
            } else {
                None
            }
        }

        pub fn buffer_data(ty: BufferType, data: &[u8], usage:gl::types::GLenum) {
           unsafe {
            gl::BufferData(
                ty as gl::types::GLenum,
                data.len().try_into().unwrap(),
                data.as_ptr() as *const gl::types::GLvoid,
                usage 
            )
           } 
        }

        pub fn bind(&self, ty:BufferType) {
            unsafe {gl::BindBuffer(ty as gl::types::GLenum, self.vbo)}
        }

        pub fn clear_bind(ty:BufferType) {
            unsafe {gl::BindBuffer(ty as gl::types::GLenum, 0)}
        }
    }
    pub struct ShaderRenderer {
        program: Program,
    }

    impl ShaderRenderer {
        pub fn create_vertex_buffer_array(vertices: Vec<f32>, draw_type: u32, size: gl::types::GLsizei) {
            let mut vbo: gl::types::GLuint= 0;
            unsafe {gl::GenBuffers(size,&mut vbo)}

            unsafe {
                gl::BindBuffer(gl::ARRAY_BUFFER, vbo);
                gl::BufferData(
                    gl::ARRAY_BUFFER,
                    (vertices.len() * std::mem::size_of::<f32>()) as gl::types::GLsizeiptr,
                    vertices.as_ptr() as *const gl::types::GLvoid,
                    draw_type
                )
            }

        }
    }
    #[derive(Clone)]
    pub struct Program {
        id: GLuint,
    }

    impl Program {
        pub fn from_shaders(shaders: &[Shader]) -> Result<Program, String> {
            let program_id = unsafe { gl::CreateProgram() };

            for shader in shaders {
                unsafe {
                    gl::AttachShader(program_id, shader.id());
                }
            }
            unsafe {
                gl::LinkProgram(program_id);
            }

            let mut success: GLint = 1;
            unsafe {
                gl::GetProgramiv(program_id, gl::LINK_STATUS, &mut success);
            }

            if success == 0 {
                let mut len: GLint = 0;

                unsafe {
                gl::GetProgramiv(program_id, gl::INFO_LOG_LENGTH, &mut len);
                }

                let error = create_whitespace_cstring(len as usize);
                unsafe {
                gl::GetProgramInfoLog(
                    program_id,
                    len,
                    std::ptr::null_mut(),
                    error.as_ptr() as *mut GLchar,
                );
            }
            return Err(error.to_string_lossy().into_owned());
        }
        
        for shader in shaders {
            unsafe { gl::DetachShader(program_id, shader.id());}
        }
        Ok(Program {id: program_id})
    }

    pub fn id(&self) -> GLuint{
        self.id
    }

    pub fn set_used(&self) {
        unsafe { gl::UseProgram(self.id)}
    }
}

    impl Drop for Program {
        fn drop(&mut self) {
            unsafe { gl::DeleteProgram(self.id) }
        }
    }
    
    pub struct Shader {
        id: GLuint,
    }

    impl Shader{

        pub fn from_source(source: &CStr, kind: GLenum) -> Result<Shader, String> {
            let id = shader_from_source(source,kind)?;
            Ok(Shader{id})
        }

        pub fn from_vert_source(source: &CStr) -> Result<Shader, String> {
            Shader::from_source(source, gl::VERTEX_SHADER)
        }
    
        pub fn from_frag_source(source: &CStr) -> Result<Shader, String> {
            Shader::from_source(source, gl::FRAGMENT_SHADER)
        }
    
        pub fn id(&self) -> GLuint {
            self.id
        }
    }

    impl Drop for Shader {
        fn drop(&mut self){
            unsafe {
                gl::DeleteShader(self.id);
            }
        }
    }

fn shader_from_source(source: &CStr, kind: GLenum) -> Result<GLuint, String> {
    let id = unsafe { gl::CreateShader(kind) };
    unsafe {
        gl::ShaderSource(id, 1, &source.as_ptr(), std::ptr::null());
        gl::CompileShader(id);
    }

    let mut success: GLint = 1;
    unsafe {
        gl::GetShaderiv(id, gl::COMPILE_STATUS, &mut success);
    }
    
    if success == 0 {
        let mut len: gl::types::GLint = 0;
        unsafe {
            gl::GetShaderiv(id, gl::INFO_LOG_LENGTH, &mut len);
        } 

        let error = create_whitespace_cstring(len as usize);

        unsafe {
            gl::GetShaderInfoLog(id,len, std::ptr::null_mut(), error.as_ptr() as *mut GLchar);
        }

        return  Err(error.to_string_lossy().into_owned());
    }
    Ok(id)
}

fn create_whitespace_cstring(len: usize) -> CString {
        let mut buffer: Vec<u8> = Vec::with_capacity(len + 1);
        buffer.extend([b' '].iter().cycle().take(len));
        unsafe {
            return CString::from_vec_unchecked(buffer);
        };
    }
