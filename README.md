# Process and resource

## Description:
&emsp;&emsp;It reads in sets of instructions to create processes with priority, destroy processes, request/release resources, and time out. Return values are the running process after each instruction.
<br/><br/>
&emsp;&emsp;Processes created will have a priority level, the highest priority processes must all be complete or waiting for resources before lower priority processes start to run. When a process requests resources that aren’t available, it will be placed on the waitlist and will not be in the queue to run until it receives what it needs. When resources are released, if a higher-priority process receives the resources it was waiting for, the active process will change. 
<br/><br/>
&emsp;&emsp;It currently has a fixed amount of resources and a fixed amount of units for each resource, but I am working on a new version using TypeScript and React, to create a web version for visualization and have flexible resources/units. 
<br/><br/>

## Commands:
&emsp;&emsp;The command should start with “in”, to initialize the system. “in” can also call during the process to restart the system. The output file will not be cleared, but it will start on a new line.
<br/><br/>
&emsp;&emsp;**“in”:** initialize/reset system.
<br/>
&emsp;&emsp;**“cr int”:** create a new process, with a priority level of [int].
<br/>
&emsp;&emsp;**“de int”:** delete process [int] and all processes created by process [int].
<br/>
&emsp;&emsp;**“rq int1 int2”:** request from resource [int1] for an amount of [int2] unit.
<br/>
&emsp;&emsp;**“rl int1 int2”:** release resource [int1] with the amount of [int2] unit.
<br/>
&emsp;&emsp;**“to”:** timeout and go to the next process with the highest priority.
<br/>
&emsp;&emsp;**“ex”:** exit (only in manual input).

<br/><br/>
## How to Run:

Run the program with an input file:
```./out input_file_name```
<br/>
Run the program without an input file:
	```./out```
*(You will need to manually input one command at a time)*


if ./out does not exist, use: ``` make ``` and refer to above




