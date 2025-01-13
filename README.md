# THIS IS NOT MEANT TO BE PRACTICAL
It's just for fun and to demonstrate how much low-level power you have in C.
It ONLY works on x86-64 architecture. Here be dragons if you're on Windows or Mac, I have no idea if it's os-specific.
It currently asks to map an entire page for every function caller, and the kernel does not optimize this. If you plan on using this at all, I recommend writing an allocator so each caller gets a 23 byte section of a single page. I may patch this in the future. Until then, have fun leaking 4kb per method.

# The implementation:
the core of this is a tiny function in obj.c. `mkcaller(object, function)`, as the name implies creates a caller for the `function` that binds the `object` to it. It returns a clone of the caller template (system-dependent bytecode), allocated inside executable memory.
The function it returns only has three jobs:
- place the `object` in question onto the register `rax`
- place the `function` in question onto the register `r10`
- call `r10`

The object and function are embedded in constants in the bytecode.

The other important factor is a macro defined in obj.h. The `SELF(type)` macro needs to go at the beginning of any method, and has two jobs:
- initialize `self` as a pointer to the specified type.
- use an assembly routine to move `rax` into `self`. (because the caller function places the object onto `rax`, this is where we can expect to find it).
Using this macro keeps the actual implementation abstract, and makes methods easy to create.

# To use:
- create objecttype.c and objecttype.h files (objecttype being whatever type you want to make. In this example, I use `string`).
- in the objecttype.h file, define the struct that your object uses. each method you intend on writing should also be included, as a function pointer matching the signature of the function.
  Doing this first is a good way to map out how you want your object to work. Just make sure any changes you make are reflected here.
- declare an init function under any name (this example just uses the object name with the first letter capitalized).
  These should be the only two declarations in your header. This keeps your namespace crystal clear.
  As a recommendation, include a parameter in your init function that's a pointer to your object type. Instead of allocating new memory, let the caller allocate as they please and write the initialized data into that pointer.
- in the objecttype.c file, after making sure you include the headers for both obj and your custom objecttype, write each method, and declare them all as static. This keeps them from being put into the name pool. Make sure the FIRST line in every function is `SELF(objecttype)`. If you call any functions before this, the method will segfault.
- finally, write your init function. this should provide initial values for each field in the struct, as well as assigning each member field to the relevant function. Make sure you do this as `s->method = mkcaller(s, method)`.
- Now, you should be able to use your object in other files. Make a main.c, include your objecttype.h header, and go to town. compile by linking all three .c files, like `gcc main.c objecttype.c obj.c`.

# To use the test:
assuming you're on the right system, just run `gcc main.c string.c obj.c` and then `./a.out`. You should see the output:
```
  hello
  hellop world
  hello world
  test
  testing, 1 2 3
```
Have fun
