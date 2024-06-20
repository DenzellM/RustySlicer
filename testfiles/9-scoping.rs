fn main() {
    let mut v = 5;
    test(false, v);
    println!("{v}");
}

fn test(i : i32){
    let v : i32 = true - i;
    println!("{v}");
}