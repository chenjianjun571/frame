
#ifndef _I18N_H_
#define _I18N_H_


#include <locale.h>

#ifdef WIN32

#define _(String)  (String)
#define N_(String) (String)

#endif

#ifdef _AIX
//#include <libintl.h>
//
//#define _(String)  gettext(String)
//#define N_(String)  gettext(String)

#define _(String)  (String)
#define N_(String) (String)

#endif

#ifdef _LINUX
#include <libintl.h>

#define _(String)  gettext(String)
#define N_(String)  gettext(String)

#endif


#endif
