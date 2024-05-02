# 42Philosophers
Simulation of the famous philosophers' dining problem.

Uses threads to simulate each philosopher and a mutex for each chopstick. A philosopher will either be eating, sleeping, or thinking. A philosopher will think until the chopsticks on both its left and right are available. It will then take the chopsticks and start eating for a set amount of time. The philosopher will then sleep for a set amount of time before sleeping again. If a philosopher does not start eating before a set amount of time, it will die and the simulation will end.

## Restrictions

42 imposes a strict set of restrictions that must be followed. The program must be built with the flags `-Wall -Wextra -Werror` and all build errors or warnings will mark the project as a failure. There are also a list of allowed function and style guild that must be followed exactly. 

### Allowed functions

Only functions from the list below can be used or the project is marked as a failure:

memset, printf, malloc, free, write, usleep, gettimeofday, pthread_create, pthread_detach, pthread_join, pthread_mutex_init, pthread_mutex_destroy, pthread_mutex_lock, pthread_mutex_unlock

### Notable style restrictions

An assignment will be marked as a failure if the style guide is not followed exactly and there is a program to flag errors. The full style guide can be viewed [here](https://github.com/42School/norminette/blob/master/pdf/en.norm.pdf), however below are some notable restrictions:
- Only 25 lines are allowed in any function
- Only 5 functions are allowed in each file
- Only 4 arguments are allowed for each function
- Lines must contain 80 characters or less
- The do..while, for, ternary, case..switch, and GOTO operators are forbidden
- Curly brackets must be on the line following a statement

## Build

To build this code, clone the repository and run the `make` command in the root directory. It will use Clang to build the project and output the file `philo`.

## Run

To run this program, use the following syntax:

```./philo number_of_philosophers time_to_die time_to_eat time_to_sleep```

an optional argument can also be added to the end, specifying a termination point in the program after every philosopher has eaten X times.
