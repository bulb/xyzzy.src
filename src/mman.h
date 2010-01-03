#ifndef _MMAN_H_
# define _MMAN_H_

class mapf
{
#if defined(_MSC_VER)
  HANDLE mf_hfile;
  HANDLE mf_hmap;
#else  // __GNUG__
  ///@todo
#endif // __GNUG__
  DWORD mf_size;
  void *mf_base;

  void init ()
    {
#if defined(_MSC_VER)
      mf_hfile = INVALID_HANDLE_VALUE;
      mf_hmap = 0;
#else  // __GNUG__
  ///@todo
#endif // __GNUG__
      mf_base = 0;
    }

public:
  mapf () {init ();}
  ~mapf () {close ();}
  void close ();
#if defined(_MSC_VER)
  int open (const char *, int = FILE_FLAG_SEQUENTIAL_SCAN, int = 0);
#else  // __GNUG__
  int open (const char *, int, int = 0); ///@todo
#endif // __GNUG__
  const void *base () const {return mf_base;}
  DWORD size () const {return mf_size;}
#if defined(_MSC_VER)
  operator HANDLE () const {return mf_hfile;}
#else  // __GNUG__
  ///@todo
#endif // __GNUG__
};

#endif
