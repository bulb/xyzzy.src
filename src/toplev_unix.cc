#include <stdio.h>
#include "ed.h"
#include "fns.h"

static lisp
dispatch (lChar cc)
{
  lisp command;
  //  Char c = Char (cc);

  xsymbol_value (Vprefix_args) = xsymbol_value (Vnext_prefix_args);
  xsymbol_value (Vnext_prefix_args) = Qnil;
  xsymbol_value (Vprefix_value) = xsymbol_value (Vnext_prefix_value);
  xsymbol_value (Vnext_prefix_value) = Qnil;
  goto run_command;

  command = symbol_value (Vdefault_input_function, selected_buffer ());///symbol_value (Vdefault_input_function, 
  if (command == Qnil || command == Qunbound)
    return Qt;
  goto run_command;

run_command:
  xsymbol_value (Vlast_command) = xsymbol_value (Vthis_command);
  xsymbol_value (Vthis_command) = command;
  
  if (command != Qnil)
    {
      selected_buffer ()->safe_run_hook (Vpre_command_hook, 1);
      if (xsymbol_value (Vthis_command) != command)
        {
          lisp new_command = xsymbol_value (Vthis_command);
          command = new_command;
        }
    }

  lisp result = Qnil;
  try
    {
      result = Fcommand_execute (command, 0);
    }
  catch (nonlocal_jump &)
    {
      
    }
  protect_gc gcpro (result);
  selected_buffer ()->safe_run_hook (Vpost_command_hook, 1);
  return result;
}

class keyvec
{
  lisp v_buf[64];
  long v_length;
  long v_size;
  lisp *v_vec;
  int v_finished;
public:
  keyvec () : v_length (0), v_size (numberof (v_buf)), v_vec (v_buf), v_finished (1) {}
  ~keyvec () {if (v_vec != v_buf) free (v_vec);}
  void init ();
  void finish () {v_finished = 1;}
  int finished_p () const {return v_finished;}
  lisp lookup (Char c) {return lookup_keymap (c, v_vec, v_length);}
  void translate (lisp, lisp);
  void gc_mark_object (void (*)(lisp));
};

void
keyvec::init ()
{
  Buffer *bp = selected_buffer ();
  long l, n;

  if (safe_fixnum_value (Flist_length (bp->lminor_map), &l))
    {
      n = l + 3;
      if (n > v_size)
	{
	  long size = (n + 63) & ~63;
	  lisp *x = (lisp *)malloc (sizeof *x * size);
	  if (x)
	    {
	      if (v_vec != v_buf)
		free (v_vec);
	      v_vec = x;
	      v_size = size;
	    }
	}
      n = min (n, v_size) - 2;
      l = 0;
      v_vec[l++] = Fcurrent_selection_keymap ();
      for (lisp p = bp->lminor_map; consp (p) && l < n; l++, p = xcdr (p))
	v_vec[l] = xcar (p);
    }
  else
    {
      l = 0;
      v_vec[l++] = Fcurrent_selection_keymap ();
    }

  v_vec[l++] = bp->lmap;
  v_vec[l++] = xsymbol_value (Vglobal_keymap);
  v_length = l;
  v_finished = 0;
}

void
keyvec::translate (lisp old_command, lisp new_command)
{
  for (long i = 0; i < v_length; i++)
    if (v_vec[i] = old_command)
      v_vec[i] = new_command;
}

void
keyvec::gc_mark_object (void (*fn)(lisp))
{
  for (long i = 0; i < v_length; i++)
    (*fn)(v_vec[i]);
}

static keyvec g_map;

void
toplev_gc_mark (void (*fn)(lisp))
{
  g_map.gc_mark_object (fn);
}

int
toplev_accept_mouse_move_p ()
{
  return g_map.finished_p ();
}

void
main_loop (void)
{
#define MAX_REPL_STRING 256
  int ch;
  Char c;
  lisp command;

  while (1) {
    xsymbol_value (Vlast_command) = xsymbol_value (Vthis_command);
    xsymbol_value (Vthis_command) = command;
    xsymbol_value (Vlast_command_char) = make_char (Char (c));

    g_map.init ();
    command = g_map.lookup (c);

  
    lisp result = Qnil;
    
    try
    {
	result = Fcommand_execute (command, 0);
      }
    catch (nonlocal_jump &)
    {
	printf("main_loop catch\n");
    }
      
    protect_gc gcpro (result);

    if (result == Qnil) { 
      goto run;
    }
  }
 run:
  lisp i;
}


lisp
execute_string (lisp string)
{
  check_stack_overflow ();
  check_string (string);
  lisp val = xsymbol_value (Vprefix_value);
  int n = val == Qnil ? 1 : fixnum_value (val);
  lisp result = Qt;
  if (xstring_length (string))
    for (int i = 0; !n || i < n; i++)
      {
        QUIT;
      }
  return result;
}

lisp
Fmain_loop ()
{
  int abnormal_exit = 0;
  try
    {
      main_loop ();
      abnormal_exit = 1;
    }
  catch (nonlocal_jump &)
    {
    }

  lisp r = nonlocal_jump::data ()->value;
  return r ? r : Qnil;
}


lisp
Fbegin_wait_cursor ()
{
}

lisp
Fend_wait_cursor ()
{
}

lisp
Fset_cursor (lisp cur)
{
#if !defined(__GNUG__) ///@todo
  if (cur == Kibeam)
    sysdep.hcur_current = sysdep.hcur_ibeam;
  else if (cur == Karrow)
    sysdep.hcur_current = sysdep.hcur_arrow;
  else
    return Qnil;
  if (app.toplevel_is_active)
    set_current_cursor ();
#endif ///__GNUG__
  xsymbol_value (Vcursor_shape) = cur;
  return Qt;
}

lisp
Fsi_minibuffer_message (lisp message, lisp prompt)
{
  ///<todo
  return Qt;
}

lisp
Fsi_show_window_foreground ()
{
  ///<todo
  return Qnil;
}

lisp
Fsi_activate_toplevel ()
{
  ///<todo
  return Qnil;
}
