use std::io;
use std::fmt;
use rand::Rng;

pub struct pstack_metadata {
    size: usize,
    numbers: usize,
    stack: Vec <i32>,
}

mod pstack {
    use crate::pstack_metadata;

    pub fn new(size: usize) -> pstack_metadata {
        let tmp = Vec::with_capacity(size);
        pstack_metadata {
            size: size,
            numbers: 0,
            stack: tmp,
        }
    }

    pub fn push(metadata: &mut pstack_metadata, value: i32) -> bool {
        if metadata.size == metadata.numbers {
            return false;
        }

        metadata.stack.push(0);
        metadata.stack[metadata.numbers] = value;
        metadata.numbers += 1;
        true
    }
    
    pub fn pop(metadata: &mut pstack_metadata, value: &mut i32) -> bool {
        if metadata.numbers == 0 {
            return false;
        }

        metadata.numbers -= 1;
        *value = metadata.stack[metadata.numbers];
        true

    }
    
    pub fn show(metadata: &pstack_metadata) -> () {
        println!("stack size is {}", metadata.size);
        for n in (1..=metadata.numbers).rev() {
            print!("element {n}, {}\n", metadata.stack[n - 1]);
        }
        println!("");
    }
}

#[derive(Debug, Copy, Clone)]
enum menu_option {
    PUSH,
    POP,
    SHOW,
    EXIT,
}

#[derive(Debug)]
struct input_error;

impl menu_option {

    fn from_str(s: &str) -> Result<menu_option, input_error> {
        match s {
            "0" => Ok(menu_option::PUSH),
            "1" => Ok(menu_option::POP),
            "2" => Ok(menu_option::SHOW),
            "3" => Ok(menu_option::EXIT),
            _ => Err(input_error),
        }
    }
}

impl fmt::Display for menu_option {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        match *self {
            menu_option::PUSH => write!(f, "0"),
            menu_option::POP => write!(f, "1"),
            menu_option::SHOW => write!(f, "2"),
            menu_option::EXIT => write!(f, "3"),
        }
    }
}

fn main()
{
    let mut input = String::new();
    let mut stack = pstack::new(3);

    loop {
        input.clear();
        print!("\n\n\n");
        println!("Stack operation");
        println!("---------------");
        println!("{}, {:?}", menu_option::PUSH, menu_option::PUSH);
        println!("{}, {:?}", menu_option::POP, menu_option::POP);
        println!("{}, {:?}", menu_option::SHOW, menu_option::SHOW);
        println!("{}, {:?}", menu_option::EXIT, menu_option::EXIT);
        println!("---------------");
        io::stdin().read_line(&mut input).unwrap();
        let option = menu_option::from_str(input.trim()).unwrap();

        match option {
            menu_option::PUSH => {
                //print!("{}\n", line!());
                let mut rng = rand::thread_rng();

                let num: i32 = rng.gen();
                print!("push {}\n", num);

                if pstack::push(&mut stack, num) {
                    print!("sucessful\n");
                } else {
                    print!("fail\n");
                    println!("stack is full");
                }
            },

            menu_option::POP => {
                //print!("{}\n", line!());
                let mut num: i32 = 0;

                if pstack::pop(&mut stack, &mut num) {
                    print!("sucessful\n");
                    print!("pop {}\n", num);
                } else {
                    print!("fail\n");
                    println!("stack is emty");
                }
            },
            
            menu_option::SHOW => {
                /*
                /print!("{}\n", line!());
                for (i, x) in stack.stack.iter().enumerate() {
                    print!("element {i}, {x}\n");
                }
                */
                pstack::show(&stack);
            },

            menu_option::EXIT => {
                //print!("{}\n", line!());
                break;
            },

            _ => print!("Invalid input\n"),
        }
    }
}
