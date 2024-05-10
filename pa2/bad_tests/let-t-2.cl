class A {
    let_func (): Int {
        let 
            x : Int <- 1,,
            z : Bool <- false,
        in 
        {
            hello <- 2;
            case hello of
                x : Int => 3 * 3;
                y : Bool => true;
                yz : Int => 4 - 2;
            esac;
        }
    };
};


class A {
    let_func (): Int {
        let 
            , x : Int <- 1
        in 
        {
            hello <- 2;
            case hello of
                x : Int => 3 * 3;
            esac;
        }
    };
};

class A {
    let_func (): Int {
        let 
            Int : Int,
            Bool : Bool,
            x : Int
        in 
        {
            hello <- 2;
            case hello of
                x : Int => 3 * 3;
            esac;
        }
    };
};

class A {
    let_func (): Int {
        let 
            x : Int, 
            yz : String 
            zz : String,
            yyyy : yyyy
        in 
        {
            hello <- 2;
            case hello of
                x : Int => 3 * 3;
                y : Bool => true;
                yz : Int => 4 - 2;
            esac;
        }
    };
};

class A {
    let_func (): Int {
        let 
            x : Int, 
            x : Int,
            IO: IO,
            yz : String <- "hello",
        in 
        {
            hello <- 2;
            case hello of
                x : Int => 3 * 3;
                y : Bool => true;
                yz : Int => 4 - 2;
            esac;
        }
    };
};

class A {
    let_func (): Int {
        let 
            x : Int
            x : Int 
            IO: IO 
            yz : String <- "hello"
        in 
        {
            hello <- 2;
            case hello of
                x : Int => 3 * 3;
                y : Bool => true;
                yz : Int => 4 - 2;
            esac;
        }
    };
};

class A {
    let_func (): Int {
        let 
            x : Int; 
            x : ; Int
            IO: new IO,
            yz : String <- "hello"
        in 
        {
            hello <- 2;
            case hello of
                x : Int => 3 * 3;
                y : Bool => true;
                yz : Int => 4 - 2;
            esac;
        }
    };
};