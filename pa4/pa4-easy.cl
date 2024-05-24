class Main inherits IO {
    
    main(): Object 
    {{
        while true loop
        { if true then
            out_string("true\n")
         else
            out_string("false\n")
        fi;
        out_string("the secret number is: ");
        out_int(16 + 2);
        out_string("\n");
        out_string("hello world\n");
        out_int(16 - 2);
        out_string("\n");
        out_int(16 * 2);
        out_string("\n");
        out_int(16 / 2);
        out_string("\n");
        if 2 < 3 then 
            out_string("true\n")
         else
            out_string("false\n")
        fi;

        if 2 <= 2 then 
            out_string("true\n")
         else
            out_string("false\n")
        fi;

        if 3 = 2 then 
            out_string("true\n")
         else
            out_string("false\n")
        fi;
        }
        pool;

    }};
};