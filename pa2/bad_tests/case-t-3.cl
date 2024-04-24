class A {
    case_func (): Int {
        {
            hello <- 2;
            case hello of
                Int : Int => 3 * 3;
                b : Bool => true;
                Bool : Int => 4 - 2;
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