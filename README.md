# PersonalProjects

DevLog: /ScreenString - 'Big Update' 12 March 2022
I've realized that trying to treat all this data as a string is holding the library back. It needs to be all void pointers for this system to work. This means some refactoring, but I don't anticipate significant changes. I was already treating them like void pointers, so it will most likely just need some type casting here and there.

However, I am starting to see a pattern that may or may not be correct in program structure. It would seem that I am building an encapsulated GUI handler that can load all the visual elements from a ".gui" file. The object then only needs directions on what to show, when and where. With properly formatted ".gui" file, visually, the programmer would only need to control input location. From this I am anticipating a diamond (or football, if that's your preference) shaped structure implementation:
  1. The GUI: Focused, easy to use structure, but provides direct control for precise use.
  2. The mid-layer: This is what the user of the library controls. It interprets the input and handles how the screen responds. This is the 'active' portion of the program, thus the widest portion of the diamond diagram. After setting them up, the other two parts of the diamond (GUI, IO) are dependent and respond only at the behest of this core.
  3. The IO: Still undetermined, but I hope it will be like the GUI. Set up input methods, and poll with simple function arguments to return data.

I'm happy with the progress this library is making, and I hope to be able to implement the GUI portion for my wordle program in a week.
