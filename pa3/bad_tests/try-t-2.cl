Class Main {
    main(): Int {
        28
    };
};

Class A {
    a : Int <- 4;
};

Class B inherits A {
    b : Bool <- true;
};

Class C inherits B {
    c : Int <- b + 5; 
    d : Int <- c + b + a;
};