Class Main {
    main(): Int {
        28
    };
};

Class A {
    a: A <- new A;
    func(): Int {
       4
    };
};

Class B inherits A {
    func2(): Int {
        a.func()
    };
};