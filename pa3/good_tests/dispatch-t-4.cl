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
    b: B <- new B;
    func(): Int {
        a.func()
    };
};

Class C inherits B {
    c: A <- new C;
    func(): Int {
        {
            c.func();
            b.func();
            a.func();
            a.func();
        }
    };
};

Class D inherits B {
    d: D <- new D;
    e: C <- new C;
    func(): Int {
        {
            d.func();
            b.func();
            e.func();
            a.func();
            d.func();
        }
    };
};

Class E inherits D {
    f: E <- new E;
    g: B <- new D;
    func(): Int {
        {
            g.func();
            f.func();
            e.func();
            a.func();
            b.func();
        }
    };
};