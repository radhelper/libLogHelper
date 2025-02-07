# libLogHelper

This lib is necessary for radiation experiments to log benchmark data. This library is meant to be used within the
benchmark source code.

# Dependencies

- CMake >=3.16
- GCC with std=C17
- SWIG for Python applications >= 4.0.1
- Python >=3.8
- Python libraries: python3-dev python3-distutils python3-venv

# Getting started

To use with C or C++, you have to build the library and then link with your code. **The library functions are not thread
safe**.

## Building libLogHelper

You can set some library functionalities on CMake:

- -DLOGGING_TYPE=\<logging approach to set the logging approach, that can be LOCAL, UDP, LOCAL_AND_UDP.\>
    - Default: LOCAL
- -DWITH_PYTHON_SUPPORT=\<ON/OFF to disable Python 3 wrapper building\>
    - Default: ON
- -DWITH_DEBUG=\<ON/OFF to disable debug printing information\>
    - Default: OFF
- -DECC_INFO_FILE_DIR=\<Path to file that will contain 1/0 that refers to ECC
  enabled or disabled respectively\>
    - Default: /tmp/ecc-info-file.txt
- -DSERVER_IP=\<Server that will receive the messages IP\>
    - Default: 192.168.1.5
- -DSERVER_PORT=\<server port that will receive the messages\>
    - Default: 1024
- -DLOG_DIR=\<path to where the logs will be saved\>
    - Default: /var

```shell
cd libLogHelper
mkdir build && cd build
# To build the whole lib
cmake ..
# To set a specific configuration
# In this case we set the server ip to 192.168.1.4
# cmake -DSERVER_IP=192.168.1.4 ..
make
```

If you wish to install in the whole system run the following commands.

```shell
sudo make install
sudo ldconfig
```

After the installation, the file /etc/rc.local will be created.
This file will run a script called atBoot.sh, containing any additional configurations
to be performed immediately after the boot process.
After installation, you can find and edit the default atBoot.sh commands in your HOME directory.

To uninstall the library (LOG_DIR/radiation-benchmarks/ path is not deleted)

```shell
sudo make uninstall
```

## How to use the library


Then to use you just have to build the benchmark with this library use -lLogHelper with -I<install_path>/include -L<
install_path>/lib (if not installed in the system).
Then include in your C/C++ code as follows:

```C++
// include the header in your C code
#include "log_helper.h"
...
// include the header in your C++ code
#include "log_helper.hpp"
```

### Python Wrapper

It is possible to use the Python >=3.8 wrapper to use the library on Python apps. You have to set the path to the build
folder, then just call the same name as:

```python
import log_helper as lh
...
```

### Examples and docs

Some dummy source codes in [examples/](https://github.com/radhelper/libLogHelper/tree/main/examples) directory contain
the library's essential functions usage. 
A description of the main libLogHelper functions can be found in 
[the docs](https://github.com/radhelper/libLogHelper/tree/main/docs.md)

