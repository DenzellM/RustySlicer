fn main() {
    let v = test();
    println!("{v}");
}

fn test() -> i32{
    let mut x : float = -1;
    while(true){
        x+=1;
        if(x / 2 == 3 || x / 2 == 4.5){
            continue;
        }
        println!("{x}");
        if(x == 10){
            break;
        }

    }
    1
}