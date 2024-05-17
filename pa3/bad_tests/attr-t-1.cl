Class Main {
    main(): Int {
        a <- 4
    };
};

Class A {
    a : Int <- 4;
    func() : String {
        a <- "hello"
    };
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
    self: D <- new D;
    h: Int <- new SELF_TYPE;
};