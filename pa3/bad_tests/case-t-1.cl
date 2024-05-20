class Main {

	main(): Int {
	    4
	};

    a : Object <- {
        case self of 
            x: SELF_TYPE => true;
            self: Object => false;
            y: String => self;
        esac;
    };
};