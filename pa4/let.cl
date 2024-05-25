class Main inherits IO {
    x : Int <- 12;
    m : Main;
    main(): Object {
        let  x: Int <- 50, y: Bool, z: Int <- 111 in
            {
                out_int(z + x);
                out_string("\n");
                out_int(x);
                out_string("\n");
                out_int(foo(x, z));
                out_string("\n");
                let x: Int <- 3, z: Int <- 4 in {
                    out_int(x + z);
                    out_string("\n");
                    out_int(x - z);
                    out_string("\n");
                    out_int(x / z);
                    out_string("\n");
                    out_int(x * z);
                    out_string("\n");
                    out_int(foo(x, z));
                    out_string("\n");
                };
                x <- x + z;
                out_int(x - z);
                out_string("\n");
                out_int(x / z);
                out_string("\n");
                out_int(x * z);
                out_string("\n");
                out_int(foo(x, z));
                out_string("\n");
                y <- x = z;
                out_string("\n");
                if y then out_string("hooray") else out_string("yipee") fi;
            }
    };

    foo (a : Int, z : Int): Int {
        a + z
    };
};