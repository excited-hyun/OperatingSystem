# 2020-2 Operating System
CSE4070-01

## Prj1
### 현재의 Pintos는 system call implementation, system call handler, argument passing, and user memory access 기능이 아직 구현되지 않은 상태로 user program을 적절하게 수행해 주지 못한다. 
### 따라서 user program이 어떤 과정으로 실행되는 지 이해한 후, 
### system call implementation, system call handler, argument passing, and user memory access 를 구현하여 pintos에서 user program이 적절히 수행될 수 있도록 해주었다.

## Prj2
### file system과 관련된 system call을 수행할 수 있도록 하기 위해서 
### 프로젝트 1에서는 구현하지 않았던 system call들 중 read, write, create, remove, open, close, seek, tell, filesize이 handle될 수 있도록 구현하였다.
### file system call들 사이엔 critical section이 존재하게 되기 때문에 critical section이 보호될 수 있도록 lock을 사용하여 file system에서의 동기화를 구현하였다. 
### 또한 프로젝트1에서는 동기화의 방법으로 busy waiting을 사용하였는데, 이를 semaphore방식으로 바꾸어 구현하였다.

## Prj3
### 기존의 최소한의 기능을 가진 thread에서 alarm clock을 개선하고 priority scheduling이 될 수 있도록 구현하였다.
### 이 과정에서 starvation문제의 발생을 방지하기 위하여 aging을 구현하고주고 더 나아가 BSD scheduler를 추가적으로 구현하였다.

## Prj4
### 프로젝트 3까지는 pintos가 물리적인 메모리 이상의 메모리 공간을 사용할 수 없었기 때문에 page fault가 발생하는 경우 바로 exit(-1)을 호출하여 프로그램을 종료시켰다. 
### 이를 보완하기 위해 virtual memory를 이용하여 여러 경우의 page fault들을 handle할 수 있도록 구현하였다.

*모든 프로젝트는 c언어로 구현되었습니다.*
