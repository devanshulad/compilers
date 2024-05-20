Class Main {
    main(): Object {
        new Main
    };

    method1(num : Int) : SELF_TYPE {  -- same
        self
    };

    method2(num1 : Int, num2 : Int) : Main {  -- plus
        (let x : Int in
            {
                x <- num1 + num2;
                (new Main).method1(x);
            }
        )
    };
};
