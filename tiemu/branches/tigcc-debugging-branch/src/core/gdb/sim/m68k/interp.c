/* Simulator for the Motorola 68k architecture.

   Original version:
   -----
   Written by William Cohen of Red Hat,
   wcohen@redhat.com

   This file is part of SH sim


              THIS SOFTWARE IS NOT COPYRIGHTED

   Cygnus offers the following for use in the public domain.  Cygnus
   makes no warranty with regard to the software or it's performance
   and the user accepts the software "AS IS" with all faults.

   CYGNUS DISCLAIMS ANY WARRANTIES, EXPRESS OR IMPLIED, WITH REGARD TO
   THIS SOFTWARE INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
   -----

   Modifications for TiEmu:
   -----
   Copyright (C) 2005 Kevin Kofler

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
 
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
 
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
   -----

*/

#include "config.h"

#include <sys/types.h>
#include <setjmp.h>

#include <signal.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "sysdep.h"
#include "bfd.h"
#include "gdb/callback.h"
#include "gdb/remote-sim.h"

#include "config.h"
#include "../../../uae/libuae.h"
#include "dis-asm.h"

int trace = 0;
int verbose_trace = 0;
int cloanto_rom = 0;

static bfd *current_bfd = 0;

static jmp_buf interp_trap;

/* This file is local - if newlib changes, then so should this.  */
#include "syscall.h"

#include <math.h>

#ifdef _WIN32
#include <float.h>            /* Needed for _isnan() */
#define isnan _isnan
#endif

#ifndef SIGBUS
#define SIGBUS SIGSEGV
#endif

#ifndef SIGQUIT
#define SIGQUIT SIGTERM
#endif

#ifndef SIGTRAP
#define SIGTRAP 5
#endif


struct regstruct saved_state;
struct uae_prefs currprefs;
static int opt_cpu_level = 0; /* 68000 default */
static int opt_mem_size = 20; /* 1M default */

/* These variables are at file scope so that functions other than
   sim_resume can use the fetch/store macros */

static int target_little_endian;
static int global_endianw, endianb;
static int target_dsp;
static int host_little_endian;

static SIM_OPEN_KIND sim_kind;
static char *myname;

static FILE *profile_file;

static int sim_profile_size = 17;
static int nsamples;

static host_callback *callback;

#define MMASKB ((saved_state.msize -1) & ~0)

#if defined(__GO32__) || defined(_WIN32)
int sim_memory_size = 19;
#else
int sim_memory_size = 24;
#endif

static unsigned INLINE
swap (n)
     unsigned n;
{
  if (endianb)
    n = (n << 24 | (n & 0xff00) << 8
       | (n & 0xff0000) >> 8 | (n & 0xff000000) >> 24);
  return n;
}

int 
fail ()
{
  abort ();
}

#define RAISE_EXCEPTION(x) \
  (saved_state.exception = x, saved_state.insn_end = 0)

/* This function exists mainly for the purpose of setting a breakpoint to
   catch simulated bus errors when running the simulator under GDB.  */

void
raise_exception (x)
     int x;
{
  RAISE_EXCEPTION(x);
}

void
raise_buserror ()
{
  raise_exception (SIGBUS);
}

void
control_c (sig, code, scp, addr)
     int sig;
     int code;
     char *scp;
     char *addr;
{
  raise_exception (SIGINT);
}

/* Set the memory size to the power of two provided. */

void
sim_size (power)
     int power;

{
  opt_mem_size = power;
}

static int initted_memory = 0;

static void
parse_and_set_memory_size (str)
     char *str;
{
#if 0
  int n;

  /* NOTE figure out the amount of memory that should be allocated */
  n = strtol (str, NULL, 10);

  if (n > 0 && n <= 24)
    sim_memory_size = n;
  else
    callback->printf_filtered (callback, "Bad memory size %d; must be 1 to 24, inclusive\n", n);

  currprefs.chipmem_size = n * 1024*1024;
#endif
}


SIM_DESC
sim_open (kind, cb, abfd, argv)
     SIM_OPEN_KIND kind;
     host_callback *cb;
     struct bfd *abfd;
     char **argv;
{
  char **p;
  int endian_set = 0;
  int i;
  union
    {
      int i;
      short s[2];
      char c[4];
    }
  mem_word;

  current_bfd = abfd;

  sim_kind = kind;
  myname = argv[0];
  callback = cb;

#if 0
  /* default 1Mb memory, at 0x80000000 */
  currprefs.chipmem_size = 1 << opt_mem_size;
  currprefs.cpu_level = opt_cpu_level;
#endif

  if (getenv("VERBOSE_TRACE"))
    verbose_trace = 1;

  for (p = argv + 1; *p != NULL; ++p)
    {
      if (strcmp (*p, "-E") == 0)
      {
        ++p;
        if (*p == NULL)
          {
            /* FIXME: This doesn't use stderr, but then the rest of the
               file doesn't either.  */
            callback->printf_filtered (callback, "Missing argument to `-E'.\n");
            return 0;
          }
        target_little_endian = strcmp (*p, "big") != 0;
          endian_set = 1;
      }
      else if (isdigit (**p))
      parse_and_set_memory_size (*p);
    }

  memory_init();

  if (abfd != NULL && ! endian_set)
      target_little_endian = ! bfd_big_endian (abfd);

  for (i = 4; (i -= 2) >= 0; )
    mem_word.s[i >> 1] = i;
  global_endianw = mem_word.i >> (target_little_endian ? 0 : 16) & 0xffff;

  for (i = 4; --i >= 0; )
    mem_word.c[i] = i;
  endianb = mem_word.i >> (target_little_endian ? 0 : 24) & 0xff;

  init_m68k ();

  /* fudge our descriptor for now */
  return (SIM_DESC) 1;
}

void
sim_set_cpu_variant(int which)
{
#if 0
  switch (which)
    {
    case '0':
    case '1':
    case '2':
    case '4':
      opt_cpu_level = which - '0';
      break;
    case 8:
      opt_cpu_level = 3;
      break;
    }
  printf("cpu_variant set to %d\n", opt_cpu_level);
#endif
}

SIM_RC
sim_create_inferior (sd, prog_bfd, argv, env)
     SIM_DESC sd;
     struct bfd *prog_bfd;
     char **argv;
     char **env;
{
  current_bfd = prog_bfd;
  setlinebuf(stdout);
  /* clear the registers */
  memset (&saved_state, 0,
        (char*)&saved_state.end_of_registers - (char*)&saved_state);
  /* set the PC */
  saved_state.pc = 0x80000000;
  if (prog_bfd != NULL)
    saved_state.pc = bfd_get_start_address (prog_bfd);
  saved_state.pc_p = saved_state.pc_oldp = get_real_address(saved_state.pc);
  saved_state.regs[7+8] = /* $a7 */
    saved_state.usp =
    saved_state.isp =
    /*saved_state.msp =*/ 0x4c00 /*0x80000000 + currprefs.chipmem_size - 16*/;
  return SIM_RC_OK;
}

int
sim_trace (sd)
     SIM_DESC sd;
{
  trace++;
  sim_resume(sd, 1, 0);
  trace--;
  return 1;
}


int
sim_stop (sd)
     SIM_DESC sd;
{
  raise_exception (SIGINT);
  return 1;
}

void
sim_set_trace (void)
{
  trace = 1;
}

void
sim_set_callbacks (p)
     host_callback *p;
{
  callback = p;
}

static void
init_pointers ()
{
  host_little_endian = 0;
  *(char*)&host_little_endian = 1;
  host_little_endian &= 1;

  /* NOTE figure out if memory size has changed */

  if (saved_state.profile && !profile_file)
    {
      profile_file = fopen ("gmon.out", "wb");
      /* Seek to where to put the call arc data */
      nsamples = (1 << sim_profile_size);

      fseek (profile_file, nsamples * 2 + 12, 0);

      if (!profile_file)
      {
        fprintf (stderr, "Can't open gmon.out\n");
      }
      else
      {
        saved_state.profile_hist =
          (unsigned short *) calloc (64, (nsamples * sizeof (short) / 64));
      }
    }
}

static int exit_code = 0;

void
sim_stop_reason (sd, reason, sigrc)
     SIM_DESC sd;
     enum sim_stop *reason;
     int *sigrc;
{
  /* The m68k simulator uses SIGQUIT to indicate that the program has
     exited, so we must check for it here and translate it to exit.  */
  if (saved_state.exception == SIGQUIT)
    {
      *reason = sim_exited;
      *sigrc = exit_code;
    }
  else
    {
      *reason = sim_stopped;
      *sigrc = saved_state.exception;
    }
}

void
sim_info (sd, verbose)
     SIM_DESC sd;
     int verbose;
{
#if 0
  char *cpu;
  switch (currprefs.cpu_level)
    {
    case 1: cpu="68010"; break;
    case 2: cpu="68020"; break;
    case 3: cpu="68020/881"; break;
    case 4: cpu="68040"; break;
    default: cpu="68000"; break;
    }
  callback->printf_filtered (callback, "\nCPU Configuration: %s\n", cpu);
  callback->printf_filtered (callback, "Memory: %dKb from 0x%08x to 0x%08x\n",
                           currprefs.chipmem_size/1024,
                           0x80000000, 0x80000000 + currprefs.chipmem_size);
#endif
}

// FIXME: This is crude and will most likely not work as expected.
extern int hw_m68k_run(int n, unsigned maxcycles);
static void m68k_go_sim(int step)
{
  if (step)
    hw_m68k_run(1, 0);
  else
    while(1) hw_m68k_run(1, 0);
}

void
sim_resume (sd, step, siggnal)
     SIM_DESC sd;
     int step, siggnal;
{
  void (*prev) ();
  void (*prev_fpe) ();
  /* NOTE need to actually run things */

  /*  prev = signal (SIGINT, control_c);*/
  prev_fpe = signal (SIGFPE, SIG_IGN);

  init_pointers ();
  saved_state.exception = 0;
  if (step)
    saved_state.exception = SIGTRAP;

  saved_state.pc_p = saved_state.pc_oldp = get_real_address(saved_state.pc);
  memcpy (&regs, &saved_state, sizeof(regs));
  if (setjmp(interp_trap) == 0)
    m68k_go_sim (step);
  memcpy (&saved_state, &regs, sizeof(regs));
  saved_state.pc = m68k_getpc();

  signal (SIGFPE, prev_fpe);
  /*  signal (SIGINT, prev);*/
}


SIM_RC
sim_load (sd, prog, abfd, from_tty)
     SIM_DESC sd;
     char *prog;
     bfd *abfd;
     int from_tty;
{
  extern bfd *sim_load_file (); /* ??? Don't know where this should live.  */
  bfd *prog_bfd;

  prog_bfd = sim_load_file (sd, myname, callback, prog, abfd,
                          sim_kind == SIM_OPEN_DEBUG,
                          0, sim_write);
  if (prog_bfd == NULL)
    return SIM_RC_FAIL;
  if (abfd == NULL)
    bfd_close (prog_bfd);
  return SIM_RC_OK;
}

int
sim_write (sd, addr, buffer, size)
     SIM_DESC sd;
     SIM_ADDR addr;
     unsigned char *buffer;
     int size;
{
  int i;

  init_pointers ();
  for (i = 0; i < size; i++)
    put_byte(addr + i, buffer[i]);
  return size;
}


int
sim_read (sd, addr, buffer, size)
     SIM_DESC sd;
     SIM_ADDR addr;
     unsigned char *buffer;
     int size;
{
  int i;

  init_pointers ();

  for (i = 0; i < size; i++)
    {
      buffer[i] = get_byte(addr+i);
    }
  return size;
}


void
sim_close (sd, quitting)
     SIM_DESC sd;
     int quitting;
{
  /* nothing to do */
}

int
sim_fetch_register (sd, rn, memory, length)
     SIM_DESC sd;
     int rn;
     unsigned char *memory;
     int length;
{
  int val;
  switch (rn)
    {
      /* data regs "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7" */
    case  0: case  1: case  2: case  3: case  4: case  5: case  6: case  7:
      /* address regs "a0", "a1", "a2", "a3", "a4", "a5", "fp", "sp" */
    case  8: case  9: case 10: case 11: case 12: case 13: case 14: case 15:
      val = saved_state.regs[rn];
      break;
      /*  "ps" */
    case 16:
      val = saved_state.usp;
      break;
      /* "pc" */
    case 17:
      val = saved_state.pc;
      break;
      /*  "fp0", "fp1", "fp2", "fp3", "fp4", "fp5", "fp6", "fp7" */
    case 18: case 19: case 20: case 21:
    case 22: case 23: case 24: case 25:
      return 0;
      /* "fpcontrol" */
    case 26:
      val = saved_state.fpcr;
      break;
      /* "fpstatus" */
    case 27:
      val = saved_state.fpsr;
      break;
      /* "fpiaddr" */
    case 28:
      val = saved_state.fpiar;
      break;
      /* "fpcode" */
    case 29:
      return 0;
      /* "fpflags" */
    case 30:
      return 0;
    default:
      return 0;
    }
  
  * (int *) memory = swap (val);
  return -1;
}

int
sim_store_register (sd, rn, memory, length)
     SIM_DESC sd;
     int rn;
     unsigned char *memory;
     int length;
{
  unsigned val;

  val = swap (* (int *)memory);
  switch (rn)
    {
      /* data regs "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7" */
    case  0: case  1: case  2: case  3: case  4: case  5: case  6: case  7:
      /* address regs "a0", "a1", "a2", "a3", "a4", "a5", "fp", "sp" */
    case  8: case  9: case 10: case 11: case 12: case 13: case 14: case 15:
      saved_state.regs[rn] = val;
      break;
      /*  "ps" */
    case 16:
      saved_state.usp = val;
      break;
      /* "pc" */
    case 17:
      saved_state.pc = val;
      break;
      /*  "fp0", "fp1", "fp2", "fp3", "fp4", "fp5", "fp6", "fp7" */
    case 18: case 19: case 20: case 21:
    case 22: case 23: case 24: case 25:
      return 0;
      /* "fpcontrol" */
    case 26:
      saved_state.fpcr = val;
      break;
      /* "fpstatus" */
    case 27:
      saved_state.fpsr = val;
      break;
      /* "fpiaddr" */
    case 28:
      saved_state.fpiar = val;
      break;
      /* "fpcode" */
    case 29:
      return 0;
      /* "fpflags" */
    case 30:
      return 0;
    default:
      return 0;
    }

  return -1;
}

void
sim_do_command (sd, cmd)
     SIM_DESC sd;
     char *cmd;
{
  if (cmd == NULL || *cmd == '\0')
    {
      cmd = "help";
    }
}
  
  
/* stuff from UAE's main.c */

void write_log_standard (const char *fmt, ...)
{
    va_list ap;
    va_start (ap, fmt);
#ifdef HAVE_VFPRINTF
    vfprintf (stderr, fmt, ap);
#else
    /* Technique stolen from GCC.  */
    {
      int x1, x2, x3, x4, x5, x6, x7, x8;
      x1 = va_arg (ap, int);
      x2 = va_arg (ap, int);
      x3 = va_arg (ap, int);
      x4 = va_arg (ap, int);
      x5 = va_arg (ap, int);
      x6 = va_arg (ap, int);
      x7 = va_arg (ap, int);
      x8 = va_arg (ap, int);
      fprintf (stderr, fmt, x1, x2, x3, x4, x5, x6, x7, x8);
    }
#endif
}

int quit_program = 0;

void uae_reset (void)
{
    if (quit_program == 0)
      quit_program = -2;
}

void uae_quit (void)
{
    if (quit_program != -1)
      quit_program = -1;
}

REGPARAM void call_calltrap (int which)
{
  fprintf(stderr, "calltrap %d\n", which);
}

void customreset ()
{
}

void
sim_exception (int which, int pc, int oldpc)
{
  /*  if (trace)*/
  switch (which) {
  case 47:
    regs.exception = SIGTRAP;
    regs.pc = pc - 2;
    regs.pc_p = regs.pc_oldp = get_real_address(regs.pc);
    break;
  default:
    regs.exception = which;
    break;
  }
  longjmp(interp_trap, 1);
}

static int
get_arg (int which)
{
  return get_long (m68k_areg(regs, 7) + 4 + 4*which);
}

static void
put_arg (int which, int value)
{
 put_long (m68k_areg(regs, 7) + 4 + 4*which, value);
}

#define S_ERR(c) \
      if (c) { \
              put_arg(0, errno); SET_CFLG(1); \
      } else { \
              m68k_dreg(regs,0) = rv; SET_CFLG(0); \
      }; return

static void
put (unsigned char *addr, int size, int value)
{
  switch (size)
    {
    case 1:
      do_put_mem_byte (addr, value);
      break;
    case 2:
      do_put_mem_word ((uae_u16 *)addr, value);
      break;
    case 4:
      do_put_mem_long ((uae_u32 *)addr, value);
      break;
    }
}

int
sim_os_trap (int which)
{
  int rv;
  unsigned char *addr;
  struct stat s;

  switch (which)
    {
    case 1:
      if (trace)
      printf("exit (%d)\n", get_arg(0));
      regs.exception = SIGQUIT;
      exit_code = get_arg(0);
      longjmp(interp_trap, 1);

    case 3:
      rv = read(get_arg(0), get_real_address(get_arg(1)), get_arg(2));
      if (trace)
      printf("read (%d,%08x,%d) = %d\n", get_arg(0), get_arg(1), get_arg(2), rv);
      S_ERR(rv<0);

    case 4:
      rv = write(get_arg(0), get_real_address(get_arg(1)), get_arg(2));
      if (trace)
      printf("write (%d,%08x,%d) = %d\n", get_arg(0), get_arg(1), get_arg(2), rv);
      S_ERR(rv<0);

    case 5:
      rv = open(get_real_address(get_arg(0)), get_arg(1), get_arg(2));
      if (trace)
      printf("open (%s,%d,%d) = %d\n", get_real_address(get_arg(0)), get_arg(1), get_arg(2), rv);
      S_ERR(rv<0);

    case 6:
      if (get_arg(0) >= 3)
      rv = close(get_arg(0));
      else
      rv = 0;
      if (trace)
      printf("close (%d) = %d\n", get_arg(0), rv);
      S_ERR(rv<0);

    case 17:
      rv = 0;
#if 0
      if (get_arg(0) > 0x80000000 + currprefs.chipmem_size)
      rv  = -1;
#endif
      if (trace)
      printf("brk (0x%08x) = %d\n", get_arg(0), rv);
      S_ERR(rv<0);

    case 28:
      rv = fstat(get_arg(0), &s);
      addr = get_real_address(get_arg(1));
      put(addr+0, 2, s.st_dev);
      put(addr+2, 2, s.st_ino);
      put(addr+4, 4, s.st_mode);
      put(addr+8, 2, s.st_nlink);
      put(addr+10, 2, s.st_uid);
      put(addr+12, 2, s.st_gid);
      put(addr+14, 2, s.st_rdev);
      put(addr+16, 4, s.st_size);
      put(addr+20, 4, s.st_atime);
      put(addr+28, 4, s.st_mtime);
      put(addr+36, 4, s.st_ctime);
      put(addr+44, 4, s.st_blksize);
      put(addr+48, 4, s.st_blocks);
      S_ERR(rv<0);

    case 29:
      rv = isatty(get_arg(0));
      S_ERR(0);

    case 199:
      rv = lseek(get_arg(0), get_arg(1), get_arg(2));
      if (trace)
      printf("lseek (%d,%d,%d) = %d\n", get_arg(0), get_arg(1), get_arg(2), rv);
      S_ERR(rv<0);

    default:
      printf("sim_os_trap(%d)\n", which);
      abort();
    }
  return 2;
}


static int
sim_dis_read (memaddr, ptr, length, info)
     bfd_vma memaddr;
     bfd_byte *ptr;
     unsigned int length;
     struct disassemble_info *info;
{
  sim_read (0, memaddr, ptr, length);
  return 0;
}

/* Filter out (in place) symbols that are useless for disassembly.
   COUNT is the number of elements in SYMBOLS.
   Return the number of useful symbols. */

static long
remove_useless_symbols (symbols, count)
     asymbol **symbols;
     long count;
{
  register asymbol **in_ptr = symbols, **out_ptr = symbols;

  while (--count >= 0)
    {
      asymbol *sym = *in_ptr++;

      if (strstr(sym->name, "gcc2_compiled"))
      continue;
      if (sym->name == NULL || sym->name[0] == '\0')
      continue;
      if (sym->flags & (BSF_DEBUGGING))
      continue;
      if (bfd_is_und_section (sym->section)
        || bfd_is_com_section (sym->section))
      continue;

      *out_ptr++ = sym;
    }
  return out_ptr - symbols;
}

static int 
compare_symbols (ap, bp)
     const PTR ap;
     const PTR bp;
{
  const asymbol *a = *(const asymbol **)ap;
  const asymbol *b = *(const asymbol **)bp;

  if (bfd_asymbol_value (a) > bfd_asymbol_value (b))
    return 1;
  else if (bfd_asymbol_value (a) < bfd_asymbol_value (b))
    return -1;
  return 0;
}

static char opbuf[1000];

int
op_printf(char *buf, char *fmt, ...)
{
  int ret;
  va_list ap;

  va_start (ap, fmt);
  ret = vsprintf (opbuf+strlen(opbuf), fmt, ap);
  va_end (ap);
  return ret;
}

void
sim_trace_one(pc)
     int pc;
{
  static int initted = 0;
  static asymbol **symtab = 0;
  static int symcount = 0;
  static int last_sym = -1;
  static struct disassemble_info info;
  int storage, sym, bestsym, bestaddr;
  int min, max, i, pnl;
  static uae_u32 prevregs[16];

  if (current_bfd == 0)
    return;

  if (!initted)
    {
      initted = 1;
      memset(prevregs, 0, sizeof(prevregs));
      memset(&info, 0, sizeof(info));
      INIT_DISASSEMBLE_INFO(info, stdout, op_printf);
      info.read_memory_func = sim_dis_read;

      storage = bfd_get_symtab_upper_bound (current_bfd);
      if (storage > 0)
      {
        symtab = (asymbol **) xmalloc (storage);
        symcount = bfd_canonicalize_symtab (current_bfd, symtab);
        symcount = remove_useless_symbols (symtab, symcount);
        qsort (symtab, symcount, sizeof(asymbol *), compare_symbols);
      }
    }

  min = -1; max = symcount;
  while (min < max-1) {
    bfd_vma sa;
    sym = (min+max)/2;
    sa = bfd_asymbol_value (symtab[sym]);
    /*printf("checking %4d %08x %s\n", sym, sa, bfd_asymbol_name (symtab[sym]));*/
    if (sa > pc)
      max = sym;
    else if (sa < pc)
      min = sym;
    else
      {
      min = sym;
      break;
      }
  }
  bestaddr = bfd_asymbol_value (symtab[min]);
  if (min != -1 && min != last_sym)
    {
      printf("%s", bfd_asymbol_name (symtab[min]));
      if (bestaddr != pc)
      printf("+%d", pc-bestaddr);
      printf(":\n");
      last_sym = min;
    }
  opbuf[0] = 0;
  max = print_insn_m68k(pc, &info);
  printf("  %8x: ", pc, 0);
  for (i=0; i<max; i++)
    printf(" %02x", get_byte(pc+i));
  for (; i<7; i++)
    printf("   ");
  printf("%s", opbuf);

  pnl = 0;
  for (i=0; i<16; i++)
    if (prevregs[i] != regs.regs[i])
      {
      if (pnl == 0)
        printf("\t");
      prevregs[i] = regs.regs[i];
      printf(" r%d=%08x", i, regs.regs[i]);
      pnl = 1;
      }

  printf("\n");
}
