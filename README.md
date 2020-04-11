# 3Draw

This is a toy project that I created during the Christmas break and pushed to
GitHub during Easter+Corona break. 

This is a simple library for creation of STL files containing 3D models with programmable 3D drawing primitives.
STL-files are explained for instance in https://en.wikipedia.org/wiki/STL_(file_format).

These STL files you can then print with the printing software of your 3D printer. 
One anticipated use case is 3D fractals.

NOTE: the software does not do any cleaning, which means that trianges of some shapes
can go inside other. Fixing this is possible future work. If your printing software
cannot cope with those errors, software like "meshmixer" can be used. 

For examples of use, see these files
- "makestl.c" - output in my FlashPrint software is shown in "makestl.png"
- "trig.c" - output is "trig.png"
- "ladders.c - output is "ladders.c"

This software is implemented with good old ANSI C. I did consider Java, JavaScript,
Python, Go, Rust, etc., but I wanted to ensure that the software compiles without
modifications next Christmas, too. If you have been there you understand...
