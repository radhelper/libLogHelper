SWIG = swig
PYTHON_INCLUDE=/usr/include/python3.8/

SWIG_SOURCE=log_helper.i
all:log_helper_python
CC=gcc
CFLAGS=-fPIC -Wall -shared -std=c11
LDFLAGS=-L../build -lLogHelper
INCLUDE=-I../include
WRAP_FILE=log_helper_wrap.c

log_helper_python: $(SWIG_SOURCE)
	$(SWIG) $(INCLUDE) -Wall -o $(WRAP_FILE) -python -py3 $(SWIG_SOURCE)
	$(CC) $(INCLUDE) $(CFLAGS) $(WRAP_FILE) -o _log_helper.so $(LDFLAGS) -I$(PYTHON_INCLUDE) -lpython3.8
clean:
	rm -rf *.o *py *so *pyc $(WRAP_FILE)
