# libLogHelper
Library necessary to radiation experiments
Library in charge to log benchmark data. This library will be used together the benchmark in execution, inside the benchmark source code.

# Dependencies
- CMake >=3.16
- GCC with std=C11
- SWIG for Python applications

# Getting started

To use with C or C++ you have to build the library and then
link with your code.

## Building libLogHelper

You can set to disable some of the library functionalities:
- -DWITH_PYTHON_SUPPORT=OFF to disable Python 3.8 wrapper building
- -DWITH_DEBUG=OFF to disable debug printing information

```shell
cd libLogHelper
mkdir build && cd build
# To build the whole lib
cmake ..
make
```
If you wish to install in the whole system
```shell
sudo make install
```
Then to use you just have to build the benchmark with this library use -lLogHelper
with -I<path_to_this_repo>/include/ -L<path_to_this_repo>/
(if not installed in the system)


The library contains the following functions to be included in your app.
```C
/**
* Set the max errors that can be found for a single iteration
* If more than max errors is found, exit the program
 */
unsigned long int set_max_errors_iter(unsigned long int max_errors);

/**
 * Set the max number of infos logged in a single iteration
 */
unsigned long int set_max_infos_iter(unsigned long int max_infos);

/**
 *  Set the interval the program must print log details,
 *  default is 1 (each iteration)
 */
int set_iter_interval_print(int interval);

/**
 * Disable double error kill
 * this will disable double error kill if
 * two errors happened sequentially
 */
void disable_double_error_kill();

/**
 * Generate the log file name, log info from user about the test
 * to be executed and reset log variables
 */
int start_log_file(const char *benchmark_name, const char *test_info);

/**
 * Log the string "#END" and reset global variables
 */
int end_log_file();

/**
 *  Start time to measure kernel time, also update
 *  iteration number and log to file
 */
int start_iteration();

/**
 * Finish the measured kernel time log both
 * time (total time and kernel time)
 */
int end_iteration();

/**
 * Update total errors variable and log both
 * errors(total errors and kernel errors)
 */
int log_error_count(unsigned long int kernel_errors);

/**
 * Update total infos variable and log both infos(total infos and iteration infos)
 */
int log_info_count(unsigned long int info_count);

/**
 * Print some string with the detail of an error to log file
 */
int log_error_detail(char *string);

/**
 * Print some string with the detail of an error/information to log file
 */
int log_info_detail(char *string);

/**
 * Update with current timestamp the file where the software watchdog watches
 */
void update_timestamp();

/**
 * Return the name of the log file generated
 */
char *get_log_file_name();
```

To use Python 3.8 lib just call the same name as 