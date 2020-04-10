# 3Draw

This is a toy project that I created during the Christmas break and pushed to GitHub during Easter+Corona break. 

This is a simple library to create STL files containing 3D models with 3 drawing primitives. These STL files you can
then print with the printing software of your 3D printer. One anticipated use case is 3D fractals.

NOTE: the software does not do any cleaning, which means that trianges of some shaped can go inside other. Fixing this is
possible future work. If your printing software cannot cope with those errors, software like "meshmixer" can be used. 

For examples see files
- "makestl.c" - output is "makestl.png"
- "trig.c" - output is "trig.png"
- "ladders.c - output is "ladders.c"

This software is implemented with good old ANSI C. I did consider Java, JavaScript, Python, Go, Rust, etc., but wanted to ensure
the software compiled without modifications next Christmas, too.
