Class A {
    try (): Int {
        4
    };

    foo(a: Int, c: Int, b: Bool): Bool {
        true
    };
};

Class B inherits A {
    try (a: Bool): Int {
        4
    };
};

Class C inherits B {
};

Class D inherits C {
    foo(a: Int, b: Bool, c: Bool): Bool {
        true
    };
};

Class Main {
    try (a: Bool, a: Int): Int {
        4
    };

    main(): Bool {
        true
    };
};