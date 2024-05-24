class Main inherits IO {
    a: Bool;
    b: Int <- 10;
    c: Bool;
    d: Bool;
    main(): Object {{
        d;
       out_int(b);
       foo(10, true);
       bar(10, 12);
    }};

    foo(b: Int, a: Bool): Object {{
        out_string("\n");
        out_int(b);
    }};
    
    bar(b: Int, a: Int): Object {
        {
            out_string("\n");
            out_int(b);
            out_string(" + ");
            out_int(a);
            out_string(" = ");
            out_int(a+b);
            out_string("\n");
        }
    };
};