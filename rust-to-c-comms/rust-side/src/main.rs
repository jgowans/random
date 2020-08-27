use std::str;
use std::os::unix::net::UnixDatagram;
use std::os::unix::io::FromRawFd;
use std::ffi::{c_void, CStr};
use std::os::raw::c_char;

#[repr(C, packed)]
#[derive(Debug, Copy, Clone)]
struct Bob {
    x: u64,
    y: u64,
    pub c_string: [u8; 32],
    z: u64,
}


fn main() {
    println!("Hello, world!");
    let socket;
    unsafe {
        socket = UnixDatagram::from_raw_fd(4);
    }

    let mut buf = vec![0; 100];

    match socket.recv(buf.as_mut_slice()) {
        Ok(byte_cnt) => {
            println!("Byte count: {}", byte_cnt);
            let bob_raw = buf.as_ptr() as *const Bob;
            let bob: Bob = unsafe { *bob_raw };
            match str::from_utf8(&bob.c_string) {
                Ok(msg) => println!("Received : {}", msg),
                Err(e) => println!("You talking this kak to me? {:?}", e),
            }
            println!("Bob: {:?}", bob);
        }
        Err(e) => println!("Error getting vmm message: {:?}", e),
    }
}
