#include "ed.h"
#include "data.h"
#include "Window.h"

#if defined(__GNUG__)
# include <stdio.h>
# include <stdlib.h>
#endif ///__GNUG__

Application app;

Application::Application ()
{
}

Application::~Application ()
{
}

char enable_quit::q_enable;

static void
init_command_line (int ac)
{
}

static lisp
make_path (const char *s, int append_slash = 1)
{
  Char *b = (Char *)alloca ((strlen (s) + 1) * sizeof (Char));
  Char *be = s2w (b, s);
#if defined(_MSC_VER)
  map_backsl_to_sl (b, be - b);
#endif // _MSC_VER
  if (append_slash && be != b && be[-1] != '/')
    *be++ = '/';
  return make_string (b, be - b);
}

static void
init_module_dir ()
{
}

static inline void
init_current_dir ()
{
  char *dir = getenv("PWD");
  char cur_path[PATH_MAX];
  if  (dir) {
    strcpy (cur_path, dir);
  }
  xsymbol_value (Qdefault_dir) = make_path (cur_path);
}

static void
init_windows_dir ()
{
#if defined(_MSC_VER)
#endif // _MSC_VER
}

static int
init_home_dir (const char *home_path)
{
  xsymbol_value (Qhome_dir) = make_path (home_path);
}

static void
init_home_dir ()
{
#if defined(__GNUG__)
  char path[PATH_MAX];
  char *dir = getenv("HOME");
  if (dir)
    {
      strcpy (path, dir);
      if (init_home_dir(path))
	return;
    }
#endif ///__GNUG__
  xsymbol_value (Qhome_dir) = xsymbol_value (Qmodule_dir);
}

static void
init_load_path ()
{
  lisp l = make_string ("lisp");
  xsymbol_value (Vload_path) = Qnil;
  if (Fequalp (xsymbol_value (Qmodule_dir),
	       xsymbol_value (Qdefault_dir)) == Qnil)
    xsymbol_value (Vload_path) =
      xcons (Fmerge_pathnames (l, xsymbol_value (Qdefault_dir)),
	     xsymbol_value (Vload_path));
}

static void
init_env_symbols ()
{
  xsymbol_value (Vfeatures) = xcons (Kxyzzy, xcons (Kieee_floating_point, Qnil));
  init_module_dir ();
  init_current_dir ();
  init_home_dir ();
  init_load_path ();
  init_windows_dir ();
} 

static void
init_math_symbols ()
{
#define CP(T, F) (xsymbol_value (T) = xsymbol_value (F))
}

static void
init_symbol_value_once ()
{
  xsymbol_value (Qt) = Qt;

  xsymbol_value (Vprint_readably) = Qnil;
  xsymbol_value (Vprint_escape) = Qt;
  xsymbol_value (Vprint_pretty) = Qt;
  xsymbol_value (Vprint_base) = make_fixnum (10);
  xsymbol_value (Vprint_radix) = Qnil;
  xsymbol_value (Vprint_circle) = Qnil;
  xsymbol_value (Vprint_length) = Qnil;
  xsymbol_value (Vprint_level) = Qnil;

  xsymbol_value (Vload_verbose) = Qt;
  xsymbol_value (Vload_print) = Qnil;
  xsymbol_value (Vload_pathname) = Qnil;

  xsymbol_value (Vrandom_state) = Fmake_random_state (Qt);
  xsymbol_value (Vdefault_random_state) = xsymbol_value (Vrandom_state);

  xsymbol_value (Qcall_arguments_limit) = make_fixnum (MAX_VECTOR_LENGTH);
  xsymbol_value (Qlambda_parameters_limit) = make_fixnum (MAX_VECTOR_LENGTH);
  xsymbol_value (Qmultiple_values_limit) = make_fixnum (MULTIPLE_VALUES_LIMIT);

  init_math_symbols ();
  init_readtable ();

  xsymbol_value (Qchar_code_limit) = make_fixnum (CHAR_LIMIT);

  xsymbol_value (Qarray_rank_limit) = make_fixnum (ARRAY_RANK_LIMIT);
  xsymbol_value (Qarray_dimension_limit) = make_fixnum (MAX_VECTOR_LENGTH);
  xsymbol_value (Qarray_total_size_limit) = make_fixnum (MAX_VECTOR_LENGTH);

  xsymbol_value (Qinternal_time_units_per_second) = make_fixnum (1000);

  xsymbol_value (Vcreate_buffer_hook) = Qnil;
  xsymbol_value (Vdefault_fileio_encoding) = xsymbol_value (Qencoding_sjis);
  xsymbol_value (Vexpected_fileio_encoding) = xsymbol_value (Qencoding_auto);
  xsymbol_value (Vdefault_eol_code) = make_fixnum (eol_crlf);
  xsymbol_value (Vexpected_eol_code) = make_fixnum (eol_guess);

  xsymbol_value (Qor_string_integer) =
    xcons (Qor, xcons (Qstring, xcons (Qinteger, Qnil)));
  xsymbol_value (Qor_symbol_integer) =
    xcons (Qor, xcons (Qsymbol, xcons (Qinteger, Qnil)));
  xsymbol_value (Qor_string_character) =
    xcons (Qor, xcons (Qstring, xcons (Qcharacter, Qnil)));
  xsymbol_value (Qor_integer_marker) =
    xcons (Qor, xcons (Qinteger, xcons (Qmarker, Qnil)));
  xsymbol_value (Qor_character_cons) =
    xcons (Qor, xcons (Qcharacter, xcons (Qcons, Qnil)));
  xsymbol_value (Qor_symbol_string) =
    xcons (Qor, xcons (Qsymbol, xcons (Qstring, Qnil)));
  xsymbol_value (Qor_string_stream) =
    xcons (Qor, xcons (Qstring, xcons (Qstream, Qnil)));

  xsymbol_value (Vread_default_float_format) = Qsingle_float;

  xsymbol_value (Vscroll_bar_step) = make_fixnum (2);

  xsymbol_value (Vglobal_keymap) = Fmake_keymap ();
  xsymbol_value (Vselection_keymap) = Qnil;
  xsymbol_value (Vkept_undo_information) = make_fixnum (1000);
  xsymbol_value (Vbuffer_read_only) = Qnil;
  xsymbol_value (Venable_meta_key) = Qt;
  xsymbol_value (Vlast_command_char) = Qnil;
  xsymbol_value (Vneed_not_save) = Qnil;
  xsymbol_value (Vauto_save) = Qt;
  xsymbol_value (Vbeep_on_error) = Qt;
  xsymbol_value (Vbeep_on_warn) = Qt;
  xsymbol_value (Vbeep_on_never) = Qnil;
  xsymbol_value (Vprefix_value) = Qnil;
  xsymbol_value (Vprefix_args) = Qnil;
  xsymbol_value (Vnext_prefix_value) = Qnil;
  xsymbol_value (Vnext_prefix_args) = Qnil;
  xsymbol_value (Vdefault_syntax_table) = Fmake_syntax_table ();
  xsymbol_value (Vauto_fill) = Qnil;
  xsymbol_value (Vthis_command) = Qnil;
  xsymbol_value (Vlast_command) = Qnil;

  ///  xsymbol_value (Qsoftware_type) = make_string (ProgramName);
  /// xsymbol_value (Qsoftware_version) = make_string (VersionString);
  ///  xsymbol_value (Qsoftware_version_display_string) =
  ///    make_string (DisplayVersionString);

  xsymbol_value (Qtemporary_string) = make_string_simple ("", 0);

  xsymbol_value (Vversion_control) = Qt;
  xsymbol_value (Vkept_old_versions) = make_fixnum (2);
  xsymbol_value (Vkept_new_versions) = make_fixnum (2);
  xsymbol_value (Vmake_backup_files) = Qt;
  xsymbol_value (Vmake_backup_file_always) = Qnil;
  xsymbol_value (Vpack_backup_file_name) = Qt;
  xsymbol_value (Vauto_save_interval) = make_fixnum (256);
  xsymbol_value (Vauto_save_interval_timer) = make_fixnum (30);
  xsymbol_value (Vbackup_by_copying) = Qnil;

  xsymbol_value (Vinverse_mode_line) = Qt;
  xsymbol_value (Vbuffer_list_sort_ignore_case) = Qt;
  //xsymbol_value (Veat_mouse_activate) = Qt;
  xsymbol_value (Vindent_tabs_mode) = Qt;

  xsymbol_value (Slock_file) = Qnil;
  xsymbol_value (Vexclusive_lock_file) = Qnil;

  xsymbol_value (Vcursor_shape) = Karrow;
  xsymbol_value (Vhide_restricted_region) = Qnil;

  xsymbol_value (Vfiler_last_command_char) = Qnil;
  xsymbol_value (Vfiler_dual_window) = Qnil;
  xsymbol_value (Vfiler_left_window_p) = Qt;
  xsymbol_value (Vfiler_secondary_directory) = Qnil;
  xsymbol_value (Vfiler_click_toggle_marks_always) = Qt;

  xsymbol_value (Vdll_module_list) = Qnil;

  ///  xsymbol_value (Vfunction_bar_labels) =
    ///    make_vector (MAX_FUNCTION_BAR_LABEL, Qnil);

  xsymbol_value (Vkeyword_hash_table) = Qnil;
  xsymbol_value (Vhighlight_keyword) = Qt;
  xsymbol_value (Vhtml_highlight_mode) = Qnil;

  xsymbol_value (Vblink_caret) = Qt;

  xsymbol_value (Vparentheses_hash_table) = Qnil;

  xsymbol_value (Vdefault_kinsoku_bol_chars) = Qnil;
  xsymbol_value (Vdefault_kinsoku_eol_chars) = Qnil;

  xsymbol_value (Vdde_timeout) = make_fixnum (30000);
  xsymbol_value (Vbrackets_is_wildcard_character) = Qt;

  init_char_encoding ();

  xsymbol_value (Vbypass_evalhook) = Qnil;
  xsymbol_value (Vbypass_applyhook) = Qnil;

  xsymbol_value (Vtitle_bar_format) = Qnil;
  xsymbol_value (Vstatus_bar_format) = Qnil;
  xsymbol_value (Vlast_status_bar_format) = Qnil;
  xsymbol_value (Vscroll_margin) = make_fixnum (0);
  xsymbol_value (Vjump_scroll_threshold) = make_fixnum (3);
  xsymbol_value (Vauto_update_per_device_directory) = Qt;
  xsymbol_value (Vmodal_filer_save_position) = Qt;
  xsymbol_value (Vmodal_filer_save_size) = Qt;
  xsymbol_value (Vfiler_echo_filename) = Qt;
  xsymbol_value (Vfiler_eat_esc) = Qt;
  xsymbol_value (Vsupport_mouse_wheel) = Qt;
  xsymbol_value (Vminibuffer_save_ime_status) = Qt;
  xsymbol_value (Vuse_shell_execute_ex) = Qt;
  xsymbol_value (Vshell_execute_disregards_shift_key) = Qt;
  xsymbol_value (Vregexp_keyword_list) = Qnil;
  xsymbol_value (Vunicode_to_half_width) = Qt;
  xsymbol_value (Vcolor_page_enable_dir_p) = Qnil;
  xsymbol_value (Vcolor_page_enable_subdir_p) = Qnil;
}

static void
init_symbol_value ()
{
  xsymbol_value (Vquit_flag) = Qnil;
  xsymbol_value (Vinhibit_quit) = Qnil;
  xsymbol_value (Voverwrite_mode) = Qnil;
  xsymbol_value (Vprocess_list) = Qnil;
  xsymbol_value (Vminibuffer_message) = Qnil;
  xsymbol_value (Vsi_find_motion) = Qt;
  xsymbol_value (Vdefault_menu) = Qnil;
  xsymbol_value (Vlast_active_menu) = Qnil;

  xsymbol_value (Vreader_in_backquote) = Qnil;
  xsymbol_value (Vreader_preserve_white) = Qnil;
  xsymbol_value (Vread_suppress) = Qnil;
  xsymbol_value (Vreader_label_alist) = Qnil;

  xsymbol_value (Vclipboard_newer_than_kill_ring_p) = Qnil;
  xsymbol_value (Vkill_ring_newer_than_clipboard_p) = Qnil;

  xsymbol_value (Vkbd_encoding) = xsymbol_value (Qencoding_sjis);
  xsymbol_value (Vsi_accept_kill_xyzzy) = Qt;
  xsymbol_value (Vlast_match_string) = Qnil;
}

static int init_lisp_object (void)
{
  init_syms ();
  init_symbol_value_once ();
  init_condition ();
  init_symbol_value ();
  init_syntax_spec ();
  init_env_symbols ();
  create_std_streams ();
}

static int
init_editor_objects ()
{
  try
    {
      Window::create_default_windows ();
      create_default_buffers ();
    }
  catch (nonlocal_jump &)
    {
      app.active_frame.selected = 0;
      return 0;
    }
  return 1;
}

lisp
Fsi_startup ()
{
  return Fsi_load_library (make_string ("startup"), Qnil);
}

void main1 (void)
{
    init_lisp_object();
    init_editor_objects ();
    
    int init_ok = 0;
    try
    {
        Ffuncall (Ssi_startup, Qnil);
        init_ok = 1;
    }
    catch (nonlocal_jump &)
    {
      assert (1);
      print_condition (nonlocal_jump::data ());
    }
}
