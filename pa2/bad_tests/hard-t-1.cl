class A {

};

class A {

};

class B inherits A {

};

class Cons inherits List {
    xcar : Int;
    xcdr : List;
    
    isNil() : Bool { false };
    
    init(hd : Int, tl : List) : Cons {
        {
        xcar <- hd;
        xcdr <- tl;
        self;
        }
        4
    };

};

class NonDistinctParams {
    nonDistinctParams (a: Int, a: Int, a: Int) : Int {5};

    nonDistinctParams (a: Int, a: Int) : String {5};
}

class NonDistinctParams2 {
    self : String;
    a : Int;
    func () : Int {
        {
            a <- "hello";
        }
        5;
    }

    func2 () : Int {
        { /* While loop does not have expr as a bool */
            while new NonDistinctParams loop 
            {
                new NonDistinctParams;
            }
            pool

        }
        5;
    }

    func3 () : Int {
        { /* While loop does not have expr as a bool */
            {
                ;
                new NonDistinctParams;
                ;
            }

        }
        5;
    }
}

class AB {
    let_func (): Int {
        let 
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

    let_func_2 (): Int {
        let 
        x: Int,
        x: Int
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

    y: Int <- 4;

    let_func_3 (): Int {
        let 
        y: Int <- 3,
        x: Int
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

    case_func (): Int {
        case hello of
        esac;
    }

    rando_func (): Int {
        {
            new new;
            new rando;
            isvoid isvoid;
        }
        4;
    }

    type_compare_expr (): Int {
        {
            4 == "hello";
            true == 1;
            ~44;
            ~true;
            ~"hello";
            not 1;
            not true;
            not "boo";
        }
        1;
    }
};

class IO {

};

class Int {

};

class SuperInt inherits Int {};

class String {

};

class SuperString inherits String {};

class Bool {

};

class SuperBool inherits Bool {};