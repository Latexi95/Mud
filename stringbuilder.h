#ifndef STRINGBUILDER_H
#define STRINGBUILDER_H
#include <string>
#include <cstring>
#include <cmath>
#include <cstdint>
#include <iostream>

template<typename T>
struct StringBuilderConcatenable {};

template<typename A, typename B>
struct StringBuilder {
		StringBuilder(const A &a_, const B &b_) : a(a_), b(b_) {}
		operator std::string() {
			std::string s(StringBuilderConcatenable<A>::size(a) + StringBuilderConcatenable<B>::size(b), '\0');
			char *ptr = &s[0];
			StringBuilderConcatenable<A>::appendTo(a, ptr);
			StringBuilderConcatenable<B>::appendTo(b, ptr);
			std::cout << (StringBuilderConcatenable<A>::size(a) + StringBuilderConcatenable<B>::size(b)) << std::endl;
			return s;
		}

		const A &a;
		const B &b;
};


template<typename INTTY, int BASE = 10>
struct SBInteger {
	SBInteger(INTTY i) : mValue(i) {}
	INTTY mValue;
};

template<typename INTTY, int BASE = 10>
SBInteger<INTTY, BASE> SBInt(INTTY i) {
	return SBInteger<INTTY, BASE>(i);
}



template<typename T>
struct SBWrapper {
		SBWrapper(const T &v) : mValue(v) {}
		const T &mValue;
};

template<typename T>
struct StringBuilderConcatenable<SBWrapper<T>> {
		typedef SBWrapper<T> type;
		static int size(const SBWrapper<T> &v) { return StringBuilderConcatenable<T>::size(v.mValue); }
		static inline void appendTo(const SBWrapper<T> &s, char *&out) {
			StringBuilderConcatenable<T>::appendTo(s.mValue, out);
		}
};


template <typename T>
SBWrapper<T> SB(const T &v) {
	return SBWrapper<T>(v);
}



template<>
struct StringBuilderConcatenable<std::string> {
		typedef std::string type;
		static int size(const std::string &str) { return str.size(); }
		static inline void appendTo(const std::string &str, char *&out) {
			for (std::string::const_iterator i = str.begin(); i != str.end(); ++i) {
				*out++ = *i;
			}
		}
};

template<int N>
struct StringBuilderConcatenable<char[N]> {
		typedef char type[N];
		static int size(const char[N]) { return N - 1; }
		static inline void appendTo(const char a[N], char *&out) {
			for (int i = 0; i != N - 1; i++) {
				*out++ = a[i];
			}
		}
};

template<>
struct StringBuilderConcatenable<const char*> {
		typedef const char * type;
		static int size(const char *str) { return strlen(str); }
		static inline void appendTo(const char *i, char *&out) {
			for (;*i;) {
				*out++ = *i++;
			}
		}
};

template<>
struct StringBuilderConcatenable<char> {
		typedef char type;
		static int size(char) { return 1; }
		static inline void appendTo(char c, char *&out) {
			*out++ = c;
		}
};

template<typename INTTY>
struct StringBuilderConcatenable<SBInteger<INTTY, 2> > {
		typedef SBInteger<INTTY, 2> type;
		static int size(type) {
			return sizeof(INTTY) * 8;
		}
		static inline void appendTo(type v, char *&out) {
			INTTY i = v.mValue;
			uint64_t bit = sizeof(INTTY) * 8 - 1;
			do {
				*out++ = '0' + (uint64_t)((i & (1 << bit)) >> bit);
				--bit;
			} while (bit > 0);
			*out++ = '0' + (i & 1);
		}
	private:
};

template<typename INTTY>
struct StringBuilderConcatenable<SBInteger<INTTY, 10> > {
		typedef SBInteger<INTTY, 10> type;
		static int size(type i) {
			return i.mValue < 0 ? unsignedLen(-i.mValue) + 1 : unsignedLen(i.mValue);
		}
		static inline void appendTo(type v, char *&out) {
			INTTY i = v.mValue;
			static const char nums[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
			char *str = out;
			int size = 0;
			if (i < 0) {
				*str = '-';
				i = -i;
				str++;
				++size;
			}

			INTTY shifter = i;
			do{
				++str;
				shifter = shifter/10;
			}while(shifter);

			while (i) {
				*(--str) = nums[i % 10];
				++size;
				i /= 10;
			}
			out += size;
		}

	private:
		static int unsignedLen(INTTY x) {
			if (x < 10) return 1;
			if (x < 100) return 2;
			if (x < 1000) return 3;
			if (x < 10000) return 4;
			if (x < 100000) return 5;
			return (INTTY)log10((double)x) + 1;
		}
};

template<typename INTTY>
struct StringBuilderConcatenable<SBInteger<INTTY, 16> > {
		typedef SBInteger<INTTY, 16> type;
		static int size(type i) {
			return i.mValue < 0 ? unsignedLen(-i.mValue) + 1 : unsignedLen(i.mValue);
		}
		static inline void appendTo(type v, char *&out) {
			INTTY i = v.mValue;
			static const char nums[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
			char *str = out;
			int size = 0;
			if (i < 0) {
				*str = '-';
				i = -i;
				str++;
				++size;
			}

			INTTY shifter = i;
			do{
				++str;
				shifter = shifter/0x10;
			}while(shifter);

			while (i) {
				*(--str) = nums[i % 0x10];
				++size;
				i /= 0x10;
			}
			out += size;
		}

	private:
		static int unsignedLen(INTTY x) {
			if (x < 0x10) return 1;
			if (x < 0x100) return 2;
			if (x < 0x1000) return 3;
			if (x < 0x10000) return 4;
			if (x < 0x100000) return 5;
			return (INTTY)(log((double)x)/log(0x10)) + 1;
		}
};

template<typename A, typename B>
struct StringBuilderConcatenable<StringBuilder<A, B> > {
		typedef StringBuilder<A, B> type;
		static int size(const type &p) {
			return StringBuilderConcatenable<A>::size(p.a) + StringBuilderConcatenable<B>::size(p.b);
		}
		static inline void appendTo(const type &p, char *&out) {
			StringBuilderConcatenable<A>::appendTo(p.a, out);
			StringBuilderConcatenable<B>::appendTo(p.b, out);
		}
};

template <typename A, typename B>
StringBuilder<typename StringBuilderConcatenable<A>::type, typename StringBuilderConcatenable<B>::type>
operator+(const A &a, const B &b) {
	return StringBuilder<typename StringBuilderConcatenable<A>::type, typename StringBuilderConcatenable<B>::type>(a, b);
}

template <typename A, typename B>
StringBuilder<typename StringBuilderConcatenable<A>::type, typename StringBuilderConcatenable<B>::type>
operator%(const A &a, const B &b) {
	return StringBuilder<typename StringBuilderConcatenable<A>::type, typename StringBuilderConcatenable<B>::type>(a, b);
}

#endif // STRINGBUILDER_H
