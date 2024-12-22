# Main LibLogHelper functions

The functions of libLogHelper are:

- **start_log_file**: Generate the log file name, log info from user about the test to be executed
  and reset log variables. The C++ can be used with the namespace log_helper.
  For the python wrapper the functions have the same name and parameters.

  ```C++
  int start_log_file(const char *benchmark_name, const char *test_info);
  ```

- **set_max_errors_iter**: Set the max errors that can be logged for a single iteration.
  If more than max errors is found, exit the program. Default is 500.

  ```C++
  unsigned long int set_max_errors_iter(unsigned long int max_errors);
  ```

- **set_max_infos_iter**: Set the max number of infos logged in a single iteration. 
Default is 500.

  ```C++
  unsigned long int set_max_infos_iter(unsigned long int max_infos);
  ```

- **set_iter_interval_print**: Set the interval the program must log iteration details, 
default is 1 (each iteration will be logged). 
This is generally useful when using networking logging.

  ```C++
  int set_iter_interval_print(int interval);
  ```

- **disable_double_error_kill**: Disable the double error kill mechanism. 
This will prevent the program from terminating if two errors occur sequentially. 
  ```C++
    void disable_double_error_kill();
    ```

- **end_log_file**: Log the string "#END" to the log file and reset global variables. 

    ```C++
    int end_log_file();
    ```

- **start_iteration**: Start timing the kernel execution, update the iteration number, 
and log this information to the file.

    ```C++
    int start_iteration();
    ```

- **end_iteration**: Complete the kernel time measurement and log both the total execution 
time and kernel time to the log file. 

    ```C++
    int end_iteration();
    ```

- **log_error_count**: Update the total error count and log both the total errors 
and iteration i kernel-specific errors to the log file. 
Be aware that if the total errors is higher than the value set_max_errors_iteration 
the function will throw an exception. 

    ```C++
    int log_error_count(unsigned long int kernel_errors);
    ```

- **log_info_count**: Update the total info count and log both the total and 
iteration-specific info count to the log file.

    ```C++
    int log_info_count(unsigned long int info_count);
    ```

- **log_error_detail**: Log the provided error detail string to the log file. 

    ```C++
    int log_error_detail(const char *error_detail);
    ```

- **log_info_detail**: Log the provided information detail string to the log file. 

    ```C++
    int log_info_detail(const char *info_detail);
    ```

- **get_log_file_name**: Store the name of the generated log file in the `log_file_name` variable and return a pointer
  to it. 

    ```C++
    char *get_log_file_name(char *log_file_name);
    ```
