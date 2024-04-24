class A {
    hello : Bool <- true;

    func_dispatch_1 (): Bool {
        hello ()
    };

    func_dispatch_2 (): Bool {
        hello (43)
    };

    func_dispatch_3 (): Bool {
        hello (43, "hello")
    };

    func_dispatch_4 (): Bool {
        hello (true, "h", 124124, 777, new A)
    };

    func_dispatch_at_1 (): Bool {
        (3+4) @ Int.hello()
    };

    func_dispatch_at_2 (): Bool {
        (true) @ Bool.hwg(33)
    };

    func_dispatch_at_3 (): Bool {
        5 < 8 @ Bool.hwg(33, true, "hello", new A)
    };

    func_dispatch_at_4 (): Bool {
        new A @ Object.hello (true, "h", 124124, 777, new A)
    };

    func_dispatch_dot_1 (): Bool {
        new A .hello()
    };

    func_dispatch_dot_2 (): Bool {
        (2 * 19).hel("hello", "haghwp")
    };

    func_dispatch_dot_3 (): Bool {
       5 = 11 .yay(20, true, "hi", 11, new Bool)
    };

    func_dispatch_dot_4 (): Bool {
        not true.bye (true, "h", 124124, 777, new A)
    };
};