
pub fn multiple_times(foo: &mut Foo, f: impl Fn(&mut Foo, u64)) {
    foo.c = 1;
    println!("Received a foo: {}", foo.name);
    f(foo, 0);
    f(foo, 1);
}

pub struct Foo {
    pub c: u32,
    name: String,
}

impl Foo {

    pub fn new(name: String) -> Foo {
        Foo { c: 0, name }
    }

    pub fn inc(&mut self) {
        self.c = self.c + 1;
    }

    pub fn say_hello(&mut self) {
        multiple_times(self, |foo, cnt| {
            println!("{} from {}", cnt, &foo.name);
            foo.inc();
        });
    }

}

fn main() {
    let mut foo = Foo::new(String::from("Bob"));
    foo.say_hello();
}
