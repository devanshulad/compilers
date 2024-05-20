Class Main {
    x : Int <- 0;
    main(): Object {
        {
            case new YXX of
                y: Object => x + x;
                bv: Bool => x + x;
                z: Z => bv + bv;
            esac;
        }
    };
};
