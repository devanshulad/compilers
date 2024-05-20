class Main {
    b: B;
    main(): Object {{
    b <- new B;
    b. foo() ;
    }};
};


class A {
    i : Int <- 1;
    a: SELF_TYPE <- self;
    foo() : Int {i};
};

class B inherits A {
    j : Int <- 1;
    baz(): Int {i <- 2 + i};
    foo() : Int {
    j <- a.baz() + a@A.foo()
  };
};

(*
class A {
    i : Int <- 1;
    a: SELF_TYPE;
    foo() : Int {i};
};

class B inherits A {
    j : Int <- 1;
    baz(): Int {i <- i + j};
    foo() : Int {{
        a <- new SELF_TYPE;
        j <- a.baz() + a@A.foo();
        j <- 1 / 0;
    }};
};

*)