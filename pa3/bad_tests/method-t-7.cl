Class A {
    try (): Int {
        4
    };

    foo(a: Int, c: Int, b: Bool): Bool {
        true
    };
};

Class B inherits A {
    foo(a: Int, b: Int, c: Bool): Int {
        5
    };
};

Class C inherits B {
    foo(a: Int, b: Int, c: Bool): Int {
        5
    };
    try (a: Int): Int {
        4
    };
    try2 (): Int {
        3
    };
};

Class D inherits A {
    foo(a: Int, b: Int, c: Bool): Int {
        5
    };
    try2 (a: Int): Int {
        3
    };
};

Class Main {
    try (): Int {
        4
    };

    main(): Bool {
        true
    };
};