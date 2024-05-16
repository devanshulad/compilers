Class Main {
    main(): Int {
        28
    };
};

Class A {
    a: Main <- new Main;
    func(): Int {
       4
    };

    func2(): Int {
        {
            a.func();
            a.maine();
            a.func2();
        }
    };
};