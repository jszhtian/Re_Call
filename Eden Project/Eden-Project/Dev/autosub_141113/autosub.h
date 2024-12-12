
#ifndef __strmif_h__
typedef void (__stdcall * pCallback_t)(unsigned int, void *);
#else
typedef void (__stdcall * pCallback_t)(unsigned int, IGraphBuilder *);
#endif

#define NO_ERROR 0L
#define ERROR__BUSY -1L
#define ERROR__VSFILTER_INIT -2L
#define ERROR__FILTERGRAPH_CREATE -3L

#ifdef __cplusplus
extern "C"
#else
extern
#endif
int __stdcall RegisterCallBack(pCallback_t);

#ifdef __cplusplus
extern "C"
#else
extern
#endif
void __stdcall SetSubtitleData(unsigned char *, unsigned int);
