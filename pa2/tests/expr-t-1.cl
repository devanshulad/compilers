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

    func_expr (): Bool {
        hello <- false
    };

};