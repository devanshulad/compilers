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
};

Class C inherits B {
    b: B <- new B;
    func(): Int {
        {
            b@B.func();
        }
    };
};
