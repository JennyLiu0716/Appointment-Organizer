1. Title and Description:
APpointment Organizer(APO)
Member:
HU Wenqing
LIU Chenxi
ZHANG Tianyi 
ZHANG Wanyu
This system allows users to specify their own schedule or participate in group activities,
where the system automatically identifies and schedules suitable times and sends them 
to the corresponding users' schedules. 
This enables both personal schedule planning and team time coordination functionality.

 
2. SetUp Instruction
input the following code in command line:
gcc G01_APO.c -o apo

then initialize to begin the system(the user should be limited from 3 to 10)
for example:
./apo 20230401 20230430 john mary lucy paul
20230401 refers to the start time of the schedule, 
20230430 refers to the end time of the schedule
john mary lucy, and paul refers to the total users included in the schedule.

3.User Guide
Format:
there are 4 types of activity which can be scheduled:
(1)privateTime
(2)groupStudy
(3)projectMeeting
(4)gathering
Input Sample1:
privateTime paul 20230401 1800 2.0
Here is a sample of single person activity.
privateTime refers to the activity type
paul refers to the activity caller
20230401 means the start date. 1800 means starting time.
2.0 means duration hours, also can be represented as "2".

Input Sample2:
projectMeeting john 20230402 1900 2.0 paul mary
Here is a sample of group activity.
projectMeeting refers to the activity type,
20230402 means the start date. 2.0 means the starting time,
also can be represented as "2".
john is caller; paul and mary are callee.

The program prints out reasonable time schedules based on 
different user requirements using various algorithms. 
In case of conflicts in the schedule, 
the program selects effective time schedules and removes invalid ones.
Tree Types of Algorithm
(1)FCFS(First Come First Serve): According to the sequence of events arrangement.
(2)Priority: Selection based on priority of time
(3)LJFASSJ: Prioritize tasks that take a long time and then automatically reschedule
tasks that take a short time.
(4)ALL: print all of the above three algorithm results.

Input Sample:
printSchd FCFS
printSchd PRIORITY
printSchd NEW
printSchd ALL

The APO system will check if there are any events happening at the same time
for the same user in the system. 
If there is no conflict, the activity initiated by the caller will be sent to the callee's
 account. In case of a scheduling conflict, the APO system will write out the best 
schedule using different algorithms based on user preferences, 
and present it to the user in the form of a file. 
The final result of the time arrangement will be sent back to the system 
based on the user's response.

Batch Input:
This system can process files in batches.
Each line in the file must store a corresponding command (excluding "file xxx.txt"). 
The program will execute the commands sequentially as read from the file.
Sample Input:
G01_01_FCFS.txt

endProgram
the user can input: endProgram
to exit the program.

4. Notice
the schedule has a time limitation: beginning from 18:00
and ending before 23:00. So the duration must be less or 
equal to 5 hours.
time has to be integer time, like 2 or 2.0, cannot be 2.5.

5. Testing

6. Version History:
APO 1.0 Prototype.
APO 2.0: Current Version, has including all the described function.