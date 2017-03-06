# Contributing to the documentation

For the project we are using Doxygen (generates documentation from source code) to document the Audio Synth Wavetable class. Doxygen reads specially-formatted comments in the source files and outputs html pages for each class. We are also using Doxygen to generate additional standalone pages from markdown files. 

For more on Doxygen, please see their website: http://www.stack.nl/~dimitri/doxygen/index.html

# How to document the source code

The Audio Synth Wavetable code is commented using the `JavaDoc` style: http://www.stack.nl/~dimitri/doxygen/manual/docblocks.html#cppblock.


# Add a standalone page 

Standalone pages are created from markdown-formatted files (with a `.md` extension). To create a new page, add a markdown file to the `docs/additional_pages` folder. If the document's first line is a header, it will be used for the name of the page on the Doxygen site. Otherwise, the name of the file will be used for this purpose. 

# Generating the documentation with Doxygen

After adding or modifying any comments to the source code or any additional pages, update the web page by running `doxygen Doxyfile` from within the project's `docs` folder (please see the Doxygen documentation for information on installing Doxygen). Preview the changes by opening the `index.html` file within the `docs` folder. 
