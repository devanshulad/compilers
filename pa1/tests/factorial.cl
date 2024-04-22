class Main inherits IO {
    main(): Object {
        let n: Int <- 5 in
            out_int(factorial(n))
    };

    factorial(n: Int): Int {
        if n = 0 then
            1
        else
            n * factorial(n - 1)
    };
};