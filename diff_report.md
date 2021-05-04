# Task 1 (SYSCALL Tracing)

## Result Image

## code diff
in this task i only modified syscall.c and Makefile to enable xv6 to print syscall trace. 

### syscall.c

```diff
diff --git a/syscall.c b/syscall.c
index 9105b52..5d3d225 100644
--- a/syscall.c
+++ b/syscall.c
@@ -172,6 +181,9 @@ syscall(void)
   num = curproc->tf->eax;
   if(num > 0 && num < NELEM(syscalls) && syscalls[num]) {
     curproc->tf->eax = syscalls[num]();
+    #ifdef PRINT_SYSCALLS
+    cprintf("%s -> %d\n", syscallnames[num], curproc->tf->eax);
+    #endif
   } else {
     cprintf("%d %s: unknown sys call %d\n",
             curproc->pid, curproc->name, num);
```

### Makefile
```diff
diff --git a/Makefile b/Makefile
index 6483959..acba3d9 100644
--- a/Makefile
+++ b/Makefile
@@ -1,6 +1,6 @@
 # Set flag to correct CS333 project number: 1, 2, ...
 # 0 == original xv6-pdx distribution functionality
-CS333_PROJECT ?= 0
+CS333_PROJECT ?= 1
 PRINT_SYSCALLS ?= 0
 CS333_CFLAGS ?= -DPDX_XV6
 ifeq ($(CS333_CFLAGS), -DPDX_XV6)
```
