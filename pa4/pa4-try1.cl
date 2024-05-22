class Main {
    d : Int <- 30;
    e : Int <- 35;
    main(): Int {
        1
    };
};

class A inherits Main {
    a : Int <- 15;
    b : Int <- 18;
    c : Int <- 20;
    foo (): Int {
        200
    };
};

class B inherits A {
    f : Int <- 11;
    g : Int <- 345;
};