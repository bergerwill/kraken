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
6. cat scenario1.csv | netcat -u 127.0.0.1 12345
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


### Final Thoughts - Areas of Improvement

I really ran short on time.  Given a few more hours I would get this a 100% there.

Let me get my disclaimor out there.  The orderbook is not fully debugged, but I thought it was more important you see what I would do with the full project.

### Structure and Organization
Note the project structure, the organization, the focus on design, documentation, unit testing.  These are all critical aspects especially when it comes to building high volume complex C++ apps.

### The Software - Design Patterns

The classes could use some refactoring, use of more design patterns, and performance optimizations.  Using common well known published design patterns, solves common problems, increases understandability, promotes code resuse, enforaces overall good architeccture.  To that end some patterns I would apply and why.

 * Singleton Pattern singleton for the BookOrderManager as we want to ensure only one instance of the OrderBookManager especially since it is holdiing the global order of books.  However please note the singleton pattern can be viewed as an anti-pattern because it can make unit testing difficult.

* Observer Pattern on the OrderBook when publishing publishAcknowledgment(), publishTrade(), publishTopOfBook().  There maybe other components/systems that are interested in these state changes.

* Command Pattern could be used on the OrderBook's processOrder() whwere each order type 'B', 'S', etc can be a seperate CommandObject where each command object implements an interface for executing the order.  This would alllow us to extend the types of orders we suport without modifying processOrder().  To generate we could apply the Factory Pattern to create the appropriate command objects based or order type promoting more decoupling.

### The Software - High Concurrency, Threading, and Race Conditions

* Shared Resources - As this solution is scaled up to support high concurrency, he OrderBook's buyOrders and sellOrders needs to account for multiple threads operating on these resources simultaneously that not handled properly could lead to race conditions and data corruption.  Applying locking mechanims such us mutexes or locks to ensure that only one thread can access shared resource.

* Infinite Loops in OrderBookManager::processMessages and publishUpdates are infinite loops with termination condition.  This could lead to thread never end or consume to many resources.  A potentional solution here is to apply Thread Pooling.

* Thread Safety of Publish mehthods currently write to standard out.  If multiple threads are using these methods simoultaneously, the output could interleave and be difficult to interpret.

### Handling Errors, Exceptions 

This is a long ways from being production ready.  Handling Exception, Errors, is not fully flushed out.  

* Exception for Error Handling should be added for automatically propagate error up the call stack leading to simpler, more manageble, robust code
* Validating return values and asseting on pre-conditions


### Unit Testing

The unit tests was a quick stab at it, needs some careful thought.   This is a good time to review with the stake holders to ensure the  behavior of the system is meeting their requirements.  I really am a fan of test driven development, by building out unit tests before designing and implementing a system.

### Scalability and Robustness

Scallability has not been incorporated, while the application creates a thread for the processing of orders and publishing of acknowledgements and trades and orders, it needs overhauling.  Adding a message bus in the middle would go a long way for improving scalability and robustness of the system.  Also adding in a containerized orchestrator and distributed archtiecture like kubernetes can also help scale the system horizontally and vertically and bring in the concept of auto-healing and rolling updates.

