# ICS 45C Homework 4

## Getting the assignment

1. Accept the assignment by clicking the link in Canvas
2. Once you accept the invite, you will reach a page that says "You're ready to go"
3. Click the URL from that page that looks similar to this: `https://github.com/klefstad-teaching/ics-45c-hw4-<YourGitHubUsername>`. It may take a bit of time for the repo to be ready.
4. Click the green `<> Code` Button on the top right, and then click the middle tab `SSH`
5. Copy that link
6. Go to your hub and go into the ICS45C folder, and open up the terminal and type in the following command:
```bash
git clone git@github.com:klefstad-teaching/ics-45c-hw4-<YourGitHubUserName>.git HW4
```
7. Go to VSCode and open up the `HW4` Folder

## Directory Structure

```bash
├── CMakeLists.txt
├── CMakePresets.json
├── gtest
│   ├── gtestmain.cpp
│   ├── string_gtests.cpp
│   ├── standard_main_gtests.cpp
│   └── student_gtests.cpp
├── README.md
└── src
    ├── alloc.cpp
    ├── alloc.hpp
    ├── standard_main.cpp
    ├── string.cpp
    └── string.hpp
```

## Overview and Objectives

As with Homework 3, for Homework 4 you write a `String` class as a wrapper class for C-style strings, but now implement class `String` as **dynamically allocated arrays** instead of the fixed arrays used in Homework 3. `String` **logical lengths** can now vary, and now the physical size of the array which holds the characters inside the `String` will match the logical length (plus one for the null terminator). That is, the `char` array that `buf` is pointing to must be only as large as necessary to hold the current C-string plus the null terminator. The array inside a `String` may change to an array of a different size, for example, with operations such as `operator +=()`, `operator =()`, or `read()`.

Fortunately, we factored out many useful C-string helper functions for Homework 3, and many of these may be reused with our new Homework 4 implementation. However, some interface methods must change to reflect our new implementation.

The provided wrapper class named `AllocationTracker` records and tracks allocations by wrapping C++ operations `new[]` (heap allocation) and `delete[]` (heap deallocation), so that `new[]` and `delete[]` are not called directly, but through these methods and functions that handle the underlying details. `AllocationTracker` makes it possible to see whether calls to `new[]` and `delete[]` are matched, and how many total allocations of heap storage are made.

You will develop and then refine your implementation of class `String` in multiple steps to **eliminate unnecessary allocations**. These steps provide an opportunity to measure and observe the improvement of each type of change we make!

## Relation to Course Objectives

As described in detail in Homework 3 Course Objectives, you learn to implement dynamic allocation of arrays, gain more experience with classes, and become more skilled at test-driven development. You also learn about **move constructor** and **move assignment**, how they work, and when and why they should be used.

## Overview of Classes and Files

 > Be sure to use exactly the names given in these instructions for files, functions, and classes, because the autograder will be expecting exactly those names.

The declaration of class `String` and related functions are in a file named `string.hpp`, while the definitions of the `String` methods and related functions are in a file called `string.cpp`. The provided class `AllocationTracker` declaration is in `alloc.hpp`, and its definition is in `alloc.cpp`.

`standard_main.cpp` offers some initial basic tests for each interface function defined in your classes, and `student_gtests.cpp` contains your extensive testing of your functions. You may also reuse any part of **your own program** from Homework 3 (e.g., `string.cpp` and `standard_main.cpp`) to help you make incremental progress throughout this assignment.

## 1 Class String

## 1.1 string.hpp

In a file named `string.hpp`, declare a `String` class which is implemented as a pointer to C-string. **Note that there are changes to class `String` from Homework 3, so be sure to start with the new `string.hpp` provided here.** (For the remainder of this document, `string.hpp` refers to the new version you implement for Homework 4).

Class `String `has only one data member named buf, which is a pointer to a C-string, which is an array of char.

```cpp
char * buf; // Note the change to char*
```

`string.hpp` must declare all the methods from Homework 3, including constructors, destructor, assignment, equality and relational operators, the index `operator []`, reverse, indexOf, print, and read.

**An additional static helper method**, `strdup()`, allocates the right amount of storage and copies the characters into it to make an exact duplicate of the string passed in as argument, then returns the address of that `new[]` storage.  `strdup()` is useful to implement many methods, including the constructors. `strdup()` can be tricky, because you must think carefully about how many character slots the new array needs. This is the function you should call whenever you want to make a string duplicate. Do not keep calling `new[]` and `strcpy()` separately each time you need to make a string copy!

An additional private helper constructor `String(int length)` creates a String object with an array of a specified size, but the values in the array are not yet initialized. This constructor is useful for implementing methods like `reverse()` and `operator +()` to avoid allocating a C-string for the result more than once.

Some of the methods work using the implementation from Homework 3 (assuming that those worked correctly), but you must determine which ones still work versus which must be re-written to implement dynamic allocation.

 > As before, do not use any of the `C str` functions (e.g. `std::strcmp()`, or `std::strcpy()`); reuse your functions written previously, or write new versions to implement dynamic allocation, as appropriate.

**Two additional interface methods** transfer ownership from a dying `String` to another `String`:

 - **The move constructor** (a third constructor for class `String`):  constructor for the new `String` being born, from a `String` that is about to die
 - **Move assignment:**  assignment into a `String`, from a `String` that is about to die

Copy assignment and move assignment can both be implemented using the new method `String::swap` which simply exchanges the `buf` of `this` with the `buf` of a given `String`. Remember that moved-from objects still have their destructor called. You can use that to leave the clean-up of `this->buf` to the destructor of the moved-from String. To implement `String::swap`, you can use `std::swap` from the `<utility>` header if you want to.

 > Remember from this week’s reading, you don’t want to swap the entire `String` objects; instead, swap the pointers inside the `Strings`, so that the arrays of `char` inside the `Strings` are swapped. Note: if you try to implement the move **constructor** using swap the same way that the reading does, you will likely introduce a memory error - can you see why?

**Wait to define these until step 8 below.  Defining empty or incorrect versions of these will result in fatal errors.**

string.hpp screenshot

![string.hpp screenshot](/assets/1-1-1.png)

## 1.2 string.cpp

In the file named string.cpp, define the methods and functions declared in `string.hpp`, following the [Steps of Development](#steps-of-incremental-development) below. Note that some parameter modes have been changed in `string.hpp`, so ensure your parameter modes in string.cpp match the supplied `string.hpp` for Homework 4.

> ❗`string.cpp` must `#include string.hpp`.

Arrays are dynamically allocated using `new[]` and deallocated using `delete[]`. For Homework 4, when you call `new[]` or `delete[]`, it calls the special version defined in `alloc.hpp/alloc.cpp`, detailed below. Remember how to use `new[]` and `delete[]`:

```cpp
char *P = new char[len]; // returns the address of the new[] storage and saves it in the pointer P 
delete[] P; // frees the storage, where P is a pointer that holds the address of the array to deallocate
```

## 2 Class AllocationTracker

In the provided files `alloc.hpp` and `alloc.cpp`, class `AllocationTracker` contains custom implementations of `new[]` and `delete[]` that track and record information about all heap allocations and deallocations.  

> ❗Do not modify anything in `alloc.hpp` or `alloc.cpp`.

In Homework 4, we use this information to learn two things:

1. Are we matching calls to `new[]` and `delete[]` correctly? and
2. How much do certain language features reduce the number of expensive heap allocations (and thus subsequent data copying)?

`AllocationTracker` objects will be created and called in `standard_main.cpp` at different [Steps of Development](#steps-of-incremental-development), detailed below.

## 3 standard_main.cpp

Call and use your functions in the file `standard_main.cpp`, using `AllocationTracker` functions to report the number of heap allocations, following the [Steps of Development](#steps-of-incremental-development) below. Make the changes step by step, then report the allocations to see how each change reduces the number of unnecessary allocations.

## 4 student_gtests.cpp:  A program to test each function in class String

![comic](/assets/3-1.png)

Write a test program, `student_gtests.cpp`, to thoroughly test each static method defined in class `String`. You may reuse and adapt part or all of your tests from Homework 3 to help you make progress with `student_gtests`. Following the model of Homework 3, `student_gtests` must test nominal and boundary/edge cases. As in Homework 3, tests of non-static member functions should go in `string_gtests.cpp`.

> Note: since there is a new static helper - `String::strdup` - in this homework, make sure to add a new test for this method if you're reusing your `student_gtests` from Homework 3! Use the syntax:

```cpp
TEST(StringFunction, strdup) {
     // Your test of strdup
}
```

> **⚠️You ***NEED*** a decent `student_gtests.cpp` in order to make this Homework 4 work. If you did not write good tests for Homework 3, you will NEED to write them now.**

## Steps of Incremental Development

**Write** class String to use dynamic arrays

1. Be sure to start with the files givenin this repo. The starting point for class `String` has different parameters than in Homework 3. Be sure to define methods and functions in `string.cpp` that match those declared in the Homework 4 `string.hpp`.

> 💡TIP: Make regular commits of your files as you work, in case any change completely breaks your program!

2. Implement `strdup()`, a static helper method in class `String` that is an important helper function in this program. `strdup()` takes a C-string parameter `s` and returns an exact duplicate C-string. The duplicate has its own storage allocated from the heap. `strdup()` must call `new[]`. Use your functions `strlen()` and `strcpy()` from Homework 3 as helper functions to write `strdup()`. All the other static helper methods should be fine to reuse as-is from your implementation of Homework 3, assuming you implemented them correctly.

3. Identify which interface methods of class `String` must be changed in order to change the representation of your class `String` to use a dynamically allocated array of char instead of the fixed `MAXLEN` array of char. For example, constructors and `read()` must change, while operator `==()` and `print()` are the same as in Homework 3 and may be reused.  Helper functions can probably all be reused if they were implemented correctly in Homework 3.

> Following the same one-thing-at-a-time method given in Homework, choose one function (or the smallest possible number of dependent functions) to write at a time, then run (Step 4 below) until it works. Repeat steps 3 and 4 until all methods are written EXCEPT the move constructor and assignment.

4. Run `standard_main`, and `student_gtests` with the `sanitizers`. **The C++ compiler cannot catch memory errors for you. The sanitizers help you by catching your errors and reporting them to you. Carefully read the error messages printed by them to see if you can isolate which function was running when the error was detected. Focus your debugging effort on that function.**

 - The `sanitizers` will catch many kinds of errors:
   - accessing memory that has not been initialized
   - accessing through an invalid pointer, perhaps that has not been initialized
   - memory leaks, which are the result of allocating memory but then failing to free it (deallocate it)
   - redundant deletes, where you attempt to delete the storage for an object that has already been deleted
   - indexing an array out of bounds

5. Now add calls wherever needed to `delete[]` until you eliminate all of your memory leaks.
   
6. **Measure an efficiency baseline**:  After your program is working, use `AllocationTracker` in `standard_main` to report the number of allocations. Take a screenshot of the number of allocations reported by `AllocationTracker::print_allocation_report(cout)`. This screenshot is not submitted, but helps you observe the effects of performance improvements made in the next steps.

To report allocation stats,
    declare an object of type `AllocationTracker`,
    then call `run_test()`,
    then call `AllocationTracker::print_allocation_report(cout)` to see how many allocations a specific version of your program has made.

Here is an example of using `AllocationTracker`:

```cpp
int main() {
    AllocationTracker tracker;
    run_tests();  // these call new[]/delete[]
    tracker.print_allocation_report(std::cout);
}
```

7. **Efficiency improvement 1:** Change all the pass-by-copy/value (`String s` or `const String s`) parameters to pass-by-const-reference (`const String & s`).  Leave all `String & s` parameters as they are, because those must be passed by reference to allow modification to the actual parameter. Compile and run, test until it works. Make another note/screenshot of the number of allocations reported by `AllocationTracker::print_allocation_report(cout)`, which should decrease.
   
8. **Efficiency improvement 2:** Define the move constructor, compile and run, test until it works. (Make another note/screenshot of the number of allocations reported by `AllocationTracker::print_allocation_report(cout)`, which should decrease again.)
    
9. **Efficiency improvement 3:** Define the move assignment operator, compile and run, test until it works. Make a note/screenshot of the number of allocations reported by `AllocationTracker::print_allocation_report(cout)` should decrease again.
    
> Notice that you have eliminated many unnecessary allocations by adding the move assignment and constructor and by changing many value (copy) parameters to `const &` parameters, improving the efficiency greatly by reducing expensive allocations and data copying!

## Grading criteria

The autograder works by actually compiling and running your code with our set of thorough test cases. The points it awards and deducts may not seem linear, because when you have memory-related errors, you may get different results every time you run your program, even though you have not made any changes! If you suddenly introduce an error that causes a segfault, your score may drop to zero. Hopefully, after that error is fixed, your score may jump back to where it was before the segfault.

The autograder is not intended to be your debugger, nor can it award points for all of the good coding practices you should do for this Homework. You should do those good practices anyway, for your own learning. A goal of this course is for you to learn to test your own code at least as thoroughly as, and sometimes more thoroughly than, the autograder. Any time the autograder reveals an error in your code, ask yourself, What test cases can I design to catch and correct that error myself? Use the feedback to improve your testing skill. Designing test cases **while** you approach a coding problem is a core focus of most technical interviews.

Points are allotted for
 - Preventing memory leaks detected by the sanitizers
 - Passing tests of functional correctness from Homework 3
 - Reducing excessive memory allocations by implementing the allocation-reducing techniques, including defining move constructor, move assignment, and using `const&` parameters as much as possible

## Build Instructions

```bash
# Produce the `build` folder with the presets provided for the homework:
cmake --preset default

# NOTE : If you want to use valgrind, use this preset INSTEAD of default:
cmake --preset valgrind

# Build all targets at once:
cmake --build build

# Build only standard_main.cpp:
cmake --build build --target standard_main

# Build only string_main gtests:
cmake --build build --target standard_main_gtests

# Build string and student gtests:
cmake --build build --target string_gtests

# Build student gtests:
cmake --build build --target student_gtests
```

To run the above targets after compiling them:

```bash
./build/standard_main        # Runs the 'main' function from src/standard_main.cpp
./build/standard_main_gtests # Runs the 'standard_main' gtest set of tests
./build/string_gtests        # Runs the 'string' and 'student' gtest sets
./build/student_gtests       # Runs the 'student' gtests
```

Once you have run the code above and it either produces the output you expected or passes
all provided tests, congratulations! You are now ready to [submit](#submission) your homework!

## Submission

As with previous submissions, submit via `GitHub` by the following steps:

1. `git add .`
2. `git commit -a -m "Commit Message Here"`
3. `git push --set-upstream origin main`

to push your changes to your private GitHub repository, and then submit `hw4` to `Gradescope`.

## Credit

All code moved from [ICS45c](https://github.com/RayKlefstad/ICS45c/tree/hw4)
