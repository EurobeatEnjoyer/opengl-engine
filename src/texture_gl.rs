use gl::{self, types::GLuint};
use image::ImageReader;

pub struct Texture<'a> {
    id: GLuint,
    location: &'a str,
}

impl<'a> Texture<'a> {
    pub fn init(location: &'a str) -> Self {
        let mut id: GLuint = 0;
        unsafe {
            gl::GenTextures(1, &mut id);
            gl::BindTexture(gl::TEXTURE_2D, id);
        }
        Texture { id, location }
    }
    // TODO: Custom enums for setting manual 2D texture defaults.
    pub fn set_defaults(&self) {
        self.defaults();
    }

    pub fn id(&self) -> u32 {
        self.id
    }
    pub fn location(&self) -> &str {
        self.location
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
        let loc = self.location;

        let f = ImageReader::open(loc)
            .expect("I couldn't open the file broski")
            .decode()
            .expect("I couldnt decode this broski");

        // let colors = f.color();
        // let channel_count = colors.channel_count();

        let data = f.as_rgba8().unwrap();

        if !data.is_empty() {
            unsafe {
                gl::TexImage2D(
                    gl::TEXTURE_2D,
                    0,
                    gl::RGB as i32,
                    data.width() as i32,
                    data.height() as i32,
                    0,
                    gl::RGB,
                    gl::UNSIGNED_BYTE,
                    data.as_raw().as_ptr() as *const gl::types::GLvoid,
                );
            }
        }
    }
}
//
// struct TEXTURE_DATA<'a> {
//     data: &'a Vec<u8>,
//     width: i32,
//     height: i32,
//     channels: i32,
// }
// impl<'a> TEXTURE_DATA<'a> {
//     fn new(d: &'a Vec<u8>, w: i32, h: i32, comp: i32) -> TEXTURE_DATA<'a> {
//         TEXTURE_DATA {
//             data: d,
//             width: w,
//             height: h,
//             channels: comp,
//         }
//     }
// }
