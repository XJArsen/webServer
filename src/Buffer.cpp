#include "Buffer.h"
#include <iostream>

Buffer::Buffer(int initBuffSize) : buf_(initBuffSize), readPos_(0), writePos_(0) {
}

size_t Buffer::WritableBytes() const {
    return buf_.size() - writePos_;
}
size_t Buffer::ReadableBytes() const {
    return writePos_ - readPos_;
}
size_t Buffer::PrependableBytes() const {
    return readPos_;
}

const char* Buffer::Peek() const {
    return &buf_[readPos_];
}
void Buffer::EnsureWriteable(size_t len) {
    if (len > WritableBytes()) {
        MakeSpace(len);
    }
}
void Buffer::HasWritten(size_t len) {
    writePos_ += len;
}

void Buffer::Retrieve(size_t len) {
    readPos_ += len;
}
void Buffer::RetrieveUntil(const char* end) {
    // assert(Peek() <= end);
    Retrieve(end - Peek());
}

void Buffer::RetrieveAll() {
    bzero(&buf_[0], buf_.size());
    readPos_ = writePos_ = 0;
}
std::string Buffer::RetrieveAllToStr() {
    std::string str(Peek(), ReadableBytes());
    RetrieveAll();
    return str;
}

const char* Buffer::BeginWriteConst() const {
    return &buf_[writePos_];
}
char* Buffer::BeginWrite() {
    return &buf_[writePos_];
}

void Buffer::Append(const char* str, size_t len) {
    EnsureWriteable(len);
    std::copy(str, str + len, BeginWrite());
    HasWritten(len);
}
void Buffer::Append(const std::string& str) {
    Append(str.c_str(), str.size());
}
void Buffer::Append(const void* data, size_t len) {
    Append(static_cast<const char*>(data), len);
}
void Buffer::Append(const Buffer& buff) {
    Append(buff.Peek(), buff.ReadableBytes());
}

ssize_t Buffer::ReadFd(int fd, int* Errno) {
    char buff[65535];  // 栈区
    struct iovec iov[2];
    size_t writeable = WritableBytes();
    iov[0].iov_base = BeginWrite();
    iov[0].iov_len = writeable;
    iov[1].iov_base = buff;
    iov[1].iov_len = sizeof(buff);

    ssize_t len = readv(fd, iov, 2);
    if (len < 0) {
        *Errno = errno;
    } else if (static_cast<size_t>(len) <= writeable) {
        writePos_ += len;
    } else {
        writePos_ = buf_.size();
        Append(buff, static_cast<size_t>(len - writeable));
    }
    return len;
}
ssize_t Buffer::WriteFd(int fd, int* Errno) {
    ssize_t len = write(fd, Peek(), ReadableBytes());
    if (len < 0) {
        *Errno = errno;
    } else {
        Retrieve(len);
    }
    return len;
}
char* Buffer::BeginPtr() {
    return &buf_[0];
}
const char* Buffer::BeginPtr() const {
    return &buf_[0];
}
void Buffer::MakeSpace(size_t len) {
    if (WritableBytes() + PrependableBytes() < len) {
        buf_.resize(writePos_ + len + 1);
    } else {
        size_t readable = ReadableBytes();
        std::copy(BeginPtr() + readPos_, BeginPtr() + writePos_, BeginPtr());
        readPos_ = 0;
        writePos_ = readable;
    }
}
