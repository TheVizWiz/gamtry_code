# Arduino Code


## Command Architecture

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
    static const int MAX_TOKENS_PER_COMMAND = 10;
```


Commands are identified by the `command char`, the first character within the token. The command chars and their definitions are as follows:

```c
static const char COMMAND_X             = 'X';
static const char COMMAND_Y             = 'Y';
static const char COMMAND_Z             = 'Z';
static const char COMMAND_THETA         = 'T';
static const char COMMAND_TIME          = 't';

static const char COMMAND_HEAD_1        = 'h';
static const char COMMAND_HEAD_2        = 'i';
static const char COMMAND_HEAD_3        = 'j';


static const char COMMAND_HEAD_CHANGE   = 'H';
static const char COMMAND_SPECIAL       = 'S';
```


`X, Y, Z, THETA` are used to control the four degrees of freedom of the base gantry. If a specific axis is not mentioned within a command, it is assumed to be omitted on purpose, and will not move.


`HEAD_1, HEAD_2, HEAD_3` are commands meant for controlling the parts on board a specific head. These will be used to control functions for each specific head.

`HEAD_CHANGE` is used to tell the gantry that the current command is a head changing command. The head to change to is signified by the number portion of the token.

`SPECIAL` is used to tell the gantry that the current command is a special command. Examples of these include homing commands, axis resetting commands, and testing commands.




### Command Overriding

Token are overridden in the order that they are presented. For example, the command

```c
X100 Y200 X300 Z100
```

is read as 

```c
Y200 X300 Z100
```

The first `X` token is replaced by the second `X` token and its accompanying value.

This only holds for the first `MAX_TOKENS_PER_COMMAND` token. If there are more tokens than this within a single command, only the first `MAX_TOKENS_PER_COMMAND` tokens will be read, and the rest will be omitted.

If a command contains specific tokens, those tokens will take priority. Tokens in order of priority are:

1. `SPECIAL`
2. `HEAD_CHANGE`
3. `BASE`
4. `NONE`

If a command contains tokens for more than one command type, then it is assumed to be a command of the type with the highest priority.




