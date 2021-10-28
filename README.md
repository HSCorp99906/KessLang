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

###### If Statements

```
if (name == "Ian") {
    out("Hello, Ian!");
}
```


## Questions?

***Email me at teaqllabs@gmail.com***
