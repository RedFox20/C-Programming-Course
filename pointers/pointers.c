/**
 * Several examples of dealing with pointers in C99
 * Uses C99 dialect, so compile with -std=gnu99 or -std=c99
 * or via IDE-s Visual Studio 2013 (and higher) / DevC++5.7 (and higher)
 */
#include <stdio.h>  // printf
#include <stdlib.h> // malloc,free,system



/**
 * @note The number of examples is substantial and requires quite a bit of code walking
 *       to understand some of the more subtle pointer hacks.
 *       Some stepping through with a debugger will also be required. You have been warned.
 */



typedef struct _vector2 {
    int x, y;
} vector2;                   // a simple 8-byte struct definition


char    buffer[128];         // a regular byte buffer
int     intArray[10];        // an array of 10 integers
vector2 vectorArray[4];      // an array of 4 vector2's






/**
 * Part 1 - Declaring pointers, basic use of pointers
 */
void swap(int* a, int* b);
void part1()
{
    // declaring pointers:
    // type * varname;
    int* iptr;

    // declaring multiple pointers:
    // type *var1, *var2;
    int *ptr1, *ptr2;
    int var1 = 5, var2 = 10; // initializing some vars


    // assigning a pointer
    iptr = &var1;         // get the address of var1
    iptr = &intArray[0];  // get the address of the first element of intArray
    iptr = intArray;      // get the address of intArray, which is also the address of the first element


    // dereferencing a pointer - getting the value pointed by the address
    printf("value   = %d\n", *iptr);  // prints out the value at address @iptr
    printf("address = %d\n", (int)iptr);   // prints out the numerical value of the address
    printf("address = %p\n", iptr);   // prints out the hexadecimal value of the address


    // dereferencing and assigning value - setting the value at the pointed address
    *iptr = 5;


    printf("var1 = %2d    var2 = %2d\n", var1, var2);
    swap(&var1, &var2);   // passing pointers of variables to swap
    printf("var1 = %2d    var2 = %2d\n", var1, var2);
}
void swap(int* a, int* b) // swaps the values of 2 variables
{
    int temp = *a;   // get the value from address @a
    *a = *b;         // assign value from address @b to address pointed by @a
    *b = temp;       // assign the temp value to address pointed by @b
}






/**
 * Part 2 - Basic pointer arithmetics
 */
void printArray() // prints out all elements of intArray
{
    int i, size;
    // Using sizeof() on a static array always gives us the size of the array
    // in bytes. Using sizeof on a type, gives us the size of that type in bytes.
    // 
    // Warning! Using sizeof() on a regular int* variable, will NOT give us the
    // sizeof array, but the sizeof an int*, which is 4 on a 32-bit system.
    //
    size = sizeof(intArray) / sizeof(int); // calculate the number of elements


    // if intArray has 10 elements, then size evaluates to:
    // size = 40 / 4; // (10)
    // the value 20 is resolved during compilation, so it might as well be:
    // size = 10;
    printf("int[%d] { ", size);
    for (i = 0; i < size; i++)
        printf("%d, ", intArray[i]);
    printf("}\n");
}
void part2()
{
    int *iptr;
    int i = 0;

    iptr = intArray; // get the value intArray, which is already an int*


    // initialize first 5 elements of intArray
    while (i < 5)     // 0..4
    {
        *iptr = i;    // assign the value of i to the address pointed by @iptr
        iptr += 1;    // increment the pointer @iptr, the address number increases by sizeof(int)
        i++;          // advance the loop
    }

    printArray();


    // initialize elements 5..9 of intArray
    while (i < 10)      // 5..9
    {
        *iptr++ = i++;  // exactly the same as previous loop, but more compact
    }

    printArray();
}




void my_strcpy1(char* dst, const char* src)
{
    while (*src != 0)
    {
        *dst = *src;  // copy from src to dst

        ++dst, ++src; // increment pointers
    }
}



void my_strcpy2(char* dst, const char* src)
{
    while (*dst = *src) // copy from src to dst
    {
        ++dst, ++src; // increment pointers
    }
}



void my_strcpy3(char* dst, const char* src)
{
    while (*dst++ = *src++) // copy from src to dst, increment pointers
        ;
}







/**
 * Part 3 - Pointer casting, advanced pointer arithmetics
 */
void printVectors() // prints out all 4 vectorArray elements
{
    int i, size;
    vector2* vp = vectorArray;
    size = sizeof(vectorArray) / sizeof(vector2); // calculate the number of elements in vectorArray

    printf("vec2[%d] { ", size);
    for (i = 0; i < 4; i++) { // 0..3
        printf("[%d,%d] ", vp->x, vp->y);   // !! struct pointer subelement access uses '->' !!
        vp++;                               // advance the address by sizeof(vector2)
    }
    printf("}\n");
}
void part3()
{
    int *iptr, *eptr, i;
    iptr = intArray;                         // start of intArray
    eptr = (char*)iptr + sizeof(intArray);  // calculate the end of array
    i = 0;

    while (iptr != eptr)  // while iptr address is not eptr address
        *iptr++ = i++;    // initialize intArray variables

    // pointer hacking
    iptr = vectorArray;                        // start of vectorArray instead
    eptr = (char*)iptr + sizeof(vectorArray); // calculate the end of array
    i = 0;

    while (iptr != eptr)
        *iptr++ = i++; // this time, vectorArray elements are initialized

    printVectors(); // display the resulting vector
}






/**
 * Part 4 - Pointer recasting, struct hacking
 */
void part4()
{
    vector2 *vptr, v;

    // getting the pointer to an element of vectorArray[]:
    vptr = vectorArray + 1;  // pointer syntax
    vptr = &vectorArray[1];  // array syntax
    vptr = &1[vectorArray];  // antique syntax (rarely used)

    // getting the value of an element of vectorArray[]:
    v = vectorArray[1];      // array syntax
    v = *(vectorArray + 1);  // pointer syntax


    // pointer recasting: reinterpreting the pointer type
    vptr = ((int*) vectorArray) + 1; // vectorArray re-interpreted as an integer array
    vptr->x = 100;    // vectorArray[0].y = 100
    vptr->y = 200;    // vectorArray[1].x = 200


    // something even more powerful:
    vptr = (vector2*)buffer;

    printVectors(); // prints out all the vectors

    return;
}






/**
 * Part 5 - function pointers, typedefing function pointers
 */
int myIntFunction(int arg) // a random function to be used in fn-pointer example
{
    static int sint = 0;
    printf("myIntFunction() => %d\n", sint);
    return sint += arg;
}
void part5()
{
    // syntax: "return_type (*fnptr_var_name)( argument types(if any), ... );"

    int(*func)(int);              // regular function pointer variable declaration
    typedef int(*int_func)(int);  // typedef'ing a function pointer

    int_func tdfunc;              // using the typedef to declare the function pointer variable

    func = &myIntFunction;        // initializing function pointers
    tdfunc = &myIntFunction;

    func(2);                      // calling the function pointers like regular functions
    tdfunc(3);
}






/**
 * Part 6 - arrays, multidimensional arrays, arrays of pointers to arrays
 */
void part6()
{
    // static multidimensional arrays are laid out sequentiall in memory
    int a[10];              // [10 ints]            // regular array
    int marr[10][10];       // [10 ints][10 ints]   // multidimensional array


    // non-static multidimensional arrays are just arrays of pointers to arrays
    // or arrays of arrays of pointers; in which case, the layout in memory varies
    int(*ptr2arr[10])[10];  // [10 int*][10 ints]    // array of pointers to arrays.
    // !! pointers not initialized !!


    int* arr2ptr[10][10];   // [10 ints][10 int*]   // array of arrays of pointers
    // !! pointers not initialized !!


    int i, *iptr;
    for (i = 0; i < 10; i++) // initialize all ptr2arr pointers
        ptr2arr[i] = &marr[i];

    iptr = (int*) ptr2arr[0]; // getting the pointer to the first array of ptr2arr is easy
}






/**
 * Part 7 - Object-oriented programming by using struct pointers
 */

typedef struct _ivector { // an integer vector - a dynamic array
    int  size;            // that changes its size on demand
    int  capacity;
	int* data;
} ivector;


ivector* iv_new() // initializes an existing vector
{
    ivector* iv = malloc(sizeof(ivector));
    iv->size     = 0;   // no elements yet
    iv->capacity = 0;   // mark the initial capacity
	iv->data     = NULL;// lazy init array data
    return iv;
}
void iv_free(ivector* iv) // destroys the vector and frees any allocated data
{
	if (iv) 
	{
		if (iv->data) 
			free(iv->data); // free array data
		free(iv);           // destroy this object
	}
}
void iv_add(ivector* iv, int item) // adds an item to the end of the vector
{
    if (iv->size == iv->capacity) // grow needed?
    {
        iv->capacity += 4 + iv->capacity / 2; // amortized increase
        iv->data = realloc(iv->data, sizeof(int) * iv->capacity);
    }
    iv->data[iv->size++] = item; // append the item
}



void part7()
{
    printf("--------------------------\n");
    printf("ivector test:\n");

    ivector* iv = iv_new();  // allocate a new vector
    iv_add(iv, 10);          // add a few items
    iv_add(iv, 20);
    iv_add(iv, 30);

    for (int i = 0; i < iv->size; ++i) // print out added items
        printf("ivec[%d] = %d\n", i, iv->data[i]);

    iv_free(iv); // make sure to free any allocated memory
}






int main(int argc, char** argv)
{
    part1();
    part2();
    part3();
    part4();
    part5();
    part6();
    part7();

	#if _MSC_VER
		system("pause");
	#endif
    return 0;
}

