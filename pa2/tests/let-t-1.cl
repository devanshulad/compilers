class A {
    let_func (): Int {
        let 
            x : Int <- 1, 
            y : Bool <- true,
            z : Bool <- false,
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
            x : Int <- 1
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
            y : Bool,
            z : Bool,
            yz : String 
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
