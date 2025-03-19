use gl::types::GLsizeiptr;
use gl::{self, types::GLuint, types::GLvoid};
use image;
use stb_image_rust::{c_runtime, stbi_load_from_memory, STBI_rgb_alpha};
use std::fs::File;
use std::io::{BufReader, Read};

pub struct Texture {
    id: GLuint,
}

impl Texture {
    pub fn init() -> Self {
        let mut id: GLuint = 0;
        unsafe { gl::GenTextures(1, &mut id) }
        Texture { id }
    }
    // TODO: Custom enums for setting manual 2D texture defaults.
    pub fn set_defaults(&self, location: &str) {
        self.defaults();
        self.getdata(location);
    }

    pub fn id(&self) -> GLuint {
        self.id
    }

    fn defaults(&self) {
        unsafe {
            gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_S, gl::REPEAT as i32);
            gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_T, gl::REPEAT as i32);
            gl::TexParameteri(
                gl::TEXTURE_2D,
                gl::TEXTURE_MIN_FILTER,
                gl::LINEAR_MIPMAP_LINEAR as i32,
            );
            gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MAG_FILTER, gl::LINEAR as i32);
        }
    }

    fn getdata(&self, location: &str) {
        let f = File::open(location).unwrap();
        let mut reader = BufReader::new(f);
        let mut buffer = Vec::new();
        let success = reader.read_to_end(&mut buffer);
        let mut x: i32 = 0;
        let mut y: i32 = 0;
        let mut comp: i32 = 0;
        let img: *mut u8;

        unsafe {
            img = stbi_load_from_memory(
                buffer.as_mut_ptr(),
                buffer.len() as i32,
                &mut x,
                &mut y,
                &mut comp,
                STBI_rgb_alpha,
            )
        }
        if !img.is_null() {
            unsafe {
                gl::TexImage2D(
                    gl::TEXTURE_2D,
                    0,
                    gl::RGB as i32,
                    x,
                    y,
                    0,
                    gl::RGB,
                    gl::UNSIGNED_BYTE,
                    img as *const GLvoid,
                );
                gl::GenerateMipmap(gl::TEXTURE_2D);
            }
        } else {
            println!("Failed to load texture for some reason");
        }

        print!("texture info width = {x}, height = {y}",);

        unsafe { c_runtime::free(img) }
    }
}
