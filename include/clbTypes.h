#ifndef _RMTTYPES_H__INCLUDED_
#define _RMTTYPES_H__INCLUDED_

#pragma once

#include <regex>
#ifndef _WINBASE_
#include <iostream>
#include <cstdarg>
#endif


/**
 *\brief Аналог BOOST_PP_CAT (см. http://www.boost.org/doc)
 */
#define RMT_CAT(a,b)						RMT_CAT_IMPL(a,b)
#define RMT_CAT_IMPL(a,b)					a ## b

/**
 *\brief Аналог BOOST_PP_STRINGIZE (см. http://www.boost.org/doc)
 */
#define RMT_STRINGIZE(x)					RMT_STRINGIZE_((x))
#define RMT_STRINGIZE_(x)					RMT_STRINGIZE_IMPL x
#define RMT_STRINGIZE_IMPL(x)				#x

/**
 *\brief Аналог BOOST_PP_WSTRINGIZE (см. http://www.boost.org/doc)
 */
#define RMT_WSTRINGIZE(x)					RMT_WSTRINGIZE_(x)
#define RMT_WSTRINGIZE_(x)					RMT_WSTRINGIZE_IMPL(#x)
#define RMT_WSTRINGIZE_IMPL(x)				L ## x

#ifdef _UNICODE
	#define RMT_TSTRINGIZE(x)				RMT_WSTRINGIZE(x)
#else
	#define RMT_TSTRINGIZE(x)				RMT_STRINGIZE(x)
#endif

/**
 * RMT_LOC_A, RMT_LOC_W, RMT_LOC - Определение компилируемой строки формата - "Файл(Линия): "
 */
#define RMT_LOC_A __FILE__ "(" RMT_STRINGIZE(__LINE__) "): "
#define RMT_LOC_W RMT_CAT(L, __FILE__) L"(" RMT_WSTRINGIZE(__LINE__) L"): "
#ifdef _UNICODE
	#define RMT_LOC RMT_LOC_W
#else
	#define RMT_LOC RMT_LOC_A
#endif

/**
 * Debug memory leaks (Supports MFC and pure VCRT, look at vcruntime_new_debug.h)
 */
#if defined(_DEBUG) && (RMT_COMPILER==RMT_COMPILER_MSVS)
	#ifndef _MFC_OVERRIDES_NEW
		#define THIS_FILE __FILE__
		#define RMT_DEBUG_NEW new(_NORMAL_BLOCK, THIS_FILE, __LINE__)
	#else
		#define RMT_DEBUG_NEW DEBUG_NEW
	#endif
#endif

#if defined(_AFX)
#else
	// raw compilation
	#define RMT_THROW_HRESULT(hr)			throw hr
	#define	RMT_TRY							try
	#define RMT_CATCH(e)
	#define RMT_CATCHALL()					catch (...)
	#define RMT_DELETEEXCEPTION(e)
	#define RMT_ENDTRY()					RMT_CATCHALL() {}
	#define RMT_ENDTRY_FC(FCode)			RMT_CATCHALL() { FCode; }
	#define RMT_ENDTRY_HRESULT(r)			RMT_CATCHALL() {r = E_UNEXPECTED;}
	#define RMT_ENDTRY_HRESULT_FC(r, FCode)	RMT_CATCHALL() {r = E_UNEXPECTED; FCode; }
	#define RMT_ENDTRY_BOOL(r)				RMT_CATCHALL() {r = false;}
	#define RMT_ENDTRY_BOOL_FC(r, FCode)	RMT_CATCHALL() {r = false; FCode; }

	#define RMT_UNUSED_ALWAYS(x)			x
	#ifdef _DEBUG
		#define RMT_UNUSED(x)
	#else
		#define RMT_UNUSED(x)				x
	#endif
	#ifdef _DEBUG
		#define RMT_TRACE(x, ...)			trace(x,__VA_ARGS__)
		#define RMT_ASSERT_VALID(pObj)		RMT_ASSERT(pObj!=nullptr)
	#else
		#define RMT_TRACE(x, ...)			((void)0)
		#define RMT_ASSERT_VALID(pObj)		((void)0)
	#endif
	#define RMT_MANAGE_MODULESTATE
#endif

#define RMT_THROW_FALSE(hr)					{ HRESULT hERR = rmt::type2HRESULT(hr); if ( hERR == S_OK ) {} else { RMT_THROW_HRESULT(hERR); }; }
#define RMT_TRYALLOC(x)						{ RMT_TRY{x;} RMT_ENDTRY() }
#define RMT_TRYALLOC_FC(x, FCode)			{ RMT_TRY{x;} RMT_ENDTRY_FC(FCode) }
#define RMT_TRYALLOC_HRESULT(x,r)			{ RMT_TRY{x;} RMT_ENDTRY_HRESULT(r) }
#define RMT_TRYALLOC_HRESULT_FC(x,r,FCode)	{ RMT_TRY{x;} RMT_ENDTRY_HRESULT_FC(r, FCode) }
#define RMT_TRYALLOC_BOOL(x,r)				{ RMT_TRY{x;} RMT_ENDTRY_BOOL(r) }
#define RMT_TRYALLOC_BOOL_FC(x,r,FCode)		{ RMT_TRY{x;} RMT_ENDTRY_BOOL_FC(r, FCode) }
#define RMT_TRYCODE(x)						RMT_TRYALLOC(x)
#define RMT_TRYCODE_FC(x, FCode)			RMT_TRYALLOC_FC(x, FCode)
#define RMT_TRYCODE_HRESULT(x,r)			RMT_TRYALLOC_HRESULT(x,r)
#define RMT_TRYCODE_HRESULT_FC(x,r,FCode)	RMT_TRYALLOC_HRESULT_FC(x,r,FCode)
#define RMT_TRYCODE_BOOL(x,r)				RMT_TRYALLOC_BOOL(x,r)
#define RMT_TRYCODE_BOOL_FC(x,r,FCode)		RMT_TRYALLOC_BOOL_FC(x,r,FCode)

//макросы для работы с переменным количеством аргументов.
#define __VA_ARGS__1(...) SUBST(__VA_ARGS__1_HELPER(__VA_ARGS__))
#define __VA_ARGS__2(...) SUBST(__VA_ARGS__2_HELPER(__VA_ARGS__))
#define __VA_ARGS__2ANDMORE(...) SUBST(__VA_ARGS__2ANDMORE_HELPER(__VA_ARGS__))
#define __VA_ARGS__3ANDMORE(...) SUBST(__VA_ARGS__3ANDMORE_HELPER(__VA_ARGS__))
#define __VA_ARGS__1_HELPER(x,...) x
#define __VA_ARGS__2_HELPER(x,y, ...) y
#define __VA_ARGS__2ANDMORE_HELPER(a, ...) __VA_ARGS__
#define __VA_ARGS__3ANDMORE_HELPER(a,b, ...) __VA_ARGS__
#define SUBST(arg) arg
#define GLUE(x, y) x y
#define RETURN_ARG_COUNT(_1_, _2_, _3_, _4_, _5_,_6,_7,_8,_9, count, ...) count 
#define EXPAND_ARGS(args) RETURN_ARG_COUNT args
#define COUNT_ARGS_MAX9(...) EXPAND_ARGS((__VA_ARGS__,N,N,N,N,N,N,N,2,1))
#define OVERLOAD_MACRO2(name, count) name##count
#define OVERLOAD_MACRO1(name, count) OVERLOAD_MACRO2(name, count)
#define OVERLOAD_MACRO(name, count) OVERLOAD_MACRO1(name, count)
#define CALL_OVERLOAD(name, ...) GLUE(OVERLOAD_MACRO(name, COUNT_ARGS_MAX9(__VA_ARGS__)), (__VA_ARGS__))

#ifdef _DEBUG

	#if defined(_INC_CRTDBG)

		// Если есть доступ к _crtdbg.h (MSVC++ CRT) пользуемся им
		// для организации RMT_ASSERT макросов.
		// MFC и ATL библиотеки так же используют методы этого файла для отладки.

		#ifdef _UNICODE
			#define _RPTFT0		_RPTFW0
			#define _RPTFTN		_RPTFWN
		#else
			#define _RPTFT0		_RPTF0
			#define _RPTFTN		_RPTFN
		#endif
		
		#define RMT_ASSERT_1(exp)			RMT_ASSERT_2(exp, "")
		#define RMT_ASSERT_2(exp,dsc)		( (void) ((!!(exp)) || (RMT_TRACE(RMT_LOC_A "- ASSERTION spawned! Expression: " #exp "\n"), 0) || (_RPTFT0(_CRT_ASSERT, dsc), 0)) )
		#define RMT_ASSERT_N(exp,...)		( (void) ((!!(exp)) || (RMT_TRACE(RMT_LOC_A "- ASSERTION spawned! Expression: " #exp "\n"), 0) || (_RPTFTN(_CRT_ASSERT, __VA_ARGS__1(__VA_ARGS__), __VA_ARGS__2ANDMORE(__VA_ARGS__)), 0)) )

		#define RMT_ASSERT(...) CALL_OVERLOAD(RMT_ASSERT_, __VA_ARGS__)

	#else
		// Иначе используем стандартные методы C++
		#define RMT_ASSERT		assert
	#endif

	#define RMT_VERIFY(...)					RMT_ASSERT(__VA_ARGS__)
#else
	#define RMT_ASSERT(...)					((void)0)
	#define RMT_VERIFY(exp,...)				((void)(exp))
#endif


//// Если включена отладка модуля, то формируем необходимые для этого макросы
//#ifdef _RMT_MODULE_DEBUG
//	#define RMT_MODULE_TRACE(x, ...)				RMT_TRACE(x, __VA_ARGS__)
//#else
//	#define RMT_MODULE_TRACE(x, ...)				((void)0)
//#endif


//namespace std
//{
	//#if defined(_UNICODE)
	//	typedef wstring tstring;
	//	typedef wifstream tifstream;
	//	typedef wofstream tofstream;
	//	typedef wfstream tfstream;

	//	#define tcout wcout
	//	#define tcin wcin
	//	#define tcerr wcerr
	//	#define tclog wclog
	//	#if defined(_REGEX_)
	//		typedef wregex tregex; 
	//		typedef wcmatch tmatch; 
	//		typedef wcregex_iterator tregex_iterator;
	//		typedef wsregex_iterator tsregex_iterator;
	//		typedef wcregex_token_iterator tregex_token_iterator;
	//		typedef wsregex_token_iterator tsregex_token_iterator;
	//	#endif
	//#else
	//	typedef string tstring;
	//	typedef ifstream tifstream;
	//	typedef ofstream tofstream;
	//	typedef fstream tfstream;

	//	#define tcout cout
	//	#define tcin cin
	//	#define tcerr cerr
	//	#define tclog clog
	//	#if defined(_REGEX_)
	//		typedef regex tregex; 
	//		typedef cmatch tmatch; 
	//		typedef cregex_iterator tregex_iterator;
	//		typedef sregex_iterator tsregex_iterator;
	//		typedef cregex_token_iterator tregex_token_iterator;
	//		typedef sregex_token_iterator tsregex_token_iterator;
	//	#endif
	//#endif

	//// Аналог (B ? i1 : i2);
	//template<bool B, typename T1, typename T2>
	//constexpr typename std::conditional<B, T1, T2>::type&& conditional_initialize(T1&& i1, T2&& i2) {
	//	return std::move(std::get<B ? 0 : 1>(std::tie(i1, i2)));
	//}

	//template<typename T>
	//struct add_cref
	//{
	//	typedef typename add_lvalue_reference<typename add_const<typename T>::type>::type type;
	//};

#ifdef _DEBUG
	inline void trace(const std::string& szData)
	{
#if defined(_WINBASE_)
		//::OutputDebugStringA(szData.c_str());
		_CrtDbgReport(_CRT_WARN, nullptr, NULL, nullptr, "%s", szData.c_str());
#else
		std::clog << szData;
#endif
	}

	inline void trace(const std::wstring& szData)
	{
#if defined(_WINBASE_)
		//::OutputDebugStringW(szData.c_str());
		_CrtDbgReportW(_CRT_WARN, nullptr, NULL, nullptr, L"%ls", szData.c_str());
#else
		std::wclog << szData;
#endif
	}

	inline void trace(LPCSTR lpszFormat, ...)
	{
		va_list args;
		va_start(args, lpszFormat);
		std::string szBuffer(vsnprintf(nullptr, 0, lpszFormat, args) + 1, '\0');
		vsnprintf(&szBuffer.front(), szBuffer.size(), lpszFormat, args);
		va_end(args);

		trace(szBuffer);
	}

	inline void trace(LPCWSTR lpszFormat, ...)
	{
		va_list args;
		va_start(args, lpszFormat);
		std::wstring szBuffer(vswprintf(nullptr, 0, lpszFormat, args) + 1, '\0');
		vswprintf(&szBuffer.front(), szBuffer.size(), lpszFormat, args);
		va_end(args);

		trace(szBuffer);
	}
#endif

//#if defined(_VECTOR_)
//	
//	//template<typename T, typename TAlloc = allocator<T>>
//	//class vector_shrinked
//	//	: public vector < T, TAlloc >
//	//{
//	//public:
//	//	typedef vector < T, TAlloc > TBase;
//	//	typedef vector_shrinked < T, TAlloc > TThis;
//
//	//public:
//
//	//	vector_shrinked()
//	//		: TBase()
//	//	{}
//
//	//	explicit vector_shrinked(const typename TAlloc& _Al)
//	//		: TBase(_Al)
//	//	{}
//
//	//	explicit vector_shrinked(typename TBase::size_type _Count)
//	//		: TBase(_Count)
//	//	{}
//
//	//	vector_shrinked(typename TBase::size_type _Count, const typename TBase::value_type& _Val)
//	//		: TBase(_Count, _Val)
//	//	{}
//
//	//	vector_shrinked(typename TBase::size_type _Count, const typename TBase::value_type& _Val, const TAlloc& _Al)
//	//		: TBase(_Count, _Val, _Al)
//	//	{}
//
//	//	vector_shrinked(const TThis& _Right)
//	//		: TBase(_Right)
//	//	{}
//
//	//	vector_shrinked(TThis&& _Right)
//	//		: TBase(_Right)
//	//	{}
//
//	//	vector_shrinked(const TThis& _Right, const TAlloc& _Al)
//	//		: TBase(_Right, _Al)
//	//	{}
//
//	//	template<typename _Iter>
//	//	vector_shrinked(_Iter _First, _Iter _Last)
//	//		: TBase(_First, _Last)
//	//	{}
//
//	//	template<typename _Iter>
//	//	vector_shrinked(_Iter _First, _Iter _Last, const _Alloc& _Al)
//	//		: TBase(_First, _Last, _Al)
//	//	{}
//
//	//	void resize(typename TBase::size_type _Newsize) {
//	//		TBase::resize(_Newsize);
//	//		shrink_to_fit();
//	//	}
//
//	//	void resize(typename TBase::size_type _Newsize, const typename TBase::value_type& _Val) {
//	//		TBase::resize(_Newsize, _Val);
//	//		shrink_to_fit();
//	//	}
//
//	//	void pop_back() {
//	//		TBase::pop_back();
//	//		if ((TBase::size() % 10) == 0) {
//	//			shrink_to_fit();
//	//		}
//	//	}
//
//	//	void clear() {
//	//		TBase::clear();
//	//		shrink_to_fit();
//	//	}
//	//};
//
//
//	//template<typename T>
//	//inline std::vector<T> make_vector(size_t nCount, ...)
//	//{
//	//	std::vector<T> aRes;
//	//	
//	//	va_list argList;
//	//	va_start(argList, nCount);
//	//	for (; nCount>0; nCount--) aRes.push_back( va_arg(argList, T) );
//	//	va_end(argList);
//
//	//	return aRes;
//	//}
//
//	//template< typename T, size_t N >
//	//inline std::vector<T> make_vector(const T(&data)[N])
//	//{
//	//	return std::vector<T>(data, data + N);
//	//}
//
//#endif

//#if defined(GUID_DEFINED)
//	
//	template<>
//	struct hash<GUID>
//		: public unary_function<GUID, size_t>
//	{
//		typedef GUID _Kty;
//
//		size_t operator()(const _Kty& _Keyval) const
//		{
//			// Версия 2 (сравнение двух 64х битных чисел) работает быстрее
//			union CQuickCmpID
//			{
//				GUID		m_nID;
//				ULONGLONG	m_nCode[2];
//			};
//			static_assert(sizeof(GUID)==(sizeof(ULONGLONG)*2), "Static assert");
//			static_assert(sizeof(CQuickCmpID)==sizeof(GUID), "Static assert");
//
//			return std::hash<ULONGLONG>()( ((CQuickCmpID*) &_Keyval)->m_nCode[0] ) ^ std::hash<ULONGLONG>()( ((CQuickCmpID*) &_Keyval)->m_nCode[1] );
//		}
//	};
//	
//	template<>
//	struct equal_to<GUID>
//		: public binary_function<GUID, GUID, bool>
//	{
//	public:
//
//		bool operator()(const GUID& rguid1, const GUID& rguid2) const
//		{
//			// Версия 3 прямое сравнение (чистый С++ код независит от платформы)
//			return (rguid1.Data1 == rguid2.Data1) & (rguid1.Data2 == rguid2.Data2) & (rguid1.Data3 == rguid2.Data3) & std::equal(std::begin(rguid1.Data4), std::end(rguid1.Data4), std::begin(rguid2.Data4));
//		}
//	};
//
//	template<>
//	struct not_equal_to<GUID>
//		: public binary_function<GUID, GUID, bool>
//	{
//	public:
//
//		bool operator()(const GUID& rguid1, const GUID& rguid2) const
//		{
//			return !equal_to<GUID>()(rguid1, rguid2);
//		}
//	};
//
//	template<>
//	struct greater<GUID>
//		: public binary_function<GUID, GUID, bool>
//	{
//	public:
//
//		bool operator()(const GUID& rguid1, const GUID& rguid2) const
//		{
//			return memcmp(&rguid1, &rguid2, sizeof(GUID))>0;
//		}
//	};
//
//	template<>
//	struct less<GUID>
//		: public binary_function<GUID, GUID, bool>
//	{
//	public:
//
//		bool operator()(const GUID& rguid1, const GUID& rguid2) const
//		{
//			return memcmp(&rguid1, &rguid2, sizeof(GUID))<0;
//		}
//	};
//#endif


//#if defined(_TYPE_TRAITS_)
//
//	// class template type2type
//	// Converts each type into a unique, insipid type
//	// Invocation type2type<T> where T is a type
//	// Defines the type OriginalType which maps back to T
//	template <typename T>
//	struct type2type
//	{
//		typedef T TThis;
//		type2type() {};
//	};
//
//	template <typename T>
//	struct type2parent
//		: public T
//	{
//		typedef T TBase;
//		type2parent() {};
//	};
//	static_assert( is_base_of<std::pair<int, float>, type2parent<std::pair<int, float>> >::value, "Invalid std::is_base_of implementation");
//	static_assert( is_base_of<std::pair<int, float>, std::pair<int, float> >::value, "Invalid std::is_base_of implementation");
//
//	template<class Base, class Der>
//	struct is_base_or_same
//	{
//	public:
//		enum {value = is_base_of<Base, Der>::value | is_same<Base, Der>::value};
//	};
//
//	static_assert( !is_base_or_same<int, float>::value, "Invalid std::is_base_or_same implementation");
//	static_assert( is_base_or_same<int, int>::value, "Invalid std::is_base_or_same implementation");
//
//	
//	template<class Base, class Der>
//	struct is_base_of_strict
//	{
//	public:
//		enum {value = is_base_of<Base, Der>::value & !is_same<Base, Der>::value};
//	};
//	static_assert( !is_base_of_strict<std::pair<int, float>, std::pair<int, float>>::value, "Invalid std::is_base_of_strict implementation");
//	static_assert( is_base_of_strict<std::pair<int, float>, type2parent<std::pair<int, float>> >::value, "Invalid std::is_base_of_strict implementation");
//	
//#endif

//#ifdef _TUPLE_
//
//	template<size_t nIndex, typename ...Types>
//	using variadic_index = tuple_element<nIndex, tuple<Types...>>;
//
//	template<typename ...Types>
//	using variadic_first = variadic_index<0, Types...>;
//
//	template<typename ...Types>
//	using variadic_last = variadic_index<sizeof...(Types)-1, Types...>;
//
//#endif

	//template<typename InputIt, class Function>
	//inline InputIt find_if_it(InputIt first, InputIt last, Function f) {
	//	for (; first != last; ++first) {
	//		if (f(first)) {
	//			return first;
	//		}
	//	}

	//	return last;
	//}

	//template<typename InputIt, class Function>
	//inline Function for_each_it(InputIt first, InputIt last,  Function f) {
	//	for (; first != last; ++first) {
	//		f(first);
	//	}

	//	return f;
	//}

	//template<typename InputIt1, typename InputIt2, class Function>
	//inline Function for_each_bi(InputIt1 first1, InputIt1 last1, InputIt2 first2, Function f) {
	//	while (first1!=last1) {
	//		f(*first1, *first2);
	//		++first1; ++first2;
	//	}

	//	return f;
	//}

	//template<typename InputIt1, typename InputIt2, typename T, class Function>
	//inline T accumulate_bi(InputIt1 first1, InputIt1 last1, InputIt2 first2, T init, Function f) {
	//	while (first1!=last1) {
	//		init = f(init, *first1, *first2);
	//		++first1; ++first2;
	//	}

	//	return init;
	//}

	//template<typename InputIt1, typename InputIt2, typename OutputIt, class Function>
	//inline OutputIt transform_bi(InputIt1 first1, InputIt1 last1, InputIt2 first2, OutputIt res, Function f) {
	//	while (first1!=last1) {
	//		(*res) = f(*first1, *first2);
	//		++first1; ++first2; ++res;
	//	}

	//	return res;
	//}

	//template<typename InputIt1, typename OutputIt, class Function>
	//inline OutputIt transform_step(InputIt1 first, InputIt1 last, OutputIt res, intptr_t nInStep, intptr_t nOutStep, Function f) {
	//	if (first!=last) {
	//		(*res) = f(*first); advance(first, nInStep); 

	//		while (first<last) {
	//			advance(res, nOutStep); (*res) = f(*first);
	//			advance(first, nInStep); 
	//		}
	//	}

	//	return res;
	//}

	///**
	// * \brief Аналог for_each, но с передачей функциональному объекту f двух параметров: индекс элемента и самого элемента.
	// *
	// * Индексованный перебор контейнера. Вызывает функтор с двумя параметрами: индекс элемента и сам элемент.
	// */
	//template<typename InputIterator, class Function>
	//inline Function enumerate(InputIterator first, InputIterator last, Function f) {
	//	for (typename std::make_unsigned<typename std::iterator_traits<InputIterator>::difference_type>::type nIndex = 0; first!=last; ++first, ++nIndex )
	//		f(nIndex, *first);

	//	return f;
	//}

	///**
	// *\breif Алгоритм удаления из контейнера items всех элементов для которых функтор f вернeт true.
	// *\return Если удаление выполнялось алгоритм вернет true, если ничего удалено не было - false.
	// */
	//template< typename Container, typename Function >
	//bool erase_if(Container& items, Function f) {

	//	auto it = remove_if(std::begin(items), std::end(items), f);
	//	const bool bRes = (it != std::end(items));
	//	if (bRes) {
	//		items.erase(it, std::end(items));
	//	}

	//	return bRes;
	//}

	///**
	//*\breif Алгоритм удаления из контейнера items всех элементов для которых функтор f вернeт true.
	//*\return НОВЫЙ конейнер без удаленных элементов.
	//*/
	//template< typename Container, typename Function >
	//Container erase_if(Container&& items, Function f) {

	//	Container res(std::move(items));
	//	erase_if(res, f);
	//	return res;
	//}

	///**
	//*\breif Алгоритм удаления из контейнера items всех элементов из множества заданного итераторами [firstRemove, lastRemove).
	//*\return НОВЫЙ конейнер без удаленных элементов.
	//*/
	//template< typename Container, typename IteratorForRemovable >
	//Container erase_if(Container&& items, IteratorForRemovable firstRemove, IteratorForRemovable lastRemove) {

	//	Container res(std::move(items));
	//	erase_if(res, [firstRemove, lastRemove](typename iterator_traits<decltype(std::begin(res))>::reference v) -> bool {
	//		return std::find(firstRemove, lastRemove, v) != lastRemove;
	//	});
	//	return res;
	//}


	///**
	//*\breif Алгоритм поиска в множестве заданного итераторами [first, last) любого элемента из множества заданного итераторами [firstFind, lastFind).
	//*\return Возвращает пару итераторов: первый - итератор обнаруженного элемента из [first, last), второй - итератор какой именно из [firstFind, lastFind) элемент найден.
	//*/
	//template< typename Iterator, typename IteratorForFind>
	//std::pair<Iterator, IteratorForFind> find_if(Iterator first, Iterator last, IteratorForFind firstFind, IteratorForFind lastFind) {

	//	std::pair<Iterator, IteratorForFind> res(last, lastFind);
	//	res.first = std::find_if(first, last, [firstFind, lastFind, &res](typename iterator_traits<Iterator>::reference v) -> bool {
	//		auto r = std::find(firstFind, lastFind, v);
	//		const bool bRes = (r != lastFind);
	//		if (bRes) {
	//			res.second = std::move(r);
	//		}
	//		return bRes;
	//	});

	//	return res;
	//}

	//template< typename InputIterator >
	//std::vector< typename std::iterator_traits<InputIterator>::value_type > reverse_ret(InputIterator first, InputIterator last)
	//{
	//	std::vector< typename std::iterator_traits<InputIterator>::value_type > aRes;
	//	std::reverse_copy(first, last, std::inserter(aRes, std::begin(aRes)) );

	//	return aRes;
	//}

//#if defined(_STRING_)
//
//	/**
//	 *\brief Обрезать строку по первому найденному символу chCropBy.
//	 */
//	template<typename T_CHAR /*= TCHAR*/>
//	inline size_t crop_string(std::basic_string<T_CHAR>& str, T_CHAR chCropBy = static_cast<T_CHAR>(0) )
//	{
//		const size_t nSize = std::distance(str.begin(), std::find(str.begin(), str.end(), chCropBy));
//		if (nSize != str.size()) {
//			str.resize(nSize);
//		}
//		return nSize;
//	}
//
//	inline LPCSTR c_str(const char* pStr) {
//		return pStr;
//	}
//
//	inline LPCWSTR c_str(const wchar_t* pStr) {
//		return pStr;
//	}
//
//	inline LPCSTR c_str(const std::string& str) {
//		return str.c_str();
//	}
//
//	inline LPCWSTR c_str(const std::wstring& str) {
//		return str.c_str();
//	}
//
//#if defined(__ATLSTR_H__) | defined(__AFXSTR_H__)
//	inline LPCSTR c_str(const CStringA& str) {
//		return str.operator LPCSTR();
//	}
//
//	inline LPCWSTR c_str(const CStringW& str) {
//		return str.operator LPCWSTR();
//	}
//#endif
//
//	inline std::wstring make_string(const wchar_t* pStr) {
//		return (pStr != nullptr) ? pStr : L"";
//	}
//
//	inline std::wstring make_string(wchar_t* pStr) {
//		return make_string(static_cast<const wchar_t*>(pStr));
//	}
//
//	inline std::string make_string(const char* pStr) {
//		return (pStr != nullptr) ? pStr : "";
//	}
//
//	inline std::string make_string(char* pStr) {
//		return make_string(static_cast<const char*>(pStr));
//	}
//
//	inline std::wstring make_string(wstring&& str) {
//		return std::move(str);
//	}
//
//	inline std::wstring make_string(wstring& str) {
//		return str;
//	}
//
//	inline std::wstring make_string(const wstring& str) {
//		return str;
//	}
//
//	inline std::string make_string(string&& str) {
//		return std::move(str);
//	}
//
//	inline std::string make_string(string& str) {
//		return str;
//	}
//
//	inline std::string make_string(const string& str) {
//		return str;
//	}
//
//#if defined(__ATLSTR_H__) | defined(__AFXSTR_H__)
//	inline std::wstring make_string(const CStringW& str) {
//		return str.operator LPCWSTR();
//	}
//
//	inline std::string make_string(const CStringA& str) {
//		return str.operator LPCSTR();
//	}
//#endif
//#endif


//#if defined(_VECTOR_) && defined(_SET_)
//	
//	/**
//	 * Аналог шаблона set, но сохраняет порядок элементов в последовательности их добавления.
//	 * В отличии от set и unordered_set хранит в себе пару: элемент и его хеш, по которому идет быстрый поиск наличия элемента в наборе.
//	 */
//	template <typename T, class Hash = hash<T>, class Alloc = allocator< pair<size_t, T> > >
//	class insertordered_set
//	{
//	public:
//		typedef pair<T, size_t>							item;
//		typedef vector<item, Alloc>						collection;
//		typedef typename collection::size_type			size_type;
//		typedef typename collection::difference_type	difference_type;
//		typedef typename collection::iterator			iterator;
//		typedef typename collection::const_iterator		const_iterator;
//		typedef typename collection::reverse_iterator	reverse_iterator;
//		typedef typename collection::const_reverse_iterator const_reverse_iterator;
//		
//	protected:
//		collection	m_aItems;
//
//	public:
//		insertordered_set() {};
//		insertordered_set(const insertordered_set& Src) : m_aItems(Src.m_aItems) {};
//		insertordered_set(insertordered_set&& Src) : m_aItems(move(Src.m_aItems)) {};
//		
//		
//		iterator begin() {return m_aItems.begin();}
//		const_iterator begin() const {return m_aItems.begin();}
//		const_iterator cbegin() const {return m_aItems.cbegin();}
//		reverse_iterator rbegin() {return m_aItems.rbegin();}
//		const_reverse_iterator rbegin() const {return m_aItems.rbegin();}
//		
//		iterator end() {return m_aItems.end();}
//		const_iterator end() const {return m_aItems.end();}
//		const_iterator cend() const {return m_aItems.cend();}
//		reverse_iterator rend() {return m_aItems.rend();}
//		const_reverse_iterator rend() const {return m_aItems.rend();}
//
//		size_type size() const {return m_aItems.size();};
//
//		bool empty() const {return m_aItems.empty();};
//
//		void clear() const {return m_aItems.clear();};
//
//		iterator find(const T& val) { const size_t h = Hash()(val); return std::find_if(begin(), end(), [&val, &h](const item& a) -> bool {return (get<1>(a) == h) && (get<0>(a) == val); }); }
//		const_iterator find(const T& val) const { const size_t h = Hash()(val); return std::find_if(cbegin(), cend(), [&val, &h](const item& a) -> bool {return (get<1>(a) == h) && (get<0>(a) == val); }); }
//		
//		pair<iterator, bool> insert(T&& val) { 
//			iterator it(find(val)); 
//			const bool bNew = it==end(); 
//			if (bNew) {
//				m_aItems.emplace_back(forward<T>(val), Hash()(val));
//				it = --m_aItems.end();
//			}
//			return make_pair(it, bNew);
//		}
//	};
//
//#endif

//#ifdef _ITERATOR_
//
//	template<typename T>
//	class CBuff_const_iterator
//		: public iterator<input_iterator_tag, T, ptrdiff_t, const T*, const T&>
//	{
//	public:
//		typedef typename CBuff_const_iterator<T> TThis;
//
//	protected:
//		const T*		m_pBuff;
//		difference_type	m_nCount;
//
//	public:
//		CBuff_const_iterator(const T* pBuff, size_t nCount)
//			: m_pBuff(pBuff)
//			, m_nCount(nCount)
//		{}
//
//		CBuff_const_iterator(const TThis& Src)
//			: m_pBuff(Src.m_pBuff)
//			, m_nCount(Src.m_nCount)
//		{}
//
//		CBuff_const_iterator(TThis&& Src)
//			: m_pBuff(Src.m_pBuff)
//			, m_nCount(Src.m_nCount)
//		{ Src.m_nCount = 0; Src.m_pBuff = nullptr; }
//
//		// preincrement
//		inline TThis& operator++()
//		{
//#if defined(_ITERATOR_DEBUG_LEVEL) && (_ITERATOR_DEBUG_LEVEL > 0)
//			if ( (m_pBuff==nullptr) | (m_nCount<1) )
//			{	// report error
//#if _ITERATOR_DEBUG_LEVEL >= 2
//				RMT_ASSERT(FALSE, _T("CBuff_const_iterator<> not incrementable"));
//#endif
//				throw std::out_of_range("out of range");
//			}
//#endif
//			m_pBuff++; m_nCount--;
//			return (*this);
//		}
//
//		// postincrement
//		inline TThis operator++(int)
//		{
//			TThis _Tmp = *this;
//			++*this;
//			return (_Tmp);
//		}
//
//		// increment by integer
//		inline TThis& operator+=(difference_type _Off)
//		{
//#if defined(_ITERATOR_DEBUG_LEVEL) && (_ITERATOR_DEBUG_LEVEL > 0)
//			if ( (m_pBuff==nullptr) | (m_nCount<_Off) )
//			{	// report error
//#if _ITERATOR_DEBUG_LEVEL >= 2
//				RMT_ASSERT(FALSE, _T("CBuff_const_iterator<> not incrementable"));
//#endif
//				throw std::out_of_range("out of range");
//			}
//#endif
//			m_pBuff += _Off; 
//			m_nCount-= _Off;
//			return (*this);
//		}
//
//		inline bool operator==(const TThis& rhs) const 
//		{
//			return (m_pBuff==rhs.m_pBuff);
//		}
//
//		inline bool operator!=(const TThis& rhs) const 
//		{
//			return (m_pBuff!=rhs.m_pBuff);
//		}
//
//		inline reference operator*() const
//		{
//#if defined(_ITERATOR_DEBUG_LEVEL) && (_ITERATOR_DEBUG_LEVEL > 0)
//			if ( (m_pBuff==nullptr) | (m_nCount==0) )
//			{	// report error
//#if _ITERATOR_DEBUG_LEVEL >= 2
//				RMT_ASSERT(FALSE, _T("CBuff_const_iterator<> not dereferencable"));
//#endif
//				throw std::out_of_range("out of range");
//			}
//#endif
//			return (*m_pBuff);
//		}
//	};
//
//	template<typename T>
//	CBuff_const_iterator<T> begin(const T* pBuff, size_t nCount)
//	{
//		return CBuff_const_iterator<T>(pBuff, nCount);
//	}
//
//	template<typename T>
//	CBuff_const_iterator<T> end(const T* pBuff, size_t nCount)
//	{
//		return CBuff_const_iterator<T>(&(pBuff[nCount]), 0);
//	}
//
//	/**
//	 *\breif Однонаправленный input итератор-преопразователь.
//	 *
//	 * Этот шаблон реализует однонаправленный итератор, выполняющий online преобразование исходнных данных при помощи унарной функции.
//	 * В качестве исходных данных выступает другой итератор Iterator. Таким образом этот итератор является надстройкой над любым input итератором
//	 * и прежде чем вернуть данные от адрессуемые этим итератором выполняет с ними преобразование заданные унарной функцией, лямдой или функтором UnaryFunction.
//	 *
//	 * Данный итератор сделан для того чтобы можно было использовать стандарные алгоритмы STL над массивами данных, при этом модифицируя входные данные без использования
//	 * промежуточных буферов.
//	 */
//	template<typename Iterator, typename UnaryFunction>
//	class forward_transform_iterator
//		: public iterator<input_iterator_tag, typename iterator_traits<Iterator>::value_type, typename iterator_traits<Iterator>::difference_type, typename iterator_traits<Iterator>::pointer, typename iterator_traits<Iterator>::reference>
//	{
//	public:
//		typedef typename forward_transform_iterator<Iterator, UnaryFunction> TThis;
//		typedef typename iterator<input_iterator_tag, typename iterator_traits<Iterator>::value_type, typename iterator_traits<Iterator>::difference_type, typename iterator_traits<Iterator>::pointer, typename iterator_traits<Iterator>::reference> TBase;
//
//		typedef typename TBase::value_type		value_type;
//		typedef typename TBase::reference		reference;
//		typedef typename TBase::difference_type	difference_type;
//
//		typedef std::pair<value_type, bool>		value_pair;
//
//	protected:
//
//		Iterator		m_iterator;
//		value_pair		m_val;
//		UnaryFunction	m_f;
//
//	public:
//		forward_transform_iterator(Iterator const& it, UnaryFunction f)
//			: m_iterator(it)
//			, m_f(f)
//			, m_val(std::make_pair(value_type(), false))
//		{}
//
//		forward_transform_iterator(forward_transform_iterator&& src)
//			: m_iterator(std::move(src.m_iterator))
//			, m_f(std::move(src.m_f))
//			, m_val(std::move(src.m_val))
//		{}
//
//		TThis& operator = (const forward_transform_iterator& src)
//		{
//			m_iterator	= src.m_iterator;
//			m_f			= src.m_f;
//			m_val		= src.m_val;
//			
//			return (*this);
//		}
//
//		inline TThis& operator++()
//		{
//			this->operator *();
//			++m_iterator;
//			m_val.second = false;
//
//			return (*this);
//		}
//
//		inline TThis& operator+=(difference_type _Off)
//		{
//			for (TBase::difference_type pos = 0; pos<_Off; pos++)
//				this->operator++();
//
//			return (*this);
//		}
//
//		inline bool operator!=(const TThis& rhs) const 
//		{
//			return m_iterator!=rhs.m_iterator;
//		}
//
//		inline bool operator==(const TThis& rhs) const 
//		{
//			return m_iterator==rhs.m_iterator;
//		}
//
//		inline bool operator!=(const Iterator& rhs) const 
//		{
//			return m_iterator!=rhs;
//		}
//
//		inline bool operator==(const Iterator& rhs) const 
//		{
//			return m_iterator==m_iterator;
//		}
//
//		inline reference operator*()
//		{
//			if (!m_val.second)
//				m_val = std::move(std::make_pair(m_f(*m_iterator), true));
//
//			return m_val.first;
//		}
//
//	private:
//		forward_transform_iterator();
//	};
//
//#endif
//}


//namespace rmt
//{
//	namespace Tools
//	{
//		struct type_yes		{ char m; };
//		struct type_no		{ type_yes m[2]; };
//
//		template <typename TSign, TSign val> struct SignatureWrapper {};
//	}
//
//	/**
//	* \brief Создать объект для использования лямбда-функций в качестве callback-функций.
//	*
//	* Лямбда-функций предпологают наличие захватываемых переменных и строго определенную в С++ модель вызова.
//	* Такие условяи заставляют разработчиков реализовывать в компиляторах лямбды в виде функциональных объектов, а это в
//	* свою очередь не позволяет применять лямбды в тех местах, где API требует наличие callback-функций.
//	*
//	* Это шаблон позволяет решить эту проблему. Он создает класс с указателем на лямбда функцию и переменную этого класса. Класс
//	* содержит в себе статическую функцию(функции), которая удовлетворяет требованиям callback-функций. Использование класса простое:
//	*
//	* \code
//	*	const auto myLambda = [&](params) ->DWORD { code; return 0; };
//	*	rmt::TCallback<DWORD, params> myCallback(myLambda);
//	*
//	*  someAPIcall(APIparams, myCallback::callback, reinterpret_cast<LPVOID>(&myCallback));
//	* \endcode
//	*/
//	template<typename TRet, typename ...TArgs>
//	class TCallback
//		: public std::function<TRet(TArgs...)>
//	{
//	public:
//		using TThis = TCallback<TRet, TArgs...>;
//		using TParent = std::function<TRet(TArgs...)>;
//
//	public:
//
//		template<typename TLambda>
//		TCallback(TLambda&& l) : TParent(l) {
//		}
//
//		static TRet __stdcall callback(TArgs... args, void* lpData) {
//			TThis* pThis = reinterpret_cast<TThis*>(lpData);
//			RMT_ASSERT(pThis != nullptr);
//			return (*static_cast<TParent*>(pThis))(args...);
//		}
//
//		static TRet __stdcall callback_first(void* lpData, TArgs... args) {
//			return callback(args..., lpData);
//		}
//
//		static TRet __cdecl callback_c(TArgs... args, void* lpData) {
//			return callback(args..., lpData);
//		}
//
//		static TRet __cdecl callback_first_c(void* lpData, TArgs... args) {
//			return callback(args..., lpData);
//		}
//	};
//	
//	#ifdef _RMT_MFCBUILD
//	
//	/**
//	 * \brief Возвращает true, если класс представленный структурой CRuntimeClass - абстрактный.
//	 */
//	inline bool IsAbstract(CRuntimeClass* pClass)
//		{ return (pClass==NULL) || (pClass->m_pfnCreateObject==NULL);}
//
//	#endif
//}


//---------------------------------------------------------------------------
// types-unions/structures for low-lewel work with bytes.
//#pragma warning( push )
//#pragma warning( disable : 4201)  // Disable warning messages
typedef unsigned char byte;
/**
*\brief Эдакий тип для работы с отдельными байтами одного слова 16-бит.
*/
union rmt2bytes
{
	short 			i;
	unsigned short	w;
	
	struct { byte Lo, Hi; };
	struct { byte b0, b1; };
	
	rmt2bytes() noexcept  {}
	rmt2bytes(short data)noexcept : i(data) {}
};

union rmt4bytes
{
	int				i;
	unsigned int	dw;

	struct { byte	b0, b1, b2, b3; };
	struct { unsigned short	w0, w1; };
	struct { short			Lo, Hi; };

	rmt4bytes()noexcept {}
	rmt4bytes(int data)noexcept : i(data) {}
};

//#pragma warning( pop )


/////////////////////////////////////////////////////////////////////////////
//	inline functions
//---------------------------------------------------------------------------
#ifdef _WINBASE_
/**
* Set thread name. 
* This way works with msVS 2015.
* For VS 2017 SetThreadDescription() is better;
*
* Устанавливает имя цепочки заданной своим ИД, отображаемое в отладчике.
* подробнее см. https://msdn.microsoft.com/en-us/library/xcb2z8hs.aspx
*/
inline void SetThreadName(LPCSTR threadName, int dwThreadID = -1)
{
	struct alignas(8)
	{
		int	dwType=0x1000;				// Must be 0x1000.
		LPCSTR	szName;			// Pointer to name (in user addr space).
		int	dwThreadID;			// Thread ID (-1=caller thread).
		int	dwFlags=0;			// Reserved for future use, must be zero.
	} info;
	info.szName		= threadName;
	info.dwThreadID = dwThreadID== -1 ? GetCurrentThreadId() : dwThreadID;
	__try {RaiseException(0x406D1388, 0, sizeof(info) / sizeof(ULONG_PTR), reinterpret_cast<ULONG_PTR*>(&info));}
	__except (EXCEPTION_EXECUTE_HANDLER) {}
}
#endif
//--------------------------------------------------------------------

//---------------------------------------------------------------------------
// реализация Inline функций
/////////////////////////////////////////////////////////////////////////////

#endif