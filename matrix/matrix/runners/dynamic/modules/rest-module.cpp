#pragma once

#include "../../../../external/chaiscript/chaiscript.hpp"
#include <string>
#include "../../../../external/restclient/include/restclient.h"
#include "../../../../logger/logger.h"

using namespace chaiscript;

// This class is instantiable from chai
class RestModule {

public :

    RestModule()
    {
        RestClient::init();
    }

    void get(const std::string& endpoint)
    {
        RestClient::Response r = RestClient::get(endpoint);
        Logger::logDebug("Got: " + r.body);
    }

    // Load the library into ChaiScript
    static ModulePtr bootstrap(ModulePtr m = std::make_shared<Module>()) {
        m->add(user_type<RestModule>(), "_Rest");
        m->add(constructor<RestModule()>(), "_Rest");
        m->add(fun(&RestModule::get), "get");
        return m;
    }

};