#include "ed.h"

#define SXHASH_DEPTH 3

lhash_table *
make_hash_table ()
{
  lhash_table *p = ldata <lhash_table, Thash_table>::lalloc ();
  p->test = 0;
  p->size = 0;
  p->used = 0;
  p->count = 0;
  p->entry = 0;
  return p;
}

static inline u_int
sxhashval (lisp object)
{
#if defined(_MSC_VER)
  return u_int (object) >> 3;
#else // __GNUG__
  return reinterpret_cast<u_int&>(object) >> 3;
#endif // __GNUG__
}

/*GENERIC_FUNCTION:IMMEDIATE*/
static u_int
sxhash_imm (lisp object, hash_test_proc test)
{
  if (short_int_p (object))
    return xshort_int_value (object);
  if (charp (object))
#if defined(_MSC_VER)
    return (test == Fequalp
            ? char_upcase (xchar_code (object))
            : xchar_code (object));
#else // __GNUG__
    return (test == hash_test_proc(Fequalp)
            ? char_upcase (xchar_code (object))
            : xchar_code (object));
#endif // __GNUG__
  return sxhashval (object);
}

/*GENERIC_FUNCTION:NUMBER*/
static u_int
sxhash_eql (lisp object)
{
  if (immediatep (object))
#if defined(_MSC_VER)
    return sxhash_imm (object, Feql);
#else // __GNUG__
    return sxhash_imm (object, hash_test_proc(Feql));
#endif // __GNUG__

  switch (object_typeof (object))
    {
    case Tlong_int:
      return xlong_int_value (object);

    case Tbignum:
      {
        bignum_rep *r = xbignum_rep (object);
        u_short *p = r->br_data;
        u_int h = 0;
        for (int i = min (int (r->br_len), 5); i > 0; i--)
          h += *p++;
        return h;
      }

    case Tfraction:
      return (sxhash_eql (xfract_num (object))
              + sxhash_eql (xfract_den (object)));

    case Tsingle_float:
      return *(u_int *)&xsingle_float_value (object);

    case Tdouble_float:
      return (((u_int *)&xdouble_float_value (object)) [0]
              + ((u_int *)&xdouble_float_value (object)) [1]);

    case Tcomplex:
      return (sxhash_eql (xcomplex_real (object))
              + sxhash_eql (xcomplex_imag (object)));

    default:
      return sxhashval (object);
    }
}

/*GENERIC_FUNCTION*/
static u_int
sxhash_equal (lisp object, int depth)
{
  u_int hashval = 0;
  while (depth++ < SXHASH_DEPTH)
    {
      if (immediatep (object))
#if defined(_MSC_VER)
        return hashval + sxhash_imm (object, Fequal);
#else // __GNUG__
        return hashval + sxhash_imm (object, hash_test_proc(Fequal));
#endif // __GNUG__
      switch (object_typeof (object))
        {
        case Tcons:
          hashval += sxhash_equal (xcar (object), depth);
          object = xcdr (object);
          break;

        case Tsimple_string:
        case Tcomplex_string:
          return hashval + hashpjw (object);

        default:
          return hashval + sxhash_eql (object);
        }
    }
  return hashval;
}

/*GENERIC_FUNCTION*/
static u_int
sxhash_equalp (lisp object, int depth)
{
  u_int hashval = 0;
  while (depth++ < SXHASH_DEPTH)
    {
      if (immediatep (object))
#if defined(_MSC_VER)
        return hashval + sxhash_imm (object, Fequalp);
#else // __GNUG__
        return hashval + sxhash_imm (object, hash_test_proc(Fequalp));
#endif // __GNUG__
      switch (object_typeof (object))
        {
        case Tcons:
          hashval += sxhash_equalp (xcar (object), depth);
          object = xcdr (object);
          break;

        case Tsimple_string:
        case Tcomplex_string:
          return hashval + ihashpjw (object);

        case Tsimple_vector:
        case Tcomplex_vector:
          {
            const lisp *p = xvector_contents (object);
            const lisp *pe = p + xvector_length (object);
            for (; p < pe; p++)
              if (!charp (*p))
                {
                  p = xvector_contents (object);
                  for (int l = min (xvector_length (object), 8); l > 0; l--, p++)
                    hashval += sxhash_equalp (*p, depth);
                  return hashval;
                }
            return hashval + ihashpjw (xvector_contents (object), pe);
          }

        case Tstring_array:
          {
            const Char *p = xstring_array_contents (object);
            for (int l = min (xarray_total_size (object), 8); l > 0; l--, p++)
              hashval += sxhash_equalp (make_char (*p), depth);
            return hashval;
          }

        case Tarray:
          {
            const lisp *p = xgeneral_array_contents (object);
            for (int l = min (xarray_total_size (object), 8); l > 0; l--, p++)
              hashval += sxhash_equalp (*p, depth);
            return hashval;
          }

        case Thash_table:
          {
            const hash_entry *p = xhash_table_entry (object);
            for (int l = min (xhash_table_size (object), 8); l > 0; l--, p++)
              {
                hashval += sxhash_equalp (p->key, depth);
                hashval += sxhash_equalp (p->value, depth);
              }
            return hashval;
          }

        case Tstruct_data:
          {
            hashval += sxhash_equalp (xstrdata_def (object), depth);
            for (int i = min (xstrdata_nslots (object) - 1, 4); i >= 0; i--)
              hashval += sxhash_equalp (xstrdata_data (object) [i], depth);
            return hashval;
          }

        default:
          return hashval + sxhash_eql (object);
        }
    }
  return hashval;
}

static u_int
sxhash (lisp object, hash_test_proc test)
{
#if defined(_MSC_VER)
  if (test == Feq)
    return u_int (object) >> 3;
  if (test == Feql)
    return sxhash_eql (object);
  if (test == Fequal)
    return sxhash_equal (object, 0);
  return sxhash_equalp (object, 0);
#else // __GNUG__
  if (test == hash_test_proc(Feq))
    return reinterpret_cast<u_int&>(object) >> 3;
  if (test == hash_test_proc(Feql))
    return sxhash_eql (object);
  if (test == hash_test_proc(Fequal))
    return sxhash_equal (object, 0);
  return sxhash_equalp (object, 0);
#endif // __GNUG__

}

static inline void
clear_hash_entry (hash_entry *entry, int size)
{
  for (int i = 0; i < size; i++, entry++)
    {
      entry->key = Qunbound;
      entry->value = Qnil;
    }
}

static inline hash_entry *
alloc_hash_entry (int size)
{
  hash_entry *p = (hash_entry *)xmalloc (sizeof (hash_entry) * size);
  clear_hash_entry (p, size);
  return p;
}

static inline void
clear_hash_table (lisp hash_table)
{
  assert (hash_table_p (hash_table));
  clear_hash_entry (xhash_table_entry (hash_table), xhash_table_size (hash_table));
  xhash_table_used (hash_table) = 0;
  xhash_table_count (hash_table) = 0;
}

static int
hash_table_good_size (int size)
{
  static int prime[] =
    {
      17, 47, 101, 149, 199, 307, 401, 499, 599, 701, 797, 907, 997,
      1103, 1499, 1999, 2999, 4001, 4999, 6007, 7001, 8009, 8999,
      10007, 19997, 29989, 39989, 49999, 59999, 70001, 79999, 90001, 99991,
    };
  for (int i = 0; i < numberof (prime); i++)
    if (size < prime[i])
      return prime[i];
  if (!(size & 1))
    size++;
  while (!(size % 3) && !(size % 5) && !(size % 7))
    size += 2;
  return size;
}

lhash_table *
make_hash_table (hash_test_proc test, int size, int rehash_size)
{
  lhash_table *ht = make_hash_table ();
  size = hash_table_good_size (size);
  ht->entry = alloc_hash_entry (size);
  ht->size = size;
  ht->used = 0;
  ht->count = 0;
  ht->rehash_size = rehash_size;
  ht->test = test;
  return ht;
}

lisp
Fmake_hash_table (lisp keys)
{
  hash_test_proc test = 0;
  lisp ltest = find_keyword (Ktest, keys);
  if (ltest == Qnil)
#if defined(_MSC_VER)
    test = Feql;
#else // __GNUG__
    test = hash_test_proc(Feql);
#endif // __GNUG__
  else
    {
      if (symbolp (ltest))
        ltest = xsymbol_function (ltest);
      if (functionp (ltest))
        {
          hash_test_proc proc = hash_test_proc (xfunction_fn (ltest));
#if defined(_MSC_VER)
          if (proc == Feq || proc == Feql || proc == Fequal || proc == Fequalp)
            test = proc;
#else // __GNUG__
          if (proc == hash_test_proc(Feq) || proc == hash_test_proc(Feql)
              || proc == hash_test_proc(Fequal) || proc == hash_test_proc(Fequalp))
            test = proc;
#endif // __GNUG__
        }
      if (!test)
        FEprogram_error (Einvalid_argument, xcons (Ktest, xcons (ltest, Qnil)));
    }
  int size = find_keyword_int (Ksize, keys);
  int rehash_size = find_keyword_int (Krehash_size, keys, 1);
  return make_hash_table (test, size, rehash_size);
}

lisp
Fhash_table_p (lisp object)
{
  return boole (hash_table_p (object));
}

static inline u_int
hashinc (u_int hashval, u_int size)
{
  u_int d = hashval / size % size;
  return d ? d : 1;
}

static hash_entry *
find_hash_entry (lisp key, lisp hash_table)
{
  check_hash_table (hash_table);

  hash_entry *entry = xhash_table_entry (hash_table);
  u_int size = xhash_table_size (hash_table);
  assert (size);
  assert (size & 1);
  hash_test_proc test = xhash_table_test_fn (hash_table);
  u_int hashval = sxhash (key, test);
  u_int h = hashval % size;
  for (u_int i = 0; i <= size; i++)
    {
      if (entry[h].key == Qunbound)
        break;
      if (entry[h].key != Qdeleted && test (entry[h].key, key) != Qnil)
        return &entry[h];
      if (!i)
        h = (h + hashinc (hashval, size)) % size;
      else
        h = (h + 2) % size;
    }
  return 0;
}

lChar
Char_hash (Char key, lisp hash_table)
{
  hash_entry *entry = xhash_table_entry (hash_table);
  u_int size = xhash_table_size (hash_table);
  u_int hashval = key;
  u_int h = hashval % size;
  for (u_int i = 0; i <= size; i++)
    {
      if (entry[h].key == Qunbound)
        break;
      if (charp (entry[h].key)
          && xchar_code (entry[h].key) == key)
        {
          if (charp (entry[h].value))
            return xchar_code (entry[h].value);
          break;
        }
      if (!i)
        h = (h + hashinc (hashval, size)) % size;
      else
        h = (h + 2) % size;
    }
  return lChar_EOF;
}

lisp
Fgethash (lisp key, lisp hash_table, lisp defalt)
{
  hash_entry *entry = find_hash_entry (key, hash_table);
  multiple_value::count () = 2;
  if (entry)
    {
      multiple_value::value (1) = Qt;
      return entry->value;
    }
  multiple_value::value (1) = Qnil;
  return defalt ? defalt : Qnil;
}

lisp
Fremhash (lisp key, lisp hash_table)
{
  hash_entry *entry = find_hash_entry (key, hash_table);
  if (!entry)
    return Qnil;
  entry->key = Qdeleted;
  entry->value = Qnil;
  xhash_table_count (hash_table)--;
  return Qt;
}

static int
add_hash_entry (lisp key, lisp value, lhash_table *table)
{
  u_int size = table->size;
  assert (size);
  assert (size & 1);
  hash_entry *entry = table->entry;
  hash_test_proc test = table->test;
  u_int hashval = sxhash (key, test);
  u_int h = hashval % size;
  int pos = -1;
  for (u_int i = 0; i <= size; i++)
    {
      if (entry[h].key == Qunbound)
        {
          if (pos == -1)
            pos = h;
          entry[pos].key = key;
          entry[pos].value = value;
          table->used++;
          table->count++;
          return 1;
        }
      if (entry[h].key == Qdeleted)
        {
          if (pos == -1)
            pos = h;
        }
      else if (test (entry[h].key, key) != Qnil)
        {
          entry[h].value = value;
          return 1;
        }
      if (!i)
        h = (h + hashinc (hashval, size)) % size;
      else
        h = (h + 2) % size;
    }
  return 0;
}

void
hash_table_rehash (lisp hash_table, int inc)
{
  assert (hash_table_p (hash_table));
  int old_size = xhash_table_size (hash_table);
  hash_entry *old_entry = xhash_table_entry (hash_table);

  lhash_table *new_hash_table = make_hash_table (xhash_table_test_fn (hash_table),
                                                 old_size + inc,
                                                 xhash_table_rehash_size (hash_table));
  int new_size = xhash_table_size (new_hash_table);
  hash_entry *new_entry = xhash_table_entry (new_hash_table);

  for (int i = 0; i < old_size; i++, old_entry++)
    if (old_entry->key != Qunbound && old_entry->key != Qdeleted)
      add_hash_entry (old_entry->key, old_entry->value,
                      new_hash_table);

  assert (xhash_table_used (new_hash_table) == xhash_table_used (hash_table));

  xfree (xhash_table_entry (hash_table));
  xhash_table_entry (hash_table) = new_entry;
  xhash_table_size (hash_table) = new_size;

  xhash_table_entry (new_hash_table) = 0;
}

lisp
Fsi_puthash (lisp key, lisp hash_table, lisp value)
{
  check_hash_table (hash_table);
  if (xhash_table_used (hash_table) > xhash_table_size (hash_table) * 8 / 10)
    {
      int inc = xhash_table_rehash_size (hash_table);
      if (inc < xhash_table_size (hash_table) / 2)
        inc = xhash_table_size (hash_table) / 2;
      hash_table_rehash (hash_table, inc);
    }
  add_hash_entry (key, value, (lhash_table *)hash_table);
  return value;
}

lisp
Fclrhash (lisp hash_table)
{
  check_hash_table (hash_table);
  clear_hash_table (hash_table);
  return hash_table;
}

lisp
Fhash_table_count (lisp hash_table)
{
  check_hash_table (hash_table);
  return make_fixnum (xhash_table_count (hash_table));
}

lisp
Fhash_table_rehash_size (lisp hash_table)
{
  check_hash_table (hash_table);
  return make_fixnum (xhash_table_rehash_size (hash_table));
}

lisp
Fhash_table_size (lisp hash_table)
{
  check_hash_table (hash_table);
  return make_fixnum (xhash_table_size (hash_table));
}

lisp
Fhash_table_test (lisp hash_table)
{
  check_hash_table (hash_table);
  hash_test_proc test = xhash_table_test_fn (hash_table);
#if defined(_MSC_VER)
  if (test == Feq)
    return Seq;
  if (test == Feql)
    return Seql;
  if (test == Fequal)
    return Sequal;
#else  // __GNUG__
  if (test == hash_test_proc(Feq))
    return Seq;
  if (test == hash_test_proc(Feql))
    return Seql;
  if (test == hash_test_proc(Fequal))
    return Sequal;
#endif // __GNUG__
  return Sequalp;
}

lisp
Fsxhash (lisp object)
{
  return make_fixnum (sxhash (object, 0));
}

int
equalp (lhash_table *x, lhash_table *y)
{
  assert (hash_table_p (x));
  assert (hash_table_p (y));

  if (xhash_table_test_fn (x) != xhash_table_test_fn (y))
    return 0;

  if (xhash_table_count (x) != xhash_table_count (y))
    return 0;

  int size = xhash_table_size (x);
  hash_entry *entry = xhash_table_entry (x);
  int i;
  for (i = 0; i < size; i++, entry++)
    if (entry->key != Qunbound && entry->key != Qdeleted)
      if (!find_hash_entry (entry->key, y))
        return 0;

  entry = xhash_table_entry (x);
  for (i = 0; i < size; i++, entry++)
    if (entry->key != Qunbound && entry->key != Qdeleted)
      {
        lisp t = Fgethash (entry->key, y, Qnil);
        if (Fequalp (entry->value, t) == Qnil)
          return 0;
      }

  return 1;
}

lisp
Fsi_enum_hash_table (lisp hash_table, lisp index)
{
  check_hash_table (hash_table);
  int i = fixnum_value (index);
  if (i < 0)
    FErange_error (index);
  int size = xhash_table_size (hash_table);
  hash_entry *entry = xhash_table_entry (hash_table) + i;
  for (; i < size; i++, entry++)
    if (entry->key != Qunbound && entry->key != Qdeleted)
      {
        multiple_value::count () = 3;
        multiple_value::value (1) = entry->key;
        multiple_value::value (2) = entry->value;
        return make_fixnum (i + 1);
      }
  return Qnil;
}

lisp
Fgethash_region (lisp from, lisp to, lisp hash_table, lisp defalt)
{
  Buffer *bp = selected_buffer ();
  point_t p1 = bp->coerce_to_restricted_point (from);
  point_t p2 = bp->coerce_to_restricted_point (to);
  if (p1 > p2)
    swap (p1, p2);
  int l = p2 - p1;
  Char *b = (Char *)alloca (sizeof *b * l);
  bp->substring (p1, l, b);
  temporary_string t (b, l);
  return Fgethash (t.string (), hash_table, defalt);
}
