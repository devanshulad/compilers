class Main inherits IO {

    main() : Object {
        {
            (*
            out_int(a.func());
            out_string("\n");
            out_int(func());
            out_string("\n");
            out_int(a@A.func());
            out_string("\n");
            out_int(a@B.func());
            out_string("\n");
            *)
            4;
        }
    };

    func() : Int {
        {
        out_int(1234);
        7;
        }
    };
};

class A inherits IO {
    b: Int <- 71;
    temp: Int <- 11111;
    func() : Int {
        {
            while b < 74 loop {
                out_int(b);
                out_string(" ");
                b <- b + 1;
            } pool;
            2;
        }
    };

    do_stuff (x : Int, y : Bool) : Int {
        {
            x = x + 1 * x;
            temp <- x / 2;
            y <- (x = temp * 2);
            if y then out_string("special x is even\n") else out_string("special x is odd\n") fi;
            16;
        }
    };
};

class B {
    c: Bool <- isvoid func(); 
    d: SELF_TYPE;
    func() : Int {
        {
            --- d <- self;
            ---            d.do_stuff(5, c);
            if c then 31 else 41 fi;
        }
    };
};