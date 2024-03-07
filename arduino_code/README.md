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
static const char COMMAND_TIME          = 'theta';

static const char COMMAND_HEAD_1        = 'h';
static const char COMMAND_HEAD_2        = 'i';
static const char COMMAND_HEAD_3        = 'j';


static const char COMMAND_HEAD_CHANGE   = 'H';
static const char COMMAND_SPECIAL       = 'S';
```


`X, Y, Z, THETA` are used to control the four degrees of freedom of the base gantry. If a specific axis is not mentioned within a command, it is assumed to be omitted on purpose, and will not move.


`HEAD_1, HEAD_2, HEAD_3` are commands meant for controlling the parts on board a specific head. These will be used to control functions for each specific head.


`TIME` is used to set a delta time for how long the command should take to execute.

`HEAD_CHANGE` is used to tell the gantry that the current command is a head changing command. The head to change to is signified by the number portion of the token.

`SPECIAL_COMMAND` is used to tell the gantry that the current command is a special command. Examples of these include homing commands, axis resetting commands, and testing commands.



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

1. `SPECIAL_COMMAND`
2. `HEAD_CHANGE`
3. `BASE`
4. `NONE`

If a command contains tokens for more than one command type, then it is assumed to be a command of the type with the highest priority.




## Time Constraints


The time taken for a command to execute only matters for `BASE` commands; all other commands have set sequences of tokens with times to execute, and cannot have a time set in the command itself. 

For a `BASE` command, the maximum speed of the entire command is defined by the minimum of the maximum speeds of all the involved motors. If motor A can go at 20 mm/s and motor B can go at 30 mm/s, but A has to go 100mm and B only has to go 60 mm, then A will take 5 seconds, and B will take 2 seconds to achieve its final position. Therefore, the entire command should take 5 seconds.

To sync motors, the minimum time required for the command to execute is first calculated using maximum motor velocity constants. If the time asked for is less than the minimum time, the minimum time is used instead. Motor speeds are calculated proportionally based on the required distance and allotted time for the movement.

