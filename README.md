# ESP8266_memory_analyser
A tool for analysing memory consumption for compiled ELF file for ESP8266.
Formats output of objdump -t in to human readable table.

## Usage
objdump -t out/build/app | ESP8266_memory_analyser

## Sample output
```
   Section|                   Description| Start (hex)|   End (hex)|Used space
------------------------------------------------------------------------------
      data|        Initialized Data (RAM)|    3FFE8000|    3FFE89F0|    2544
    rodata|           ReadOnly Data (RAM)|    3FFE89F0|    3FFE8E44|    1108
       bss|      Uninitialized Data (RAM)|    3FFE8E48|    3FFF0A28|   31712
      text|            Cached Code (IRAM)|    40100000|    401062CE|   25294
irom0_text|           Uncached Code (SPI)|    40240000|    402633EC|  144364
Total Used RAM : 35364
Free RAM : 46556
Free IRam : 7492
```

This is a re-write of ESP8266_memory_analyzer, written in c++ to remove the dependency on .net framework. Functionality is similar except ESP8266_memory_analyser expects input to be piped from standard input, e.g.

	objdump -t out/build/app | ESP8266_memory_analyser

compared with

	ESP8266_memory_analyzer objdump out/build/app
	
Source code is derived from ESP8266_memory_analyszer by Andrey Filimonov (Sermus).
Name is similar with 'z' changed to 's' just to distinguish it as a different application (and because I'm British!).