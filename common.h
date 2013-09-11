// DEMO

#ifndef PLUGINS_DEMO_COMMON_H_
#define PLUGINS_DEMO_COMMON_H_

#if defined(_WIN32) || defined(WIN32)
  #ifdef LIB_STATIC
    #define PLUGIN_DEMOS_DECL
  #else
    #ifdef PLUGINS_DEMO_EXPORT
      #define PLUGIN_DEMOS_DECL __declspec(dllexport)
    #else
      #define PLUGIN_DEMOS_DECL __declspec(dllimport)
    #endif
  #endif
#else
    #define PLUGIN_DEMOS_DECL
#endif



#endif