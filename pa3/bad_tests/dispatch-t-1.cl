Class Main {
    main(): Int {
        28
    };
};

Class A inherits Main{
        (**
    a: A <- not new A;
    b: Bool <- not true;
    x: X <- new A;
    self: Y <- new Y;
    f: SELF_TYPE <- new SELF_TYPE;
    **) 
    func(a: X,
    b: Y,
    c: D): 
    Int {
        3
    };
    func(): X {
       4
    };
(**
    func2(): Int {
        {
            a.func();
            a@A.main();
            a@X.func2();
            func();
            func2();
            x.func();
            f.func4();
        }
    }; **)
}; 