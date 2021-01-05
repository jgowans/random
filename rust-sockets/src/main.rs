use std::os::unix::net::UnixDatagram;
use std::{thread, time};
use mio::{Events, Poll, Interest, Token};
use uds::nonblocking::UnixSeqpacketConn;
//use uds::UnixSeqpacketConn;
use std::io::ErrorKind;

fn do_sending(socket: UnixSeqpacketConn) {
    let first = "First message".as_bytes();
    let second = "Second message".as_bytes();
    println!("Send first: {:?}", socket.send(first));
    thread::sleep(time::Duration::from_millis(1500));
    println!("Send second: {:?}", socket.send(second));
    thread::sleep(time::Duration::from_millis(1500));
}


fn main() {
    println!("Hello, world! from lib");
    let mut buf = vec![0; 16];
    // Construct a new `Poll` handle as well as the `Events` we'll store into
    let mut poll = Poll::new().unwrap();
    let mut events = Events::with_capacity(1024);

    let (sender, mut receiver) = UnixSeqpacketConn::pair().unwrap();
    thread::spawn(move || do_sending(sender));

    poll.registry().register(&mut receiver, Token(0), Interest::READABLE).unwrap();

    loop {
        poll.poll(&mut events, None).unwrap();

        for event in &events {
            loop {
                println!("Event: {:?}", event);
                println!("Receive: {:?}", receiver.recv(&mut buf));
                println!("Buf: {:?}\n\n", buf);
                thread::sleep(time::Duration::from_millis(500));
            }
        }
    }
}
