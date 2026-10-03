# Pointers in C

## Part 1: A pointer is also a variable

Sometimes when we learn pointers, we focus so much on the address they store that we forget something basic: **a pointer is also a variable. It has its own address.**

I want to start here because it helps separate three expressions that can look confusing at first: `p`, `&p` and `*p`. They do different jobs.

I made this note while studying [Working with pointers on YouTube](https://www.youtube.com/watch?v=X1DcpcgSUXw). The screenshot below comes from that lesson. I am using it as a reference for the explanation and do not claim the original illustration as my own work.

<p align="center">
  <img src="../images/pointer-variable-memory.png" alt="Lesson screenshot illustrating integer a at example address 204 and pointer p at example address 64. The pointer stores 204 and dereferencing it accesses a." width="1000">
  <br>
  <sub>Source: <a href="https://www.youtube.com/watch?v=X1DcpcgSUXw">Working with pointers</a>. The numbers 64 and 204 illustrate memory addresses.</sub>
</p>

### Two variables and two addresses

Let me write the example in C with the variables initialized before use:

```c
int a = 5;
int *p = &a;
```

There are two variables here. `a` holds an integer value. `p` holds a pointer to an integer. The initializer `&a` gives `p` the address of `a`.

For the picture, imagine `a` is located at address 204 and `p` is located at address 64. The value stored in `a` is initially 5. The pointer value stored in `p` identifies address 204.

**The address stored inside `p` is not the address of `p` itself.** In this example, the stored address is 204 while the pointer variable's own address is 64.

These are teaching numbers. The actual addresses depend on your program and its execution environment. Do not assume a particular address, spacing between variables or pointer size from this drawing.

### What does `p` mean?

Using `p` as a value gives the pointer value stored in that variable. After `p = &a`, it points to `a`.

In the picture, that is address 204. Reading `p` does not read the integer 5. It gives the pointer that tells us where that integer is located.

### What does `&p` mean?

The `&` operator obtains the address of an object. `&a` gives the address of `a`. In the same way, **`&p` gives the address of the pointer variable itself**.

In the picture, `&p` corresponds to address 64. Because `p` has type `int *`, the expression `&p` has type `int **`, meaning pointer to pointer to int. We will build on that idea later. For now, remember that it identifies where `p` itself lives.

### What does `*p` mean?

Here the star dereferences the pointer. It accesses the integer object that `p` points to. Since `p` points to `a`, reading `*p` gives the value currently stored in `a`.

```c
int a = 5;
int *p = &a;

int value = *p;  // value receives 5.
*p = 8;         // a now holds 8.
```

The assignment `*p = 8` changes `a`. It does not change the pointer value stored in `p`. The pointer still points to `a` and its own address is unchanged.

This also explains the screenshot. Its written steps first show a value of 5, then assign 8 through `*p`. The memory drawing shows `a = 8` after that assignment. Those are different moments in the example.

### The star has different roles in these two places

Compare the declaration with the later expression:

```c
int *p = &a;  // Declaration: p is a pointer to int.
*p = 8;      // Expression: access the pointed integer and assign 8.
```

In the declaration, `*` is part of the declarator and tells C that `p` is a pointer. It is not dereferencing anything there. The declared variable is named `p`, not `*p`.

In the expression, unary `*` is the indirection operator, usually called dereferencing. It accesses the object through the pointer. These are the two roles relevant to this example. In other expressions, a star between two operands can also mean multiplication.

### Compare them side by side

After `int a = 5; int *p = &a;`:

| Expression | What it identifies or supplies | C type | Illustration |
| --- | --- | --- | --- |
| `a` | The integer object; reading it gives its value | `int` | 5 |
| `&a` | Address of a | `int *` | Address 204 |
| `p` | The pointer variable; reading it gives its stored pointer | `int *` | Address 204 |
| `&p` | Address of the pointer variable | `int **` | Address 64 |
| `*p` | The integer object reached through p; reading it gives its value | `int` | 5 |

After `*p = 8`, reading `a` or `*p` gives 8. The addresses shown in the table do not change because of that assignment.

### Try it on your computer

This complete example prints the actual addresses instead of assuming the numbers in the picture:

```c
#include <stdio.h>

int main(void)
{
    int a = 5;
    int *p = &a;

    printf("p  = %p\n", (void *)p);
    printf("&a = %p\n", (void *)&a);
    printf("&p = %p\n", (void *)&p);
    printf("Before: a = %d, *p = %d\n", a, *p);

    *p = 8;

    printf("After:  a = %d, *p = %d\n", a, *p);
    return 0;
}
```

`%p` prints a pointer value and requires a `void *` argument, which is why the casts appear in those calls. `%d` prints an `int` value.

The lines for `p` and `&a` should show the same pointer value. `&p` identifies the separate pointer variable. The final two lines show the integer changing from 5 to 8 through `*p`.

Before dereferencing a pointer, make sure it points to a suitable live object. The examples above initialize `p` with `&a` before using `*p`. Declaring `int *p;` alone does not give a local pointer a usable target and a null pointer must not be dereferenced.

The distinction I want to remember is this: **`p` gives the stored pointer, `&p` gives the pointer variable's own address and `*p` accesses the object it points to.**

## Part 2: What happens when I add 1 to a pointer?

Now I want to understand what `p + 1` actually means. It looks like ordinary addition but there is one detail to remember: **a pointer takes steps based on the type it points to.**

<p align="center">
  <img src="../images/pointer-arithmetic-lesson.png" alt="Lesson screenshot introducing pointer arithmetic with an int pointer and illustrated address changes. See the corrected array example below before trying the code." width="1000">
  <br>
  <sub>Lesson screenshot supplied for these notes. Learning reference: <a href="https://www.youtube.com/watch?v=X1DcpcgSUXw">Working with pointers</a>. The original lesson image is not my own work.</sub>
</p>

### Think of the next element, not the next byte

Suppose I have three integers next to each other in an array:

```c
int numbers[3] = {10, 20, 30};
int *p = &numbers[0];
```

`p` points to the first integer. `p + 1` points to the next integer. `p + 2` points to the integer after that.

If one `int` takes 4 bytes on this system, one step covers 4 bytes. Two steps cover 8 bytes. C handles that step size for me because `p` has type `int *`.

| Expression | Where it points | Distance from the first element when an int is 4 bytes |
| --- | --- | --- |
| `p` | `numbers[0]` | 0 bytes |
| `p + 1` | `numbers[1]` | 4 bytes |
| `p + 2` | `numbers[2]` | 8 bytes |

The screenshot uses 2002 and 2010 to illustrate a difference of 8 bytes. Those are just teaching numbers. They are not addresses to type into your program. Real addresses also have to meet the system's alignment requirements.

**Adding 1 to an `int *` does not mean adding 1 to the integer stored there. It means moving one int forward.** The size of an `int` is not guaranteed to be 4 bytes on every system. `sizeof(int)` tells us its size on the system we are using.

### Does writing `p + 1` change p?

No. The expression calculates another pointer value but leaves `p` unchanged.

```c
int *next = p + 1;  // next points to numbers[1].
                   // p still points to numbers[0].
```

If I write `p = p + 1;` or `p++;`, I change the pointer stored in `p` itself. After that, it points to the next element.

That is different from this:

```c
*p = *p + 1;
```

This line changes the integer that `p` points to. If `p` still points to the first element, that element changes from 10 to 11. The pointer stays where it was.

### What does *(p + 1) give me?

The parentheses calculate the pointer to the next element. The star then reads the integer at that location.

For the original array `{10, 20, 30}`:

```c
*p        // 10
*(p + 1)  // 20
*(p + 2)  // 30
```

Keep these two expressions separate in your mind:

| Expression | Meaning for the original array |
| --- | --- |
| `*p + 1` | Read 10 and add 1. The result is 11. |
| `*(p + 1)` | Move to the next element and read it. The result is 20. |

Neither expression changes the array by itself. An assignment is needed to store a new value.

### A few corrections before copying the screenshot

The picture helps explain the step size but I would not copy its code as it is.

1. **Print pointers with `%p`.** Pass the pointer cast to `void *`. `%d` is for an `int`, not a pointer.
2. **Print `sizeof(int)` with `%zu`.** The result of `sizeof` has type `size_t`.
3. **Keep the label and expression consistent.** The last label says `p+1` but the expression is `p+2`.
4. **Use an array for this example.** The picture points to a single local integer and then calculates `p + 2`. That calculation goes outside the range C allows for that object, even if it is never dereferenced.

For this rule, C treats a single integer as an array with one element. You can form a pointer one past it but you cannot read or write through that pointer. You cannot keep stepping beyond it just because another variable might happen to be nearby.

With our three element array, `p + 3` is the allowed position just past the end. We must not dereference it. `p + 4` goes beyond the allowed range. For reading our array, stay with `p`, `p + 1` and `p + 2`.

### Try this corrected example

```c
#include <stdio.h>

int main(void)
{
    int numbers[3] = {10, 20, 30};
    int *p = &numbers[0];

    printf("Size of one int: %zu bytes\n", sizeof(int));
    printf("p:     %p\n", (void *)p);
    printf("p + 1: %p\n", (void *)(p + 1));
    printf("p + 2: %p\n", (void *)(p + 2));

    printf("*p:       %d\n", *p);
    printf("*(p + 1): %d\n", *(p + 1));
    printf("*(p + 2): %d\n", *(p + 2));

    return 0;
}
```

The addresses depend on your program's run. The three printed values should be **10, 20 and 30**. On a system with 4 byte integers, the element addresses will be 4 bytes apart.

What I want to remember is simple: **the pointer type tells C how big one step is. The array tells me how far I am allowed to go.**

## Part 3: Same integer, different ways to read it

Look at this picture. The integer is 1025 but the first two values read through the character pointer are 1 and 4. Where did those numbers come from?

<p align="center">
  <img src="../images/pointer-byte-view.png" alt="Lesson screenshot comparing an int pointer with a char pointer for the value 1025. The character pointer reads bytes 1 and 4 on the illustrated system. The integer read past the object is invalid." width="1000">
  <br>
  <sub>Screenshot supplied for these learning notes. It includes an invalid read that I explain below. The original lesson image is not my own work.</sub>
</p>

### Start with what has not changed

```c
int a = 1025;
int *p = &a;
char *p0 = (char *)p;
```

`a` is still an integer holding 1025. `p` points to that integer. The cast `(char *)p` gives us a character pointer to the first byte of the same object.

**The cast does not change 1025 into 1. It does not create another integer or change the bytes stored in a. It changes the pointer type through which we look at those bytes.**

Both pointers start at the same object. Reading `*p` gives the integer value. Reading `*p0` gives the value of its first byte through a character type.

### One integer can occupy several bytes

The screenshot's system uses 4 bytes for an `int`. Each byte has 8 bits on that system.

1025 can be written as:

```text
1025 = 1024 + 1
     = 4 × 256 + 1
```

In four groups of eight bits, that is:

```text
00000000  00000000  00000100  00000001
```

Those groups have values 0, 0, 4 and 1 when written with the most significant group first, as above. But that written order is not necessarily the order of bytes at increasing memory addresses.

### Why does the first byte contain 1?

The byte results shown in the picture match **little endian** storage. That means the least significant byte goes at the lowest address. Here that is the byte containing 1.

For a 4 byte integer with 8 bit bytes on such a system, the layout of 1025 is:

| Position in memory | Byte in binary | Byte value |
| --- | --- | --- |
| First byte | `00000001` | 1 |
| Second byte | `00000100` | 4 |
| Third byte | `00000000` | 0 |
| Fourth byte | `00000000` | 0 |

So `*p0` reads 1 and `*(p0 + 1)` reads 4. The integer is not broken. We are looking at one piece of its stored representation at a time.

When we read `*p`, the system reads the object as an integer using its integer representation. For this layout, the value is:

```text
1 + (4 × 256) + (0 × 65536) + (0 × 16777216) = 1025
```

On a typical big endian system with the same integer size and byte size, those bytes would appear as 0, 0, 4 and 1 at increasing addresses. C does not require every system to use the same byte order or a 4 byte `int`.

### Why does p + 1 move by 4 but p0 + 1 move by 1?

This is the step size from Part 2 again.

`p` has type `int *`, so `p + 1` moves by one `int`. On the screenshot's system, that covers 4 bytes. `p0` has type `char *`, so `p0 + 1` moves by one character, which is one C byte.

| Expression | Meaning in this example |
| --- | --- |
| `p` | Pointer to the integer a |
| `*p` | Integer value 1025 |
| `p + 1` | Position just past the single integer a |
| `p0` | Pointer to the first byte of a |
| `*p0` | First byte read as a char |
| `p0 + 1` | Pointer to the second byte of a on this system |
| `*(p0 + 1)` | Second byte read as a char |

The step size depends on the type being pointed to. It does not depend on the size of the pointer variable itself. `sizeof(int)` measures an integer while `sizeof(p)` measures the pointer variable. Those answer different questions.

`sizeof(char)` is always 1 in C. The number of bits in that byte is given by `CHAR_BIT` from `<limits.h>`. It is 8 on the system illustrated here but C does not require that on every system.

### What is that strange negative value?

The screenshot also reads:

```c
*(p + 1)  // Invalid here: a is one integer, not an array of two integers.
```

Forming `p + 1` is allowed because it is the position just past `a`. Reading through it is not allowed. There is no next integer element belonging to this object.

**The value -858993460 is not an answer we should learn or expect. The program has undefined behavior at this read.** That means C does not promise a result. A program might print something, crash or behave differently after a small change.

It is tempting to call this “the value of the next variable” or just “garbage.” Neither explains the real problem. We are reading somewhere this pointer is not allowed to read. The observed number does not make that access valid.

The screenshot also prints pointers with `%d`. That format expects an `int`, so those calls are incorrect too. Use `%p` with a `void *` argument for a pointer and `%zu` for a `sizeof` result. Because the original program contains invalid operations, its output is an illustration of the intended lesson rather than a reliable test.

### Why use unsigned char for the corrected example?

C allows an object's stored bytes to be inspected through a character type. This is a specific rule; casting to any unrelated pointer type does not give us permission to read through it.

I use `unsigned char *` below because it reads each byte as a nonnegative value. Plain `char` can be signed or unsigned depending on the implementation. The values 1 and 4 do not expose that difference but other byte values can.

I also stop after `sizeof(a)` bytes so every byte read belongs to `a`.

### A corrected example

```c
#include <limits.h>
#include <stdio.h>

int main(void)
{
    int a = 1025;
    int *p = &a;
    unsigned char *bytes = (unsigned char *)p;

    printf("Integer value: %d\n", *p);
    printf("Integer size: %zu bytes\n", sizeof(a));
    printf("Bits per byte: %d\n", CHAR_BIT);
    printf("Pointer to a: %p\n", (void *)p);

    for (size_t i = 0; i < sizeof(a); i++)
    {
        printf("Byte %zu at %p has value %u\n",
               i, (void *)(bytes + i), (unsigned int)bytes[i]);
    }

    printf("Integer after reading its bytes: %d\n", a);
    return 0;
}
```

On a usual little endian system with 4 byte integers and 8 bit bytes, the byte values will be **1, 4, 0 and 0**. The printed addresses will vary. The integer is still 1025 after the loop because we only read its bytes.

`bytes[i]` means the same thing as `*(bytes + i)`. Both select one byte at position `i`. The cast to `unsigned int` in the print call matches the `%u` format.

The main idea I want to keep is this: **the object stays the same. The pointer type decides whether I access it as an integer or inspect one of its bytes. I still have to stay inside the allowed memory range.**

For the C rule behind byte access, see the character pointer discussion in [WG14's pointer issues paper](https://open-std.org/jtc1/sc22/wg14/www/docs/n2222.htm), which quotes C 6.3.2.3 paragraph 7. The byte layout above explains the supplied screenshot; it is not a promise that every machine stores integers that way.

## Part 4: A pointer can point to another pointer

In Part 1, I said a pointer is also a variable with its own address. Now that idea becomes useful. If a pointer has an address, another pointer can store that address.

<p align="center">
  <img src="../images/pointer-to-pointer-lesson.png" alt="Lesson screenshot showing a chain from r to q to p to integer x. The declarations use int pointer, pointer to pointer and pointer to pointer to pointer." width="1000">
  <br>
  <sub>Screenshot supplied for these learning notes. The addresses are teaching examples. The original illustration is not my own work.</sub>
</p>

### Start with x and p

```c
int x = 5;
int *p = &x;
*p = 6;
```

`x` starts with the value 5. `p` stores the address of `x`. When I write `*p = 6`, I follow that pointer and change `x` to 6.

So at this point, `x` and `*p` both give me 6 when I read them. I have one integer object, not two copies of 6.

### Now point to p

```c
int **q = &p;
```

What am I giving `q`? The address of `p`, not the address of `x`.

Since `p` has type `int *`, its address has type `int **`. That is the matching type for `q`. Read the declaration as **q is a pointer to a pointer to int**.

Now follow it slowly:

| Expression | What I reach | Type |
| --- | --- | --- |
| `q` | The stored address of p | `int **` |
| `*q` | The pointer variable p | `int *` |
| `**q` | The integer x through p | `int` |

Reading `*q` gives the pointer value stored in `p`, which is `&x`. Reading `**q` follows that pointer too and gives the integer value 6.

**One star follows one pointer. A second star follows the next pointer.**

### One more level with r

```c
int ***r = &q;
```

`r` stores the address of `q`. Since `q` has type `int **`, its address has type `int ***`.

The chain is **r points to q, q points to p and p points to x**.

| Expression | What I reach | Type |
| --- | --- | --- |
| `r` | The stored address of q | `int ***` |
| `*r` | The pointer variable q | `int **` |
| `**r` | The pointer variable p through q | `int *` |
| `***r` | The integer x through p | `int` |

I do not need to memorize the final value. I can follow each pointer in order. Start at `r`, reach `q`, then reach `p` and finally reach `x`.

The stars in `int ***r` declare its type. The stars in the expression `***r` follow the pointers. That is the same declaration versus dereferencing distinction from Part 1.

### Read the addresses in the picture

The drawing gives `x` the example address 225, `p` the address 215, `q` the address 205 and `r` the address 230.

| Variable | Its own illustrated address | What it stores |
| --- | --- | --- |
| `x` | 225 | Integer value 6 |
| `p` | 215 | Address of x, illustrated as 225 |
| `q` | 205 | Address of p, illustrated as 215 |
| `r` | 230 | Address of q, illustrated as 205 |

There is a mistake in the drawing: the box for `r` appears to contain 215. For the written declaration `int ***r = &q;`, it should contain the address of `q`, which the drawing labels 205. The address 215 belongs to `p`.

These numbers only help us follow the connections. They do not show real sizes, alignment or an address order that C promises for local variables.

### Walk through the five print statements

The picture prints five expressions. After `*p = 6`, here is what each one means:

| Expression | Steps | Result when read |
| --- | --- | --- |
| `*p` | Follow p to x | 6 |
| `*q` | Follow q to p and read p | Pointer to x |
| `*(*q)` | Follow q to p, then p to x | 6 |
| `*(*r)` | Follow r to q, then q to p and read p | Pointer to x |
| `*(*(*r))` | Follow r to q, then q to p, then p to x | 6 |

`*(*q)` is another way to write `**q`. Likewise, `*(*r)` is `**r` and `*(*(*r))` is `***r`. The parentheses just make the steps easier to see.

The results for `*q` and `**r` are pointers, not integers. In the drawing, they both identify address 225. In real C code, print them with `%p` and a cast to `void *`. The screenshot uses `%d` for those pointers, which is incorrect. Use `%d` for the integer results.

### Changing the value and changing the destination are different

If I write:

```c
**q = 9;
```

I reach `x` and change its value to 9. I could reach the same integer through `***r` too.

But this does something else:

```c
int y = 20;
*q = &y;
```

`*q` is the pointer variable `p`. So this assignment changes `p` to point to `y` instead of `x`. It does not change `x` into 20.

The chain now ends at `y`. `q` still points to `p` and `r` still points to `q`. Reading `*p`, `**q` or `***r` now gives 20. The old integer `x` stays at 9.

This is one reason pointers to pointers are useful: they let us access and change a pointer variable itself. For example, a function can receive the address of a caller's pointer when it needs to update where that pointer points.

### A corrected example

```c
#include <stdio.h>

int main(void)
{
    int x = 5;
    int *p = &x;
    *p = 6;
    int **q = &p;
    int ***r = &q;

    printf("*p    = %d\n", *p);
    printf("*q    = %p\n", (void *)*q);
    printf("**q   = %d\n", **q);
    printf("**r   = %p\n", (void *)**r);
    printf("***r  = %d\n", ***r);

    **q = 9;
    printf("After **q = 9, x = %d\n", x);

    int y = 20;
    *q = &y;
    printf("After *q = &y, x = %d and *p = %d\n", x, *p);
    printf("The chain now reaches y: ***r = %d\n", ***r);

    return 0;
}
```

The first, third and fifth lines print 6. The second and fourth lines print the same pointer value, which points to `x` at that moment. The addresses will vary between runs.

After changing the value through `**q`, `x` becomes 9. After changing the pointer through `*q`, `p` points to `y` and the chain reads 20.

Every pointer in the chain must lead to a valid live object before we follow it. More stars do not make an invalid pointer usable. In this example, `x`, `p`, `q` and `y` remain alive while the code uses them.

What I want to remember is: **count the steps and check the type at each step. Stop at p to change its destination. Go through p to change the integer it points to.**

## Part 5: Passing values, passing addresses and stack frames

Suppose I have `a = 10` in `main` and I want another function to increase it. I call the function but the value is still 10 when I return. Why?

The first thing to check is what I gave the function: **a copy of the number or a copy of its address?**

### Passing the number gives the function its own copy

<p align="center">
  <img src="../images/function-pass-value-stack.png" alt="Lesson screenshot showing an integer passed to Increment by value and a drawing of separate function stack frames." width="1000">
  <br>
  <sub>Screenshot supplied for these learning notes. The memory drawing is a teaching model rather than a fixed layout required by C.</sub>
</p>

Here is the first version. I name the parameter `x` so we can easily tell it apart from `a`:

```c
void IncrementValue(int x)
{
    x = x + 1;
}

/* Inside main: */
int a = 10;
IncrementValue(a);
```

At the call, C reads the value of `a`, which is 10. The function's parameter `x` receives that value. **There are now two separate integer objects: a in main and x in IncrementValue.**

When the function writes `x = x + 1`, it changes its own `x` to 11. It has not written to `a`. After the function returns, `a` is still 10.

| Moment | a in main | x in IncrementValue |
| --- | --- | --- |
| Before the call | 10 | No active parameter yet |
| Function begins | 10 | 10 |
| After `x = x + 1` | 10 | 11 |
| Function returns | 10 | Its lifetime has ended |

Using the name `a` for the parameter would not change this behavior. Two local variables with the same name in different functions do not become the same variable.

In the call `IncrementValue(a)`, `a` is the argument expression. In `void IncrementValue(int x)`, `x` is the parameter that receives the value. The screenshot calls these the actual argument and formal argument.

### What is a stack frame?

Think of a stack frame as temporary working space for one active function call. On many systems, a call uses a frame on the call stack to keep information it needs while running.

Depending on the machine and compiler, that information can include local variables, saved register values, arguments and the information needed to return to the caller. Some of it may stay in CPU registers instead of being placed in memory.

A simple way to follow our example is:

1. `main` is running and its variable `a` is alive.
2. `main` calls `IncrementValue`. The new call has its own parameter `x`.
3. `main` has not finished. Its execution waits for the call to return while `a` remains alive.
4. `IncrementValue` finishes. Its parameter `x` stops being a live object and its temporary call storage can be reused.
5. Execution continues in `main` after the call. Its own `a` is still 10.

This is why a stack is often compared to a pile of plates. For ordinary nested calls, the most recent call finishes before the caller resumes. Calling a function does not replace the caller's variables with the new function's variables.

The second screenshot shows a `printf` call above `main`. That represents a later moment when `Increment` has returned and `main` is printing its result. In this simple model, there is no active `Increment` frame at that moment.

**The separate variables explain the result. The stack drawing helps us picture it.** C does not require every function to have a visible stack frame or every local variable to live on the stack. A compiler can keep values in registers, inline a call or remove work that has no observable effect. The copy behavior of the parameter still applies.

### Passing the address lets the function reach the original

<p align="center">
  <img src="../images/function-pass-address-stack.png" alt="Lesson screenshot showing Increment receiving int pointer p with the address of a in main and using it to change a from 10 to 11." width="1000">
  <br>
  <sub>The pointer parameter belongs to the called function but points to the caller's integer. The illustrated addresses are examples.</sub>
</p>

Now I change the parameter to a pointer and pass `&a`:

```c
void IncrementAddress(int *p)
{
    *p = *p + 1;
}

/* Inside main: */
int a = 10;
IncrementAddress(&a);
```

`&a` gives the address of `a`. The function receives a copy of that pointer value in its own parameter `p`.

The screenshot uses 308 as an example address for `a`. Its pointer parameter stores 308. The actual address on your machine will differ but the relationship is the same: **p points to the original a in main**.

Now follow the assignment:

```c
*p = *p + 1;
```

On the right side, `*p` reads the integer at that address, which is 10. Adding 1 produces 11. On the left side, `*p` identifies where to store the result. It is still the original `a`.

When the function returns, the local pointer parameter `p` stops being alive. But `a` belongs to the still active call to `main`, so it remains alive with its new value of 11.

| Moment | a in main | p in IncrementAddress |
| --- | --- | --- |
| Before the call | 10 | No active parameter yet |
| Function begins | 10 | Holds a pointer to a |
| After `*p = *p + 1` | 11 | Still points to a |
| Function returns | 11 | Its lifetime has ended |

### Is this pass by reference?

The screenshot calls this “call by reference.” You will hear that phrase used to describe changing a caller's variable through its address.

**Strictly speaking, C always passes arguments by value.** In the first version, the copied value is an integer. In the second version, the copied value is a pointer. C does not have a separate reference parameter feature like C++.

The pointer parameter is still a separate variable. It just provides a way to reach another object. That is how we get the effect people often call pass by reference in C.

| Call | Parameter | What gets copied | Effect of the shown assignment |
| --- | --- | --- | --- |
| `IncrementValue(a)` | `int x` | Integer value 10 | `x = x + 1` changes only x |
| `IncrementAddress(&a)` | `int *p` | Pointer to a | `*p = *p + 1` changes a |

### Changing p is different from changing *p

Suppose the caller already has a pointer:

```c
int a = 10;
int *caller_p = &a;
IncrementAddress(caller_p);
```

`caller_p` and the parameter `p` are separate pointer variables. They hold pointer values that lead to the same integer. Writing through `*p` changes that shared integer.

Assigning a different address to the local parameter `p` would change only that parameter. It would not redirect `caller_p`. To let a function change `caller_p` itself, we would pass `&caller_p` to a matching `int **` parameter. That connects this lesson to Part 4.

Also keep these expressions separate:

| Expression | What changes |
| --- | --- |
| `*p = *p + 1` | The integer pointed to by p |
| `(*p)++` | The integer pointed to by p |
| `p++` | The local pointer p moves by one element |

### What happens to local storage after a function returns?

Ending a function does not promise that its old bytes are immediately erased. The important point is that its ordinary local objects are no longer alive. Their old storage can be reused.

So do not return a pointer to an ordinary local variable and expect to use it later:

```c
/* Incorrect example. Do not use the returned pointer. */
int *BadPointer(void)
{
    int temporary = 10;
    return &temporary;
}
```

`temporary` stops being alive when this function returns. The returned pointer does not keep it alive. This differs from our increment example, where `a` belongs to the caller and stays alive throughout the call.

### What about the other memory boxes in the pictures?

The diagrams also show code, static/global storage and a heap. These labels describe a common way to organize program memory:

| Area | Basic idea |
| --- | --- |
| Code | The machine instructions the program executes |
| Static/global storage | Objects such as global variables and static local variables that last for the program's execution |
| Stack | Commonly used for active calls and their temporary working storage |
| Heap | Commonly used for dynamic allocation such as memory obtained with malloc |

Neither increment example uses dynamic allocation. Taking `&a` does not move `a` to the heap or make it last longer.

The exact order, addresses and growth directions in those pictures are not rules of C. On an embedded target, the linker configuration and platform decide the memory layout. A stack frame is also not the same thing as the entire stack; it is the working area associated with one call in this model.

### Compare both calls in one program

```c
#include <stdio.h>

void IncrementValue(int x)
{
    x = x + 1;
    printf("Inside IncrementValue: x = %d\n", x);
}

void IncrementAddress(int *p)
{
    *p = *p + 1;
    printf("Inside IncrementAddress: *p = %d\n", *p);
}

int main(void)
{
    int a = 10;

    IncrementValue(a);
    printf("After passing the value: a = %d\n", a);

    IncrementAddress(&a);
    printf("After passing the address: a = %d\n", a);

    return 0;
}
```

The output is:

```text
Inside IncrementValue: x = 11
After passing the value: a = 10
Inside IncrementAddress: *p = 11
After passing the address: a = 11
```

The address version requires a valid pointer to a live integer that it is allowed to modify. It does not check for a null pointer. The example meets that requirement by passing `&a`.

A function can also return a new integer for the caller to assign. Pointers are useful when we want to update an existing object through its address, but they are not the only way to send a result back.

My rule to remember is: **a function receives a copied value. If that value is a pointer, it can use the pointer to reach the original object. The pointer parameter's lifetime and the pointed object's lifetime are separate.**
