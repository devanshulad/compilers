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
       { 
            a@A.func();
            b@B.func();
            b@A.func();
       }
    };
};

Class C inherits B {
    c: A <- new C;
    func(): Int {
        {
            c.func();
            b.func();
            c@A.func();
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
            d@B.func();
            d@A.func();
            e@C.func();
            e.func();
            e@A.func();
        }
    };
};

Class E inherits D {
    f: E <- new E;
    g: B <- new D;
    func(): Int {
        {
            g@B.func();
            g@A.func();
            g@B.func();
            f@E.func();
            f.func();
            f@B.func();
        }
    };
};