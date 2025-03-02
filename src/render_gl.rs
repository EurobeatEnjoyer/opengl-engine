use gl;
use gl::types::GLuint;
use std;

pub struct Vbo {
    pub id: GLuint,
}

impl Drop for Vbo {
    fn drop(&mut self) {
        self.unbind();
        self.delete();
    }
}

impl Vbo {
    pub fn init() -> Self {
        let mut id: GLuint = 0;
        unsafe { gl::GenBuffers(1, &mut id) }
        Vbo { id }
    }
    pub fn set(&self, data: &Vec<f32>) {
        self.bind();
        self.data(data);
    }

    fn data(&self, vertices: &Vec<f32>) {
        unsafe {
            gl::BufferData(
                gl::ARRAY_BUFFER,
                (vertices.len() * std::mem::size_of::<f32>()) as gl::types::GLsizeiptr,
                vertices.as_ptr() as *const gl::types::GLvoid,
                gl::STATIC_DRAW,
            );
        }
    }

    fn bind(&self) {
        unsafe { gl::BindBuffer(gl::ARRAY_BUFFER, self.id) }
    }

    fn unbind(&self) {
        unsafe { gl::BindBuffer(gl::ARRAY_BUFFER, 0) }
    }

    fn delete(&self) {
        unsafe { gl::DeleteBuffers(1, &self.id) }
    }
}

pub struct Ebo {
    pub id: GLuint,
}

impl Drop for Ebo {
    fn drop(&mut self) {
        self.unbind();
        self.delete();
    }
}

impl Ebo {
    pub fn init() -> Self {
        let mut id: GLuint = 0;
        unsafe { gl::GenBuffers(1, &mut id) }
        Ebo { id }
    }
    pub fn set(&self, data: &Vec<u32>) {
        self.bind();
        self.data(data);
    }

    fn data(&self, indices: &Vec<u32>) {
        unsafe {
            gl::BufferData(
                gl::ELEMENT_ARRAY_BUFFER,
                (indices.len() * std::mem::size_of::<u32>()) as gl::types::GLsizeiptr,
                indices.as_ptr() as *const gl::types::GLvoid,
                gl::STATIC_DRAW,
            );
        }
    }

    fn bind(&self) {
        unsafe { gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, self.id) }
    }

    fn unbind(&self) {
        unsafe { gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, 0) }
    }

    fn delete(&self) {
        unsafe { gl::DeleteBuffers(1, &self.id) }
    }
}

pub struct Vao {
    pub id: GLuint,
}

impl Drop for Vao {
    fn drop(&mut self) {
        self.unbind();
        self.delete();
    }
}

impl Vao {
    pub fn init() -> Self {
        let mut id: GLuint = 0;
        unsafe { gl::GenVertexArrays(1, &mut id) }
        Vao { id }
    }
    pub fn set(&self) {
        self.bind();
        self.setup();
    }

    fn setup(&self) {
        unsafe {
            gl::EnableVertexAttribArray(0);
            gl::VertexAttribPointer(
                0,
                3,
                gl::FLOAT,
                gl::FALSE,
                (3 * std::mem::size_of::<f32>()) as gl::types::GLint,
                std::ptr::null(),
            );
        }
    }

    fn bind(&self) {
        unsafe { gl::BindVertexArray(self.id) }
    }

    fn unbind(&self) {
        unsafe { gl::BindVertexArray(0) }
    }

    fn delete(&self) {
        unsafe { gl::DeleteVertexArrays(1, &self.id) }
    }
}
