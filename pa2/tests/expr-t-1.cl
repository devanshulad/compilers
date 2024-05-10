class A {
    hello : Bool <- true;
    less : Bool <- 4 < 5;
    lessEqual : Bool <- 4 <= 5;
    mul : Int <- 5 * 7;
    div : Int <- 10 / 5;
    eq : Bool <- 4 = 5;
    eq1 : Bool <- 4 = 4;
    neg : Bool <- ~true;
    nott : Bool <- not true;
    a1 : String <- "abc" + "bcd";

    idCheck : String <- a1;
    id_check_2 : Bool <- eq;
    id_check_3 : Object <- new A;

    mul_paren : Int <- (5 * 5) - (4 * 4);
    mul_paren : Int <- 2 * 5 - 4;
    mul_paren_2 : Int <- 2 * (5 - 4);

    func_expr (): Bool {
        hello <- false
    };

    func_expr_2 (): Int {
        isvoid (2 * 2)
    };

    func_expr_3 (): Bool {
        isvoid false
    };

    func_expr_4 (): Int {
        isvoid (1251 * 124 * false + (true))
    };

    func_expr_5 (): Int {
        new A
    };

    func_expr_6 (): Int {
        new Int
    };

    func_loop_1 (): Bool {
        while true loop true pool
    };

    func_loop_2 (): Bool {
        while (4 * 5) loop (4 - 5) pool
    };

    func_if_1 (): Bool {
        if true then true else true fi
    };

    func_if_2 (): Bool {
        if new A then not true else isvoid false fi
    };

    func_if_3 (): Bool {
        if "hello" = "hi" then 5 <= 6 else ttt <- 6 fi
    };
};