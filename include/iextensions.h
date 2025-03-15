// Copyright (c) Dreamy Cecil; see copyright notice in LICENSE file

#ifndef CLASSICSPATCH_IEXTENSIONS_H
#define CLASSICSPATCH_IEXTENSIONS_H
#ifdef _WIN32
  #pragma once
#endif

#include "classicspatch_common.h"
#include "iplugins.h"

// Opaque handle type for referencing specific extensions
typedef class CPluginModule *HPatchPlugin;

// Define extension data for Classics Patch
// Example usage:
//    CLASSICSPATCH_DEFINE_EXTENSION("PATCH_EXT_fundomizer", k_EPluginFlagGame | k_EPluginFlagServer, MakeVersion(1, 0, 0),
//      "SAM_IS_AWESOME", "Fundomizer", "Cool plugin for randomizing various gameplay aspects. Works on dedicated servers too!");
#define CLASSICSPATCH_DEFINE_EXTENSION(extension, utility, version, author, name, description) \
  PLUGINMODULE_DEFINEMETHOD(void, GETINFO) (PluginInfo_t *pOutInfo) { \
    pOutInfo->m_ulAPI = CLASSICSPATCH_INTERFACE_VERSION; \
    pOutInfo->m_ulFlags = utility; \
    pOutInfo->SetMetadata(version, author, name, description); \
    pOutInfo->m_strExtensionIdentifier = extension; \
  };

// Extension handle symbol for easy access to the extension from within itself
// Example usage:
//    extern HPatchPlugin EXTENSIONMODULE_LOCALHANDLE;
//    ClassicsExtensions_GetInt(EXTENSIONMODULE_LOCALHANDLE, "my_int", &iValue);
#define EXTENSIONMODULE_LOCALHANDLE ClassicsPatchExtension_LocalPluginHandle

// Define a local handle to this extension
// Example usage:
//    // In global scope
//    CLASSICSPATCH_DEFINE_EXTENSION_HANDLE;
//    // In some function
//    ClassicsExtensions_SetInt(EXTENSIONMODULE_LOCALHANDLE, "my_int", 0);
#define CLASSICSPATCH_DEFINE_EXTENSION_HANDLE \
  MODULE_API HPatchPlugin EXTENSIONMODULE_LOCALHANDLE = NULL

// Get current amount of available extensions
PATCH_API int PATCH_CALLTYPE ClassicsExtensions_GetExtensionCount(void);

// Retrieve a specific extension by its index in the array (from 0 to GetExtensionCount() - 1)
// The array order is never guaranteed to be the same at all times, so different calls to this function with the same index
// may return handles to different plugins. It's recommended to use a more reliable GetExtensionByName() method instead.
PATCH_API HPatchPlugin PATCH_CALLTYPE ClassicsExtensions_GetExtensionByIndex(int iExtension);

// Retrieve a specific extension by its identifier
PATCH_API HPatchPlugin PATCH_CALLTYPE ClassicsExtensions_GetExtensionByName(const char *strExtension);

// One extension property in the array
struct ExtensionProp_t
{
  const char *m_strProperty;

  enum EType {
    k_EType_Bool   = 0,
    k_EType_Int    = 1,
    k_EType_Float  = 2,
    k_EType_Double = 3,
    k_EType_String = 4,
    k_EType_Data   = 5,
  } m_eType;

  union {
    bool        _bool;
    int         _int;
    float       _float;
    double      _double;
    const char *_string;
    void       *_data;
  } m_value;

  // Constructors for each type because initializer lists {} don't work with unions
  ExtensionProp_t(const char *str, bool        bValue) : m_strProperty(str), m_eType(k_EType_Bool)   { m_value._bool   = bValue; };
  ExtensionProp_t(const char *str, int         iValue) : m_strProperty(str), m_eType(k_EType_Int)    { m_value._int    = iValue; };
  ExtensionProp_t(const char *str, float       fValue) : m_strProperty(str), m_eType(k_EType_Float)  { m_value._float  = fValue; };
  ExtensionProp_t(const char *str, double      fValue) : m_strProperty(str), m_eType(k_EType_Double) { m_value._double = fValue; };
  ExtensionProp_t(const char *str, const char *sValue) : m_strProperty(str), m_eType(k_EType_String) { m_value._string = sValue; };
  ExtensionProp_t(const char *str, void       *pValue) : m_strProperty(str), m_eType(k_EType_Data)   { m_value._data   = pValue; };
};

// Extension properties symbols
#define EXTENSIONMODULE_PROPSARRAY  ClassicsPatchExtension_PropsArray
#define EXTENSIONMODULE_PROPSCOUNT  ClassicsPatchExtension_PropsCount

// Define array of extension properties
// Example usage:
//    CLASSICSPATCH_EXTENSION_PROPS_BEGIN {
//      ExtensionProp_t( "my_bool_property", true ),
//      ExtensionProp_t( "my_int_property", -1 ),
//      ExtensionProp_t( "my_float_property", 0.0f ),
//      ExtensionProp_t( "my_string_property", "Hello, world!" ),
//    } CLASSICSPATCH_EXTENSION_PROPS_END;
#define CLASSICSPATCH_EXTENSION_PROPS_BEGIN MODULE_API ExtensionProp_t EXTENSIONMODULE_PROPSARRAY[] =
#define CLASSICSPATCH_EXTENSION_PROPS_END ; MODULE_API const size_t EXTENSIONMODULE_PROPSCOUNT = ARRAYCOUNT(EXTENSIONMODULE_PROPSARRAY)

// Try to find some extension property; returns NULL if not found
PATCH_API ExtensionProp_t *PATCH_CALLTYPE ClassicsExtensions_FindProperty(HPatchPlugin hPlugin, const char *strProperty);

// Try to find some extension property of a specific type; returns NULL if not found
//
// It may be slightly faster than ClassicsExtensions_FindProperty() with subsequent type checking
// because it immediately skips properties with mismatching types without comparing name strings.
PATCH_API ExtensionProp_t *PATCH_CALLTYPE ClassicsExtensions_FindPropertyOfType(HPatchPlugin hPlugin, const char *strProperty, ExtensionProp_t::EType eType);

// Retrieve a boolean value from some extension property
// Returns false if the property for this value type cannot be found
PATCH_API bool PATCH_CALLTYPE ClassicsExtensions_GetBool(HPatchPlugin hPlugin, const char *strProperty, bool *pValue);

// Retrieve an integer value from some extension property
// Returns false if the property for this value type cannot be found
PATCH_API bool PATCH_CALLTYPE ClassicsExtensions_GetInt(HPatchPlugin hPlugin, const char *strProperty, int *pValue);

// Retrieve a float value from some extension property
// Returns false if the property for this value type cannot be found
PATCH_API bool PATCH_CALLTYPE ClassicsExtensions_GetFloat(HPatchPlugin hPlugin, const char *strProperty, float *pValue);

// Retrieve a float value from some extension property
// Returns false if the property for this value type cannot be found
PATCH_API bool PATCH_CALLTYPE ClassicsExtensions_GetDouble(HPatchPlugin hPlugin, const char *strProperty, double *pValue);

// Retrieve a string value from some extension property
// Returns false if the property for this value type cannot be found
PATCH_API bool PATCH_CALLTYPE ClassicsExtensions_GetString(HPatchPlugin hPlugin, const char *strProperty, const char **pValue);

// Retrieve a pointer to some data from some extension property
// Returns false if the property for this value type cannot be found
PATCH_API bool PATCH_CALLTYPE ClassicsExtensions_GetData(HPatchPlugin hPlugin, const char *strProperty, void **pValue);

// Set a boolean value to some extension property
// Returns false if the property for this value type cannot be found
PATCH_API bool PATCH_CALLTYPE ClassicsExtensions_SetBool(HPatchPlugin hPlugin, const char *strProperty, bool bValue);

// Set an integer value to some extension property
// Returns false if the property for this value type cannot be found
PATCH_API bool PATCH_CALLTYPE ClassicsExtensions_SetInt(HPatchPlugin hPlugin, const char *strProperty, int iValue);

// Set a float value to some extension property
// Returns false if the property for this value type cannot be found
PATCH_API bool PATCH_CALLTYPE ClassicsExtensions_SetFloat(HPatchPlugin hPlugin, const char *strProperty, float fValue);

// Set a float value to some extension property
// Returns false if the property for this value type cannot be found
PATCH_API bool PATCH_CALLTYPE ClassicsExtensions_SetDouble(HPatchPlugin hPlugin, const char *strProperty, double fValue);

// Set a string value to some extension property
// Returns false if the property for this value type cannot be found
PATCH_API bool PATCH_CALLTYPE ClassicsExtensions_SetString(HPatchPlugin hPlugin, const char *strProperty, const char *strValue);

// Set a pointer to some data to some extension property
// Returns false if the property for this value type cannot be found
PATCH_API bool PATCH_CALLTYPE ClassicsExtensions_SetData(HPatchPlugin hPlugin, const char *strProperty, void *pValue);

// Convenient structure for referring to specific values in extension properties
// Template argument must be one of the class types supported by ExtensionProp_t
//
// When getting & setting values, it searches for the desired property only once on the initial call
// and then reuses the value from the found property with subsequent calls.
template<class Type>
struct ExtensionPropRef_t {
  // Handle to the extension that should contain this property
  HPatchPlugin m_hExtension;

  // Name of a property that needs to be looked up
  //
  // When not NULL, it is used to find a property with that name, the value of which is saved into m_pValue.
  // After that, it's reset to NULL to prevent further lookups. It can be set to another name again later
  // in order to search for another property the next time a new value is needed.
  const char *m_strProperty;
  ExtensionProp_t::EType m_ePropType;

  // Referenced value of some property
  // Causes undefined behavior when NULL, which is the result of not being able to find the property
  Type *m_pValue;

  // Default constructor for later setup via SetupReference()
  ExtensionPropRef_t() : m_hExtension(NULL), m_strProperty(NULL), m_pValue(NULL) {};

  // Constructor that prepares a property inside some extension
  inline ExtensionPropRef_t(HPatchPlugin hExtension, const char *strProperty) {
    SetupReference(hExtension, strProperty);
  };

  // Constructor that prepares a property inside some extension
  // m_hExtension handle is found by searching for an extension with the matching identifier
  inline ExtensionPropRef_t(const char *strExtension, const char *strProperty) {
    SetupReference(ClassicsExtensions_GetExtensionByName(strExtension), strProperty);
  };

  // Setup this reference to look up a specific property in a specific extension next time
  void SetupReference(HPatchPlugin hExtension, const char *strProperty);

  // Try to look up the desired property and retrieve its value
  // Returns false if the value cannot be retrieved (no property found)
  bool GetValue(Type *pValue)
  {
    // If there's a property to look up
    if (m_hExtension != NULL && m_strProperty != NULL) {
      // Look it up and then reset it
      ExtensionProp_t *pProp = ClassicsExtensions_FindPropertyOfType(m_hExtension, m_strProperty, m_ePropType);
      m_strProperty = NULL;

      // Retrieve its value, if it exists
      if (pProp != NULL) m_pValue = (Type *)&pProp->m_value;
    }

    // See if the value has been retrieved
    if (m_pValue != NULL) {
      // And return it
      *pValue = *m_pValue;
      return true;
    }

    // Otherwise no value
    return false;
  };

  // Try to look up the desired property and set a value to it
  // Returns false if the value cannot be set (no property found)
  bool SetValue(Type value)
  {
    // If there's a property to look up
    if (m_hExtension != NULL && m_strProperty != NULL) {
      // Look it up and then reset it
      ExtensionProp_t *pProp = ClassicsExtensions_FindProperty(m_hExtension, m_strProperty);
      m_strProperty = NULL;

      // Retrieve its value, if it exists
      if (pProp != NULL) m_pValue = (Type *)&pProp->m_value;
    }

    // See if the value has been retrieved
    if (m_pValue != NULL) {
      // And set it
      *m_pValue = value;
      return true;
    }

    // Otherwise no value
    return false;
  };
};

// Setup for boolean properties
inline void ExtensionPropRef_t<bool>::SetupReference(HPatchPlugin hExtension, const char *strProperty) {
  m_hExtension = hExtension; m_strProperty = strProperty; m_ePropType = ExtensionProp_t::k_EType_Bool; m_pValue = NULL;
};

// Setup for integer properties
inline void ExtensionPropRef_t<int>::SetupReference(HPatchPlugin hExtension, const char *strProperty) {
  m_hExtension = hExtension; m_strProperty = strProperty; m_ePropType = ExtensionProp_t::k_EType_Int; m_pValue = NULL;
};

// Setup for float properties
inline void ExtensionPropRef_t<float>::SetupReference(HPatchPlugin hExtension, const char *strProperty) {
  m_hExtension = hExtension; m_strProperty = strProperty; m_ePropType = ExtensionProp_t::k_EType_Float; m_pValue = NULL;
};

// Setup for double properties
inline void ExtensionPropRef_t<double>::SetupReference(HPatchPlugin hExtension, const char *strProperty) {
  m_hExtension = hExtension; m_strProperty = strProperty; m_ePropType = ExtensionProp_t::k_EType_Double; m_pValue = NULL;
};

// Setup for string properties
inline void ExtensionPropRef_t<const char *>::SetupReference(HPatchPlugin hExtension, const char *strProperty) {
  m_hExtension = hExtension; m_strProperty = strProperty; m_ePropType = ExtensionProp_t::k_EType_String; m_pValue = NULL;
};

// Setup for data properties
inline void ExtensionPropRef_t<void *>::SetupReference(HPatchPlugin hExtension, const char *strProperty) {
  m_hExtension = hExtension; m_strProperty = strProperty; m_ePropType = ExtensionProp_t::k_EType_Data; m_pValue = NULL;
};

// Function handler of an extension signal
// Input data and output value are completely optional and depend on the function implementation
typedef int (*FExtensionSignal)(void *pOptionalData);

// One extension signal in the array
struct ExtensionSignal_t
{
  const char *m_strSignal;
  FExtensionSignal m_pHandler;
};

// Extension signals symbols
#define EXTENSIONMODULE_SIGNALARRAY  ClassicsPatchExtension_SignalArray
#define EXTENSIONMODULE_SIGNALCOUNT  ClassicsPatchExtension_SignalCount

// Define array of extension signals
// Example usage:
//    CLASSICSPATCH_EXTENSION_SIGNALS_BEGIN {
//      { "my_signal_a", &SignalFunc1 },
//      { "my_signal_b", &SignalFunc2 },
//    } CLASSICSPATCH_EXTENSION_SIGNALS_END;
#define CLASSICSPATCH_EXTENSION_SIGNALS_BEGIN MODULE_API ExtensionSignal_t EXTENSIONMODULE_SIGNALARRAY[] =
#define CLASSICSPATCH_EXTENSION_SIGNALS_END ; MODULE_API const size_t EXTENSIONMODULE_SIGNALCOUNT = ARRAYCOUNT(EXTENSIONMODULE_SIGNALARRAY)

// Try to find the handler of some extension signal; returns NULL if not found
PATCH_API FExtensionSignal PATCH_CALLTYPE ClassicsExtensions_FindSignal(HPatchPlugin hPlugin, const char *strSignal);

//================================================================================================//
// Virtual Classics Patch API
//================================================================================================//

class IClassicsExtensions
{
public:
  virtual int GetExtensionCount(void) { return ClassicsExtensions_GetExtensionCount(); };
  virtual HPatchPlugin GetExtensionByIndex(int iExtension) { return ClassicsExtensions_GetExtensionByIndex(iExtension); };
  virtual HPatchPlugin GetExtensionByName(const char *strExtension) { return ClassicsExtensions_GetExtensionByName(strExtension); };

  virtual ExtensionProp_t *FindProperty(HPatchPlugin hPlugin, const char *strProperty) { return ClassicsExtensions_FindProperty(hPlugin, strProperty); };
  virtual ExtensionProp_t *FindPropertyOfType(HPatchPlugin hPlugin, const char *strProperty, ExtensionProp_t::EType eType) { return ClassicsExtensions_FindPropertyOfType(hPlugin, strProperty, eType); };

  virtual bool GetBool(HPatchPlugin hPlugin, const char *strProperty, bool *pValue) { return ClassicsExtensions_GetBool(hPlugin, strProperty, pValue); };
  virtual bool GetInt(HPatchPlugin hPlugin, const char *strProperty, int *pValue) { return ClassicsExtensions_GetInt(hPlugin, strProperty, pValue); };
  virtual bool GetFloat(HPatchPlugin hPlugin, const char *strProperty, float *pValue) { return ClassicsExtensions_GetFloat(hPlugin, strProperty, pValue); };
  virtual bool GetDouble(HPatchPlugin hPlugin, const char *strProperty, double *pValue) { return ClassicsExtensions_GetDouble(hPlugin, strProperty, pValue); };
  virtual bool GetString(HPatchPlugin hPlugin, const char *strProperty, const char **pValue) { return ClassicsExtensions_GetString(hPlugin, strProperty, pValue); };
  virtual bool GetData(HPatchPlugin hPlugin, const char *strProperty, void **pValue) { return ClassicsExtensions_GetData(hPlugin, strProperty, pValue); };

  virtual bool SetBool(HPatchPlugin hPlugin, const char *strProperty, bool bValue) { return ClassicsExtensions_SetBool(hPlugin, strProperty, bValue); };
  virtual bool SetInt(HPatchPlugin hPlugin, const char *strProperty, int iValue) { return ClassicsExtensions_SetInt(hPlugin, strProperty, iValue); };
  virtual bool SetFloat(HPatchPlugin hPlugin, const char *strProperty, float fValue) { return ClassicsExtensions_SetFloat(hPlugin, strProperty, fValue); };
  virtual bool SetDouble(HPatchPlugin hPlugin, const char *strProperty, double fValue) { return ClassicsExtensions_SetDouble(hPlugin, strProperty, fValue); };
  virtual bool SetString(HPatchPlugin hPlugin, const char *strProperty, const char *strValue) { return ClassicsExtensions_SetString(hPlugin, strProperty, strValue); };
  virtual bool SetData(HPatchPlugin hPlugin, const char *strProperty, void *pValue) { return ClassicsExtensions_SetData(hPlugin, strProperty, pValue); };

  virtual FExtensionSignal FindSignal(HPatchPlugin hPlugin, const char *strSignal) { return ClassicsExtensions_FindSignal(hPlugin, strSignal); };
};

#endif // CLASSICSPATCH_IEXTENSIONS_H
