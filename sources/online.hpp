#pragma once
#include "../utils/Utils.hpp"

class online_manager
{
public:
    void enter_hall_room(const int uid,const wsServer_t::connection_ptr& con_ptr)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _hall_room.insert({uid,con_ptr});
    }
    void enter_game_room(const int uid,const wsServer_t::connection_ptr& con_ptr)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _game_room.insert({uid,con_ptr});
    }
    void exit_hall_room(const int uid)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _hall_room.erase(uid);
    }
    void exit_game_room(const int uid)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _game_room.erase(uid);
    }
    bool is_in_hall_room(const int uid)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        if(_hall_room.count(uid))
        {
            return true;
        }
        return false;
    }
    bool is_in_game_room(const int uid)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        if(_game_room.count(uid))
        {
            return true;
        }
        return false;
    }
    wsServer_t::connection_ptr get_connection_from_hall(const int uid)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        if(_hall_room.count(uid))
        {
            return _hall_room[uid];
        }
        return wsServer_t::connection_ptr();
    }
    wsServer_t::connection_ptr get_connection_from_room(const int uid)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        if(_game_room.count(uid))
        {
            return _game_room[uid];
        }
        return wsServer_t::connection_ptr();
    }
private:
    std::mutex _mutex;
    std::unordered_map<int,wsServer_t::connection_ptr> _hall_room;
    std::unordered_map<int,wsServer_t::connection_ptr> _game_room;
};