# PersonalProjects

DevLog: /ScreenString - 'Big Update' 12 March 2022
I've realized that trying to treat all this data as a string is holding the library back. It needs to be all void pointers for this system to work. This means some refactoring, but I don't anticipate significant changes. I was already treating them like void pointers, so it will most likely just need some type casting here and there.

However, I am starting to see a pattern that may or may not be correct in program structure. It would seem that I am building an encapsulated GUI handler that can load all the visual elements from a ".gui" file. The object then only needs directions on what to show, when and where. With properly formatted ".gui" file, visually, the programmer would only need to control input location. From this I am anticipating a diamond (or football, if that's your preference) shaped structure implementation:
  1. The GUI: Focused, easy to use structure, but provides direct control for precise use.
  2. The mid-layer: This is what the user of the library controls. It interprets the input and handles how the screen responds. This is the 'active' portion of the program, thus the widest portion of the diamond diagram. After setting them up, the other two parts of the diamond (GUI, IO) are dependent and respond only at the behest of this core.
  3. The IO: Still undetermined, but I hope it will be like the GUI. Set up input methods, and poll with simple function arguments to return data.

I'm happy with the progress this library is making, and I hope to be able to implement the GUI portion for my wordle program in a week.

15 March 2022
Cut and paste of a comment from the code:
	Special development note: The memwrite/copy_2D functions are actually dimension lowering
	functions. The 2D to linear actually takes an ND block and pulls a (N-1)D block.
	Since all memory is in truth linear, we are simply transforming the representative dimensions
	of a block. By the nature of how computers store memory, Taking a 2D slice from a 3D array is
	equivalent to treating the 2D and 3D blocks as 1D and 2D blocks, respectively.
	We can then make a recursive function to extract or modify blocks of any theoretic
	dimension!

	It is of special note, since multidimensional arrays ultimately are just uniformly spaced
	memory elements, we can apply algebra to simplify the problem:
		Dn = Size of n-th dimension
		Xn = The selected element of the n-th dimension

Further. The external file structure is linear, but this is not an effective solution. It will take great pains to make this work. Further, having to loop through image details many times is resource and time inefficient. Instead I'm going to switch to a prefix table (idk if that's what its called), with a continuous image/data section. The details of the objects will be stored and loaded as a single block.