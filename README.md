# FairlySimpleFileSystem

## To RUN : [In FSFS directory : run ‘make’ then run ‘./PORTAL’]

Within my implementation, free space and occupied space is managed similarly.
The entire process can be described as shown in this diagram:

![Structure Diagram](/img/struct.PNG)

In this system, we use iNodes to keep track of each file that is being created. Within the structure of the iNode we have:
  - id: number of the specific iNode
  - size: the size of the file
  - numOfBlocks: total number of data blocks
  - direct[2]: two direct data block pointers
  - indirect: indirect data block pointer
  - numOfFiles: total files in directory (if applicable)
  
Within the structure of our SuperBoulder we have:
  - inodeOffset: start offset of iNode region
  - dataOffset: start offset of data region
  - maxInodeNum: max num of iNodes
  - maxDataBlockNum: max num of data blocks
  - nextFreeInode: index of next available iNode
  - nextFreeBlock: index of next available block
  - sizeOfBlock: size per block

<h3>When a new File is created:</h3>We will first check if there already exists one with the same name (using mapping function),
and if not, then we will get the next available iNode and block to store it inside our SuperBoulder struct, and then create
the new iNode and link the mapping (for parent directory). Lastly, updating the superBoulder struct with the next
available iNode and block info.

<h3>When writing:</h3>The system will allocate the necessary intermediate blocks and will then write the string provided by the
user to the specific location. Since the file system keeps track of the last written byte to the file, any calls to the write
command in the shell will allow the user to append the given data to that specific location without having to specify any
particular offsets.

### The multiple other commands include:
###### (Note: commands are not in correct format, run make, then run ./PORTAL to see correct input format)
  - ls: display current directory
  - cd: change directory to path specified
  - mkdir: make empty directory within current directory
  - touch: make empty file within current directory
  - cat: read contents of specified file
  - help: command information
  - exit: leave portal
  
###### The data should be intact when exiting the PORTAL (shell) and entering it again. (Though, when make is run again, it will create a new FileSys)


![Demo One](/img/demo.PNG)
![Demo Two](/img/anotherDemo.PNG)
![Fail Cases](/img/failCases.PNG)
![Exiting Portal](/img/leavingThePortal.PNG)
