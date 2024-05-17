class A {
    case_func (): Int {
        {
            hello <- 2;
            case self of
                Bool : Int => 4 - 2;
                Int : Bool => A;
                b : Bool => true;
            esac;
        }
    };
};

class AB {
    case_func (): Int {
        {
            case hello of
                hello : Int => 4 - 2;
                Int : Int => A;
                hello : Int => 5; 
                b : Bool => true;
            esac;
        }
    };
};

class B {
    case_func (): Int {
        {
            hello <- 2;
            case hello of
            esac;

        }
    };
};

class C {
    case_func (): Int {
        {
            hello <- 2;
            case hello of
                c : Int => 4 - 2
            esac;
        }
    };
};

class D {
    case_func (): Int {
        {
            hello <- 2;
            case hello of

        }
    };
};