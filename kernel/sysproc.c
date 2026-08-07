#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  backtrace();

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64
sys_sigreturn(void) {
  struct proc* p = myproc();

  if (p->s_pc == 0) return -1;

  p->trapframe->epc = p->trapframe_copy.epc;
  p->trapframe->ra = p->trapframe_copy.ra;
  p->trapframe->sp = p->trapframe_copy.sp;
  p->trapframe->gp = p->trapframe_copy.gp;
  p->trapframe->tp = p->trapframe_copy.tp;
  p->trapframe->t0 = p->trapframe_copy.t0;
  p->trapframe->t1 = p->trapframe_copy.t1;
  p->trapframe->t2 = p->trapframe_copy.t2;
  p->trapframe->s0 = p->trapframe_copy.s0;
  p->trapframe->s1 = p->trapframe_copy.s1;
  p->trapframe->a0 = p->trapframe_copy.a0;
  p->trapframe->a1 = p->trapframe_copy.a1;
  p->trapframe->a2 = p->trapframe_copy.a2;
  p->trapframe->a3 = p->trapframe_copy.a3;
  p->trapframe->a4 = p->trapframe_copy.a4;
  p->trapframe->a5 = p->trapframe_copy.a5;
  p->trapframe->a6 = p->trapframe_copy.a6;
  p->trapframe->a7 = p->trapframe_copy.a7;
  p->trapframe->s2 = p->trapframe_copy.s2;
  p->trapframe->s3 = p->trapframe_copy.s3;
  p->trapframe->s4 = p->trapframe_copy.s4;
  p->trapframe->s5 = p->trapframe_copy.s5;
  p->trapframe->s6 = p->trapframe_copy.s6;
  p->trapframe->s7 = p->trapframe_copy.s7;
  p->trapframe->s8 = p->trapframe_copy.s8;
  p->trapframe->s9 = p->trapframe_copy.s9;
  p->trapframe->s10 = p->trapframe_copy.s10;
  p->trapframe->s11 = p->trapframe_copy.s11;
  p->trapframe->t3 = p->trapframe_copy.t3;
  p->trapframe->t4 = p->trapframe_copy.t4;
  p->trapframe->t5 = p->trapframe_copy.t5;
  p->trapframe->t6 = p->trapframe_copy.t6;

  p->s_pc = 0;
  p->sig_flag = 0;

  return 0;
}

uint64
sys_sigalarm(void) {
  int interval;
  uint64 handler;
  struct proc* p = myproc();

  if (argint(0, &interval) < 0) return-1;
  if (argaddr(1, &handler) < 0) return -1;

  p->interval = interval;
  p->handler = (void (*)(void))handler;

  return 0;
}
