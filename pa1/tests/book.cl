class Book {
    title : String;
    author : String;
    is_checked_out : Bool;

    init(t : String, a : String) : SELF_TYPE {
        {
            title <- t;
            author <- a;
            is_checked_out <- false;
        }
    };

    check_out() : Bool {
        if is_checked_out then
            false
        else {
            {
                is_checked_out <- true;
                true
            }
        }
    };

    return_book() : Bool {
        if not is_checked_out then
            false
        else {
            {
                is_checked_out <- false;
                true
            }
        }
    };

    get_info() : String {
        concat(concat(title, " by "), author)
    };
};

class User {
    name : String;
    books_checked_out : Int;

    init(n : String) : SELF_TYPE {
        {
            name <- n;
            books_checked_out <- 0;
        }
    };

    check_out_book(book : Book) : Bool {
        if book.check_out() then
            {
                books_checked_out <- books_checked_out + 1;
                true
            }
        else
            false
    };

    return_book(book : Book) : Bool {
        if book.return_book() then
            {
                books_checked_out <- books_checked_out - 1;
                true
            }
        else
            false
    };

    get_status() : String {
        concat(name, concat(" has checked out ", int_to_string(books_checked_out)))
    };
};

class Library inherits IO {
    books : List{Book};
    users : List{User};

    add_book(book : Book) : SELF_TYPE {
        { books <- books@List.append(book); self }
    };

    register_user(user : User) : SELF_TYPE {
        { users <- users@List.append(user); self }
    };

    check_out_book(user : User, book : Book) : Bool {
        user.check_out_book(book)
    };

    return_book(user : User, book : Book) : Bool {
        user.return_book(book)
    };

    print_inventory() : SELF_TYPE {
        {
            out_string("Library Inventory:\n");
            books@List.map((b : Book) : IO => out_string(b.get_info()@IO.concat("\n"))@IO);
            self
        }
    };

    main() : Object {
        let
            b1 : Book <- new Book.init("1984", "George Orwell"),
            b2 : Book <- new Book.init("The Hobbit", "J.R.R. Tolkien"),
            u1 : User <- new User.init("Alice"),
            u2 : User <- new User.init("Bob")
        in
            {
                add_book(b1);
                add_book(b2);
                register_user(u1);
                register_user(u2);

                if check_out_book(u1, b1) then
                    out_string(concat(u1.name, " checked out 1984.\n"))
                else
                    out_string("Failed to check out 1984.\n");

                if return_book(u1, b1) then
                    out_string(concat(u1.name, " returned 1984.\n"))
                else
                    out_string("Failed to return 1984.\n");

                print_inventory();
            }
    };
};
