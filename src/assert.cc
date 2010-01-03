#if defined(_MSC_VER)
# include <windows.h>
#endif // _MSC_VER
#include <stdlib.h>
#include "cdecl.h"

#ifdef DEBUG

#if defined(_MSC_VER)
static LONG double_fault = -1;
#else  // __GNUG__
static long double_fault = -1;
#endif // __GNUG__

int
assert_failed (const char *file, int line)
{
#if defined(_MSC_VER)
  char msg[MAX_PATH * 2];

  wsprintf (msg, "Assertion failed: %s: %d\n", file, line);
  OutputDebugString (msg);

  if (InterlockedIncrement (&double_fault) > 0)
    InterlockedDecrement (&double_fault);
  else
    {
      HWND hwnd = GetActiveWindow ();
      if (hwnd)
        hwnd = GetLastActivePopup (hwnd);

      int r = MessageBox (hwnd, msg, 0,
                          (MB_TASKMODAL | MB_ICONHAND
                           | MB_ABORTRETRYIGNORE | MB_SETFOREGROUND));

      InterlockedDecrement (&double_fault);

      if (r == IDIGNORE)
        return 0;

      if (r == IDABORT)
        abort ();
    }
  DebugBreak ();
#else // !_MSC_VER

#endif // !_MSC_VER
  return 0;
}

#endif
