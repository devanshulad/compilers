Class Main {
    main(): Int {
        28
    };
};

Class A {
    a : Int <- 4;
};

Class B inherits A {
    b : Bool <- false;
};

Class C inherits B {
    c : Int <- b + a;
};