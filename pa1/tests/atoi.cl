(*
   The class A2I provides integer-to-string and string-to-integer
conversion routines.  To use these routines, either inherit them
in the class where needed, have a dummy variable bound to
something of type A2I, or simpl write (new A2I).method(argument).
*)


(*
   c2i   Converts a 1-character string to an integer.  Aborts
         if the string is not "0" through "9"
*)
class A2I {

     c2i(char : String) : Int {
	if char = "0" then 0 else
	if char = "1" then 1 else
	if char = "2" then 2 else
        if char = "3" then 3 else
        if char = "4" then 4 else
        if char = "5" then 5 else
        if char = "6" then 6 else
        if char = "7" then 7 else
        if char = "8" then 8 else
        if char = "9" then 9 else
        { abort(); 0; }  -- the 0 is needed to satisfy the typchecker
        fi fi fi fi fi fi fi fi fi fi
     };

(*
   i2c is the inverse of c2i.
*)
     i2c(i : Int) : String {
	if i = 0 then "0" else
	if i = 1 then "1" else
	if i = 2 then "2" else
	if i = 3 then "3" else
	if i = 4 then "4" else
	if i = 5 then "5" else
	if i = 6 then "6" else
	if i = 7 then "7" else
	if i = 8 then "8" else
	if i = 9 then "9" else
	{ abort(); ""; }  -- the "" is needed to satisfy the typchecker
        fi fi fi fi fi fi fi fi fi fi
     };

(*
   a2i converts an ASCII string into an integer.  The empty string
is converted to 0.  Signed and unsigned strings are handled.  The
method aborts if the string does not represent an integer.  Very
long strings of digits produce strange answers because of arithmetic 
overflow.

*)
     a2i(s : String) : Int {
        if s.length() = 0 then 0 else
	if s.substr(0,1) = "-" then ~a2i_aux(s.substr(1,s.length()-1)) else
        if s.substr(0,1) = "+" then a2i_aux(s.substr(1,s.length()-1)) else
           a2i_aux(s)
        fi fi fi
     };

(*
  a2i_aux converts the usigned portion of the string.  As a programming
example, this method is written iteratively.
*)
     a2i_aux(s : String) : Int {
	(let int : Int <- 0 in	
           {	
               (let j : Int <- s.length() in
	          (let i : Int <- 0 in
		    while i < j loop
			{
			    int <- int * 10 + c2i(s.substr(i,1));
			    i <- i + 1;
			}
		    pool
		  )
	       );
              int;
	    }
        )
     };

(*
    i2a converts an integer to a string.  Positive and negative 
numbers are handled correctly.  
*)
    i2a(i : Int) : String {
	if i = 0 then "0" else 
        if 0 < i then i2a_aux(i) else
          "-".concat(i2a_aux(i * ~1)) 
        fi fi
    };
	
(*
    i2a_aux is an example using recursion.
*)		
    i2a_aux(i : Int) : String {
        if i = 0 then "" else 
	    (let next : Int <- i / 10 in
		i2a_aux(next).concat(i2c(i - next * 10))
	    )
        fi
    };

};

class Main inherits IO {
  main () : Object { 
      let a : Int <- (new A2I).a2i("678987"), 
          b : String <- (new A2I).i2a(678987) in
      { 
        out_int(a) ;
        out_string(" == ") ;
        out_string(b) ;
        out_string("\n"); 
      } 
  } ;
} ;

(*
   This file presents a fairly large example of Cool programming.  The
class List defines the names of standard list operations ala Scheme:
car, cdr, cons, isNil, rev, sort, rcons (add an element to the end of
the list), and print_list.  In the List class most of these functions
are just stubs that abort if ever called.  The classes Nil and Cons
inherit from List and define the same operations, but now as
appropriate to the empty list (for the Nil class) and for cons cells (for
the Cons class).

The Main class puts all of this code through the following silly 
test exercise:

   1. prompt for a number N
   2. generate a list of numbers 0..N-1
   3. reverse the list
   4. sort the list
   5. print the sorted list

Because the sort used is a quadratic space insertion sort, sorting
moderately large lists can be quite slow. 
*)

Class List inherits IO { 
        (* Since abort() returns Object, we need something of
	   type Bool at the end of the block to satisfy the typechecker. 
           This code is unreachable, since abort() halts the program. *)
	isNil() : Bool { { abort(); true; } };

	cons(hd : Int) : Cons {
	  (let new_cell : Cons <- new Cons in
		new_cell.init(hd,self)
	  )
	};

	(* 
	   Since abort "returns" type Object, we have to add
	   an expression of type Int here to satisfy the typechecker.
	   This code is, of course, unreachable.
        *)
	car() : Int { { abort(); new Int; } };

	cdr() : List { { abort(); new List; } };

	rev() : List { cdr() };

	sort() : List { cdr() };

	insert(i : Int) : List { cdr() };

	rcons(i : Int) : List { cdr() };
	
	print_list() : Object { abort() };
};

Class Cons inherits List {
	xcar : Int;  -- We keep the car in cdr in attributes.
	xcdr : List; 

	isNil() : Bool { false };

	init(hd : Int, tl : List) : Cons {
	  {
	    xcar <- hd;
	    xcdr <- tl;
	    self;
	  }
	};
	  
	car() : Int { xcar };

	cdr() : List { xcdr };

	rev() : List { (xcdr.rev()).rcons(xcar) };

	sort() : List { (xcdr.sort()).insert(xcar) };

	insert(i : Int) : List {
		if i < xcar then
			(new Cons).init(i,self)
		else
			(new Cons).init(xcar,xcdr.insert(i))
		fi
	};


	rcons(i : Int) : List { (new Cons).init(xcar, xcdr.rcons(i)) };

	print_list() : Object {
		{
		     out_int(xcar);
		     out_string("\n");
		     xcdr.print_list();
		}
	};
};

Class Nil inherits List {
	isNil() : Bool { true };

        rev() : List { self };

	sort() : List { self };

	insert(i : Int) : List { rcons(i) };

	rcons(i : Int) : List { (new Cons).init(i,self) };

	print_list() : Object { true };

};


Class Main inherits IO {

	l : List;

	(* iota maps its integer argument n into the list 0..n-1 *)
	iota(i : Int) : List {
	    {
		l <- new Nil;
		(let j : Int <- 0 in
		   while j < i 
		   loop 
		     {
		       l <- (new Cons).init(j,l);
		       j <- j + 1;
		     } 
		   pool
		);
		l;
	    }
	};		

	main() : Object {
	   {
	     out_string("How many numbers to sort? ");
	     iota(in_int()).rev().sort().print_list();
	   }
	};
};			    





