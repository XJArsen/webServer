// #include "EventLoop.h"
// #include "Channel.h"
// #include "Epoll.h"
// #include "ThreadPool.h"
// EventLoop::EventLoop() {
//     ep = new Epoll();
//     threadPool = new ThreadPool();
// }
// EventLoop::~EventLoop() {
//     delete threadPool;
//     delete ep;
// }

// void EventLoop::loop() {
//     while (!quit) {
//         std::vector<Channel *> chs;
//         chs = ep->poll();

//         for (Channel *it : chs) {
//             it->handleEvent();
//         }
//     }
// }

// void EventLoop::updateChannel(Channel *channel) {
//     ep->updateChannel(channel);
// }

// void EventLoop::addThread(std::function<void()> func) {
//     threadPool->add(func);
// }