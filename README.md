# Understanding the layout

bin - contains the ordersbooksimulator
build - contains the object files that are linked together at the end
include - contains the headers
src - contains the C++ source for the excercise
tests - contains the unit tests

# Instructions for running the project

1. Ensure you have g++/clang version 11.0.3 installed
2. make
3. cd bin and execute orderbooksimulator
4. In another terminal 
5. cd tests/data
6. cat inputfile.csv | netcat -u 127.0.0.1 12345
7. Observe output in the console


# Execute unit tests

1. ensure gtest is installed
        * Download the gtest source code: Visit the Google Test GitHub repository at https://github.com/google/googletest and click on the "Code" button, then select "Download ZIP". Extract the downloaded ZIP file to obtain the gtest source code.

        * Build gtest: Open a terminal, navigate to the extracted gtest directory, and run the following commands:

            cd googletest
            mkdir build
            cd build
            cmake ..
            make

        * sudo make install

2. cd tests
2. make 


### Final Thoughts

I really ran short on time.  Given a few more hours I would get this a 100% there.

Let me get my disclaimor out there.  The orderbook is not fully debugged, but I thought it was more important you see what I would do with the full project.

Note the structure, the organization, the focus on design, documentation, unit testing.  These are all critical aspects especially when it comes to building high
volume C++ apps.

This is a long ways from being production ready.  Handling Exception, Errors, is not fully flushed out.  The unit tests was a quick stab at it, needs some careful thought.   A good time to review with the stake holders.  Are we meeting their requirements.  I really am a fan of test driven development.  

Scallability has not been incorporated, while I have created a thread for the processing of orders and publishing
of acknowledgements and trades and orders, it needs some overhauling.  Adding a message bus in the middle would go a long way for improving scalability and robustness.  Adding in kubernetes can also help scale horizontally and vertically, bring in the concept of auto-healing, rolling updates.

