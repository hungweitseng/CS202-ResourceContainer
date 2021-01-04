# CS202-ResourceContainer

<b>Overview</b>

Modern operating systems promote virtualization of their underlying machines and isolation using process/thread abstractions. As a result, threads/processes also become the identities of resource scheduling for tasks in the system. However, the "matching" between abstraction and resource allocation also creates "mismatching" between tasks and their real demands of resources. For example, process scheduling is based on the CPU time, regardless their I/O usages. The system also lacks flexibilities in controlling the usage of system resources for threads/processes with different properties as they are all treated the same.

To address the above issue, <a href="https://www.usenix.org/legacy/events/osdi99/full_papers/banga/banga.pdf">resource containers</a>&nbsp;provide another abstraction other than processes and threads for resource allocation. Each resource container logically abstracts a set of system resources for tasks within the container to use. Depending on the demand of applications, the system can assign different resource containers with different amount of system resources. Each resource container can potentially implement its own scheduling policy to efficiently use its own resources. Recent cloud platforms as well as software engineering platforms further extend the concept of containers to achieve lightweight virtualization and protection among tasks.&nbsp;&nbsp;</p>

<p>As operating systems only supports processes and threads&nbsp;<span style="font-size: 1rem;">by default, implementing resource containers would require additional efforts in any operating system kernel. Fortunately,&nbsp;most modern operating systems support "loadable kernel modules". In this way, the system can boot with a simpler, smaller kernel and then load these modules into kernel space when necessary. In this project, we will implement resource containers as a loadable kernel module as well as set of library functions that create a pseudo device in the system and provide an interface for applications. By interacting with this device, processes can assign its own threads to difference resource containers.&nbsp;</span></p>

<p><span style="font-size: 1rem;">With this new facilities, threads assigned to the same resource container will/can only share the same set of resources within the container. This semester, you will be gradually building this new facilities in terms of the supports for resource allocations for processors, memory and file storage. In the first project, you will build a kernel module to allocate processor resources and schedule the execution of threads within resource containers.</span></p>

<p>In this project, you will be given the prototype of the kernel module with a core.c file in its source directory that only contains empty functions. We also provide a user-space library that allows an application to interact with this kernel module through ioctl interfaces as well as a sample benchmark application that you may extend to test if your kernel module functions correctly.</p>

<p>You are strongly encouraged to work in a group of 2. Groups do the same project as individuals. Both members will receive the same grade. Note that working in groups may or may not make the project easier, depending on how the group interactions work out. If collaboration issues arise, contact your instructor as soon as possible: flexibility in dealing with such issues decreases as the deadline approaches.&nbsp;<br></p>

<p><b>Objective</b></p>

<p>* Learning UNIX/Linux kernel programming as well as the constraints<br>* Learning UNIX/Linux system process scheduling<br>* Learning UNIX/Linux kernel modules<br>* Learning multithreading programming<br>* Learning UNIX/Linux interrupt handler implementation</p>

<p><b>How to start&nbsp;</b></p>

<p>To begin, you need to first form a group and setup the environment for developing your project. You should set up a machine or a VMWare virtual machine (CS students should have free license for that <a href="https://www.csc.ncsu.edu/vmap/">https://www.csc.ncsu.edu/vmap/</a>) with clean Ubuntu 16.04 installation.&nbsp;<span style="font-size: 1rem;">You also may use the VCL service maintained by NCSU through&nbsp;</span><a href="https://vcl.ncsu.edu/" style="background-color: rgb(255, 255, 255); font-size: 1rem;">https://vcl.ncsu.edu/</a><span style="font-size: 1rem;">&nbsp;. You may reserve one virtual machine and connect to this machine remotely by selecting reservations. We will use the "Ubuntu 16.04 Base" to test your kernel module. However, the VCL service will reset once your reservation timeout.&nbsp;</span><br></p>

<p>Then, you need to clone the code from&nbsp;<a href="https://github.ncsu.edu/htseng3/CSC501_Container_Processor">https://github.ncsu.edu/htseng3/CSC501_Container_Processor</a>&nbsp;and make your own private repository. Please do not fork for the given repository, otherwise you will be the public repository the&nbsp;<span style="font-size: 1rem;">After cloning the code, you will find three directories and a test script. These directories are:</span><br></p>

<p>1. kernel_module -- the directory where we have the kernel module code.<br></p>

<p>2.&nbsp;library -- the directory of the user-space library code.</p>

<p>3.&nbsp;benchmark -- the directory with a sample program using this kernel.</p>

<p>You may now go to the kernel_module directory and type "make" to compile the kernel module and then "sudo make install" to install headers and the module in the right place. You should be able to find a processor_container.ko if your compilation success and this is the binary of the processor_container kernel module.&nbsp;</p>

<p>However, this kernel module isn't in your kernel yet. To get this kernel module loaded into the system kernel, try "sudo insmod processor_container.ko". Upon success, you should find an "pcontainer" device file under /dev directory in your linux system. By default, this device may not be available for non-root users. Therefore, you need to use "sudo chmod 777 /dev/pcontainer" command to make it accessible by anyone and any process in the system.&nbsp;</p>

<p><span style="font-size: 1rem;">If you don't want this device to be available in the system anymore, you can use "sudo rmmod processor_container" to remove this device.</span></p>

<p>Now, you can navigate to the library path and again use "make" to generate this dynamic link library. You need to then use "sudo make install" to make this library publicly available for the system. You should read the code and figure out how this library interacts with the kernel module.&nbsp;</p>

<p>Finally, you can now go to the benchmark directory to get the benchmark program compiled. The benchmark take input parameters for generating the corresponding number of containers and the number of task in each container.&nbsp;</p>

<p>For example, if type "./benchmark 1 1", it will generate 1 container, and the first container will have 1 task inside. If type "./benchmark 2 2 4", it will generate 2 containers, and the first container will have 2 tasks and the second container will have 4 tasks. So on and so forth.</p>

<p>The benchmark program creates containers and tasks, runs some simple calculations and prints out how many time the task runs the calculations. The number of runs among tasks inside the same container should be similar (within 10% error), and the total runs among containers should be also similar (within 10% error).&nbsp;<span style="font-size: 1rem;">You may go into the benchmark.c to understand the meaning of the program. You should also use and read "test.sh" file to see how the whole framework work together.&nbsp;</span></p>

<p><span>No matter you're using VMWare, a real machine, or VCL, you should always use&nbsp;</span><a href="https://github.ncsu.edu/">https://github.ncsu.edu</a><span>&nbsp;to control/maintain/backup your work.</span><br></p>

<p><b>Your tasks</b></p>

<p>1. Implementing the process_container kernel module: it needs the following features:</p>

<p>-&nbsp;<span><b>create</b></span>:&nbsp;you will need to support create operation that creates a container if the corresponding cid hasn't been assigned yet, and assign the task to the container. These create requests are invoked by the user-space library using&nbsp;<b>ioctl&nbsp;</b>interface. The&nbsp;ioctl&nbsp;system call will be redirected to process_container_ioctl function located in src/ioctl.c<br></p>

<p>-&nbsp;<span><b>delete</b></span>:&nbsp;you will need to support delete operation that removes tasks from the container. If there is no task in the container, the container should be destroyed as well. These delete requests are invoked by the user-space library using&nbsp;<b>ioctl&nbsp;</b>interface. The&nbsp;ioctl&nbsp;system call will be redirected to&nbsp;process_container_ioctl function located in src/ioctl.c<br></p>

<p>- <b>switch</b>: you will need to support Linux process scheduling mechanism to switch tasks between threads.</p>

<p>- <b>lock/unlock</b>: you will need to support locking and unlocking that guarantees only one process can access an object at the same time. These lock/unlock functions are invoked by the user-space library using&nbsp;<b>ioctl&nbsp;</b>interface. The ioctl system call will be redirected to&nbsp;process_container_ioctl function located in src/ioctl.</p>

<p><span style="font-size: 1rem;">2. Test the developed module: It's your responsibility to test the developed kernel module&nbsp;thoroughly. Our benchmark is just a starting point of your testing. The TA/grader will generate a different test sequence to test your program when grading. Your module should support an infinite number of containers and different numbers of tasks with each container.</span><br></p>

<p><b>Turn ins</b></p>

<p>You <b>only need to (or say you can only) turn in the core.c and the ioctl.c</b> file in the kernel_module/src directory as a tarball. All your modifications should be limited within these two files. Exactly 1 member of each group should submit the source code. All group members' names and Unity IDs should be easily found in a single line comment at the beginning of the code in the following format:</p>

<p>// Project 1: 1st member's name, 1st member's Unity ID;&nbsp;2nd member's name, 2nd member's Unity ID;&nbsp;3rd member's name, 3rd &nbsp;member's Unity ID;&nbsp;</p><p>You need to name the tarball as {1st_members_unityid}_{2nd_members_unityid}_{3rd_members_unityid}_npheap.tar.gz</p>

<p><b>Reference and hints</b></p>

<p>1. This project is based on this paper:&nbsp;<a href="http://people.cs.uchicago.edu/~shanlu/teaching/33100_fa15/papers/rc-osdi99.pdf">http://people.cs.uchicago.edu/~shanlu/teaching/33100_fa15/papers/rc-osdi99.pdf</a>, you may need to read it to understand the high-level view of this project.</p>

<p>2. You may also search Linux cgroup, LXC or Docker implementations to get deeper understanding of this project.</p>

<p>3. You should try to figure out the interactions between user-space applications (e.g. benchmark) and the user-space library, the user-space library and the kernel module.&nbsp;<span style="font-size: 1rem;">You should especially understand how to context switch tasks in the user-space that the functionality is defined in&nbsp;pcontainer_init(), handler(), pcontainer_context_switch_handler() from the user-space library. And you also need to know how to wake up/pause tasks by using wake_up_process()/schedule()/set_current_state(), which are kernel-space functions. Here is the explanation of how to control the status of processes in Linux system:&nbsp;<a href="https://www.linuxjournal.com/article/8144">https://www.linuxjournal.com/article/8144</a></span></p>

<p>4.&nbsp;You may need to reference the&nbsp;<a href="http://www.tldp.org/LDP/lkmpg/2.6/lkmpg.pdf">Linux kernel programming guide&nbsp;</a>and&nbsp;<a href="https://lwn.net/Kernel/LDD3/">Linux Device Drivers, 3rd Edition</a>&nbsp;since user-space libraries will not be available for kernel code.</p><p>5. You may find the following kernel library functions useful</p>

<p>-&nbsp;kmalloc/kcalloc/kfree</p>

<p>- mutex_init/mutex_lock/mutex_unlock</p>

<p><span>-&nbsp;wake_up_process/schedule/set_current_state</span><br></p>

<p><span>-&nbsp;printk</span></p>

<p><span>- copy_from_user</span></p>

<p>6. You may need to know these variable</p>

<p>volatile long TASK_INTERRUPTIBLE;</p>

<p>volatile long TASK_RUNNING;</p>

<p>struct task_struct *current;</p>

<p>gfp_t GFP_KERNEL;</p>

