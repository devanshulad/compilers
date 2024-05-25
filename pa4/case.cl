Class Main inherits IO {
    a: Object;
    main(): Object {
        a <- {
            case 1 of 
                x: Object => out_int(111);
                y: Int => out_int(222);
            esac;
        }
    };
};
