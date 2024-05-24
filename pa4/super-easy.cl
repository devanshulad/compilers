class Main inherits IO {
    a: Int <- (4 + 2 + 43 + 1 + 3 - 2 * 3 / 2 + 5);
    --- b: Int <- 1;
    b: Bool <- false;
    (* a: Int;
    b: Bool;*)
    main(): Object {{
        out_int(a);
        out_string("\n");
        if 5 = 5 then out_string("true") else out_string("false") fi;
        out_string("\n");
        if "HI" = "hi" then out_string("true") else out_string("false") fi;
        out_string("\n");
        if true = false then out_string("true") else out_string("false") fi;
        out_string("\n");
        --- if a = b then out_string("true") else out_string("false") fi;
        out_string("\n");
        
        a <- 3;
        out_int(a);
        out_string("\n");
        b <- a < 30;
        bool_det(b);
        out_string("\n");
        b <- true = false;
        bool_det(b); 
        out_string("\n");
    }};
    
    
    bool_det(b: Bool): Object {
        if b then out_string("true") else out_string("false") fi
    };
};