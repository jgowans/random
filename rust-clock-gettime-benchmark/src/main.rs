use std::thread;
use std::time::{Duration, Instant};

fn main() {
    let ten_sec = Duration::from_millis(10000);
    let start = tsc::rdtsc();
    thread::sleep(ten_sec);
    let ten_sec_delta  = tsc::rdtsc() - start;
    println!("ten seconds took: {} tsc ticks", ten_sec_delta);

    let mut instants: [Instant; 1000] = [Instant::now(); 1000];

    let now = Instant::now();
    println!("now: {:?}", now);

    let start = tsc::rdtsc();
    for idx in 0..1000 {
        instants[idx] = Instant::now();

    }
    let instant_delta = tsc::rdtsc() - start;
    println!("instant took {} tsc ticks", instant_delta);
    println!("that's {} µs seconds", ((10 * 1000 * instant_delta) as f64) / (ten_sec_delta as f64));
    for idx in 0..999 {
        println!("{:?}", instants[idx + 1].duration_since(instants[idx]));
    }
}
