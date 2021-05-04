# Task 1 (SYSCALL Tracing)

## Result Image

## code diff
in this task i only modified syscall.c and Makefile to enable xv6 to print syscall trace. 

### syscall.c

```diff
diff --git a/syscall.c b/syscall.c
index 9105b52..c22f70b 100644
--- a/syscall.c
+++ b/syscall.c
@@ -172,6 +172,9 @@ syscall(void)
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
index 6483959..dc8d53b 100644
--- a/Makefile
+++ b/Makefile
@@ -1,7 +1,7 @@
 # Set flag to correct CS333 project number: 1, 2, ...
 # 0 == original xv6-pdx distribution functionality
-CS333_PROJECT ?= 0
-PRINT_SYSCALLS ?= 0
+CS333_PROJECT ?= 1
+PRINT_SYSCALLS ?= 1
 CS333_CFLAGS ?= -DPDX_XV6
 ifeq ($(CS333_CFLAGS), -DPDX_XV6)
 CS333_UPROGS +=	_halt _uptime
```

# Task 2 (adding new date SYSCALL) 

## Result Image

## code diff
in order to add new date syscall there will be some file that are needed to edit, 
- Makefile
- user.h, to add new date variable
- usys.S
- syscall.h, to define SYS_date
- syscall.c
  - add new extern int 
  - define the date syscall in the array
- sysproc.c, define sys_date function 

### Makefile

```diff
diff --git a/Makefile b/Makefile
index dc8d53b..acba3d9 100644
--- a/Makefile
+++ b/Makefile
@@ -1,7 +1,7 @@
 # Set flag to correct CS333 project number: 1, 2, ...
 # 0 == original xv6-pdx distribution functionality
 CS333_PROJECT ?= 1
-PRINT_SYSCALLS ?= 1
+PRINT_SYSCALLS ?= 0
 CS333_CFLAGS ?= -DPDX_XV6
 ifeq ($(CS333_CFLAGS), -DPDX_XV6)
 CS333_UPROGS +=	_halt _uptime
@@ -13,7 +13,7 @@ endif
 
 ifeq ($(CS333_PROJECT), 1)
 CS333_CFLAGS += -DCS333_P1
-CS333_UPROGS += #_date
+CS333_UPROGS += _date
 endif
 
 ifeq ($(CS333_PROJECT), 2)
```

### user.h

```diff
diff --git a/user.h b/user.h
index 31d9134..e0231eb 100644
--- a/user.h
+++ b/user.h
@@ -25,6 +25,9 @@ char* sbrk(int);
 int sleep(int);
 int uptime(void);
 int halt(void);
+#ifdef CS333_P1 
+int date(struct rtcdate*);
+#endif
```

### usys.S

```diff
diff --git a/usys.S b/usys.S
index 0d4eaed..84bd80b 100644
--- a/usys.S
+++ b/usys.S
@@ -30,3 +30,4 @@ SYSCALL(sbrk)
 SYSCALL(sleep)
 SYSCALL(uptime)
 SYSCALL(halt)
+SYSCALL(date)
```

### syscall.h

```diff
diff --git a/syscall.h b/syscall.h
index 7fc8ce1..14f3e17 100644
--- a/syscall.h
+++ b/syscall.h
@@ -22,3 +22,4 @@
 #define SYS_close   SYS_mkdir+1
 #define SYS_halt    SYS_close+1
 // student system calls begin here. Follow the existing pattern.
+#define SYS_date    SYS_halt+1
```

### syscall.c 

```diff
diff --git a/syscall.c b/syscall.c
index c22f70b..5d3d225 100644
--- a/syscall.c
+++ b/syscall.c
@@ -106,6 +106,9 @@ extern int sys_uptime(void);
 #ifdef PDX_XV6
 extern int sys_halt(void);
 #endif // PDX_XV6
+#ifdef CS333_P1
+extern int sys_date(void);
+#endif // CS333_P1
 
 static int (*syscalls[])(void) = {
 [SYS_fork]    sys_fork,
@@ -132,6 +135,9 @@ static int (*syscalls[])(void) = {
 #ifdef PDX_XV6
 [SYS_halt]    sys_halt,
 #endif // PDX_XV6
+#ifdef CS333_P1
+[SYS_date]    sys_date,
+#endif // CS333_P1
 };
 
 #ifdef PRINT_SYSCALLS
@@ -160,6 +166,9 @@ static char *syscallnames[] = {
 #ifdef PDX_XV6
   [SYS_halt]    "halt",
 #endif // PDX_XV6
+#ifdef CS333_P1
+  [SYS_date]    "date",
+#endif // CS333_P1
 };
 #endif // PRINT_SYSCALLS
```

### sysproc.c

```diff
diff --git a/sysproc.c b/sysproc.c
index 98563ea..40b676e 100644
--- a/sysproc.c
+++ b/sysproc.c
@@ -97,3 +97,18 @@ sys_halt(void)
   return 0;
 }
 #endif // PDX_XV6
+
+#ifdef CS333_P1
+int
+sys_date(void)
+{
+  struct rtcdate *d;
+
+  if(argptr(0, (void*)&d, sizeof(struct rtcdate)) < 0)
+    {return -1;}
+  cmostime(d);
+  return 0;
+
+}
+#endif
+
```

# Task 3 (Ctrl-P command)

## Result Image

## code diff

in this part, to enable Process information display we'll be edit 3 file, 
- proc.h, to add start_ticks
- proc.c, to add the process info display function

### proc.h

```diff
diff --git a/proc.h b/proc.h
index 0a0b4c5..c7ee129 100644
--- a/proc.h
+++ b/proc.h
@@ -49,6 +49,7 @@ struct proc {
   struct file *ofile[NOFILE];  // Open files
   struct inode *cwd;           // Current directory
   char name[16];               // Process name (debugging)
+  uint start_ticks;
 };
 
 // Process memory is laid out contiguously, low addresses first:
```

### proc.c

```diff
diff --git a/proc.c b/proc.c
index d030537..25e0211 100644
--- a/proc.c
+++ b/proc.c
@@ -149,6 +149,9 @@ allocproc(void)
   memset(p->context, 0, sizeof *p->context);
   p->context->eip = (uint)forkret;
 
+  // initialized start_ticks as ticks global variabel
+  p->start_ticks = ticks;
+
   return p;
 }
 
@@ -563,7 +566,14 @@ procdumpP2P3P4(struct proc *p, char *state_string)
 void
 procdumpP1(struct proc *p, char *state_string)
 {
-  cprintf("TODO for Project 1, delete this line and implement procdumpP1() in proc.c to print a row\n");
+  int temp_ticks = ticks - (p->start_ticks);
+  if (temp_ticks < 1000)
+    cprintf("%d\t%s\t       0.%d\t%s\t%d\t ", p->pid, p->name, temp_ticks, states[p->state], p->sz);
+  else{
+    int temp = temp_ticks % 1000;
+    temp_ticks = temp_ticks / 1000;
+    cprintf("%d\t%s\t       %d.%d\t%s\t%d\t ", p->pid, p->name, temp_ticks, temp, states[p->state], p->sz);
+  }
   return;
 }
 #endif
```


```git
tree 83addaa62370bd80d010b216baf3b557630edab5
parent 6a45f75d2f261196cd600b62451dafcc889fd18b
author taka <daffa.hp.dh@gmail.com> Tue May 4 22:00:33 2021 +0700
committer taka <daffa.hp.dh@gmail.com> Tue May 4 22:00:33 2021 +0700

third task, add process Information display


diff --git a/proc.c b/proc.c
index d030537..25e0211 100644
--- a/proc.c
+++ b/proc.c
@@ -149,6 +149,9 @@ allocproc(void)
   memset(p->context, 0, sizeof *p->context);
   p->context->eip = (uint)forkret;
 
+  // initialized start_ticks as ticks global variabel
+  p->start_ticks = ticks;
+
   return p;
 }
 
@@ -563,7 +566,14 @@ procdumpP2P3P4(struct proc *p, char *state_string)
 void
 procdumpP1(struct proc *p, char *state_string)
 {
-  cprintf("TODO for Project 1, delete this line and implement procdumpP1() in proc.c to print a row\n");
+  int temp_ticks = ticks - (p->start_ticks);
+  if (temp_ticks < 1000)
+    cprintf("%d\t%s\t       0.%d\t%s\t%d\t ", p->pid, p->name, temp_ticks, states[p->state], p->sz);
+  else{
+    int temp = temp_ticks % 1000;
+    temp_ticks = temp_ticks / 1000;
+    cprintf("%d\t%s\t       %d.%d\t%s\t%d\t ", p->pid, p->name, temp_ticks, temp, states[p->state], p->sz);
+  }
   return;
 }
 #endif
diff --git a/proc.h b/proc.h
index 0a0b4c5..c7ee129 100644
--- a/proc.h
+++ b/proc.h
@@ -49,6 +49,7 @@ struct proc {
   struct file *ofile[NOFILE];  // Open files
   struct inode *cwd;           // Current directory
   char name[16];               // Process name (debugging)
+  uint start_ticks;
 };
 
 // Process memory is laid out contiguously, low addresses first:
```
