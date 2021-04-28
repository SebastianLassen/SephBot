#pragma once

#include <memory>

class MapTools;
class ProductionManager;
class Workers;
class Squads;

class Global
{
    MapTools*           p_mapTools = nullptr;
    ProductionManager*  p_productionManager = nullptr;
    Workers*            p_workers = nullptr;
    Squads*             p_squads = nullptr;


    template <class T>
    void reset(T*& ptr)
    {
        delete ptr;
        ptr = nullptr;
    }

    template <class T>
    static T* get(T*& ptr)
    {
        if (ptr == nullptr) { ptr = new T(); }
        return ptr;
    }

    Global();
    static Global& Instance();
    void init();

public:

    static void                 GameStart();
    static MapTools&            Map();
    static ProductionManager&   Production();
    static Workers&             Worker();
    static Squads&              Squads();

};
