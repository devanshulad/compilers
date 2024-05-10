class A {
    case_func (): Int {
        {
            hello <- 2;
            case hello of
                a : Int => 3 * 3;
                b : Bool => true;
                c : Int => 4 - 2;
            esac;
        }
    };
};