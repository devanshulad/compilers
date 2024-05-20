Class Main {
    main(): Int {
        28
    };
};

Class A {
    a: A <- new A;
    c: Int;
    b: Bool;
    func(a: Int, b: Bool): Int {
       4
    };

    func2(): Int {
        {
            a.func(b, b);
        }
    };
};