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


###### Outputting vars

```
out(name);
out(a);
```


## Embedding C code

```
C_START:

#inclue <stdlib.h>
#include <stdio.h>

int main() {
    puts("Hello, World!");
}

C_END

```


## Questions?

***Email me at teaqllabs@gmail.com***
