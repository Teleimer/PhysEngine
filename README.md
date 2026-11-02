OVERVIEW
Started as a project to learn C++ syntax and some basic structs to physical modeling. Expanded beyond my original scope to a larger project. 
Currently building foundational model to be able to model all functions covered in current physics coursework.

ARCHITECTURE
>input string as equation
>tokenized array - Lexical analysis
>AST Generation - Recursive Descent Parsing
>RPN Generation - Linearization
>TAC Array - TAC Instruction generation
>SIMD execution

ONGOING
>managing power evaluation to not rely on power function in TAC loop
>simplification of expression / derivation prep through tree operations
>Modularize TACexecution functions:
  >>execution loop scaling -- multiple input vectors
  >>looping over t, to actually facilitate sim
>build library of default functions
>modify execution entry points -- personal function // library functions


