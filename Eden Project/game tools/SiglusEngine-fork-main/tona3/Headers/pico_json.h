/* Copyright 2009 Cybozu Labs, Inc.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY CYBOZU LABS, INC. ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL CYBOZU LABS, INC. OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * The views and conclusions contained in the software and documentation are
 * those of the authors and should not be interpreted as representing official
 * policies, either expressed or implied, of Cybozu Labs, Inc.
 *
 */
#ifndef pico_json_h
#define pico_json_h

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <vector>

#ifdef _MSC_VER
    #define SNPRINTF _snprintf_s
    #pragma warning(push)
    #pragma warning(disable : 4244) // conversion from int to char
#else
    #define SNPRINTF snprintf
#endif

typedef		std::string				PICO_JSON_STR;
//typedef		TSTR				PICO_JSON_STR;

namespace pico_json {
  
  enum {
    null_type,
    boolean_type,
	intnum_type,					// *** int ***
    number_type,
    string_type,
    array_type,
    object_type
  };
  
  struct null {};
  
  class value {
  public:
    typedef std::vector<value> array;
    typedef std::map<PICO_JSON_STR, value> object;
  protected:
    int type_;
    union {
      bool boolean_;
      int intnum_;					// *** int ***
      double number_;
      PICO_JSON_STR* string_;
      array* array_;
      object* object_;
    };
  public:
    value();
    value(int type, bool);
    explicit value(bool b);
	explicit value(int n);			// *** int ***
    explicit value(double n);
    explicit value(const PICO_JSON_STR& s);
    explicit value(const array& a);
    explicit value(const object& o);
    ~value();
    value(const value& x);
    value& operator=(const value& x);
    template <typename T> bool is() const;
    template <typename T> const T& get() const;
    template <typename T> T& get();
    operator bool() const;
    const value& get(size_t idx) const;
    const value& get(const PICO_JSON_STR& key) const;
    bool check(const PICO_JSON_STR& key) const;
    PICO_JSON_STR to_str() const;
    template <typename Iter> void serialize(Iter os) const;
    PICO_JSON_STR serialize() const;
  };
  
  typedef value::array array;
  typedef value::object object;
  
  inline value::value() : type_(null_type) {}
  
  inline value::value(int type, bool) : type_(type) {
    switch (type) {
#define INIT(p, v) case p##type: p = v; break
      INIT(boolean_, false);
      INIT(intnum_, 0);										// *** int ***
      INIT(number_, 0.0);
      INIT(string_, new PICO_JSON_STR());
      INIT(array_, new array());
      INIT(object_, new object());
#undef INIT
    default: break;
    }
  }
  
  inline value::value(bool b) : type_(boolean_type) {
    boolean_ = b;
  }

  inline value::value(int n) : type_(intnum_type) {			// *** int ***
    intnum_ = n;											// *** int ***
  }															// *** int ***
	
  inline value::value(double n) : type_(number_type) {
    number_ = n;
  }
  
  inline value::value(const PICO_JSON_STR& s) : type_(string_type) {
    string_ = new PICO_JSON_STR(s);
  }
  
  inline value::value(const array& a) : type_(array_type) {
    array_ = new array(a);
  }
  
  inline value::value(const object& o) : type_(object_type) {
    object_ = new object(o);
  }
  
  inline value::~value() {
    switch (type_) {
#define DEINIT(p) case p##type: delete p; break
      DEINIT(string_);
      DEINIT(array_);
      DEINIT(object_);
#undef DEINIT
    default: break;
    }
  }
  
  inline value::value(const value& x) : type_(x.type_) {
    switch (type_) {
#define INIT(p, v) case p##type: p = v; break
      INIT(boolean_, x.boolean_);
      INIT(intnum_, x.intnum_);								// *** int ***
      INIT(number_, x.number_);
      INIT(string_, new PICO_JSON_STR(*x.string_));
      INIT(array_, new array(*x.array_));
      INIT(object_, new object(*x.object_));
#undef INIT
    default: break;
    }
  }
  
  inline value& value::operator=(const value& x) {
    if (this != &x) {
      this->~value();
      new (this) value(x);
    }
    return *this;
  }
  
#define IS(ctype, jtype)			     \
  template <> inline bool value::is<ctype>() const { \
    return type_ == jtype##_type;		     \
  }
  IS(null, null)
  IS(bool, boolean)
  IS(int, intnum)				// *** int *** original= IS(int, number)
  IS(double, number)
  IS(PICO_JSON_STR, string)
  IS(array, array)
  IS(object, object)
#undef IS
  
#define GET(ctype, var)					      \
  template <> inline const ctype& value::get<ctype>() const { \
    return var;						      \
  }							      \
  template <> inline ctype& value::get<ctype>() {	      \
    return var;						      \
  }
  GET(bool, boolean_)
  GET(int, intnum_)									// *** int ***
  GET(double, number_)
  GET(PICO_JSON_STR, *string_)
  GET(array, *array_)
  GET(object, *object_)
#undef GET
  
  inline value::operator bool() const {
    switch (type_) {
    case null_type:
      return false;
    case boolean_type:
      return boolean_;
    case intnum_type:								// *** int ***
	  return intnum_ != 0;							// *** int ***
    case number_type:
      return number_ != 0;
    case string_type:
      return ! string_->empty();
    default:
      return true;
    }
  }
  
  inline const value& value::get(size_t idx) const {
    static value s_null;
    assert(is<array>());
    return idx < array_->size() ? (*array_)[idx] : s_null;
  }

  inline const value& value::get(const PICO_JSON_STR& key) const {
    static value s_null;
    assert(is<object>());
    object::const_iterator i = object_->find(key);
    return i != object_->end() ? i->second : s_null;
  }

  inline bool value::check(const PICO_JSON_STR& key) const {
    assert(is<object>());
    object::const_iterator i = object_->find(key);
    return i != object_->end() ? true : false;
  }
	
  inline PICO_JSON_STR value::to_str() const {
    switch (type_) {
    case null_type:      return "null";
    case boolean_type:   return boolean_ ? "true" : "false";
    case intnum_type:    {							// *** int ***
	  char buf[256];								// *** int ***
      SNPRINTF(buf, sizeof(buf), "%d", intnum_);	// *** int ***
      return buf;									// *** int ***
    }												// *** int ***
    case number_type:    {
      char buf[256];
      SNPRINTF(buf, sizeof(buf), "%f", number_);
      return buf;
    }
    case string_type:    return *string_;
    case array_type:     return "array";
    case object_type:    return "object";
    default:             assert(0);
#ifdef _MSC_VER
      __assume(0);
#endif
    }
  }
  
  template <typename Iter> void copy(const PICO_JSON_STR& s, Iter oi) {
    std::copy(s.begin(), s.end(), oi);
  }
  
  template <typename Iter> void serialize_str(const PICO_JSON_STR& s, Iter oi) {
    *oi++ = '"';
    for (PICO_JSON_STR::const_iterator i = s.begin(); i != s.end(); ++i) {
      switch (*i) {
#define MAP(val, sym) case val: copy(sym, oi); break
	MAP('"', "\\\"");
	MAP('\\', "\\\\");
	MAP('/', "\\/");
	MAP('\b', "\\b");
	MAP('\f', "\\f");
	MAP('\n', "\\n");
	MAP('\r', "\\r");
	MAP('\t', "\\t");
#undef MAP
      default:
	if ((unsigned char)*i < 0x20 || *i == 0x7f) {
	  char buf[7];
	  SNPRINTF(buf, sizeof(buf), "\\u%04x", *i & 0xff);
	  copy(buf, buf + 6, oi);
	  } else {
	  *oi++ = *i;
	}
	break;
      }
    }
    *oi++ = '"';
  }
  
  template <typename Iter> void value::serialize(Iter oi) const {
    switch (type_) {
    case string_type:
      serialize_str(*string_, oi);
      break;
    case array_type: {
      *oi++ = '[';
      for (array::const_iterator i = array_->begin(); i != array_->end(); ++i) {
	if (i != array_->begin()) {
	  *oi++ = ',';
	}
	i->serialize(oi);
      }
      *oi++ = ']';
      break;
    }
    case object_type: {
      *oi++ = '{';
      for (object::const_iterator i = object_->begin();
	   i != object_->end();
	   ++i) {
	if (i != object_->begin()) {
	  *oi++ = ',';
	}
	serialize_str(i->first, oi);
	*oi++ = ':';
	i->second.serialize(oi);
      }
      *oi++ = '}';
      break;
    }
    default:
      copy(to_str(), oi);
      break;
    }
  }
  
  inline PICO_JSON_STR value::serialize() const {
    PICO_JSON_STR s;
    serialize(std::back_inserter(s));
    return s;
  }
  
  template <typename Iter> class input {
  protected:
    Iter cur_, end_;
    int last_ch_;
    bool ungot_;
    int line_;
  public:
    input(const Iter& first, const Iter& last) : cur_(first), end_(last), last_ch_(-1), ungot_(false), line_(1) {}
    int getc() {
      if (ungot_) {
	ungot_ = false;
	return last_ch_;
      }
      if (cur_ == end_) {
	last_ch_ = -1;
	return -1;
      }
      if (last_ch_ == '\n') {
	line_++;
      }
      last_ch_ = *cur_++ & 0xff;
      return last_ch_;
    }
    void ungetc() {
      if (last_ch_ != -1) {
	assert(! ungot_);
	ungot_ = true;
      }
    }
    Iter cur() const { return cur_; }
    int line() const { return line_; }
    void skip_ws() {
      while (1) {
	int ch = getc();
	if (! (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r')) {
	  ungetc();
	  break;
	}
      }
    }
    bool expect(int expect) {
      skip_ws();
      if (getc() != expect) {
	ungetc();
	return false;
      }
      return true;
    }
    bool match(const PICO_JSON_STR& pattern) {
      for (PICO_JSON_STR::const_iterator pi(pattern.begin());
	   pi != pattern.end();
	   ++pi) {
	if (getc() != *pi) {
	  ungetc();
	  return false;
	}
      }
      return true;
    }
  };
  
  template<typename Iter> inline int _parse_quadhex(input<Iter> &in) {
    int uni_ch = 0, hex;
    for (int i = 0; i < 4; i++) {
      if ((hex = in.getc()) == -1) {
	return -1;
      }
      if ('0' <= hex && hex <= '9') {
	hex -= '0';
      } else if ('A' <= hex && hex <= 'F') {
	hex -= 'A' - 0xa;
      } else if ('a' <= hex && hex <= 'f') {
	hex -= 'a' - 0xa;
      } else {
	in.ungetc();
	return -1;
      }
      uni_ch = uni_ch * 16 + hex;
    }
    return uni_ch;
  }
  
  template<typename Iter> inline bool _parse_codepoint(PICO_JSON_STR& out, input<Iter>& in) {
    int uni_ch;
    if ((uni_ch = _parse_quadhex(in)) == -1) {
      return false;
    }
    if (0xd800 <= uni_ch && uni_ch <= 0xdfff) {
      if (0xdc00 <= uni_ch) {
	// a second 16-bit of a surrogate pair appeared
	return false;
      }
      // first 16-bit of surrogate pair, get the next one
      if (in.getc() != '\\' || in.getc() != 'u') {
	in.ungetc();
	return false;
      }
      int second = _parse_quadhex(in);
      if (! (0xdc00 <= second && second <= 0xdfff)) {
	return false;
      }
      uni_ch = ((uni_ch - 0xd800) << 10) | ((second - 0xdc00) & 0x3ff);
      uni_ch += 0x10000;
    }
    if (uni_ch < 0x80) {
      out.push_back(uni_ch);
    } else {
      if (uni_ch < 0x800) {
	out.push_back(0xc0 | (uni_ch >> 6));
      } else {
	if (uni_ch < 0x10000) {
	  out.push_back(0xe0 | (uni_ch >> 12));
	} else {
	  out.push_back(0xf0 | (uni_ch >> 18));
	  out.push_back(0x80 | ((uni_ch >> 12) & 0x3f));
	}
	out.push_back(0x80 | ((uni_ch >> 6) & 0x3f));
      }
      out.push_back(0x80 | (uni_ch & 0x3f));
    }
    return true;
  }
  
  template<typename Iter> inline bool _parse_string(value& out, input<Iter>& in) {
    // gcc 4.1 cannot compile if the below two lines are merged into one :-(
    out = value(string_type, false);
    PICO_JSON_STR& s = out.get<PICO_JSON_STR>();
    while (1) {
      int ch = in.getc();
      if (ch < ' ') {
	in.ungetc();
	return false;
      } else if (ch == '"') {
	return true;
      } else if (ch == '\\') {
	if ((ch = in.getc()) == -1) {
	  return false;
	}
	switch (ch) {
#define MAP(sym, val) case sym: s.push_back(val); break
	  MAP('"', '\"');
	  MAP('\\', '\\');
	  MAP('/', '/');
	  MAP('b', '\b');
	  MAP('f', '\f');
	  MAP('n', '\n');
	  MAP('r', '\r');
	  MAP('t', '\t');
#undef MAP
	case 'u':
	  if (! _parse_codepoint(s, in)) {
	    return false;
	  }
	  break;
	default:
	  return false;
	}
      } else {
	s.push_back(ch);
      }
    }
    return false;
  }
  
  template <typename Iter> inline bool _parse_array(value& out, input<Iter>& in) {
    out = value(array_type, false);
    array& a = out.get<array>();
    if (in.expect(']')) {
      return true;
    }
    do {
      a.push_back(value());
      if (! _parse(a.back(), in)) {
	return false;
      }
    } while (in.expect(','));
    return in.expect(']');
  }
  
  template <typename Iter> inline bool _parse_object(value& out, input<Iter>& in) {
    out = value(object_type, false);
    object& o = out.get<object>();
    if (in.expect('}')) {
      return true;
    }
    do {
      value key, val;
      if (in.expect('"')
	  && _parse_string(key, in)
	  && in.expect(':')
	  && _parse(val, in)) {
	o[key.to_str()] = val;
      } else {
	return false;
      }
    } while (in.expect(','));
    return in.expect('}');
  }
  
  template <typename Iter> inline bool _parse_number(value& out, input<Iter>& in) {
    PICO_JSON_STR num_str;
    while (1) {
      int ch = in.getc();
      if (('0' <= ch && ch <= '9') || ch == '+' || ch == '-' || ch == '.'
	  || ch == 'e' || ch == 'E') {
	num_str.push_back(ch);
      } else {
	in.ungetc();
	break;
      }
    }
    char* endp;
    out = value((int)strtod(num_str.c_str(), &endp));		// *** int ***		double を int にキャストする
    return endp == num_str.c_str() + num_str.size();
  }
  
  template <typename Iter> inline bool _parse(value& out, input<Iter>& in) {
    in.skip_ws();
    int ch = in.getc();
    switch (ch) {
#define IS(ch, text, val) case ch: \
      if (in.match(text)) { \
	out = val; \
	return true; \
      } else { \
	return false; \
      }
      IS('n', "ull", value());
      IS('f', "alse", value(false));
      IS('t', "rue", value(true));
#undef IS
    case '"':
      return _parse_string(out, in);
    case '[':
      return _parse_array(out, in);
    case '{':
      return _parse_object(out, in);
    default:
      if (('0' <= ch && ch <= '9') || ch == '-') {
	in.ungetc();
	return _parse_number(out, in);
      }
      break;
    }
    in.ungetc();
    return false;
  }
  
  // obsolete, use the version below
  template <typename Iter> inline PICO_JSON_STR parse(value& out, Iter& pos, const Iter& last) {
    PICO_JSON_STR err;
    pos = parse(out, pos, last, &err);
    return err;
  }
  
  template <typename Iter> inline Iter parse(value& out, const Iter& first, const Iter& last, PICO_JSON_STR* err) {
    input<Iter> in(first, last);
    if (! _parse(out, in) && err != NULL) {
      char buf[64];
      SNPRINTF(buf, sizeof(buf), "syntax error at line %d near: ", in.line());
      *err = buf;
      while (1) {
	int ch = in.getc();
	if (ch == -1 || ch == '\n') {
	  break;
	} else if (ch >= ' ') {
	  err->push_back(ch);
	}
      }
    }
    return in.cur();
  }
  
  inline PICO_JSON_STR parse(value& out, std::istream& is) {
    PICO_JSON_STR err;
    parse(out, std::istreambuf_iterator<char>(is.rdbuf()),
	  std::istreambuf_iterator<char>(), &err);
    return err;
  }
  
  template <typename T> struct last_error_t {
    static PICO_JSON_STR s;
  };
  template <typename T> PICO_JSON_STR last_error_t<T>::s;
  
  inline void set_last_error(const PICO_JSON_STR& s) {
    last_error_t<bool>::s = s;
  }
  
  inline const PICO_JSON_STR& get_last_error() {
    return last_error_t<bool>::s;
  }

  inline bool operator==(const value& x, const value& y) {
    if (x.is<null>())
      return y.is<null>();
#define PICO_JSON_CMP(type)					\
    if (x.is<type>())						\
      return y.is<type>() && x.get<type>() == y.get<type>()
    PICO_JSON_CMP(bool);
	PICO_JSON_CMP(int);			// *** int ***
    PICO_JSON_CMP(double);
    PICO_JSON_CMP(PICO_JSON_STR);
    PICO_JSON_CMP(array);
    PICO_JSON_CMP(object);
#undef PICO_JSON_CMP
    assert(0);
#ifdef _MSC_VER
    __assume(0);
#endif
    return false;
  }
  
  inline bool operator!=(const value& x, const value& y) {
    return ! (x == y);
  }
}

inline std::istream& operator>>(std::istream& is, pico_json::value& x)
{
  pico_json::set_last_error(PICO_JSON_STR());
  PICO_JSON_STR err = pico_json::parse(x, is);
  if (! err.empty()) {
    pico_json::set_last_error(err);
    is.setstate(std::ios::failbit);
  }
  return is;
}

inline std::ostream& operator<<(std::ostream& os, const pico_json::value& x)
{
  x.serialize(std::ostream_iterator<char>(os));
  return os;
}
#ifdef _MSC_VER
    #pragma warning(pop)
#endif

#endif
#ifdef TEST_PICO_JSON
#ifdef _MSC_VER
    #pragma warning(disable : 4127) // conditional expression is constant
#endif

using namespace std;
  
static void plan(int num)
{
  printf("1..%d\n", num);
}

static void ok(bool b, const char* name = "")
{
  static int n = 1;
  printf("%s %d - %s\n", b ? "ok" : "ng", n++, name);
}

template <typename T> void is(const T& x, const T& y, const char* name = "")
{
  if (x == y) {
    ok(true, name);
  } else {
    ok(false, name);
  }
}

#include <algorithm>

int main(void)
{
  plan(61);
  
#define TEST(in, type, cmp, serialize_test) {				\
    pico_json::value v;							\
    const char* s = in;							\
    string err = pico_json::parse(v, s, s + strlen(s));			\
    ok(err.empty(), in " no error");					\
    ok(v.is<type>(), in " check type");					\
    is(v.get<type>(), cmp, in " correct output");			\
    is(*s, '\0', in " read to eof");					\
    if (serialize_test) {						\
      is(v.serialize(), string(in), in " serialize");			\
    }									\
  }
  TEST("false", bool, false, true);
  TEST("true", bool, true, true);
  TEST("90.5", double, 90.5, false);
  TEST("\"hello\"", string, string("hello"), true);
  TEST("\"\\\"\\\\\\/\\b\\f\\n\\r\\t\"", string, string("\"\\/\b\f\n\r\t"),
       true);
  TEST("\"\\u0061\\u30af\\u30ea\\u30b9\"", string,
       string("a\xe3\x82\xaf\xe3\x83\xaa\xe3\x82\xb9"), false);
  TEST("\"\\ud840\\udc0b\"", string, string("\xf0\xa0\x80\x8b"), false);
#undef TEST

#define TEST(type, expr) {					       \
    pico_json::value v;						       \
    const char *s = expr;					       \
    string err = pico_json::parse(v, s, s + strlen(s));		       \
    ok(err.empty(), "empty " #type " no error");		       \
    ok(v.is<pico_json::type>(), "empty " #type " check type");	       \
    ok(v.get<pico_json::type>().empty(), "check " #type " array size"); \
  }
  TEST(array, "[]");
  TEST(object, "{}");
#undef TEST
  
  {
    pico_json::value v;
    const char *s = "[1,true,\"hello\"]";
    string err = pico_json::parse(v, s, s + strlen(s));
    ok(err.empty(), "array no error");
    ok(v.is<pico_json::array>(), "array check type");
    is(v.get<pico_json::array>().size(), size_t(3), "check array size");
    ok(v.get(0).is<double>(), "check array[0] type");
    is(v.get(0).get<double>(), 1.0, "check array[0] value");
    ok(v.get(1).is<bool>(), "check array[1] type");
    ok(v.get(1).get<bool>(), "check array[1] value");
    ok(v.get(2).is<string>(), "check array[2] type");
    is(v.get(2).get<string>(), string("hello"), "check array[2] value");
  }
  
  {
    pico_json::value v;
    const char *s = "{ \"a\": true }";
    string err = pico_json::parse(v, s, s + strlen(s));
    ok(err.empty(), "object no error");
    ok(v.is<pico_json::object>(), "object check type");
    is(v.get<pico_json::object>().size(), size_t(1), "check object size");
    ok(v.get("a").is<bool>(), "check property exists");
    is(v.get("a").get<bool>(), true,
       "check property value");
    is(v.serialize(), string("{\"a\":true}"), "serialize object");
  }

#define TEST(json, msg) do {				\
    pico_json::value v;					\
    const char *s = json;				\
    string err = pico_json::parse(v, s, s + strlen(s));	\
    is(err, string("syntax error at line " msg), msg);	\
  } while (0)
  TEST("falsoa", "1 near: oa");
  TEST("{]", "1 near: ]");
  TEST("\n\bbell", "2 near: bell");
  TEST("\"abc\nd\"", "1 near: ");
#undef TEST
  
  {
    pico_json::value v1, v2;
    const char *s;
    string err;
    s = "{ \"b\": true, \"a\": [1,2,\"three\"], \"d\": 2 }";
    err = pico_json::parse(v1, s, s + strlen(s));
    s = "{ \"d\": 2.0, \"b\": true, \"a\": [1,2,\"three\"] }";
    err = pico_json::parse(v2, s, s + strlen(s));
    ok((v1 == v2), "check == operator in deep comparison");
  }

  {
    pico_json::value v1, v2;
    const char *s;
    string err;
    s = "{ \"b\": true, \"a\": [1,2,\"three\"], \"d\": 2 }";
    err = pico_json::parse(v1, s, s + strlen(s));
    s = "{ \"d\": 2.0, \"a\": [1,\"three\"], \"b\": true }";
    err = pico_json::parse(v2, s, s + strlen(s));
    ok((v1 != v2), "check != operator for array in deep comparison");
  }

  {
    pico_json::value v1, v2;
    const char *s;
    string err;
    s = "{ \"b\": true, \"a\": [1,2,\"three\"], \"d\": 2 }";
    err = pico_json::parse(v1, s, s + strlen(s));
    s = "{ \"d\": 2.0, \"a\": [1,2,\"three\"], \"b\": false }";
    err = pico_json::parse(v2, s, s + strlen(s));
    ok((v1 != v2), "check != operator for object in deep comparison");
  }

  {
    pico_json::value v1, v2;
    const char *s;
    string err;
    s = "{ \"b\": true, \"a\": [1,2,\"three\"], \"d\": 2 }";
    err = pico_json::parse(v1, s, s + strlen(s));
    pico_json::object& o = v1.get<pico_json::object>();
    o.erase("b");
    pico_json::array& a = o["a"].get<pico_json::array>();
    pico_json::array::iterator i;
    i = std::remove(a.begin(), a.end(), pico_json::value(PICO_JSON_STR("three")));
    a.erase(i, a.end());
    s = "{ \"a\": [1,2], \"d\": 2 }";
    err = pico_json::parse(v2, s, s + strlen(s));
    ok((v1 == v2), "check erase()");
  }

  return 0;
}

#endif
