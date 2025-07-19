
#ifndef DEBUG_H
#define DEBUG_H


#ifdef SHOW_MESSAGE
	#define ACMSG(str) std::cout<< "[AC msg]: " << str << std::endl
#else
	#define ACMSG(str)
#endif

#ifdef SHOW_WARNING
	#define ACWARN(str) std::cout<< "[AC Warning]: " << str << std::endl
#else
	#define ACWARN(str)
#endif

#ifdef SHOW_ERROR
#define ACERR(str) std::cout<< "[AC ERROR!!!]: " << str << std::endl; abort()
#else
	#define ACERR(str) abort();
#endif

#define ACASSERT(expr, msg)  \
		if (!(expr)) { \
			ACERR(msg);\
		}



#endif
