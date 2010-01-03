#ifndef _ed_h_
# define _ed_h_

# include "version.h"
#if !defined(__GNUG__)
# include "sysdep.h"
# include "xcolor.h"
#endif // __GNUG__
# include "lisp.h"
#if !defined(__GNUG__)
# include "vfs.h"
# include "wm.h"
# include "mousemsg.h"
#endif // __GNUG__

#if defined(__GNUG__)
# include <errno.h>
#endif // __GNUG__
struct Window;
struct Buffer;

class lwindow: public lisp_object
{
public:
  Window *wp;
};

# define windowp(X) typep ((X), Twindow)

inline void
check_window (lisp x)
{
  check_type (x, Twindow, Qwindow);
}

inline Window *&
xwindow_wp (lisp x)
{
  assert (windowp (x));
  return ((lwindow *)x)->wp;
}

class lbuffer: public lisp_object
{
public:
  Buffer *bp;
};

# define bufferp(X) typep ((X), Tbuffer)

inline void
check_buffer (lisp x)
{
  check_type (x, Tbuffer, Qbuffer);
}

inline Buffer *&
xbuffer_bp (lisp x)
{
  assert (bufferp (x));
  return ((lbuffer *)x)->bp;
}

class lmarker: public lisp_object
{
public:
  Buffer *buffer;
  point_t point;
  ~lmarker ();
};

# define markerp(X) typep ((X), Tmarker)

inline void
check_marker (lisp x)
{
  check_type (x, Tmarker, Qmarker);
}

inline Buffer *&
xmarker_buffer (lisp x)
{
  assert (markerp (x));
  return ((lmarker *)x)->buffer;
}

inline point_t &
xmarker_point (lisp x)
{
  assert (markerp (x));
  return ((lmarker *)x)->point;
}

class lregexp: public lisp_object
{
public:
  Char *pattern;
  int length;
  enum
    {
      MATCH_VOID = 1,
      MATCH_BOL = 2,
      TRANSLATE = 4,
      HAS_BACKREF = 8
    };
  int flags;
  lisp source;

  ~lregexp () {xfree (pattern);}
};

# define regexpp(X) typep ((X), Tregexp)

inline Char *&
xregexp_pattern (lisp x)
{
  assert (regexpp (x));
  return ((lregexp *)x)->pattern;
}

inline int &
xregexp_length (lisp x)
{
  assert (regexpp (x));
  return ((lregexp *)x)->length;
}

inline int &
xregexp_flags (lisp x)
{
  assert (regexpp (x));
  return ((lregexp *)x)->flags;
}

inline lisp &
xregexp_source (lisp x)
{
  assert (regexpp (x));
  return ((lregexp *)x)->source;
}

inline void
check_regexp (lisp x)
{
  check_type (x, Tregexp, Qregexp);
}

#if defined(_MSC_VER)
#define MENU_ID_RANGE_MIN 0
#define MENU_ID_RANGE_MAX 1024
#define TOOL_ID_RANGE_MIN 1024
#define TOOL_ID_RANGE_MAX 2048

class lwin32_menu: public lisp_object
{
public:
  HMENU handle;
  int id;
  lisp init;
  lisp command;
  lisp tag;

  ~lwin32_menu ();
};

# define win32_menu_p(X) typep ((X), Twin32_menu)

inline void
check_win32_menu (lisp x)
{
  check_type (x, Twin32_menu, Qmenu);
}

inline HMENU &
xwin32_menu_handle (lisp x)
{
  assert (win32_menu_p (x));
  return ((lwin32_menu *)x)->handle;
}

inline int &
xwin32_menu_id (lisp x)
{
  assert (win32_menu_p (x));
  return ((lwin32_menu *)x)->id;
}

inline lisp &
xwin32_menu_init (lisp x)
{
  assert (win32_menu_p (x));
  return ((lwin32_menu *)x)->init;
}

inline lisp &
xwin32_menu_tag (lisp x)
{
  assert (win32_menu_p (x));
  return ((lwin32_menu *)x)->tag;
}

inline lisp &
xwin32_menu_command (lisp x)
{
  assert (win32_menu_p (x));
  return ((lwin32_menu *)x)->command;
}

void check_popup_menu (lisp lmenu);

class lwin32_dde_handle: public lisp_object
{
public:
  HCONV hconv;
  ~lwin32_dde_handle ();
};

# define win32_dde_handle_p(X) typep ((X), Twin32_dde_handle)

inline
lwin32_dde_handle::~lwin32_dde_handle ()
{
  if (hconv)
    DdeDisconnect (hconv);
}

inline void
check_win32_dde_handle (lisp x)
{
  check_type (x, Twin32_dde_handle, Qdde_handle);
}

inline HCONV &
xwin32_dde_handle_hconv (lisp x)
{
  assert (win32_dde_handle_p (x));
  return ((lwin32_dde_handle *)x)->hconv;
}
#endif // _MSC_VER

inline lwindow *
make_window ()
{
  lwindow *p = ldata <lwindow, Twindow>::lalloc ();
  p->wp = 0;
  return p;
}

inline lbuffer *
make_buffer ()
{
  lbuffer *p = ldata <lbuffer, Tbuffer>::lalloc ();
  p->bp = 0;
  return p;
}

inline lmarker *
make_marker ()
{
  lmarker *p = ldata <lmarker, Tmarker>::lalloc ();
  p->buffer = 0;
  return p;
}

inline lregexp *
make_regexp ()
{
  lregexp *p = ldata <lregexp, Tregexp>::lalloc ();
  p->pattern = 0;
  p->source = Qnil;
  return p;
}

#if defined(_MSC_VER)
inline lwin32_dde_handle *
make_win32_dde_handle ()
{
  lwin32_dde_handle *p = ldata <lwin32_dde_handle, Twin32_dde_handle>::lalloc ();
  p->hconv = 0;
  return p;
}
#endif // _MSC_VER

struct Window;
struct Buffer;

#if !defined(__GNUG__)
# include "process.h"
# include "dll.h"
#endif // __GNUG__
# include "chunk.h"
#if !defined(__GNUG__)
# include "oledata.h"
# include "waitobj.h"
#endif // __GNUG__#

# include "ces.h"

#if !defined(__GNUG__)
# include "kbd.h"
# include "mouse.h"
# include "statarea.h"
# include "timer.h"
# include "font.h"
# include "utimer.h"
#endif // __GNUG__

#if !defined(__GNUG__) ///<TODO
class StatusWindow
{
public:
  enum {TEXT_MAX = 1024};
  HWND sw_hwnd;
protected:
  struct swbuf
    {
      int l;
      int textf;
      ucs2_t buf[TEXT_MAX];
    } sw_last;
  ucs2_t sw_buf[TEXT_MAX];
  ucs2_t *sw_b;
  ucs2_t *sw_tail;
public:
  void restore ();
  int text (const char *);
  void puts (const Char *, int);
  int putc (Char);
  void newline ();
  void puts (const char *, int);
  void puts (int, int);
  void flush ();
  void clear (int = 0);
  StatusWindow ();
  void set (HWND);
  int paint (const DRAWITEMSTRUCT *);
};
#endif // __GNUG__

class FKWin;

#if defined(__GNUG__) ///@todo
typedef struct {
  u_long cx;
  u_long cy;
} SIZE;

typedef struct {
  long left;
  long top;
  long right;
  long bottom;
} RECT;
#endif // __GNUG__

struct Frame
{
  Window *windows;
  Window *selected;
  Window *reserved;
  Window *deleted;
  int windows_moved;

#if !defined(__GNUG__) ///<TODO
  HWND hwnd;
#endif // __GNUG__
  SIZE size;

#if !defined(__GNUG__) ///<TODO
  int has_focus;
  HWND has_caret;
  HWND has_caret_last;
  int caret_on;
  int gray_caret;
  SIZE caret_size;
  POINT caret_pos;
  COLORREF last_caret_color;

  FKWin *fnkey;
#endif // __GNUG__
};

struct ModelineParam
{
#if !defined(__GNUG__) ///<TODO
  HFONT m_hfont;
#endif // __GNUG__
  int m_height;
  int m_exlead;
  int m_exts[22];

  ModelineParam ();
  ~ModelineParam ();
#if !defined(__GNUG__) ///<TODO
  void init (HFONT);
#endif // __GNUG__
};

struct Region
{
  point_t p1;
  point_t p2;
};

class Application
{
public:
  Application ();
  ~Application ();
#if !defined(__GNUG__) ///<TODO

  static const char ToplevelClassName[];
  static const char FrameClassName[];
  static const char ClientClassName[];
  static const char ModelineClassName[];

  HINSTANCE hinst;
  HWND toplev;
  HWND hwnd_sw;

  HWND hwnd_clipboard;

  kbd_queue kbdq;
  mouse_state mouse;
  ime_comp_queue ime_compq;
#endif // __GNUG__
  Frame active_frame;
#if !defined(__GNUG__)
  FontSet text_font;
  ModelineParam modeline_param;
  StatusWindow status_window;
  key_sequence keyseq;
  itimer gc_itimer;
  itimer as_itimer;

  status_area stat_area;

#endif // __GNUG__
  int default_tab_columns;
  int auto_save_count;

#if !defined(__GNUG__)
  int toplevel_is_active;
  int ime_composition;
  int ime_open_mode;

  int last_vkeycode;
  int kbd_repeat_count;
  int wait_cursor_depth;

  u_int quit_thread_id;
  int sleep_timer_exhausted;
  int f_protect_quit;

  int f_in_drop;
  Window *drop_window;
  Window *drag_window;
  Buffer *drag_buffer;
  Region drag_region;

  DWORD last_cmd_tick;
  int f_auto_save_pending;

  UINT default_caret_blink_time;
  int last_blink_caret;

  char dump_image[PATH_MAX + 8];
  char *ini_file_path;

  lisp lquit_char;
  int quit_vkey;
  int quit_mod;

  ATOM atom_toplev;
  int minibuffer_prompt_column;

  utimer user_timer;
#endif // __GNUG__
  
  void *initial_stack;
  
#if !defined(__GNUG__)
  int in_gc;
#endif // __GNUG__
};

extern Application app;

class enable_quit
{
  static char q_enable;
  char q_save;
public:
  enable_quit () : q_save (q_enable)
    {
      if (!q_save)
        {
          q_enable = 1;
#if defined(_MSC_VER)
          PostThreadMessage (app.quit_thread_id, WM_PRIVATE_REGISTER_HOTKEY, 0, 0);
#endif // _MSC_VER
        }
    }
  ~enable_quit () {if (!q_save) disable ();}
  static void disable ()
    {
      if (q_enable)
        {
#if defined(_MSC_VER)
          PostThreadMessage (app.quit_thread_id, WM_PRIVATE_UNREGISTER_HOTKEY, 0, 0);
#endif // _MSC_VER
          q_enable = 0;
        }
    }
};

# include "syntax.h"  ///FIXME. for syntax_code
# include "Buffer.h"
# include "Window.h"
#if !defined(__GNUG__)
# include "msgbox.h"
# include "resource.h"
#else // __GNUG__
# include "process.h" ///FIXME. for eol_code, Region
#endif // __GNUG__
inline Window *
selected_window ()
{
  return app.active_frame.selected;
}

inline Buffer *
selected_buffer ()
{
  assert (selected_window ());
  return selected_window ()->w_bufp;
}

#if !defined(__GNUG__)
inline HWND
get_active_window ()
{
  HWND hwnd = GetActiveWindow ();
  return hwnd ? hwnd : app.toplev;
}
#endif // __GNUG__

inline
lmarker::~lmarker ()
{
  if (buffer)
    delq (this, &buffer->lmarkers);
}

class save_excursion
{
  Buffer *se_bufp;
  save_excursion *se_prev;
public:
  point_t se_point;
private:
  void cleanup (int);
public:
  save_excursion ();
  ~save_excursion ();
  void cleanup ();
  save_excursion *prev () {return se_prev;}
  void invalid () {se_bufp = 0;}
};

inline
save_excursion::save_excursion ()
{
  Window *wp = selected_window ();
  se_bufp = wp->w_bufp;
  se_prev = se_bufp->b_excursion;
  se_bufp->b_excursion = this;
  se_point = wp->w_point.p_point;
}

inline
save_excursion::~save_excursion ()
{
  cleanup (0);
}

inline void
save_excursion::cleanup ()
{
  cleanup (1);
}

class save_restriction
{
  Buffer *sr_bufp;
  save_restriction *sr_prev;
  int sr_depth;
public:
  Region sr_contents;

  save_restriction ();
  ~save_restriction ();
  save_restriction *prev () {return sr_prev;}
  void invalid () {sr_bufp = 0;}
};

inline
save_restriction::save_restriction ()
{
  sr_bufp = selected_buffer ();
  sr_prev = sr_bufp->b_restriction;
  sr_bufp->b_restriction = this;
  sr_contents = sr_bufp->b_contents;
  sr_depth = sr_bufp->b_narrow_depth;
}

class save_cursor_depth
{
  int odepth;
public:
#if defined(_MSC_VER)
  save_cursor_depth () : odepth (app.wait_cursor_depth) {}
#else // __GNUG__
  save_cursor_depth () {} ///<@todo
#endif // __GNUG__
  ~save_cursor_depth ()
    {
#if !defined(__GNUG__) ///@todo
      if (!odepth)
        end_wait_cursor (1);
      else
        app.wait_cursor_depth = odepth;
#endif // __GNUG__
    }
};

#endif
