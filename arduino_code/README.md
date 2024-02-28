# Arduino Code


## Architecture

The Gamtry receives commands via Serial and stores them in a `CommandQueue`, which can store a maximum of `MAX_BUFFERED_COMMANDS` commands.


```c
#define MAX_BUFFERED_COMMANDS 10
```


To receive a new command into the command queue, the arduino first pings on Serial, and then waits for a specified delay time before requesting a message. If no message is received, no command is queued.



```c
#define SERIAL_PING_TRY_TIME 100
#define SERIAL_PING_MESSAGE "pls send"
```

Commands are queued and executed in FIFO order.


## Command Syntax

Every command is made up of chunks of text, called tokens, where each token corresponds to a specific command. The syntax of a token is as follows:


```
(COMMAND_CHAR)(COMMAND_VALUE)
```

e.g.
```
X100.0
```

Multiple tokens within a single command are space-separated.

```
X100 Y-1 H0
```

There is a limit on the number of tokens that can be sent within a single command. This is mainly due to Serial buffer size constraints.

```c
    static const int MAX_COMMANDS_PER_MESSAGE = 10;
```


Commands are identified by the `command char`, the first character within the token. The command chars and their definitions are as follows:

```c
    static const char COMMAND_X             = 'X';
    static const char COMMAND_Y             = 'Y';
    static const char COMMAND_Z             = 'Z';
    static const char COMMAND_THETA         = 'T';
    static const char COMMAND_TIME          = 't';
    static const char COMMAND_HEAD_CHANGE   = 'H';
    static const char COMMAND_HEAD_1        = 'h';
    static const char COMMAND_HEAD_2        = 'i';
    static const char COMMAND_HEAD_3        = 'j';
```


`X, Y, Z, THETA` are used to control the four degrees of freedom of the base gantry.



