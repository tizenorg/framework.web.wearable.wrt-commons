/*
 * Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */
/**
 *
 * @file    common_dao_types.h
 * @author  Michal Ciepielski (m.ciepielski@samsung.com)
 * @version 1.0
 * @brief   This file contains the declaration of common data types for wrtdb
 */

#ifndef WRT_WIDGET_DAO_COMMON_DAO_TYPES_H_
#define WRT_WIDGET_DAO_COMMON_DAO_TYPES_H_

#include <set>
#include <string>
#include <map>
#include <vector>
#include <list>
#include <memory>
#include <dpl/optional_typedefs.h>

namespace WrtDB {
class PluginMetafileData
{
  public:
    struct Feature
    {
        std::string m_name;
        std::set<std::string> m_deviceCapabilities;

        bool operator< (const Feature& obj) const
        {
            return m_name < obj.m_name;
        }
    };
    typedef std::set<Feature> FeatureContainer;

  public:

    PluginMetafileData()
    {}

    std::string m_libraryName;
    FeatureContainer m_featureContainer;
};

class PluginObjectsDAO
{
  public:
    typedef std::set<std::string> Objects;
    typedef std::shared_ptr<Objects> ObjectsPtr;

  public:
    explicit PluginObjectsDAO() {}

  protected:
    ObjectsPtr m_implemented;
    ObjectsPtr m_dependent;
};

/**
 * @brief Widget id describes web-runtime global widget identifier.
 *
 * Notice that only up to one widget can exist at the same time.
 * DbWidgetHandle can be translated into corresponding WidgetModel by invoking
 * FindWidgetModel routine.
 */
typedef int DbWidgetHandle;
typedef DPL::String TizenPkgId;
typedef DPL::String TizenAppId;

/**
 * Value of invalid widget handle
 */
enum {
    INVALID_WIDGET_HANDLE = -1
};

/**
 * @brief Structure to hold the information of widget's size
 */
struct DbWidgetSize
{
    DPL::OptionalInt width;
    DPL::OptionalInt height;

    DbWidgetSize(DPL::OptionalInt w = DPL::OptionalInt::Null,
                 DPL::OptionalInt h = DPL::OptionalInt::Null) :
        width(w),
        height(h)
    {}
};

inline bool operator ==(const DbWidgetSize &objA, const DbWidgetSize &objB)
{
    if (!objA.height || !objA.width || !objB.width || !objB.height) {
        return false;
    } else {
        return *objA.height == *objB.height && *objA.width == *objB.width;
    }
}

/**
 * Widget [G]lobal [U]nique [ID]entifier
 * Orginated from appstore ID
 */
typedef DPL::OptionalString WidgetGUID;

struct WidgetAccessInfo
{
    DPL::String strIRI;                /* origin iri */
    bool bSubDomains;                  /* do we want access to subdomains ? */

    bool operator ==(const WidgetAccessInfo& info) const
    {
        return info.strIRI == strIRI &&
               info.bSubDomains == bSubDomains;
    }
};
typedef std::list<WidgetAccessInfo> WidgetAccessInfoList;

struct WidgetAllowNavigationInfo
{
    DPL::String scheme;
    DPL::String host;
};
typedef std::list<WidgetAllowNavigationInfo> WidgetAllowNavigationInfoList;

struct EncryptedFileInfo
{
    DPL::String fileName;
    int fileSize;

    bool operator==(const EncryptedFileInfo& info) const
    {
        return fileName == info.fileName;
    }

    bool operator==(const DPL::String& file) const
    {
        return fileName == file;
    }

    bool operator< (const EncryptedFileInfo& info) const
    {
        return fileName < info.fileName;
    }
};

typedef std::list<DPL::String> WindowModeList;

typedef std::list<DPL::String> PrivilegeList;

typedef std::set<EncryptedFileInfo> EncryptedFileList;

/**
 * @brief Widget feature host information about possible javascript extensions
 *        that widget may use
 *
 * Widget features are declared in configuration file in widget installation
 * package. Each declared special feature is contained in some wrt-plugin that
 * declares to implement it. After widget launch wrt searches for proper plugin
 * libraries and load needed features.
 *
 * Widget features can be required or optional. It is possible to start widget
 * without missing feature. When required feature cannot be loaded widget will
 * not start.
 */

enum {
    INVALID_PLUGIN_HANDLE = -1
};
typedef int DbPluginHandle;

struct DbWidgetFeature
{
    DPL::String name;        /// Feature name
    bool rejected;           /// Api feature was rejected by ace
    DbPluginHandle pluginId; /// Plugin id that implement this feature

    DbWidgetFeature() :
        pluginId(INVALID_PLUGIN_HANDLE)
    {}
};

inline bool operator < (const DbWidgetFeature &objA,
                        const DbWidgetFeature &objB)
{
    return objA.name.compare(objB.name) < 0;
}

inline bool operator==(const DbWidgetFeature &featureA,
                       const DbWidgetFeature &featureB)
{
    return featureA.name == featureB.name &&
           featureA.pluginId == featureB.pluginId;
}

/**
 * @brief Default container for features list
 */
typedef std::multiset<DbWidgetFeature> DbWidgetFeatureSet;

/**
 * @brief Default container with DbWidgetHandle's
 */
typedef std::list<DbWidgetHandle> DbWidgetHandleList;
typedef std::list<TizenAppId> TizenAppIdList;
typedef std::list<TizenPkgId> TizenPkgIdList;

class WidgetDAOReadOnly; //forward declaration
typedef std::shared_ptr<WidgetDAOReadOnly> WidgetDAOReadOnlyPtr;
/**
 * @brief Default container with WidgetDAOReadOnly
 */
typedef std::list<WidgetDAOReadOnlyPtr> DbWidgetDAOReadOnlyList;

/**
 * @brief Widget specific type
 *
 * Widget type describes belowed in WAC, TIZEN WebApp
 */
enum AppType
{
    APP_TYPE_UNKNOWN = 0, // unknown
    APP_TYPE_TIZENWEBAPP, // Tizen webapp
    APP_TYPE_TIZENWEBSERVICE // Tizen web service
};

class WidgetType
{
  public:
    WidgetType() :
        appType(APP_TYPE_UNKNOWN)
    {}
    WidgetType(const AppType type) :
        appType(type)
    {}
    bool operator== (const AppType& other) const
    {
        return appType == other;
    }
    bool operator!= (const AppType& other) const
    {
        return appType != other;
    }
    std::string getApptypeToString()
    {
        switch (appType) {
#define X(x) case x: return #x;
            X(APP_TYPE_UNKNOWN)
            X(APP_TYPE_TIZENWEBAPP)
            X(APP_TYPE_TIZENWEBSERVICE)

#undef X
        default:
            return "UNKNOWN";
        }
    }

    AppType appType;
};

/**
 * @brief Package specific type
 *
 * Package type describes belowed in Tizen webapp, C++ service App
 */
enum PkgType
{
    PKG_TYPE_UNKNOWN = 0, // unknown
    PKG_TYPE_NOMAL_WEB_APP,
    PKG_TYPE_DIRECTORY_WEB_APP,
    PKG_TYPE_HOSTED_WEB_APP,    // request from browser
    PKG_TYPE_HYBRID_WEB_APP // Tizen webapp with C++ service app
};

class PackagingType
{
  public:
    PackagingType() :
        pkgType(PKG_TYPE_UNKNOWN)
    {}
    PackagingType(const PkgType type) :
        pkgType(type)
    {}
    bool operator== (const PkgType& other) const
    {
        return pkgType == other;
    }
    bool operator!= (const PkgType& other) const
    {
        return pkgType != other;
    }
    std::string getPkgtypeToString()
    {
        switch (pkgType) {
#define X(x) case x: return #x;
            X(PKG_TYPE_UNKNOWN)
            X(PKG_TYPE_NOMAL_WEB_APP)
            X(PKG_TYPE_DIRECTORY_WEB_APP)
            X(PKG_TYPE_HOSTED_WEB_APP)
            X(PKG_TYPE_HYBRID_WEB_APP)
#undef X
        default:
            return "UNKNOWN";
        }
    }

    PkgType pkgType;
};

struct WidgetSetting
{
    DPL::String settingName;
    DPL::String settingValue;

    bool operator ==(const WidgetSetting& info) const
    {
        return (info.settingName == settingName &&
                info.settingValue == settingValue);
    }
    bool operator !=(const WidgetSetting& info) const
    {
        return (info.settingName != settingName ||
                info.settingValue != settingValue);
    }
};

typedef std::list<WidgetSetting> WidgetSettings;

/**
 * @brief Widget AppControl
 *
 * Application control describes details of behaviour
 * when widget receives aul bundle data.
 */
namespace AppControlPrefix {
    const char* const PROCESS_PREFIX = "-__CONTROL_PROCESS__";
}
struct WidgetAppControl
{
    enum class Disposition {
        UNDEFINE = 0,
        WINDOW   = 1,
        INLINE   = 2
    };

    DPL::String src;       /* start uri */
    DPL::String operation; /* service name */
    DPL::String uri;    /* scheme type*/
    DPL::String mime;      /* mime type */
    Disposition disposition;
    unsigned index;

    bool operator== (const WidgetAppControl& other) const
    {
        return src == other.src &&
               operation == other.operation &&
               uri == other.uri &&
               mime == other.mime &&
               disposition == other.disposition;
    }
};

typedef std::list<WidgetAppControl> WidgetAppControlList;

enum class WidgetSecurityModelVersion
{
    WIDGET_SECURITY_MODEL_V1 = 0, // WARP
    WIDGET_SECURITY_MODEL_V2      // CSP, allow-navigation
};
} // namespace WrtDB
#endif /* WRT_WIDGET_DAO_COMMON_DAO_TYPES_H_ */