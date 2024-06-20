fn main()
{
  let x = 44;
  {
    let x = 3;
    x + 1;
  println!("{x}");

  };
  println!("{x}");

  // blocks are expressions
  // (list of statements followed by expression) 
  // if no final expression: value is ()
  // println!("{y}");
}
