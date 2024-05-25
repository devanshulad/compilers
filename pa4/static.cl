Class Main inherits IO {
    main(): Int {
        let c : Int <- 4 in 
        {
            let a : Int <- 5 in 
            {
                let b : Int <- 6 in 
                {
                    out_string("a = ");
                    out_int(a);
                    out_string("\n");
                    out_string("b = ");
                    out_int(b);
                    out_string("\n");
                    out_string("c = ");
                    out_int(c);
                    out_string("\n");
                    out_int(a + b + c);
                };
            };
        };
    };
};

Class A inherits Main {
    a : Int <- 4;
};

Class B inherits A {
    b : Int <- 5;
};

Class C inherits B {
    c : Int <- b + 5; 
    d : Int <- c + b + a;
};