# naiveAES

This is a straightforward implementation of AES in C++. It should serve as the simplest target for memory trace attack.


### Compile

Run
```sh
$ make
```


### Use

Run
```sh
$ ./main <plaintext of 32 hex chars>
```
Outputs single line with AES encryption of given plaintext using embedded key `2b7e151628aed2a6abf7158809cf4f3c`, 32 hex chars.