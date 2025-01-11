In this code, I defined two dummydrivers. The first one has a global and not static buffer and the second one is externing that buffer to write to it. 

I'm protecting the reading process in dummydriver1 with mutex, which occurs while doing watch -n 1 head /dev/dummydriver1. There is a for loop which toggles the writing process to the buffer between dummydriver 1 and 2. Therefore, by observeing the logs on dmesg -w and reading the buffer, I clearly saw the mutex protecting the buffer while reading happens and the data being written when the mutex lock is not there. By ajdusting reading and writing frequencies, the data i nthe buffer and log message differ completely.

At the end, I noticed that I didn't really have to define another driver to test the mutex. However, defining the two of them, loading, accessing, writing with them and unloading them was a good experiment. 
<img width="937" alt="dd2 accessing dd1 data" src="https://github.com/user-attachments/assets/287b35dc-dd2b-48c6-9433-f565347ef9c0" />
<img width="920" alt="dd2 accessing dd1 data - 2" src="https://github.com/user-attachments/assets/63a5ca1e-c354-4c25-b94b-731d0bc76625" />
