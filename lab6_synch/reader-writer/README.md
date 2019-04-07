## Note

When testing in OS X, there'll be a problem. I think it's because unnamed POSIX semaphores, but I haven't tested for it.

Ref: <https://stackoverflow.com/questions/1413785/sem-init-on-os-x>

## Implementation

Idea is simple, when a thread is writing, no other reading or writing is allowed. Use `db` to block other threads. When the thread finish writing, unblock other threads.

When a thread is reading, other reading is allowed but writing is not allowed. There is another problem, that is modifying the `readcount` and determine its value. This step may cause race condition so we need to allow only one reading thread each time.

