# Multi threading via multi threader
This program has been created for parameter studies, and is designed such that it can a parameter configuration on each thread that it has been alotted.
The program expects an input file with on each line a command.
The input file is parsed, and multiple commands are called at one time.
The output of each command is outputted to an output file.

## Compiling the program
Make sure the `makefile` and the `multi_threader.cpp` file are in the working directory and the call the command 'make'

## Running the program
The program expects 3 arguments and should be called as follows
```./multi_threader <input_file> <output_file> <n_threads>```

### <input_file>
Expects a string describing the text file with on each line a command, e.g.:
```timeout -k 0s 120s ./program arg_1 arg_2```
This command will pe terminated if it did not finish after 120 seconds, and calls a program called `program` that takes two arguments (`arg_1` and `arg_2`)

### <output_file> 
Expects a string describing the text file that will be created if it does not exist and appended to if it does exist.
The output of each command is redirected to this file

### <n_threads>
Expects an integer describing the number of allotted threads to the multi threader