# MAKE SURE TO PUT KESSLANG IN YOUR HOME DIRECTORY!
# WORKS ONLY ON LINUX CURRENTLY.


## SETUP:

cd into setup
run get-deps.sh
then run setup.sh as root

then cd to VimSetup
run vimsetup.sh

## EXECUTION:

Run: klcc FILENAME.kess
then you are good to go!



## DOCUMENTATION

###### DATA TYPES

```
str
int
```

######  Keywords

```
__file_read_out__("filename.txt")
out("Something to output.")
bak("Backwards text.")
// Comment
```


## VARIABLES

> How do I make variables?

Glad you asked, this is how.

###### STRING VAR
```
str name = "Ian";
```

###### INT VAR
```
int a = 7;
```


###### Outputting Vars

```
out(name);
out(a);
```


## Embedding C Code

```
C_START:

#inclue <stdlib.h>
#include <stdio.h>

int main() {
    puts("Hello, World!");
}

C_END

```

## CONDITIONS

###### Output Conditions
***Currently conditional opeartors do not work on variables and needs some work.***

```
out(1 == 1);
out(1 != 2);
out(5 > 2);
out(2 < 5);
```

###### If Statements

```
if 1 == 1:
    out("1 is equal to 1.");

if 1 < 4:
    out("1 is less than 4.");

if 4 > 1:
    out("4 is greater than 1.")

if 1 != 2:
    out("1 does not equal 2.")


## Questions?

***Email me at teaqllabs@gmail.com***
