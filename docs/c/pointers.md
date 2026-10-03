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
