Class Main {
    main(): Int {
        28
    };
};

Class A {
    y : Bool;
    x: Int;
    func(): Object {
        {
            y <- true;
            {
                while y loop {
                    x + 1;
                    ~4;
                    let x: Int <- 5,
                    y: Bool in {
                        x + 5;
                        4 < 5;
                        4 = 5;
                        5 <= 5;
                        y <- true;
                        x + x;
                        if 2 + 3 = 5 then 4 else new A fi; 
                        case y of
                            x : Int => 3 * 3;
                            y : Bool => true;
                        esac;
                    };
                } pool; 
            };
        }
    };
};

Class B inherits A {
    func2(): B {
        { 
            new B;
            let x: Int <- 5,
            y: Bool in {
                x + 5;
                y <- true;
                x + x;
            };
            new B;
        }
    };
};

