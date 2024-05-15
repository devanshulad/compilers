Class Main {
    main(): Int {
        28
    };
};

Class A {
    a : Int <- 4;
};

Class B inherits A {
    b : Int <- 5;
};

Class C inherits B {
    c : Int <- d + 5; 
};

Class D inherits B {
    c : Int <- true + true;
    d : Int <- c + 5;
};