Class Main {
    main(): Int {
        28
    };
};

Class A {
    d : Bool <- true;
    c : Int <- 5 + 5;
};

Class B inherits A {
    b : Int <- 5;
    z : Int <- y;
    y : Int <- 2;
};

Class C inherits B {
    e : Int <- 12 + 5; 
};
