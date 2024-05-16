Class Main {
    main(): Int {
        28
    };
};

Class A {
    func(): Int {
        new A
    };
};

Class B inherits A {
    func2(): B {
        new A
    };
};