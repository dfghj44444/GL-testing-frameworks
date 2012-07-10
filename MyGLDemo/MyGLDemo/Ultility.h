#ifndef _UTILITY_H_
#define _UTILITY_H_

const int FORMAT_BUFFER_SIZE = 1024;
void DebugOut(const char* fmt, ... )
{
	char format[FORMAT_BUFFER_SIZE];

	va_list	argptr;
	va_start( argptr, fmt );
	if( 0==argptr ) return;
	vsprintf( format, fmt, argptr );
	va_end( argptr );

	::OutputDebugStringA(format);
};


#define ERROR_DATA -99999999
#define _RELEASE(A)			try{ if( (A) != NULL ) { (A) -> Release(); (A) = NULL; } } catch(...) { DebugOut("error ->Release!"); }
#define _DELETE(X)			try{ if( (X) != NULL ) { delete (X); (X) = NULL; } } catch(...) { DebugOut("error delete!"); }
#define _DELETE_ARRAY(X)	try{ if( (X) != NULL ) { delete[] (X); (X) = NULL; } } catch(...) { DebugOut("error delete[]!"); }
#define _FREE(X)			try{ if( (X) != NULL ) { free(X); (X)=NULL; } } catch(...) { DebugOut("error free()!"); }
#define KEYDOWN(name,key) (name[key] & 0x80)

#ifdef _DEBUG
	#define XM_ASSERT(x)    if(!(x)) {__asm {int 0x03}}
#else
	#define XM_ASSERT(x)    
#endif

const float ROUNDING_ERROR_FLOAT = 0.000001f;
const double ROUNDING_ERROR_DOUBLE = 0.00000001;
inline bool equals(const double a, const double b )
{
	return fabs( a - b ) < ROUNDING_ERROR_DOUBLE;
}


inline bool equals(const float a, const float b )
{
	return fabs( a - b ) < ROUNDING_ERROR_FLOAT;
}

inline bool equals(const int a, const int b )
{
	return (a == b);
}

inline float squareroot(const float f)
{
	return sqrtf(f);
}

inline double squareroot(const double f)
{
	return sqrt(f);
}

inline int squareroot(const int f)
{
	return static_cast<int>(squareroot(static_cast<float>(f)));
}

#define RANDOM_RANGE(high, low) ( (double)rand() / (RAND_MAX + 1) * (high - low) + low ) 


#endif