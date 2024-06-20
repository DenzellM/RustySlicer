fn main() {
    let v = test();
    println!("{v}");
}

fn test() -> i32{
    let mut x = 0;
    while(true){
        if(x / 2 == 3){
            return x
        }else{
            println!("else")
        }
        x += 1
    }
    1
}