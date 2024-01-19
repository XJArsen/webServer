// Copyright (c) RealCoolEngineer. 2024. All rights reserved.
// Author: XJArsen
// Date: 2024-01-19

#ifndef INCLUDE_EPOLL_H_
#define INCLUDE_EPOLL_H_
#include <vector>
class Channel;
class Epoll {
   private:
    int epfd;
    struct epoll_event *events;

   public:
    Epoll();
    ~Epoll();
    void updateChannel(Channel *);
    std::vector<Channel *> poll(int timeout = -1);
};
#endif
