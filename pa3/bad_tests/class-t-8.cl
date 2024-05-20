class Main {
    main(): Int {
        4
    };
};

class A inherits Int {
};

class B inherits IO {

};

class D inherits String {

};

class IO {

};

class SELF_TYPE {};

class Int inherits Bool {
};

class C inherits SELF_TYPE {
    f(): SELF_TYPE {
        self
    };
};